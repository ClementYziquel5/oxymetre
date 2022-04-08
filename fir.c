/*
	Fonction fir
	authors : Emilio Châtel, Clément Yziquel
	date : 06/04/2022
	description : cette fonction récupère en entrée la struct myAbsorb et un tableau rempli de 0
	et retourne la struct myAbsorp qui sera par la suite envoyée à la fonction iir
*/

#include "fir.h"
#include "fichiers.h"

absorp firTest(char *filename)
{
	int etat = 0; // La variable état est comparée à EOF afin de savoir quand le fichier est lu entièrement
	absorp myAbsorp;
	absorp myAbsorpOutput; // Déclaration de la variable de sortie
	absorp x[ORDRE_FILTRE] = {0};

	FILE *myFile = initFichier(filename);  // Ouverture du fichier passé en param de la fonction
	myAbsorp = lireFichier(myFile, &etat); // Lecture de la ligne 1

	do
	{
		myAbsorpOutput = fir(myAbsorp, x);	   // Traitement de acr et acir
		myAbsorp = lireFichier(myFile, &etat); // Lecture des lignes suivantes
	} while (etat != EOF);					   // Tant que le fichier n'est pas entièrement parcouru

	finFichier(myFile); // Fermeture du fichier

	return myAbsorpOutput; // Je renvoie la nouvelle absorp modifiée par fir
}

absorp fir(absorp myAbsorp, absorp *x)
{
	int i;
	float FIR_TAPS[51] = {
		1.4774946e-019, 1.6465231e-004, 3.8503956e-004, 7.0848037e-004, 1.1840522e-003, 1.8598621e-003, 2.7802151e-003, 3.9828263e-003, 5.4962252e-003, 7.3374938e-003, 9.5104679e-003, 1.2004510e-002, 1.4793934e-002, 1.7838135e-002, 2.1082435e-002, 2.4459630e-002, 2.7892178e-002, 3.1294938e-002, 3.4578348e-002, 3.7651889e-002, 4.0427695e-002, 4.2824111e-002, 4.4769071e-002, 4.6203098e-002, 4.7081811e-002, 4.7377805e-002, 4.7081811e-002, 4.6203098e-002, 4.4769071e-002, 4.2824111e-002, 4.0427695e-002, 3.7651889e-002, 3.4578348e-002, 3.1294938e-002, 2.7892178e-002, 2.4459630e-002, 2.1082435e-002, 1.7838135e-002, 1.4793934e-002, 1.2004510e-002, 9.5104679e-003, 7.3374938e-003, 5.4962252e-003, 3.9828263e-003, 2.7802151e-003, 1.8598621e-003, 1.1840522e-003, 7.0848037e-004, 3.8503956e-004, 1.6465231e-004, 1.4774946e-019};
	float y_acr = 0, y_acir = 0;

	// Mémorisation des L-1 derniers échantillons x(k) ainsi que l’échantillon courant x(n) (Décalage à droite pour insérer le nouvel élément)
	for (i = 0; i < ORDRE_FILTRE; i++)
	{
		x[ORDRE_FILTRE - i - 1] = x[ORDRE_FILTRE - i - 2];
	}
	x[0] = myAbsorp; // Insertion du nouvel élément

	// Remplissage de ACR,ACIR à partir de myAbsorb
	for (i = 0; i < ORDRE_FILTRE; i++)
	{
		y_acr += FIR_TAPS[i] * x[i].acr;
		y_acir += FIR_TAPS[i] * x[i].acir;
	}

	// On assigne les valeurs calculées dans la struct
	myAbsorp.acr = y_acr;
	myAbsorp.acir = y_acir;

	return myAbsorp; // On retourne la nnouvelle struct contenant les valeurs filtrées
}