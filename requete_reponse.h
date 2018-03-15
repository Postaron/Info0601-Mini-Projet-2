#ifndef MINI_PROJET_2_REQUETE_REPONSE_H
#define MINI_PROJET_2_REQUETE_REPONSE_H

#include <fcntl.h>

typedef struct {
	pid_t pid_processus; /* permet de déterminer si c'est une connexion ou un calcul (comparaison) */
	int connexion; /* 1 : connexion, 0 : deconnexion */
	double reponse_calcul;
} reponse_t;

#endif /*MINI_PROJET_2_REQUETE_REPONSE_H*/
