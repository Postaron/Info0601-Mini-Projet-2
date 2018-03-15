#include <signal.h>
#include <stdbool.h>

#ifndef MINI_PROJET_2_HANDLERS_H
#define MINI_PROJET_2_HANDLERS_H

extern int N;
extern int X;
extern int nbrConnected;
extern pid_t* pidTabCo;
extern bool bool_tache;

void handler_sigint(int);

void handler_connexion(int, siginfo_t*, void*);

void handler_connexion_int(int, siginfo_t*, void*);

void handler_tache(int);

#endif /*MINI_PROJET_2_HANDLERS_H*/
