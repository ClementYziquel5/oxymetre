/*
    Fonction affichage
    authors : Emilio Châtel, Clément Yziquel
    date : 06/04/2022
    description : cette fonction récupère en entrée la struct myOxy en sortie de mesure
    et écrit dans le fichier data.txt lu par l'interface graphique java
*/

#include "affichage.h"

void affichage(oxy myOxy)
{
    // initialisation de la valeur fichier qui va contenir le nom du fichier
    if (!(access(".verrouData", F_OK) != -1))
    {
        fopen(".verrouData", "r");

        FILE *fichier = NULL;
        // attribution du nom du fichier à fichier + ouverture du fichier
        // (suppression de ce qu'il y avait avant dans le fichier "w")
        fichier = fopen("Data.txt", "w");
        // affichage première valeur
        fprintf(fichier, "%d", myOxy.spo2);
        // affichage deuxième valeur
        fprintf(fichier, "\n%d", myOxy.pouls);
        // fermeture fichier
        fclose(fichier);

        remove(".verrouData");
    }
}
