# IR Generator



### Program run

```

clang++ $(llvm-config --cppflags --ldflags --libs) ../gameLife/simulation.c main.cpp -lSDL2 -I/usr/include/SDL2
./a.out

```