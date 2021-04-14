#include "iir.h"
#include "fichiers.h"

coefIIR* newCoef(){//creation d'une structure coef vide
    coefIIR* coef = (coefIIR*)malloc(sizeof(coefIIR));
    coef->prevInAcr = 0; //On initialise les valeurs précédentes à 0;
    coef->prevOutAcr = 0;
    coef->prevInAcir = 0;
    coef->prevOutAcir = 0;

    return coef;
}

absorp iirTest(char* filename){
	absorp	myAbsorp;
	absorp tmp;
	coefIIR* coef = newCoef(); //On crée notre structure pour stocker les valeurs précédentes

	int filestate = 0;
	FILE* file = initFichier(filename);


    while(filestate != EOF){ //Tant qu'on est pas à la fin du fichier on continue de le lire
        tmp = lireFichier(file,&filestate); //On lit une ligne du fichier et on la stocke dans une struct absorp temporaire
        if(filestate != EOF){ //Si c'est pas la fin du fichier on rentre dans le if
            myAbsorp = IIR(tmp,coef); //On appelle la fonction IIR qui va effectuer le filtre sur acr et acir et les stocker dans myAbsorp
        }
	}

    free(coef); //On libère la mémoire utilisé par notre structure coefIIR
    finFichier(file); //On ferme le fichier file
	
	return myAbsorp;

}

//La fonction permet de réaliser un filtre IIR (impulsion infinie) sur les valeurs acr et acir de myAbsorp
absorp IIR(absorp myAbsorb, coefIIR* coef){
    float alpha = 0.992; //coef alpha donné

    float prevIn = myAbsorb.acr; //On stocke la valeur acr temporairement
    myAbsorb.acr = myAbsorb.acr - coef->prevInAcr + alpha * coef->prevOutAcr; //On fait le calcul du filtre et on le stocke dans acr
    coef->prevInAcr = prevIn; //On stocke la valeur acr récupérée précédemment dans prevInAcr
    coef->prevOutAcr = myAbsorb.acr; //On stocke la valeur acr calculé dans prevOutAcr
    
    //Même fonctionnement pour la partie acir
    prevIn = myAbsorb.acir; // Stockage valeur acr temporaire
    myAbsorb.acir = myAbsorb.acir - coef->prevInAcir + alpha * coef->prevOutAcir; // Calcul du filtre
    coef->prevInAcir = prevIn; // Récupération valeur stockée précédemment et Stockage dans prevInAcir
    coef->prevOutAcir = myAbsorb.acir; // Stockage dans prevOutAcir de la valeur de sortie précédente

    return myAbsorb;
}

