#include <bits/types/siginfo_t.h>

#ifndef MINI_PROJET_2_HANDLERS_H
#define MINI_PROJET_2_HANDLERS_H

extern int N;
extern int X;
extern int nbrConnected;

void handler_sigint(int);

void handler_connexion(int, siginfo_t*, void*);

#endif /*MINI_PROJET_2_HANDLERS_H*/
