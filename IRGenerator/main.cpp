#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

#include "../gameLife/simulation.h"

using namespace llvm;

int main() {
    LLVMContext context;

    Module* module = new Module("game_of_life", context);
    IRBuilder<> builder(context);

    Type* voidTy = builder.getVoidTy();
    Type* i32Ty = builder.getInt32Ty();
    Type* ptrTy = PointerType::get(context, 0); 


    FunctionCallee simPutPixelFunc = module->getOrInsertFunction("simPutPixel", voidTy, i32Ty, i32Ty, i32Ty);
    FunctionCallee simFlushFunc = module->getOrInsertFunction("simFlush", voidTy);
    FunctionCallee simRandFunc = module->getOrInsertFunction("simRand", i32Ty);

    FunctionType* appType = FunctionType::get(voidTy, false);
    Function* appFunc = Function::Create(appType, Function::ExternalLinkage, "app", module);
    
    BasicBlock* entry = BasicBlock::Create(context, "entry", appFunc);
    builder.SetInsertPoint(entry);

    for (int i = 0; i < 500; ++i) {
        Value* rx = builder.CreateSRem(builder.CreateCall(simRandFunc), ConstantInt::get(i32Ty, SIM_X_SIZE));
        Value* ry = builder.CreateSRem(builder.CreateCall(simRandFunc), ConstantInt::get(i32Ty, SIM_Y_SIZE));
        builder.CreateCall(simPutPixelFunc, {rx, ry, ConstantInt::get(i32Ty, COLOR_ALIVE)});
    }

    // Основной цикл
    BasicBlock* loopBB = BasicBlock::Create(context, "loop", appFunc);
    builder.CreateBr(loopBB);
    builder.SetInsertPoint(loopBB);


    builder.CreateCall(simFlushFunc);
    builder.CreateBr(loopBB);

    if (verifyModule(*module, &outs())) return 1;

    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();

    ExecutionEngine* ee = EngineBuilder(std::unique_ptr<Module>(module)).create();
    ee->InstallLazyFunctionCreator([&](const std::string& name) -> void* {
        if (name == "simPutPixel") return (void*)simPutPixel;
        if (name == "simFlush") return (void*)simFlush;
        if (name == "simRand") return (void*)simRand;
        return nullptr;
    });

    ee->finalizeObject();
    simInit();

    auto appPtr = (void (*)())ee->getFunctionAddress("app");
    if (appPtr) appPtr();

    simExit();
    return 0;
}