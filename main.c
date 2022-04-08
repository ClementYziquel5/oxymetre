/*
    Fonction main
    authors : Emilio Châtel, Clément Yziquel
    date : 06/04/2022
    description : cette fonction appelle les fonctions de test
*/

#include "define.h"
#include "fichiers.h"
#include "fir.h"
#include "iir.h"
#include "integration.h"
#include "lecture.h"
#include "mesure.h"
#include "affichage.h"
#include <unistd.h>

int main()
{
    firTest("record1.dat");
    iirTest("record1_fir.dat");
    mesureTest("record1_iir.dat");
    integrationTest("record1_bin.dat");

    return EXIT_SUCCESS;
}