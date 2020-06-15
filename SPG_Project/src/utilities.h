#pragma once
#include <queue>
#include <algorithm>
#include <time.h>
#include "data.h"
#include "UIManager/keyboard.h"
#include "UIManager/mouse.h"

template <typename T>
void clear_queue(std::queue<T>& q)
{
    std::queue<T> empty;
    std::swap(q, empty);
}

template <typename T>
bool list_contain_element(std::list<std::pair<T, T>>& list, T pair_item_1, T pair_item_2)
{
    return std::find(list.begin(), list.end(), std::make_pair(pair_item_1, pair_item_2)) != list.end();
}

//copie informatia tablei pentru a putea face undo()
void copy_array(struct square from[ROWS][COLUMNS], struct square to[ROWS][COLUMNS]) {
	int i, j;
	for (i = 0; i < ROWS; i++)
		for (j = 0; j < COLUMNS; j++)
			to[i][j] = from[i][j];
}

//returneaza 1 daca informatia de pe doua table este la fel, pentru undo()
int are_identic(struct square a[ROWS][COLUMNS], struct square b[ROWS][COLUMNS]) {
	int i, j;
	for (i = 0; i < ROWS; i++)
		for (j = 0; j < COLUMNS; j++)
			if (a[i][j].check != b[i][j].check || a[i][j].type != b[i][j].type)
				return 0;
	return 1;
}

//permite atirnarea programului pentru "Sec" secunde
void sleep(unsigned int sec) {
	clock_t ticks1 = clock(), ticks2 = ticks1;
	while ((ticks2 / CLOCKS_PER_SEC - ticks1 / CLOCKS_PER_SEC) < sec)
		ticks2 = clock();
}

//converteste coordonatele in locatie
std::pair<int, int> coords_to_index(int x, int y) {
	std::pair<int, int> location;
	for (location.first = 0; location.first < ROWS; location.first++)
		for (location.second = 0; location.second < COLUMNS; location.second++)
			if (x < board[location.first][location.second].x + 30 && x > board[location.first][location.second].x - 30 && y < board[location.first][location.second].y + 30 && y > board[location.first][location.second].y - 30)
				return location;
}

//initializarea tablei de dame din fisierul "joc.check"
void init_from_file() {
	FILE* in;
	fopen_s(&in, "joc.check", "r");
	if (!in) {
		glutDestroyWindow(WIN);
		exit(1);
	}

	fscanf_s(in, "%d %d %d %d %d %d %d %d %d %d", &uimanager::SIDE_COEF, &uimanager::MOUSEX, &uimanager::MOUSEY, &sel.first, &sel.second, &to.first, &to.second, &GO, &POS_MOVES, &HELP);

	int check, type;
	for (auto i = 0; i < ROWS; i++)
		for (auto j = 0; j < COLUMNS; j++) {
			fscanf_s(in, "%f %f %d %d", &board[i][j].x, &board[i][j].y, &check, &type);
			board[i][j].check = check;
			board[i][j].type = type;
		}

	fclose(in);
}


//functia de salvare in fisier
void save_to_file() {
	FILE* out;
	fopen_s(&out, "joc.check", "w");
	if (!out) {
		glutDestroyWindow(WIN);
		exit(1);
	}

	fprintf(out, "%d %d %d %d %d %d %d %d %d %d", uimanager::SIDE_COEF, uimanager::MOUSEX, uimanager::MOUSEY, sel.first, sel.second, to.first, to.second, GO, POS_MOVES, HELP);

	for (auto i = 0; i < ROWS; i++)
		for (auto j = 0; j < COLUMNS; j++)
			fprintf(out, "%f %f %d %d\n", board[i][j].x, board[i][j].y, board[i][j].check, board[i][j].type);

	fclose(out);
}