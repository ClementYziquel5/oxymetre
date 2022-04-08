/*
	Fonction mesure
	authors : Emilio Châtel, Clément Yziquel
	date : 06/04/2022
	description : cette fonction récupère en entrée la struct myAbsorb sortie de iir
	et retourne la struct myOxy qui sera récupérée par l'affichage
*/

#include "mesure.h"
#include "fichiers.h"
#include <stdlib.h>
#include "affichage.h"
#include <unistd.h>

oxy mesureTest(char *filename)
{
	oxy myOxy = {0};
	absorp myAbsorp;
	param_mesure mes = {0};
	int i;

	for (i = 0; i < 10; i++) // Mise à 0 du tableau de moyenne des BPM et de spo2
	{
		mes.tab_moy_BPM[i] = 0;
		mes.tab_moy_spo2[i] = 0;
	}

	int etat = 0;						  // Variable état comparée à EOF
	FILE *myFile = initFichier(filename); // Ouverture du fichier

	myAbsorp = lireFichier(myFile, &etat); // Lecture de la première ligne pour le premier passage dans le do while
	do
	{
		myOxy = mesure(myAbsorp, myOxy, &mes); // Appel de la fonction mesure pour le calcul de spo2 et de pouls
		myAbsorp = lireFichier(myFile, &etat); // Lecture des lignes suivantes du fichier
	} while (etat != EOF);					   // Tant que le fichier n'est pas lu complètement

	finFichier(myFile); // Fermeture du fichier
	return myOxy;		// Renvoie la srtuct contenant spo2 et pouls
}

oxy mesure(absorp myAbsorp, oxy myOxy, param_mesure *param_mes)
{
	float ratio, BPM = 0, spo2 = 0;
	int i;

	// Je stocke les valeurs min et max de ACR dans une struct en paramètre d'entrée-sortie
	if (myAbsorp.acr < param_mes->val_min_acr)
	{
		param_mes->val_min_acr = myAbsorp.acr;
	}
	else if (myAbsorp.acr > param_mes->val_max_acr)
	{
		param_mes->val_max_acr = myAbsorp.acr;
	}

	// Je stocke les valeurs min et max de ACIR dans une struct en paramètre d'entrée-sortie
	if (myAbsorp.acir < param_mes->val_min_acir)
	{
		param_mes->val_min_acir = myAbsorp.acir;
	}
	else if (myAbsorp.acir > param_mes->val_max_acir)
	{
		param_mes->val_max_acir = myAbsorp.acir;
	}

	/******* Je moyenne les valeurs min et max de AC(I)R et DC(I)R *******/
	// Je calcule les moyennes
	/*
	param_mes->val_min_acr = (param_mes->val_min_acr + param_mes->val_min_acr_n_1) / 2;
	param_mes->val_max_acr = (param_mes->val_max_acr + param_mes->val_max_acr_n_1) / 2;
	param_mes->val_min_acir = (param_mes->val_min_acir + param_mes->val_min_acir_n_1) / 2;
	param_mes->val_max_acir = (param_mes->val_max_acir + param_mes->val_max_acir_n_1) / 2;

	// Je stocke les valeurs des n-1 pour calculer la moyenne sur 2 périodes
	param_mes->val_min_acr_n_1 = param_mes->val_min_acr;
	param_mes->val_max_acr_n_1 = param_mes->val_max_acr;
	param_mes->val_min_acir_n_1 = param_mes->val_min_acir;
	param_mes->val_max_acir_n_1 = param_mes->val_max_acir;
	*/

	// Je parcours une seule période
	if (myAbsorp.acr * param_mes->acr_n_1 < 0 && param_mes->acr_n_1 < 0 && myAbsorp.dcr != 0 && myAbsorp.dcir != 0) // Je détecte les fronts montants
	{

		// Calcul de spo2 qui correspond à l'oxygénation du sang
		ratio = ((param_mes->val_max_acr - param_mes->val_min_acr) / myAbsorp.dcr) / ((param_mes->val_max_acir - param_mes->val_min_acir) / myAbsorp.dcir);
		if (ratio <= 1 && ratio > 0.4)
		{
			myOxy.spo2 = 110 + ratio * -25;
		}

		else if (ratio > 1 && ratio < 3.4)
		{
			myOxy.spo2 = 120.394 + ratio * -35.41;
		}

		// Calcul du pouls
		myOxy.pouls = (int)(60 / (param_mes->compteur * 0.002));

		// Décallage du tableau pour garder en mémoire les 10 dernières valeurs
		// Calcul de la moyenne des BPM
		spo2 = 0;
		for (i = 1; i > 0; i--)
		{
			param_mes->tab_moy_spo2[i] = param_mes->tab_moy_spo2[i - 1];
			param_mes->tab_moy_BPM[i] = param_mes->tab_moy_BPM[i - 1];

			spo2 += param_mes->tab_moy_spo2[i];
			BPM += param_mes->tab_moy_BPM[i];
		}
		param_mes->tab_moy_BPM[0] = myOxy.pouls; // Insertion du nouvel élément
		param_mes->tab_moy_spo2[0] = myOxy.spo2; // Insertion du nouvel élément

		myOxy.pouls = (int)((BPM + myOxy.pouls) / 2);
		// myOxy.spo2 = (int)((spo2 + myOxy.spo2) / 2); // Calcul de la moyenne de spo2 sur les 10 dernières périodes

		// printf("BPM=%d,  spo2= %d\n", myOxy.pouls, myOxy.spo2);

		*param_mes = reset_mesure(*param_mes); // Reset des valeurs
	}
	param_mes->acr_n_1 = myAbsorp.acr; // Mise en mémoire de ACR n-1
	param_mes->compteur++;			   // Compte le nombre de points par période

	// usleep(2000);
	printf("spo2=%d   BPM=%d\n", myOxy.spo2, myOxy.pouls);

	//  Je renvoie la struct contenant les valeurs de spo2 et de pouls mises à jour
	return myOxy;
}

// Cette fonction est appellée pour reset les valeurs d'une struct param_mesure
param_mesure reset_mesure(param_mesure mes)
{
	mes.val_max_acr = 0;
	mes.val_min_acr = 0;
	mes.val_max_acir = 0;
	mes.val_min_acir = 0;
	mes.val_min_acr_n_1 = 0;
	mes.val_max_acr_n_1 = 0;
	mes.val_min_acir_n_1 = 0;
	mes.val_max_acir_n_1 = 0;
	mes.acr_n_1 = 0;
	mes.compteur = 0;
	return mes;
}