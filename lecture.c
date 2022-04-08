/*
	Fonction lecture
	authors : Emilio Châtel, Clément Yziquel
	date : 06/04/2022
	description : cette fonction récupère en entrée le fichier "record1_bin.dat"
	et retourne la struct myAbsorp qui sera par la suite envoyée à la fonction fir
*/

#include "lecture.h"

absorp lecture(FILE *file_pf, int *file_state)
{
	char var = 1;
	int chiffre = 0;
	int compteur = 0;
	absorp myAbsorp;

	do
	{
		// récupération de la valeur USB
		var = fgetc(file_pf);
		// supression du premier chiffre 32 -> 2
		var &= (0x0F);
		// Première boucle qui sert à créer le chiffre qui est donné caractère par caractère par le fgetc.
		// Compteur%5 permet d'obtenir toujours une valeur entre 0 et 4 sachant que le compteur part de 0 et va jusqu'à 20
		switch (compteur % 5)
		{
		case 0:
			// milliers
			chiffre = chiffre + var * 1000;
			break;
		case 1:
			// centaines
			chiffre = chiffre + var * 100;
			break;
		case 2:
			// dizaine
			chiffre = chiffre + var * 10;
			break;
		case 3:
			// unité
			chiffre = chiffre + var;
			break;
		}
		// ajouts des chiffres à myAbsorp
		// On enlève 2048 à ACR et ACIR pour centrer les valeurs qui sont initialement entre 0 et 4096
		switch (compteur)
		{
		case 4:
			myAbsorp.acr = chiffre - 2048;
			chiffre = 0;
			break;
		case 9:
			myAbsorp.dcr = chiffre;
			chiffre = 0;
			break;
		case 14:
			myAbsorp.acir = chiffre - 2048;
			chiffre = 0;
			break;
		case 19:
			myAbsorp.dcir = chiffre;
			chiffre = 0;
			break;
		}
		// incrémentation du compteur
		compteur++;
	} while (var != 13 && !feof(file_pf));

	if (feof(file_pf))
	{					   // si fin fichier
		*file_state = EOF; // je change l'état à -1
	}
	else if (!feof(file_pf) && compteur == 21)
	{ // si bonne longueur + pas fin fichier
		return myAbsorp;
	}
	else
	{											 // si il n'y a pas la bonne longueur mais ce n'est pas la fin du fichier
		myAbsorp = lecture(file_pf, file_state); // on retourne la valeur suivante de myabsorb
		return myAbsorp;
	}
}
