#pragma once
#include <queue>
#include <algorithm>
#include "data.hpp"
#include "UIManager/keyboard.hpp"
#include "UIManager/mouse.hpp"

template <typename T>
void clearQueue(std::queue<T>& q)
{
    std::queue<T> empty;
    std::swap(q, empty);
}

template <typename T>
bool listContainElement(std::list<std::pair<T, T>>& list, T pair_item_1, T pair_item_2)
{
    return std::find(list.begin(), list.end(), std::make_pair(pair_item_1, pair_item_2)) != list.end();
}

//copie informatia tablei pentru a putea face undo()
void copyArray(struct square from[ROWS][COLUMNS], struct square to[ROWS][COLUMNS]) {
	int i, j;
	for (i = 0; i < ROWS; i++)
		for (j = 0; j < COLUMNS; j++)
			to[i][j] = from[i][j];
}

//returneaza 1 daca informatia de pe doua table este la fel, pentru undo()
int areIdentic(struct square a[ROWS][COLUMNS], struct square b[ROWS][COLUMNS]) {
	int i, j;
	for (i = 0; i < ROWS; i++)
		for (j = 0; j < COLUMNS; j++)
			if (a[i][j].check != b[i][j].check || a[i][j].type != b[i][j].type)
				return 0;
	return 1;
}

//permite atirnarea programului pentru "Sec" secunde
void sleep(unsigned int Sec) {
	clock_t ticks1 = clock(), ticks2 = ticks1;
	while ((ticks2 / CLOCKS_PER_SEC - ticks1 / CLOCKS_PER_SEC) < Sec)
		ticks2 = clock();
}

//converteste coordonatele in locatie
std::pair<int, int> coordsToIndex(int x, int y) {
	std::pair<int, int> location;
	for (location.first = 0; location.first < ROWS; location.first++)
		for (location.second = 0; location.second < COLUMNS; location.second++)
			if (x < _board[location.first][location.second].x + 30 && x > _board[location.first][location.second].x - 30 && y < _board[location.first][location.second].y + 30 && y > _board[location.first][location.second].y - 30)
				return location;
}

//initializarea tablei de dame din fisierul "joc.check"
void initFromFile() {
	FILE* in;
	fopen_s(&in, "joc.check", "r");
	if (!in) {
		glutDestroyWindow(WIN);
		exit(1);
	}

	fscanf_s(in, "%d %d %d %d %d %d %d %d %d %d", &uimanager::SIDE_COEF, &uimanager::MOUSEX, &uimanager::MOUSEY, &sel.first, &sel.second, &to.first, &to.second, &GO, &POS_MOVES, &HELP);

	int i, j, check, type;
	for (i = 0; i < ROWS; i++)
		for (j = 0; j < COLUMNS; j++) {
			fscanf_s(in, "%f %f %d %d", &_board[i][j].x, &_board[i][j].y, &check, &type);
			_board[i][j].check = check;
			_board[i][j].type = type;
		}

	fclose(in);
}


//functia de salvare in fisier
void saveToFile() {
	FILE* out;
	fopen_s(&out, "joc.check", "w");
	if (!out) {
		glutDestroyWindow(WIN);
		exit(1);
	}

	fprintf(out, "%d %d %d %d %d %d %d %d %d %d", uimanager::SIDE_COEF, uimanager::MOUSEX, uimanager::MOUSEY, sel.first, sel.second, to.first, to.second, GO, POS_MOVES, HELP);

	int i, j;
	for (i = 0; i < ROWS; i++)
		for (j = 0; j < COLUMNS; j++)
			fprintf(out, "%f %f %d %d\n", _board[i][j].x, _board[i][j].y, _board[i][j].check, _board[i][j].type);

	fclose(out);
}