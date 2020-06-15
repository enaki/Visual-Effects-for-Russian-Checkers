#pragma once
#include <list>
/*
  			<--  0 --><--  1  --><--  2 --><--  3 -->
  			|		  |			|		  |			|
************+----+----+----+----+----+----+----+----+
			|    |####|    |####|    |####|    |####|
0			|    |####|    |####|    |####|    |####|
************+----+----+----+----+----+----+----+----+
			|####|    |####|    |####|    |####|    |
1			|####|    |####|    |####|    |####|    |
************+----+----+----+----+----+----+----+----+
			|    |####|    |####|    |####|    |####|
2			|    |####|    |####|    |####|    |####|
************+----+----+----+----+----+----+----+----+
			|####|    |####|    |####|    |####|    |
3			|####|    |####|    |####|    |####|    |
************+----+----+----+----+----+----+----+----+
			|    |####|    |####|    |####|    |####|
4			|    |####|    |####|    |####|    |####|
************+----+----+----+----+----+----+----+----+
			|####|    |####|    |####|    |####|    |
5			|####|    |####|    |####|    |####|    |
************+----+----+----+----+----+----+----+----+
			|    |####|    |####|    |####|    |####|
6			|    |####|    |####|    |####|    |####|
************+----+----+----+----+----+----+----+----+
			|####|    |####|    |####|    |####|    |
7			|####|    |####|    |####|    |####|    |
************+----+----+----+----+----+----+----+----+

 */
#include "utilities.h"


int count_checkers(int);
bool move_is_legal(int);
void list_of_jumpes(std::list<std::pair<int, int>>& jump_list, std::list<std::pair<int, int>>& check_list);
void list_of_moves(std::list<std::pair<int, int>>& move_list);
bool no_more_moves(unsigned int);


//check possible moves of pieces
void list_of_moves(std::list<std::pair<int, int>>& move_list) {
	for (auto i = 0; i < ROWS; i++) {
		for (auto j = 0; j < COLUMNS; j++) {
			if (board[i][j].check != NO_CHECKER) {
				int k;
				if (i % 2)
					k = j + 1;
				else
					k = j - 1;
				if (board[i][j].type == KING) {
					if ((board[i + 1][j].check == NO_CHECKER && i + 1 < ROWS) ||
						(board[i - 1][j].check == NO_CHECKER && i - 1 >= 0) ||
						(board[i + 1][k].check == NO_CHECKER && i + 1 < ROWS && k < COLUMNS && k >= 0) ||
						(board[i - 1][k].check == NO_CHECKER && i - 1 >= 0 && k < COLUMNS && k >= 0)) {
						move_list.emplace_back(i, j);
					}
				}
				else if (board[i][j].type == CHECKER) {
					if ((board[i + 1][j].check == NO_CHECKER && i + 1 < ROWS && board[i][j].check == WHITE_CHECKER) ||
						(board[i - 1][j].check == NO_CHECKER && i - 1 >= 0 && board[i][j].check == BLACK_CHECKER) ||
						(board[i + 1][k].check == NO_CHECKER && i + 1 < ROWS && k < COLUMNS && k >= 0 && board[i][j].check == WHITE_CHECKER) ||
						(board[i - 1][k].check == NO_CHECKER && i - 1 >= 0 && k < COLUMNS && k >= 0 && board[i][j].check == BLACK_CHECKER)) {
						move_list.emplace_back(i, j);
					}
				}
			}
		} 
	} 
}

//check if possible jumpes
void list_of_jumpes(std::list<std::pair<int, int>>& jump_list, std::list<std::pair<int, int>>& check_list) {
	for (auto i = 0; i < ROWS; i++) {
		for (auto j = 0; j < COLUMNS; j++) {
			if (board[i][j].check != NO_CHECKER) {
				int k, s;
				if (i % 2) {
					k = j + 1;
					s = j - 1;
				}
				else {
					k = j - 1;
					s = j + 1;
				}

				if (board[i][j].type == CHECKER) {
					if ((board[i + 1][j].check != NO_CHECKER && board[i + 1][j].check != board[i][j].check && board[i + 2][s].check == NO_CHECKER && i + 2 < ROWS && s < COLUMNS && s >= 0) ||
						(board[i - 1][j].check != NO_CHECKER && board[i - 1][j].check != board[i][j].check && board[i - 2][s].check == NO_CHECKER && i - 2 >= 0 && s < COLUMNS && s >= 0) ||
						(board[i + 1][k].check != NO_CHECKER && board[i + 1][k].check != board[i][j].check && board[i + 2][k].check == NO_CHECKER && i + 2 < ROWS && k < COLUMNS && k >= 0) ||
						(board[i - 1][k].check != NO_CHECKER && board[i - 1][k].check != board[i][j].check && board[i - 2][k].check == NO_CHECKER && i - 2 >= 0 && k < COLUMNS && k >= 0)) {
						if (board[i][j].check == GO)
							jump_list.emplace_back(i, j);
						check_list.emplace_back(i, j);
					}
				}
				else if (board[i][j].type == KING) {
					auto count = 0;
					if (i % 2) {
						k = i + 1;
						s = j + 1;
					}
					else {
						k = i;
						s = j;
					}
					auto need = 0;
					//dreapta sus
					for (; s < COLUMNS; k++) {
						++need;

						if (k == i && s == j)
							continue;

						//if found piece of the opposite color
						if (board[k][s].check != NO_CHECKER && board[k][s].check != board[i][j].check)
							++count;

						//if found piece of the same color
						if (board[k][s].check == board[i][j].check || count > 1)
							break;

						if (need == 2) {
							++s;
							need = 0;
						}

						if (count == 1) {
							++k;
							check_list.emplace_back(i, j);
							if (board[i][j].check == GO && board[k][s].check == NO_CHECKER && k < ROWS && s < COLUMNS)
								jump_list.emplace_back(i, j);
							break;
						}
					}

				   //stinga sus
					if (i % 2) {
						k = i;
						s = j;
					}
					else {
						k = i + 1;
						s = j - 1;
					}

					count = 0;
					need = 0;
					for (; s >= 0; k++) {
						++need;
						if (k == i && s == j)
							continue;

						if (board[k][s].check != NO_CHECKER && board[k][s].check != board[i][j].check)
							++count;

						if (board[k][s].check == board[i][j].check || count > 1)
							break;

						if (need == 2) {
							--s;
							need = 0;
						}

						if (count == 1) {
							++k;
							if (board[k][s].check == NO_CHECKER && k < ROWS && s >= 0 && board[i][j].check == GO)
								jump_list.emplace_back(i, j);
							check_list.emplace_back(i, j);
							break;
						}
					}

					//dreapta jos
					if (i % 2) {
						k = i - 1;
						s = j + 1;
					}
					else {
						k = i;
						s = j;
					}

					count = 0;
					need = 0;


					for (; s < ROWS; k--) {
						++need;

						if (k == i && s == j) {
							continue;
						}

						if (board[k][s].check != NO_CHECKER && board[k][s].check != board[i][j].check)
							++count;

						if (board[k][s].check == board[i][j].check || count > 1)
							break;

						if (need == 2) {
							++s;
							need = 0;
						}

						if (count == 1) {
							--k;
							if (board[k][s].check == NO_CHECKER && k >= 0 && s < COLUMNS && board[i][j].check == GO)
								jump_list.emplace_back(i, j);
							check_list.emplace_back(i, j);
							break;
						}
					}


					//stinga jos
					if (i % 2) {
						k = i;
						s = j;
					}
					else {
						k = i - 1;
						s = j - 1;
					}

					count = 0;
					need = 0;


					for (; s >= 0; k--) {
						++need;
						if (k == i && s == j)
							continue;

						if (board[k][s].check != NO_CHECKER && board[k][s].check != board[i][j].check)
							++count;

						if (board[k][s].check == board[i][j].check || count > 1)
							break;

						if (need == 2) {
							--s;
							need = 0;
						}

						if (count == 1) {
							--k;
							if (board[k][s].check == NO_CHECKER && k >= 0 && s >= 0 && board[i][j].check == GO)
								jump_list.emplace_back(i, j);
							check_list.emplace_back(i, j);
							break;
						}
					}


				} 

			}
		} 
	} 

}


//returneaza 1 daca miscarea efectuata de user este posibila, 0 in caz contrar
bool move_is_legal(const int p) {
	JUMPED = false;
	if (board[to.first][to.second].check != NO_CHECKER) {
		return false;
	}
	if (board[sel.first][sel.second].type == CHECKER ) {
		//saritura
		if (sel.first == to.first - 2) {
			if (sel.first % 2) {
				if (sel.second == to.second - 1 || sel.second == to.second + 1)
				{
					const auto temp = board[sel.first + 1][std::max(sel.second, to.second)].check;
					if (temp != board[sel.first][sel.second].check && temp != NO_CHECKER) {
						if (p) {
							board[sel.first + 1][std::max(sel.second, to.second)].check = NO_CHECKER;
							board[sel.first + 1][std::max(sel.second, to.second)].type = CHECKER;
						}
						JUMPED = true;
						goto end;
					}
				}
			}
			else {
				if (sel.second == to.second + 1 || sel.second == to.second - 1)
				{
					const auto temp = board[sel.first + 1][std::min(sel.second, to.second)].check;
					if (temp != board[sel.first][sel.second].check && temp != NO_CHECKER) {
						if (p) {
							board[sel.first + 1][std::min(sel.second, to.second)].check = NO_CHECKER;
							board[sel.first + 1][std::min(sel.second, to.second)].type = CHECKER;
						}
						JUMPED = true;
						goto end;
					}
				}
					
			}
		}
		else if (sel.first == to.first + 2) {
			if (sel.first % 2) {
				if (sel.second == to.second - 1 || sel.second == to.second + 1)
				{
					const auto temp = board[sel.first - 1][std::max(sel.second, to.second)].check;
					if (temp != board[sel.first][sel.second].check && temp != NO_CHECKER) {
						if (p) {
							board[sel.first - 1][std::max(sel.second, to.second)].check = NO_CHECKER;
							board[sel.first - 1][std::max(sel.second, to.second)].type = CHECKER;
						}
						JUMPED = true;
						goto end;
					}
				}
			}
			else {
				if (sel.second == to.second + 1 || sel.second == to.second - 1)
				{
					const auto temp = board[sel.first - 1][std::min(sel.second, to.second)].check;
					if (temp != board[sel.first][sel.second].check && temp != NO_CHECKER) {
						if (p) {
							board[sel.first - 1][std::min(sel.second, to.second)].check = NO_CHECKER;
							board[sel.first - 1][std::min(sel.second, to.second)].type = CHECKER;
						}
						JUMPED = true;
						goto end;
					}
				}
					
			}
		}

		//miscare simpla
		else if (sel.first == to.first - 1) {
			if (board[sel.first][sel.second].check == WHITE_CHECKER) {
				if (sel.first % 2) {//impar
					if (sel.second == to.second || sel.second == to.second - 1) {
						goto end;
					}
				}
				else {
					if (sel.second == to.second || sel.second == to.second + 1) {
						goto end;
					}
				}
			}
		}
		else if (sel.first == to.first + 1) {
			if (board[sel.first][sel.second].check == BLACK_CHECKER) {
				if (sel.first % 2) {//impar
					if (sel.second == to.second || sel.second == to.second - 1) {
						goto end;
					}
				}
				else {
					if (sel.second == to.second || sel.second == to.second + 1) {
						goto end;
					}
				}
			}
		}
	} //end of if piesa is checker


	//begin of if type is King
	else if (board[sel.first][sel.second].type == KING) {
		std::pair<int, int> rem;
		rem.first = rem.second = -1;
		int count = 0, i, j;
		int is = 0;

		//daca ma duc in dreapta sus
		if (sel.first % 2) {
			i = sel.first + 1;
			j = sel.second + 1;
		}
		else {
			i = sel.first;
			j = sel.second;
		}

		if (to.first > sel.first && to.second >= j) { 
			int need = 0;
			for (; j < COLUMNS; i++) {
				++need;

				if (i == sel.first && j == sel.second)
					continue;


				if (board[i][j].check != NO_CHECKER && board[i][j].check != board[sel.first][sel.second].check) {
					if (!count) {
						rem.first = i;
						rem.second = j;
					}
					++count;
				}

				if (board[i][j].check == board[sel.first][sel.second].check)
					return 0;

				if (to.first == i && to.second == j) {
					++is;
					break;
				}

				if (need == 2) {
					++j;
					need = 0;
				}
			}

			if (count == 1 && is) {
				if (p) {
					board[rem.first][rem.second].check = NO_CHECKER;
					board[rem.first][rem.second].type = CHECKER;
				}
				JUMPED = true;
				goto end;
			}

			if (count > 1)
				return 0;

			if (is) {
				goto end;
			}
		}

		//daca ma duc in stinga sus
		if (sel.first % 2) {
			i = sel.first;
			j = sel.second;
		}
		else {
			i = sel.first + 1;
			j = sel.second - 1;
		}

		if (to.first > sel.first && to.second <= j) { 
			int need = 0;

			for (; j >= 0; i++) {
				++need;
				if (i == sel.first && j == sel.second)
					continue;

				if (board[i][j].check != NO_CHECKER && board[i][j].check != board[sel.first][sel.second].check) {
					if (!count) {
						rem.first = i;
						rem.second = j;
					}
					++count;
				}

				if (board[i][j].check == board[sel.first][sel.second].check)
					return false;

				if (to.first == i && to.second == j) {
					++is;
					break;
				}

				if (need == 2) {
					--j;
					need = 0;
				}
			}

			if (count == 1 && is) {
				if (p) {
					board[rem.first][rem.second].check = NO_CHECKER;
					board[rem.first][rem.second].type = CHECKER;
				}
				JUMPED = true;
				goto end;
			}

			if (count > 1)
				return false;

			if (is) {
				goto end;
			}
		}

		//daca ma duc in dreapta jos
		if (sel.first % 2) {
			i = sel.first - 1;
			j = sel.second + 1;
		}
		else {
			i = sel.first;
			j = sel.second;
		}

		if (to.first < sel.first && to.second >= j) {  
			int need = 0;


			for (; j < ROWS; i--) {
				++need;

				if (i == sel.first && j == sel.second) {
					continue;
				}

				if (board[i][j].check != NO_CHECKER && board[i][j].check != board[sel.first][sel.second].check) {
					if (!count) {
						rem.first = i;
						rem.second = j;
					}
					++count;
				}

				if (board[i][j].check == board[sel.first][sel.second].check)
					return false;

				if (to.first == i && to.second == j) {
					++is;
					break;
				}

				if (need == 2) {
					++j;
					need = 0;
				}
			}

			if (count == 1 && is) {
				if (p) {
					board[rem.first][rem.second].check = NO_CHECKER;
					board[rem.first][rem.second].type = CHECKER;
				}
				JUMPED = true;
				goto end;
			}

			if (count > 1)
				return 0;

			if (is) {
				goto end;
			}
		}

		//daca ma duc in stinga jos 
		if (sel.first % 2) {
			i = sel.first;
			j = sel.second;
		}
		else {
			i = sel.first - 1;
			j = sel.second - 1;
		}

		count = 0;
		if (to.first < sel.first && to.second <= j) { 
			int need = 0;

			for (; j >= 0; i--) {
				++need;
				if (i == sel.first && j == sel.second)
					continue;

				if (board[i][j].check != NO_CHECKER && board[i][j].check != board[sel.first][sel.second].check) {
					if (!count) {
						rem.first = i;
						rem.second = j;
					}
					++count;
				}

				if (board[i][j].check == board[sel.first][sel.second].check)
					return false;

				if (to.first == i && to.second == j) {
					++is;
					break;
				}

				if (need == 2) {
					--j;
					need = 0;
				}
			}

			if (count == 1 && is) {
				if (p) {
					board[rem.first][rem.second].check = NO_CHECKER;
					board[rem.first][rem.second].type = CHECKER;
				}
				JUMPED = true;
				goto end;
			}

			if (count > 1)
				return false;

			if (is) {
				goto end;
			}
	} 
	} 
	return false;

end:
	if (list_contain_element<int>(jump_list, sel.first, sel.second) && !JUMPED)
		return false;
	return true;
}


//returneaza true daca piesele de tip "checker" nu mai au nici o miscare
bool no_more_moves(const unsigned int checker) {
	for (auto& it : move_list)
	{
		if (board[it.first][it.second].check == checker) {
			return false;
		}
	}

	for (auto& it : check_list)
	{
		if (board[it.first][it.second].check == checker) {
			return false;
		}
	}
	return true;
}


//returneaza numarul de piese de tip "checker"
int count_checkers(const int checker) {
	int count = 0;
	for (auto i = 0; i < ROWS; i++)
		for (auto j = 0; j < COLUMNS; j++)
			if (board[i][j].check == checker)
				++count;
	return count;
}