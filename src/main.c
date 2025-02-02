/**
 * @file main.c
 * @author Zoé Marquis (zoe_marquis@ens.univ-artois.fr)
 * @author Enzo Nulli (enzo_nulli@ens.univ-artois.fr)
 * @brief Le fichier main du jeu puissance quatre.
 * @version 0.1
 * @date 2022-12-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "choixModes.h"
#include "console.h"
#include "graphique.h"
#include "ia.h"
#include "puissance_quatre.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Fonction principale du jeu du puissance 4.
 *
 * @return int EXIT_SUCCESS si tout s'est bien passé, EXIT_FAILURE en cas de
 * problème
 */
int main() {
  char interface, mode, niveau, niveau2;
  printf("\e[1;1H\e[2J");
  printf("PUISSANCE QUATRE\n");

  choix_interface(&interface);
  choix_mode(&mode);
  if (mode == 'i' || mode == 'a') {
    choix_niveau(&niveau);
  } else if (mode == 's') {
    choix_niveaux(&niveau, &niveau2);
  }

  userInterface *ui = NULL;
  Puissance4 *game = initPuissance4();
  if (!game)
    goto Quitter;

  if (interface == 'c') {
    ui = makeConsole();
    if (!ui)
      goto Quitter;

    if (mode == 'h') {
      game->j1 = makeHumainConsole(J1);
      game->j2 = makeHumainConsole(J2);
    } else if (mode == 'a') {
      game->j1 = makeHumainConsole(J1);
      game->j2 = makeIA(J2, niveau);
    } else if (mode == 'i') {
      game->j1 = makeIA(J1, niveau);
      game->j2 = makeHumainConsole(J2);
    } else if (mode == 's') {
      game->j1 = makeIA(J1, niveau);
      game->j2 = makeIA(J2, niveau2);
    } else {
      perror("Erreur inopinée !");
      goto Quitter;
    }
  } else if (interface == 'g') {
    ui = makeGraphique(game);
    if (!ui)
      goto Quitter;

    if (mode == 'h') {
      game->j1 = makeHumainGraphique(J1);
      game->j2 = makeHumainGraphique(J2);
    } else if (mode == 'a') {
      game->j1 = makeHumainGraphique(J1);
      game->j2 = makeIA(J2, niveau);
    } else if (mode == 'i') {
      game->j1 = makeIA(J1, niveau);
      game->j2 = makeHumainGraphique(J2);
    } else if (mode == 's') {
      game->j1 = makeIA(J1, niveau);
      game->j2 = makeIA(J2, niveau2);
    } else {
      perror("Erreur inopinée !");
      goto Quitter;
    }
  } else {
    perror("Erreur inopinée !");
    goto Quitter;
  }
  if (!game->j1 || !game->j2)
    goto Quitter;

  launchGame(game, ui);
  if (game->rageQuit)
    goto Quitter;

  clean(game, ui);
  return EXIT_SUCCESS;

Quitter:
  clean(game, ui);
  return EXIT_FAILURE;
}