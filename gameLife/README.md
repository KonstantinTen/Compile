# Game of life

Implementing my Game of life graphics application and generating IR for [game.c](./game.c)

### Program run

```
clang main.c simulation.c game.c -lSDL2
./a.out
```

### IR generation
```
clang -S -emit-llvm game.c -o game.ll
```