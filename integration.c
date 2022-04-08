/*
    Fonction integration
    authors : Emilio Châtel, Clément Yziquel
    date : 06/04/2022
    description : cette fonction fait appel aux fonctions lireFichier, fir, iir, mesure et affichage
    Elle est appelée dans le main en passant en paramètre le fichier à lire afin de mettre en cascade
    les fonctions précédemment citées
*/

#include "define.h"
#include "fichiers.h"
#include "fir.h"
#include "iir.h"
#include "integration.h"
#include "lecture.h"
#include "mesure.h"
#include "affichage.h"

void integrationTest(char *filename)
{
    // Initialisation de tous les paramètres nécessaires au bon fonctionnement des fonctions appelées dans l'intégration
    int etat = 0;
    absorp myAbsorpTemp = {0}, myAbsorpFinal = {0};
    oxy myOxy = {0};
    absorp x[ORDRE_FILTRE] = {0};
    param_iir param_iir_old = {0};
    param_mesure mes = {0};

    FILE *myFile = initFichier(filename); // Ouverture du fichier

    myAbsorpTemp = lireFichier(myFile, &etat); // Lecture de la première ligne

    do
    {
        myAbsorpFinal = fir(myAbsorpTemp, x);
        myAbsorpFinal = iir(myAbsorpFinal, &param_iir_old);
        myOxy = mesure(myAbsorpFinal, myOxy, &mes); // Appel de la fonction mesure qui calcule les valeurs de pouls et spo2
        affichage(myOxy);                           // Appel de la fonction afichage pour mettre à jour l'interface java
        myAbsorpTemp = lireFichier(myFile, &etat);  // Lecture des lignes suivantes
    } while (etat != EOF);                          // Attente du retour de la fonction lecture

    finFichier(myFile); // Fermeture du fichier
}