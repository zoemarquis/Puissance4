/**
 * @file test_p4.c
 * @author Zoé Marquis (zoe_marquis@ens.univ-artois.fr)
 * @author Enzo Nulli (enzo_nulli@ens.univ-artois.fr)
 * @brief Tests unitaires du fichier ia.
 * @version 0.1
 * @date 2023-01-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/puissance_quatre.h"
#include "test_p4.h"

/**
 * @brief Un pointeur sur un jeu.
 *
 */
Puissance4 *jeu;
/**
 * @brief Un pointeur sur une interface.
 *
 */
userInterface *ui;

/**
 * @brief Initialise la suiteBeginning: crée une structure de jeu et deux
 * joueurs.
 *
 * @return int un code d'erreur ou de succès
 */
int initSuiteBeginning(void) {
  jeu = initPuissance4();
  if (!jeu)
    return CUE_NOMEMORY;
  jeu->j1 = malloc(sizeof(Joueur));
  if (!jeu->j1)
    return CUE_NOMEMORY;
  jeu->j1->type = J1;
  jeu->j2 = malloc(sizeof(Joueur));
  if (!jeu->j2)
    return CUE_NOMEMORY;
  jeu->j2->type = J2;
  return CUE_SUCCESS;
}
/**
 * @brief Initialise une suite parmi les suites qui ne sont pas suiteBeginning:
 * crée une structure de jeu et deux joueurs et initialise une partie.
 *
 * @return int un code d'erreur ou de succès
 */
int initSuite(void) {
  jeu = initPuissance4();
  if (!jeu)
    return CUE_NOMEMORY;
  jeu->j1 = malloc(sizeof(Joueur));
  if (!jeu->j1)
    return CUE_NOMEMORY;
  jeu->j1->type = J1;
  jeu->j2 = malloc(sizeof(Joueur));
  if (!jeu->j2)
    return CUE_NOMEMORY;
  jeu->j2->type = J2;
  initGame(jeu);
  return CUE_SUCCESS;
}
/**
 * @brief Nettoie une suite : supprime le jeu.
 *
 * @return int un code de succès
 */
int cleanSuite(void) {
  clean(jeu, ui);
  return CUE_SUCCESS;
}

// suite Beginning :

/**
 * @brief Vérifie que la fonction initGame est fonctionnelle.
 *
 */
void test_initGame(void) {
  CU_ASSERT_PTR_NOT_NULL(jeu);
  initGame(jeu);
  CU_ASSERT_PTR_NOT_NULL(jeu->j1);
  CU_ASSERT_PTR_NOT_NULL(jeu->j2);
  CU_ASSERT_PTR_NOT_NULL(jeu->courant);
  CU_ASSERT_PTR_EQUAL(jeu->courant, jeu->j2);
  CU_ASSERT_PTR_NOT_EQUAL(jeu->j1, jeu->j2);
  CU_ASSERT_FALSE(jeu->rageQuit);
  CU_ASSERT_EQUAL(jeu->nb_jetons, 0);
  CU_ASSERT_EQUAL(jeu->j1->type, J1);
  CU_ASSERT_EQUAL(jeu->j2->type, J2);
  CU_ASSERT_EQUAL(jeu->courant->type, J2);
}
/**
 * @brief Vérifie que le plateau est vide. (Pour vérifier après d'autres tests
 * censés nettoyer le plateau)
 *
 */
void test_plateauVide(void) {
  for (int i = 0; i < NB_LIGNE; i++) {
    for (int j = 0; j < NB_COLONNE; j++) {
      CU_ASSERT_EQUAL(jeu->plateau[i][j], VIDE);
    }
  }
}
/**
 * @brief Vérifie que la fonction testColonne est fonctionnelle en début de
 * partie.
 *
 */
void test_descendDerniereLigne(void) {
  for (int i = 0; i < NB_COLONNE; i++) {
    CU_ASSERT_EQUAL(testColonne(jeu->plateau, i), NB_LIGNE - 1);
  }
}
/**
 * @brief Ajoute puis enlève deux jetons et vérifie que testColonne et
 * modifJeton sont fonctionnelles.
 *
 */
void test_ajoutEtSuppr2Pions(void) {
  modifJeton(jeu, NB_LIGNE - 1, 3, J1);
  CU_ASSERT_EQUAL(jeu->plateau[NB_LIGNE - 1][3], J1);
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 3), NB_LIGNE - 2);

  modifJeton(jeu, NB_LIGNE - 2, 3, J2);
  CU_ASSERT_EQUAL(jeu->plateau[NB_LIGNE - 2][3], J2);
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 3), NB_LIGNE - 3);

  modifJeton(jeu, NB_LIGNE - 2, 3, VIDE);
  CU_ASSERT_EQUAL(jeu->plateau[NB_LIGNE - 2][3], VIDE);
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 3), NB_LIGNE - 2);

  modifJeton(jeu, NB_LIGNE - 1, 3, VIDE);
  CU_ASSERT_EQUAL(jeu->plateau[NB_LIGNE - 1][3], VIDE);
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 3), NB_LIGNE - 1);
}
/**
 * @brief Vérifie que testAlign et testENdsont fonctionnelles, avec ajout d'un
 * seul jeton.
 *
 */
void test_alignement1Jeton(void) {
  jeu->plateau[NB_LIGNE - 1][4] = J1;
  CU_ASSERT_FALSE(testAlign(jeu->plateau, NB_LIGNE - 1, 4, 0, 1));
  CU_ASSERT_FALSE(testAlign(jeu->plateau, NB_LIGNE - 1, 4, 1, 0));
  CU_ASSERT_FALSE(testAlign(jeu->plateau, NB_LIGNE - 1, 4, 1, 1));
  CU_ASSERT_FALSE(testAlign(jeu->plateau, NB_LIGNE - 1, 4, 1, -1));
  CU_ASSERT_FALSE(testEnd(jeu, NB_LIGNE - 1, 4));
}

static CU_TestInfo test_array_Beginning[] = {
    {"vérifie que le jeu est bien initialisé", test_initGame},
    {"vérifie que le plateau est bien initialisé", test_plateauVide},
    {"vérifie que les pions descendent tout en bas de la grille",
     test_descendDerniereLigne},
    {"vérifie que les ajouts (et les suppressions) de pions fonctionnent et "
     "que les calculs qui en découlent sont bons",
     test_ajoutEtSuppr2Pions},
    {"vérifie qu'après les ajouts et suppressions le plateau est de nouveau "
     "à vide",
     test_plateauVide},
    {"ajoute un jeton et test ses alignements", test_alignement1Jeton},
    CU_TEST_INFO_NULL};

// suite FIN :
/*
plateau 1: égalité
  1   2   3   4   5   6   7
+---+---+---+---+---+---+---+
| O | O | X | O | O | X | X |
+---+---+---+---+---+---+---+
| O | X | O | X | O | O | X |
+---+---+---+---+---+---+---+
| O | X | O | X | O | X | O |
+---+---+---+---+---+---+---+
| X | O | X | O | X | O | X |
+---+---+---+---+---+---+---+
| X | O | X | O | X | O | X |
+---+---+---+---+---+---+---+
| X | O | X | O | X | O | X |
+---+---+---+---+---+---+---+
  1   2   3   4   5   6   7

plateau 2: gagner avec un alignement
  1   2   3   4   5   6   7
+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+
|   |   |   |   |   | X |   |
+---+---+---+---+---+---+---+
|   |   |   | O | X | X |   |
+---+---+---+---+---+---+---+
|   |   |   | X | X | O |   |
+---+---+---+---+---+---+---+
|   | O | X | O | O | X | O |
+---+---+---+---+---+---+---+
  1   2   3   4   5   6   7

plateau 3: gagner avec deux alignements
  1   2   3   4   5   6   7
+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+
|   |   |   | O |   |   |   |
+---+---+---+---+---+---+---+
|   |   |   | O | O | X |   |
+---+---+---+---+---+---+---+
|   |   |   | O | X | O |   |
+---+---+---+---+---+---+---+
| X | X | X | O | X | X | O |
+---+---+---+---+---+---+---+
  1   2   3   4   5   6   7

plateau 4 : gagner avec plateau plein
  1   2   3   4   5   6   7
+---+---+---+---+---+---+---+
| O | X | X | O | O | X | X |
+---+---+---+---+---+---+---+
| O | O | O | X | O | O | X |
+---+---+---+---+---+---+---+
| O | X | O | X | O | X | O |
+---+---+---+---+---+---+---+
| X | O | X | O | X | O | X |
+---+---+---+---+---+---+---+
| X | O | X | O | X | O | X |
+---+---+---+---+---+---+---+
| X | O | X | O | X | O | X |
+---+---+---+---+---+---+---+
  1   2   3   4   5   6   7
*/

/**
 * @brief Vérifie des fonctions du module puissance_quatre lorsqu'une partie se
 * termine par égalité.
 *
 */
void test_egalite(void) {
  // plateau 1
  modifJeton(jeu, 0, 0, J2);
  modifJeton(jeu, 0, 1, J2);
  modifJeton(jeu, 0, 2, J1);
  modifJeton(jeu, 0, 3, J2);
  modifJeton(jeu, 0, 4, J2);
  modifJeton(jeu, 0, 5, J1);
  modifJeton(jeu, 0, 6, J1);

  modifJeton(jeu, 1, 0, J2);
  modifJeton(jeu, 1, 1, J1);
  modifJeton(jeu, 1, 2, J2);
  modifJeton(jeu, 1, 3, J1);
  modifJeton(jeu, 1, 4, J2);
  modifJeton(jeu, 1, 5, J2);
  modifJeton(jeu, 1, 6, J1);

  modifJeton(jeu, 2, 0, J2);
  modifJeton(jeu, 2, 1, J1);
  modifJeton(jeu, 2, 2, J2);
  modifJeton(jeu, 2, 3, J1);
  modifJeton(jeu, 2, 4, J2);
  modifJeton(jeu, 2, 5, J1);
  modifJeton(jeu, 2, 6, J2);

  for (int i = 3; i < 6; i++) {
    modifJeton(jeu, i, 0, J1);
    modifJeton(jeu, i, 1, J2);
    modifJeton(jeu, i, 2, J1);
    modifJeton(jeu, i, 3, J2);
    modifJeton(jeu, i, 4, J1);
    modifJeton(jeu, i, 5, J2);
    modifJeton(jeu, i, 6, J1);
  }

  for (int j = 0; j < NB_COLONNE; j++) {
    CU_ASSERT_EQUAL(-1, testColonne(jeu->plateau, j));
  } // test de testColonne

  // dernière piéce posée en colonne 4
  CU_ASSERT_TRUE(testEnd(jeu, 0, 4));
  CU_ASSERT_PTR_NULL(jeu->courant);
}

/**
 * @brief Vérifie des fonctions du module puissance_quatre lorsqu'une partie se
 * termine par une victoire via 1 alignement de 4 jetons.
 *
 */
void test_gagner1Alignement(void) {
  initGame(jeu);
  // plateau 2
  modifJeton(jeu, 2, 5, J1);

  modifJeton(jeu, 3, 4, J1);
  modifJeton(jeu, 3, 5, J1);
  modifJeton(jeu, 3, 3, J2);

  modifJeton(jeu, 4, 3, J1);
  modifJeton(jeu, 4, 4, J1);
  modifJeton(jeu, 4, 5, J2);

  modifJeton(jeu, 5, 2, J1);
  modifJeton(jeu, 5, 5, J1);
  modifJeton(jeu, 5, 1, J2);
  modifJeton(jeu, 5, 3, J2);
  modifJeton(jeu, 5, 4, J2);
  modifJeton(jeu, 5, 6, J2);

  // test de testColonne
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 0), 5);
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 1), 4);
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 2), 4);
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 3), 2);
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 4), 2);
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 5), 1);
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 6), 4);

  // dernière pièce posée en colonne 5
  CU_ASSERT_TRUE(testEnd(jeu, 2, 5));
  CU_ASSERT_PTR_NOT_NULL(jeu->courant);
}

/**
 * @brief Vérifie des fonctions du module puissance_quatre lorsqu'une partie se
 * termine par une victoire via 2 alignements de 4 jetons.
 *
 */
void test_gagner2Alignements(void) {
  initGame(jeu);
  // plateau 3

  modifJeton(jeu, 2, 3, J2);

  modifJeton(jeu, 3, 5, J1);
  modifJeton(jeu, 3, 4, J2);
  modifJeton(jeu, 3, 3, J2);

  modifJeton(jeu, 4, 3, J2);
  modifJeton(jeu, 4, 4, J1);
  modifJeton(jeu, 4, 5, J2);

  modifJeton(jeu, 5, 0, J1);
  modifJeton(jeu, 5, 1, J1);
  modifJeton(jeu, 5, 2, J1);
  modifJeton(jeu, 5, 4, J1);
  modifJeton(jeu, 5, 5, J1);

  modifJeton(jeu, 5, 3, J2);
  modifJeton(jeu, 5, 6, J2);

  // test de testColonne
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 0), 4);
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 1), 4);
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 2), 4);
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 3), 1);
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 4), 2);
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 5), 2);
  CU_ASSERT_EQUAL(testColonne(jeu->plateau, 6), 4);

  CU_ASSERT_TRUE(testAlign(jeu->plateau, 2, 3, 1, 0));
  CU_ASSERT_TRUE(testAlign(jeu->plateau, 2, 3, 1, 1));
  CU_ASSERT_FALSE(testAlign(jeu->plateau, 2, 3, 0, 1));
  CU_ASSERT_FALSE(testAlign(jeu->plateau, 2, 3, -1, 1));

  // dernière pièce posée en colonne 3
  CU_ASSERT_TRUE(testEnd(jeu, 2, 3));
  CU_ASSERT_PTR_NOT_NULL(jeu->courant);
}

/**
 * @brief Vérifie des fonctions du module puissance_quatre lorsqu'une partie se
 * termine par une victoire via 1 alignement et le plateau est plein.
 *
 */
void test_gagnerPlateauPlein(void) {
  initGame(jeu);
  // plateau 4

  modifJeton(jeu, 0, 0, J2);
  modifJeton(jeu, 0, 1, J1);
  modifJeton(jeu, 0, 2, J1);
  modifJeton(jeu, 0, 3, J2);
  modifJeton(jeu, 0, 4, J2);
  modifJeton(jeu, 0, 5, J1);
  modifJeton(jeu, 0, 6, J1);

  modifJeton(jeu, 1, 0, J2);
  modifJeton(jeu, 1, 1, J2);
  modifJeton(jeu, 1, 2, J2);
  modifJeton(jeu, 1, 3, J1);
  modifJeton(jeu, 1, 4, J2);
  modifJeton(jeu, 1, 5, J2);
  modifJeton(jeu, 1, 6, J1);

  modifJeton(jeu, 2, 0, J2);
  modifJeton(jeu, 2, 1, J1);
  modifJeton(jeu, 2, 2, J2);
  modifJeton(jeu, 2, 3, J1);
  modifJeton(jeu, 2, 4, J2);
  modifJeton(jeu, 2, 5, J1);
  modifJeton(jeu, 2, 6, J2);

  for (int i = 3; i < 6; i++) {
    modifJeton(jeu, i, 0, J1);
    modifJeton(jeu, i, 1, J2);
    modifJeton(jeu, i, 2, J1);
    modifJeton(jeu, i, 3, J2);
    modifJeton(jeu, i, 4, J1);
    modifJeton(jeu, i, 5, J2);
    modifJeton(jeu, i, 6, J1);
  }

  for (int j = 0; j < NB_COLONNE; j++) {
    CU_ASSERT_EQUAL(-1, testColonne(jeu->plateau, j));
  } // test de testColonne

  // dernière piéce posée en colonne 0
  CU_ASSERT_TRUE(testEnd(jeu, 0, 0));
  CU_ASSERT_PTR_NOT_NULL(jeu->courant);
}

static CU_TestInfo test_array_Fin[] = {
    {"vérifie toutes les priorités liées à une partie à égalité", test_egalite},
    {"vérifie toutes les priorités liées à une partie gagnée avec un "
     "alignement de 4 jetons",
     test_gagner1Alignement},
    {"vérifie toutes les priorités liées à une partie gagnée avec deux "
     "alignements de 4 jetons",
     test_gagner2Alignements},
    {"vérifie toutes les priorités liées à une partie gagnée avec le plateau "
     "complètement rempli",
     test_gagnerPlateauPlein},
    CU_TEST_INFO_NULL};

static CU_SuiteInfo suites[3] = {
    {"suiteBeginning", initSuiteBeginning, cleanSuite, NULL, NULL,
     test_array_Beginning},
    {"suiteEnd", initSuite, cleanSuite, NULL, NULL, test_array_Fin},
    CU_SUITE_INFO_NULL};

/**
 * @brief Get the Test P4 Suites object
 *
 * @return CU_SuiteInfo* un tableau de suites de tests
 */
CU_SuiteInfo *getTestP4Suites() { return suites; }