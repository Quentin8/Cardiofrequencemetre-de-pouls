#include "define.h"
//structure permettant de recuperer toutes les valeurs utiles pour realiser les mesures
typedef struct{
//valeurs des minimums et maximums au cours d'une periode afin de calculer le PtP sur celle ci
    int minAcr;
    int maxAcr;
    int minAcir;
    int maxAcir;

    int compteurAcr;//permet de compter nombre de valeurs dans une periode
    int etatAcr;//variable etatAcr du switch case (pour mesurer la periode, donc le pouls)
    int poulsAcr;//Variable qui stock le pouls
    int spo2;//Stockage du Spo2
} mesures;
mesures* newMesures();//initialisation de la strcuture de mesure
void calculPouls(absorp myAbsorp, mesures* mesures);//mesure du pouls
oxy mesure(absorp myAbsorp, mesures* mesures);//Permet la mesure de Spo2 et du pouls
oxy mesureTest(char* filename);//Fonction permettant de tester le fonctionnement de la fonction mesure
void calculSpo2(absorp myAbsorp,mesures* mesures);//mesure du spo2
void calculMinMax(absorp myAbsorp, mesures* mesures);//Détermination des valeurs minimums et maximums au cours d'une période (pour calcul PtP)



	
