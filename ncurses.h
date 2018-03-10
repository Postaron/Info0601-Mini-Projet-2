#ifndef NCURSES_H_
#define NCURSES_H_

void ncurses_initialiser(void);
void ncurses_stopper(void);
void ncurses_couleurs(void);
void ncurses_souris(void);
int souris_getpos(int *x, int *y, int *bouton);

#endif /* NCURSES_H_ */
