#include "lecture.h"
#include "fichiers.h"

// si on a une tram erronée, cette fonction permet de faire avancer le curseur jusqu'à la prochaine ligne du fichier (nouvelle valeur)
void verifTrame(FILE* file_pf){
    char c = fgetc(file_pf); // On utilise la fonction fgetc pour récupérer caractère par caractère et ainsi décaler le curseur
    while(c != '\n'){//on avance le curseur du fichier jusqu'à la prochaine ligne (supposée correcte) car \n\n a la fin
        c = fgetc(file_pf);
    }
    c = fgetc(file_pf);//On déplace le curseur de 1 pour le mettre au début de la prochaine ligne
}

absorp lecture(FILE* file_pf, int* file_state){//permet de lire le fichier et s'assure de retourner une tram valide
	absorp myAbsorp;
    char x,y;
    int a=0,b=0,c=0,d=0;
    *file_state = fscanf(file_pf,"%d,%d,%d,%d%c%c",&a,&b,&c,&d,&x,&y);//Récupère les données, avec un format bien précis et les stocks dans des variables
    while(*file_state < 6 && *file_state != EOF){ //Vérifie si on a bien récupéré les 6 variables avec le fscanf et qu'on n'a pas une trame erronée
    //Le != EOF permet de contrer le pb lié a veriftram (si le curseur arrive a la fin)
        verifTrame(file_pf); //permet de se rendre à la prochaine tram (prochaine mesure) si celle ci est erronée
        *file_state = fscanf(file_pf,"%d,%d,%d,%d%c%c",&a,&b,&c,&d,&x,&y); //Après avoir décalé le pointeur, on refait un fscanf
        //Via le while on re vérifie que cette tram est elle aussi bonne
    }
    //En sortie de while on a forcément une tram bonne, on peut ainsi récupérer les valeurs et les stockés dans la structure
    if(*file_state!=EOF){ //Vérifie si ce n'est pas la fin du fichier

        myAbsorp.acr=a-2048; //On effectue le recadrage à 0 sur les valeurs acr et acir
        myAbsorp.dcr=b;
        myAbsorp.acir=c-2048;
        myAbsorp.dcir=d;

    }
	return myAbsorp; // return EOF flag (à la fin du fichier)

}

