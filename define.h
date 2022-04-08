#ifndef DEFINE_H
#define DEFINE_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define ORDRE_FILTRE 51

/*! ------------------------------------------------------------

  \struct absorp

  \brief measures

  ------------------------------------------------------------*/

typedef struct
{
  float acr;  /*!< AC R */
  float dcr;  /*!< DC R */
  float acir; /*!< AC IR */
  float dcir; /*!< DC IR */
} absorp;

/*! ------------------------------------------------------------

  \struct oxy

  \brief

  ------------------------------------------------------------*/

typedef struct
{
  int spo2;  /*!< SPO2 */
  int pouls; /*!< Pouls */
} oxy;

typedef struct
{
  float myAbsorp_x_acr;
  float myAbsorp_x_acir;
  float myAbsorp_y_acr;
  float myAbsorp_y_acir;
} param_iir;

typedef struct
{
  float val_min_acr;
  float val_max_acr;
  float val_min_acir;
  float val_max_acir;

  float val_min_acr_n_1;
  float val_max_acr_n_1;
  float val_min_acir_n_1;
  float val_max_acir_n_1;

  float tab_moy_BPM[10];
  float tab_moy_spo2[10];

  float acr_n_1;

  int compteur; // Compte le nombre de points par période
} param_mesure;

#endif // DEFINE_H
