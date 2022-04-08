/*
	Fonction iir
	authors : Emilio Châtel, Clément Yziquel
	date : 06/04/2022
	description : cette fonction récupère en entrée la struct myAbsorb sortie de fir
	et une struct param_iir_old précédement initialisé et retourne la struct myAbsorp
	qui sera par la suite envoyée à la fonction mesure
*/

#include "iir.h"
#include "fichiers.h"
#include <math.h>
#include <stdlib.h>

absorp iirTest(char *filename)
{
	int etat = 0; // La variable état est comparée à EOF afin de savoir quand le fichier est lu entièrement
	absorp myAbsorp;
	absorp myAbsorpOutput; // Déclaration de la variable de sortie
	param_iir param_iir_old = {0};

	FILE *myFile = initFichier(filename);  // Ouverture du fichier passé en param de la fonction
	myAbsorp = lireFichier(myFile, &etat); // Lecture de la ligne 1

	do
	{
		myAbsorpOutput = iir(myAbsorp, &param_iir_old); // Traitement de acr et acir
		myAbsorp = lireFichier(myFile, &etat);			// Lecture des lignes suivantes
	} while (etat != EOF);								// Tant que le fichier n'est pas entièrement parcouru

	finFichier(myFile); // Fermeture du fichier

	return myAbsorpOutput; // Je renvoie la nouvelle absorp modifiée par fir
}

absorp iir(absorp myAbsorp, param_iir *param_iir_old)
{
	float alpha = 0.992; // Constante donnée dan le sujet

	// Calcul de DCR,DCIR à partir de myAbsorb
	param_iir_old->myAbsorp_y_acr = myAbsorp.acr - param_iir_old->myAbsorp_x_acr + alpha * param_iir_old->myAbsorp_y_acr;
	param_iir_old->myAbsorp_y_acir = myAbsorp.acir - param_iir_old->myAbsorp_x_acir + alpha * param_iir_old->myAbsorp_y_acir;

	// Mémorisation des n-1 ACR et ACIR d'entrée
	param_iir_old->myAbsorp_x_acr = myAbsorp.acr;
	param_iir_old->myAbsorp_x_acir = myAbsorp.acir;

	// On assigne les valeurs de ACR et ACIR centrées dans la struct
	myAbsorp.acr = round(param_iir_old->myAbsorp_y_acr);
	myAbsorp.acir = round(param_iir_old->myAbsorp_y_acir);

	return myAbsorp; // On retourne la nnouvelle struct contenant les valeurs filtrées}
}