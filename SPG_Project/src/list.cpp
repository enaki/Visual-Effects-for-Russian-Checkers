#include <stdlib.h>
#include <stdio.h>
#include "list.hpp"

//functiile de prelucrare a cozii cu miscari
void addTomList(coada*& mListFirst, coada*& mListLast, int i, int j) {
	coada* t;
	t = (coada*)malloc(sizeof(coada));
	t->i = i;
	t->j = j;
	t->next = NULL;

	if (!mListFirst) {
		mListFirst = t;
		mListLast = t;
	}
	else {
		mListLast->next = t;
		mListLast = t;
	}
}

void delOnemList(coada*& mListFirst, coada*& mListLast) {
	coada* firstTemp;
	firstTemp = mListFirst;
	mListFirst = mListFirst->next;
	if (!mListFirst) mListLast = mListFirst;

	free(firstTemp);
}

void delAllmList(coada*& mListFirst, coada*& mListLast) {
	while (mListFirst != NULL)
		delOnemList(mListFirst, mListLast);
}

int isEmptymList(coada*& mListFirst, coada*& mListLast) {
	if (!mListFirst && !mListLast) return 1;
	return 0;
}


//returneaza 1 daca piesa este in lista
int isInList(coada* list, int i, int j) {
	while (list != NULL) {
		if (list->i == i && list->j == j)
			return 1;
		list = list->next;
	}
	return 0;
}
