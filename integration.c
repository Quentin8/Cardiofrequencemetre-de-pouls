#include "lecture.h"
#include "fir.h"
#include "iir.h"
#include "mesure.h"
#include "fichiers.h"
#include "affichage.h"
#include "define.h"

void integrationTest(char* filename)
{
    float FIR_TAPS[51]={//coefficients du filtre d'ordre 51
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
    //objets permettant le traitement des données
    absorp myAbsorp;//Récupère les valeurs du capteurs et passe par des filtres
    oxy myOxy;//Est renvoyé par la fonction mesure afin d'avoir spo2 et pouls
    mesures* mesures = newMesures(); //On initialise notre structure mesures
    coefIIR* coef = newCoef(); //On initialise notre structure coef

    int tableACIR[51]; // On crée le tableau des 51 valeurs Acir précédentes
    int tableACR[51]; // On crée le tableau des 51 valeurs Acr précédentes
    int i =0;
    for(i =0;i<51;i++){
        tableACIR[i] = 0; // On initialise les valeurs des tableaux à 0 pour éviter d'avoir des valeurs aléatoires
        tableACR[i] = 0;
    }

    FILE* file = initFichier(filename);//donnees arrivant du capteur
    int filestate = 0;
    //Valeur de myAbsorp brut : en entrée (sortie de capteur)
    while(filestate != EOF){
        myAbsorp = lecture(file,&filestate); // On lit une ligne de fichier et on stocke les valeurs dans la structure myAbsorp
        if(filestate != EOF){ // On vérifie que ce n'est pas la fin du fichier
            myAbsorp = FIR(myAbsorp,tableACIR,tableACR,FIR_TAPS); // On fait passer my absorp dans le filtre FIR
            myAbsorp = IIR(myAbsorp,coef); // On fait passer ensuite myAbsorp dans le filtre IIR
            myOxy = mesure(myAbsorp, mesures); // Ensuite on calcule le pouls et le spo2 à partir de myAbsorp obtenu précédemment
            affichage(myOxy); // Après avoir récupérer les valeurs de pouls et spo2 on les affiche dans une interface java
        }
    }
    printf("%d / %d",myOxy.pouls,myOxy.spo2);
    finFichier(file); // On ferme le fichier
    free(mesures); // On libère l'espace alloué à la structure mesures 
    free(coef);// On libère l'espace alloué à la structure coef
}