#ifndef CONSOMMATEUR_HANDLERS_H
#define CONSOMMATEUR_HANDLERS_H

#define _POSIX_C_SOURCE 199309L

#include <fcntl.h>
#include <signal.h>

extern pid_t pid_connexion;
extern int tubeTache;
extern int tubeResultat;

void handler_sigint(int);

void handler_rtmin_plus1(int, siginfo_t*, void*);

#endif /*CONSOMMATEUR_HANDLERS_H*/
