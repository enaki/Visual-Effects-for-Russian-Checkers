#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>

//Structura de data: lista de tip coada
typedef struct coada {
	int i;
	int j;
	struct coada *next;
} coada;

//declararea listei de miscari, sarituri si verificare
coada *mListFirst, *mListLast;
coada *jListFirst, *jListLast;
coada *vListFirst, *vListLast;

//prototipurilor functiilor de prelucrare a cozii
void addTomList(int, int);
void delOnemList();
void delAllmList();
int isEmptymList();

void addTojList(int, int);
void delOnejList();
void delAlljList();
int isEmptyjList();

void addTovList(int, int);
void delOnevList();
void delAllvList();
int isEmptyvList();

int isInList(coada *, int, int); 

#endif
