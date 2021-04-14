#include "define.h"

void verifTrame(FILE* file_pf);//si on a une tram erronée, cette fonction permet de faire avancer le curseur jusqu'à la prochaine ligne du fichier (nouvelle valeur)
absorp lecture(FILE* file_pf, int* file_state);//permet de lire le fichier et s'assure de retourner une tram valide
