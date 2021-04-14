#include "define.h"

absorp firTest(char* filename);
void update(absorp echantillon, int* tableauACIR, int* tableauACR);
void calculFIR(int* tableXTEST, int* tableauACR, float* FIR_TAPSTEST,  float* donnees);
absorp FIR(absorp echantillon, int* tableauX, int* tableauACR, float* FIR_TAPSTEST);