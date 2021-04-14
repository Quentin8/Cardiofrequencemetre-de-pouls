#include <stdbool.h>
#include "mesure.h"
#include "fichiers.h"

void calculPouls(absorp myAbsorp,mesures* mesures){//mesure du pouls
    //Cette fonction modifie la structure mesures (via pointeur) et permet le calcul du pouls ; on effectue les mesures sur Acr
    //Elle met à jours le compteurs d'echantillons (nb de valeurs pendant une période), la variable etatAcr
    //Le switch permet de s'assurer qu'on a bien une période avant de calculer le pouls
    //Le case 1 sert à calibrer la mesure
    //le case 2 démarre la mesure au premier passage par 0
    //le case 3 s'assure que l'on passe une seconde fois par 0 et le 4 que l'on repasse par 0, donc que l'on a fait une période
    switch (mesures->etatAcr){
        case 1 :
            if(myAbsorp.acr <= 0){
                mesures->etatAcr = 2;
            }
            break;
        case 2 :
            if( myAbsorp.acr > 0){
                mesures->etatAcr = 3;
                mesures->compteurAcr = 0;
            }
            break;
        case 3 :
            if(myAbsorp.acr <= 0){
                mesures->etatAcr = 4;
            }
            break;
        case 4 :
            if(myAbsorp.acr > 0){
                mesures->poulsAcr = (mesures->poulsAcr + 30000/mesures->compteurAcr)/2; //On calcule le pouls à l'aide du compteur d'échanrtillon d'avant
                mesures->compteurAcr = 0;//reset le compteurAcr pour une nouvelle mesure
                mesures->etatAcr = 2; //On repasse à l'état 2 pour permettre une nouvelle mesure de période
                //On reset le min et le max a chaque nouvelle periode afin d'avoir la valeur Spo2 correspondante à cete periode
                mesures->maxAcr = 0;
                mesures->minAcr = 0;
                mesures->maxAcir = 0;
                mesures->minAcir = 0;
            }
            break;
        default:
            break;
    }
}
mesures* newMesures(){//initialisation de la strcuture de mesure
    mesures* newStruct = (mesures*) malloc(sizeof(mesures));//allocation dynamique de la mémoire

    newStruct->minAcr = 0; //On initialise les valeurs min et max de acr et acir
    newStruct->maxAcr = 0;
    newStruct->minAcir = 0;
    newStruct->maxAcir = 0;

    newStruct->compteurAcr = 0; //On initialise le compteur
    newStruct->etatAcr = 1; //On initialise l'état à 1 pour que le switch fonctionne bien
    newStruct->poulsAcr = 0; //On initialise le pouls à 0 en attendant que le calcul soit effectué
    newStruct->spo2 = 0; //On initialise spo2 à 0 en attendant que le calcul soit effectué


    return newStruct;
}
void calculMinMax(absorp myAbsorp, mesures* mesures){//modifie la structure mesures minAcir et maxAcir (via pointeur)
    //On upload les valeurs de minAcir et maxAcir dans la structure
    //Recherche du minAcir et du maxAcir
    if(myAbsorp.acir > mesures->maxAcir ){
        mesures->maxAcir = myAbsorp.acir;
    }
    if(myAbsorp.acir < mesures->minAcir){
        mesures->minAcir = myAbsorp.acir;
    }
    //calcul des min et max de Acr
    if(myAbsorp.acr > mesures->maxAcr ){
        mesures->maxAcr = myAbsorp.acr;
    }
    if(myAbsorp.acr < mesures->minAcr){
        mesures->minAcr = myAbsorp.acr;
    }
}
void calculSpo2(absorp myAbsorp, mesures* mesures){
    //calcul de SPO2
    //Au debut pas bonnes valeurs car il faut au moins une periode pour balayer toute la courbe
    float RsIr = 0;//Correspond au ratio
    int spo2 = 0;//valeur finale de Spo2
    if((mesures->maxAcir - mesures->minAcir) != 0 && (mesures->maxAcr - mesures->minAcr) != 0){ //S'assure que ne valent pas 0
        //Valeurs pike to pike
        int PtPACR = mesures->maxAcr - mesures->minAcr;
        int PtPACIR = mesures->maxAcir - mesures->minAcir;
        RsIr = (PtPACR / myAbsorp.dcr) / (PtPACIR/myAbsorp.dcir);//Ratio
        //Determination de spo2 grace à la courbe Spo2 en fonction de ration (RsIr)
        //On determine le coeficient de la droite à l'aide
        float Xc = 1;//Valeur a partir de laquelle la pente n'est plus la même
        if(RsIr < Xc){//on se trouve dans la premiere partie de la droite
            spo2 = 110 - 25*RsIr;
        }
        else{//2 eme partie de la droite
            spo2 = 120 - 35*RsIr;
        }

    }else{//Eviter de diviser par 0 et donc les potentiels erreurs
        RsIr = 0;
        spo2 = mesures->spo2;//afin d'éviter d'écraser la valeur anciennement présente
    }
    mesures->spo2 = (spo2 + mesures->spo2)/2;//calcul de la moyenne de spo2 avec la valeur précédente afin d'afiner la précision
}
oxy mesure(absorp myAbsorp, mesures* mesures) {//Fonction permettant d'obtenir le pouls et le spo2
    oxy myOxy;
    calculMinMax(myAbsorp,mesures);//determination du min et max par période
    mesures->compteurAcr++;//Incrémentation du compteur d'échantillons
    calculPouls(myAbsorp,mesures);//determine le pouls et l'actualise à chaque période
    calculSpo2(myAbsorp,mesures);//détermine le spo2

    myOxy.pouls = mesures->poulsAcr; // On met la valeur du pouls calculée dans notre structure myOxy
    myOxy.spo2 = mesures->spo2; // On met la valeur du spo2 calculée dans notre structure myOxy

    return myOxy;
}

oxy mesureTest(char* filename) {
    oxy myOxy;
    absorp myAbsorp;
    mesures* mesures = newMesures();

    int filestate = 0;
    FILE *file = initFichier(filename);

    while (filestate != EOF) { // On rentre dans la boucle tant qu'on n'atteint pas la fin du fichier
        myAbsorp = lireFichier(file, &filestate); //On stocke la ligne du fichier dans myAbsorp
        if (filestate != EOF) { //si la ligne n'est pas la fin du fichier on rentre dans le if
            myOxy = mesure(myAbsorp,mesures); //On appelle la fonction mesure pour calculer le pouls et le spo2

        }
    }

    finFichier(file); // On ferme le fichier
    return myOxy;
}