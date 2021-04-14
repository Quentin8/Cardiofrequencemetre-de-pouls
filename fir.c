#include "fir.h"
#include "fichiers.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
absorp FIR(absorp echantillon, int* tableauACIR, int* tableauACR, float* FIR_TAPSTEST){//filtrage FIR, renvoit le nouveau absorp (avec Acr et Acirfiltré)
    absorp myAbs = echantillon;//copie de echantillon dans myAbs
    update(myAbs, tableauACIR, tableauACR);//buffer circulaire : permet de rajouter une valeur au début et de décaler les 50 autres à droite
    float donnees[2] ={0,0};//Variable qui reçoit les valeurs ACR et ACIR (modifié via pointeur dans calculFIR)
    calculFIR(tableauACIR, tableauACR, FIR_TAPSTEST, donnees);//calcul de FIR pour un échantillon n
    myAbs.acir = donnees[0];//modifit valeur de ACIR
    myAbs.acr = donnees[1];//modifie valeur de ACR

    return myAbs;
}
void update(absorp echantillon, int* tableauACIR, int* tableauACR){
    //Fonction qui decale toutes les valeurs vers la droite (pour faire place aux nouvelles valeurs venant du capteur)
    int i = 0;
    //variables pour memoriser les valeurs à décaller
    int previousACIR = 0;
    int previousACR = 0;
    int newACIR = echantillon.acir;
    int newACR = echantillon.acr;
    //décalage de tous le tableau
    for(i=0;i<51;i++){
        previousACIR = tableauACIR[i];
        previousACR = tableauACR[i];

        tableauACIR[i] = newACIR;
        tableauACR[i] = newACR;

        newACIR = previousACIR;
        newACR = previousACR;
    }

}
void calculFIR(int* tableACIR, int* tableauACR, float* FIR_TAPSTEST, float* donnees){//modifit tableau de ACIR : 0  et ACR :1
    int i=0;
    //FIR_TAPSTEST est le tableau des coefficients du filtre
    //Application de la formule pour le calcul de FIR
    for(i=0;i<51;i++){
        donnees[0] += FIR_TAPSTEST[i] * tableACIR[50-i];//nouvelle valeur ACIR
        donnees[1] += FIR_TAPSTEST[i] * tableauACR[50-i];//nouvelle valeur ACR
    }
}
absorp firTest(char* filename){
    //calcul de y[n]
    float FIR_TAPS[51]={//coefficients du filtre
            1.4774946e-019,
            1.6465231e-004,
            3.8503956e-004,
            7.0848037e-004,
            1.1840522e-003,
            1.8598621e-003,
            2.7802151e-003,
            3.9828263e-003,
            5.4962252e-003,
            7.3374938e-003,
            9.5104679e-003,
            1.2004510e-002,
            1.4793934e-002,
            1.7838135e-002,
            2.1082435e-002,
            2.4459630e-002,
            2.7892178e-002,
            3.1294938e-002,
            3.4578348e-002,
            3.7651889e-002,
            4.0427695e-002,
            4.2824111e-002,
            4.4769071e-002,
            4.6203098e-002,
            4.7081811e-002,
            4.7377805e-002,
            4.7081811e-002,
            4.6203098e-002,
            4.4769071e-002,
            4.2824111e-002,
            4.0427695e-002,
            3.7651889e-002,
            3.4578348e-002,
            3.1294938e-002,
            2.7892178e-002,
            2.4459630e-002,
            2.1082435e-002,
            1.7838135e-002,
            1.4793934e-002,
            1.2004510e-002,
            9.5104679e-003,
            7.3374938e-003,
            5.4962252e-003,
            3.9828263e-003,
            2.7802151e-003,
            1.8598621e-003,
            1.1840522e-003,
            7.0848037e-004,
            3.8503956e-004,
            1.6465231e-004,
            1.4774946e-019
    };

    absorp	myAbsorp;
    absorp	myAbsorpTMP;//permet à la fin lorsque filestate = EOF de conservé la bonne valeur de myabsorp
    int filestate = 0;//variable qui stocke la valeur et la position du curseur
    //tableau permettant le stockage des 51 dernières valeurs de ACR
    int tableACIR[51];
    //tableau permettant le stockage des 51 dernières valeurs de ACR
    int tableACR[51];
    int i =0;
    //initialise toutes les cellules à 0 afin de ne pas se retrouver au debut avecd des valeurs fausses
    for(i =0;i<51;i++){
        tableACIR[i] = 0;
        tableACR[i] = 0;
    }
    //-------------------------------------------------------------------------------------------
    FILE* dataFile = initFichier(filename);//ouverture du fichier en lecture seule
    if(dataFile != NULL){//simule reception des valeurs
        while (filestate != EOF){
            myAbsorpTMP = lireFichier(dataFile, &filestate);//Lecture du fichier
            if(filestate != EOF){
                myAbsorp = FIR(myAbsorpTMP, tableACIR, tableACR, FIR_TAPS);
                //printf("ACR : %f\t", myAbsorp.acr);
                //printf("ACIR : %f\n", myAbsorp.acir);

            }
        }

    }

    finFichier(dataFile);//fermeture du fichier

    return myAbsorp;

}
