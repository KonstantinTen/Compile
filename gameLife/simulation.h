#ifndef SIM_H
#define SIM_H

#define SIM_X_SIZE 512
#define SIM_Y_SIZE 256

// Цвета в формате ARGB
#define COLOR_ALIVE 0xFFFF0000 // Красный
#define COLOR_DEAD  0xFF000000 // Черный

void simInit();
void app();
void simExit();
void simFlush();
void simPutPixel(int x, int y, int argb);
int  simRand();

#endif