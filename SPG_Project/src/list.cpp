#include <stdlib.h>
#include <stdio.h>
#include "list.hpp"

//functiile de prelucrare a cozii cu miscari
void addTomList(int i, int j){
	coada *t;
	t = (coada*)malloc(sizeof(coada));
	t->i = i;
	t->j = j;
	t->next = NULL;

	if(!mListFirst){
		mListFirst = t;
		mListLast = t;
	} else {
		mListLast->next = t;
		mListLast = t;
	}
}

void delOnemList(){
	coada *firstTemp;
	firstTemp = mListFirst;
	mListFirst = mListFirst->next;
	if(!mListFirst) mListLast = mListFirst;

	free(firstTemp);
}

void delAllmList(){
	while(mListFirst != NULL)
		delOnemList();
}

int isEmptymList(){
	if(!mListFirst && !mListLast) return 1;
	return 0;
}

//functiile de prelucrare a listei cu sarituri
void addTojList(int i, int j){
	coada *t;
	t = (coada*)malloc(sizeof(coada));
	t->i = i;
	t->j = j;
	t->next = NULL;

	if(!jListFirst){
		jListFirst = t;
		jListLast = t;
	} else {
		jListLast->next = t;
		jListLast = t;
	}
}

void delOnejList(){
	coada *firstTemp;
	firstTemp = jListFirst;
	jListFirst = jListFirst->next;
	if(!jListFirst) jListLast = jListFirst;

	free(firstTemp);
}

void delAlljList(){
	while(jListFirst != NULL)
		delOnejList();
}

int isEmptyjList(){
	if(!jListFirst && !jListLast) return 1;
	return 0;
}

//functiile de prelucrare a cozii cu verificari
void addTovList(int i, int j){
	coada *t;
	t = (coada*)malloc(sizeof(coada));
	t->i = i;
	t->j = j;
	t->next = NULL;

	if(!vListFirst){
		vListFirst = t;
		vListLast = t;
	} else {
		vListLast->next = t;
		vListLast = t;
	}
}

void delOnevList(){
	coada *firstTemp;
	firstTemp = vListFirst;
	vListFirst = vListFirst->next;
	if(!vListFirst) vListLast = vListFirst;

	free(firstTemp);
}

void delAllvList(){
	while(vListFirst != NULL)
		delOnevList();
}
int isEmptyvList(){
	if(!vListFirst && !vListLast) return 1;
	return 0;
}

//returneaza 1 daca piesa este in lista
int isInList(coada *list, int i, int j){
	while(list != NULL){
		if(list->i == i && list->j == j)
			return 1;
		list = list->next;
	}
	return 0;
}
