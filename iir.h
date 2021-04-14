#include "define.h"
#include <math.h>
typedef struct{
    float prevInAcr; //Valeur d'entrée acr précédente
    float prevOutAcr; //Valeur de sortie acr précédente
    float prevInAcir; //Valeur d'entrée acir précédente
    float prevOutAcir; //Valeur de sortie acir précédente
}coefIIR;

coefIIR* newCoef();//initialisation de la structure
absorp iirTest(char* filename);//fonction de test de la fonction IIR
absorp IIR(absorp myAbsorb, coefIIR* coef);//filtrage de myAbsorp dans la fonction IIR