#pragma once

//Structura de data: lista de tip coada
typedef struct coada {
	int i;
	int j;
	struct coada* next;
} coada;



//prototipurilor functiilor de prelucrare a cozii
void addTomList(coada*& mListFirst, coada*& mListLast, int, int);
void delOnemList(coada*& mListFirst, coada*& mListLast);
void delAllmList(coada*& mListFirst, coada*& mListLast);
int isEmptymList(coada*& mListFirst, coada*& mListLast);


int isInList(coada*, int, int);

