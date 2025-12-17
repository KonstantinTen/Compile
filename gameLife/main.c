#include "simulation.h"

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    
    simInit();
    app();
    simExit();
    
    return 0;
}