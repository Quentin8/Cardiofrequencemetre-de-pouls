#include "affichage.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "define.h"
#include "fichiers.h"
#include <string.h>

void affichage(oxy myOxy){
//verification de la présence du fichier verrouData, pour etre sûre que pas de lecture en cours de la part du Java
    if( access( ".verrouData", F_OK ) == -1 )//Si fichier existe pas
    {
        FILE* verrou = fopen(".verrouData","w");//on créer le fichier verrou afin d'empecher toute lecture du fichier pendant la modification
        FILE* dataFile = fopen("Data.txt","w");//Ouverture de Data.txt
        fprintf(dataFile, "%d\t\n%d", myOxy.spo2, myOxy.pouls);//ecriture de la donnée dans Data.txt
        fclose(dataFile);//fermeture du fichier
        fclose(verrou);//fermeture du fichier
        remove(".verrouData");//Suppression du fichier verrou pour indiquer que fini d'écrire et possibilité de lecture pour le java
    }

}

