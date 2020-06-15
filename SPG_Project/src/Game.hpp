#pragma once
#include "utilities.hpp"

int count_checkers(int);
int move_is_legal(int);
void list_of_jumpes(std::list<std::pair<int, int>>& jumpList, std::list<std::pair<int, int>>& checkList);
void list_of_moves(std::list<std::pair<int, int>>& moveList);

bool no_more_moves(unsigned int);

//completarea cozii mListFirst cu piese care pot efectua miscari
void list_of_moves(std::list<std::pair<int, int>>& moveList) {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			if (_board[i][j].check != NO_CHECKER) {
				int k;
				if (i % 2)
					k = j + 1;
				else
					k = j - 1;
				if (_board[i][j].type == KING) {
					if ((_board[i + 1][j].check == NO_CHECKER && i + 1 < ROWS) ||
						(_board[i - 1][j].check == NO_CHECKER && i - 1 >= 0) ||
						(_board[i + 1][k].check == NO_CHECKER && i + 1 < ROWS && k < COLUMNS && k >= 0) ||
						(_board[i - 1][k].check == NO_CHECKER && i - 1 >= 0 && k < COLUMNS && k >= 0)) {
						moveList.push_back(std::make_pair(i, j));
					} //if lung
				} //if KING
				if (_board[i][j].type == CHECKER) {
					if ((_board[i + 1][j].check == NO_CHECKER && i + 1 < ROWS && _board[i][j].check == WHITE_CHECKER) ||
						(_board[i - 1][j].check == NO_CHECKER && i - 1 >= 0 && _board[i][j].check == BLACK_CHECKER) ||
						(_board[i + 1][k].check == NO_CHECKER && i + 1 < ROWS && k < COLUMNS && k >= 0 && _board[i][j].check == WHITE_CHECKER) ||
						(_board[i - 1][k].check == NO_CHECKER && i - 1 >= 0 && k < COLUMNS && k >= 0 && _board[i][j].check == BLACK_CHECKER)) {
						moveList.push_back(std::make_pair(i, j));

					} //if lung
				} //if CHECKER
			} //if !NO_CHECHER
		} //for j
	} //for i	
} //end listOfMoves
//completarea cozii jListFirst cu piesele care trebuie sa sara
void list_of_jumpes(std::list<std::pair<int, int>>& jumpList, std::list<std::pair<int, int>>& checkList) {
	int i, j;

	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLUMNS; j++) {
			if (_board[i][j].check != NO_CHECKER) {
				int k, s;
				if (i % 2) {
					k = j + 1;
					s = j - 1;
				}
				else {
					k = j - 1;
					s = j + 1;
				}

				if (_board[i][j].type == CHECKER) {
					if ((_board[i + 1][j].check != NO_CHECKER && _board[i + 1][j].check != _board[i][j].check && _board[i + 2][s].check == NO_CHECKER && i + 2 < ROWS && s < COLUMNS && s >= 0) ||
						(_board[i - 1][j].check != NO_CHECKER && _board[i - 1][j].check != _board[i][j].check && _board[i - 2][s].check == NO_CHECKER && i - 2 >= 0 && s < COLUMNS && s >= 0) ||
						(_board[i + 1][k].check != NO_CHECKER && _board[i + 1][k].check != _board[i][j].check && _board[i + 2][k].check == NO_CHECKER && i + 2 < ROWS && k < COLUMNS && k >= 0) ||
						(_board[i - 1][k].check != NO_CHECKER && _board[i - 1][k].check != _board[i][j].check && _board[i - 2][k].check == NO_CHECKER && i - 2 >= 0 && k < COLUMNS && k >= 0)) {
						if (_board[i][j].check == GO)
							jumpList.push_back(std::make_pair(i, j));
						checkList.push_back(std::make_pair(i, j));
					} // if lung

				} // if CHECKER


				if (_board[i][j].type == KING) {

					int count = 0;
					//dreapta sus
					if (i % 2) {
						k = i + 1;
						s = j + 1;
					}
					else {
						k = i;
						s = j;
					}

					int need = 0;
					for (; s < COLUMNS; k++) {
						++need;

						if (k == i && s == j)
							continue;


						if (_board[k][s].check != NO_CHECKER && _board[k][s].check != _board[i][j].check)
							++count;


						if (_board[k][s].check == _board[i][j].check || count > 1)
							break;

						if (need == 2) {
							++s;
							need = 0;
						}

						if (count == 1) {
							++k;
							if (_board[k][s].check == NO_CHECKER && k < ROWS && s < COLUMNS)
								if (_board[i][j].check == GO)
									jumpList.push_back(std::make_pair(i, j));
							checkList.push_back(std::make_pair(i, j));
							break;
						}
					}
					// end of dreapta sus


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

						if (_board[k][s].check != NO_CHECKER && _board[k][s].check != _board[i][j].check)
							++count;

						if (_board[k][s].check == _board[i][j].check || count > 1)
							break;

						if (need == 2) {
							--s;
							need = 0;
						}

						if (count == 1) {
							++k;
							if (_board[k][s].check == NO_CHECKER && k < ROWS && s >= 0)
								if (_board[i][j].check == GO)
									jumpList.push_back(std::make_pair(i, j));
							checkList.push_back(std::make_pair(i, j));
							break;
						}
					}
					//end of stinga sus



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


					for (; s <= 3; k--) {
						++need;

						if (k == i && s == j) {
							continue;
						}

						if (_board[k][s].check != NO_CHECKER && _board[k][s].check != _board[i][j].check)
							++count;

						if (_board[k][s].check == _board[i][j].check || count > 1)
							break;

						if (need == 2) {
							++s;
							need = 0;
						}

						if (count == 1) {
							--k;
							if (_board[k][s].check == NO_CHECKER && k >= 0 && s < COLUMNS)
								if (_board[i][j].check == GO)
									jumpList.push_back(std::make_pair(i, j));
							checkList.push_back(std::make_pair(i, j));
							break;
						}
					}

					//end of dreapta jos

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

						if (_board[k][s].check != NO_CHECKER && _board[k][s].check != _board[i][j].check)
							++count;

						if (_board[k][s].check == _board[i][j].check || count > 1)
							break;

						if (need == 2) {
							--s;
							need = 0;
						}

						if (count == 1) {
							--k;
							if (_board[k][s].check == NO_CHECKER && k >= 0 && s >= 0)
								if (_board[i][j].check == GO)
									jumpList.push_back(std::make_pair(i, j));
							checkList.push_back(std::make_pair(i, j));
							break;
						}
					}

					//end of stinga jos


				} //if KING		

			} // if !NO_CHECKER
		} // for j
	} // for i

} //end of listOfJumpes


//returneaza numarul de piese de tip "checker"
int count_checkers(int checker) {
	int i, j, count = 0;
	for (i = 0; i < ROWS; i++)
		for (j = 0; j < COLUMNS; j++)
			if (_board[i][j].check == checker)
				++count;
	return count;
}

//returneaza 1 daca miscarea efectuata de user este posibila, 0 in caz contrar
int move_is_legal(int p) {
	int ret = 0;
	JUMPED = 0;
	if (_board[sel.first][sel.second].type == CHECKER) {
		if (_board[to.first][to.second].check == NO_CHECKER) {

			//saritura
			if (sel.first == to.first - 2) {
				if (sel.first % 2) {
					if (sel.second == to.second - 1 || sel.second == to.second + 1)
						if (_board[sel.first + 1][std::max(sel.second, to.second)].check != _board[sel.first][sel.second].check && _board[sel.first + 1][std::max(sel.second, to.second)].check != NO_CHECKER) {
							if (p) {
								_board[sel.first + 1][std::max(sel.second, to.second)].check = NO_CHECKER;
								_board[sel.first + 1][std::max(sel.second, to.second)].type = CHECKER;
							}
							JUMPED = 1;
							ret = 1;
							goto end;
						}
				}
				else {
					if (sel.second == to.second + 1 || sel.second == to.second - 1)
						if (_board[sel.first + 1][std::min(sel.second, to.second)].check != _board[sel.first][sel.second].check && _board[sel.first + 1][std::min(sel.second, to.second)].check != NO_CHECKER) {
							if (p) {
								_board[sel.first + 1][std::min(sel.second, to.second)].check = NO_CHECKER;
								_board[sel.first + 1][std::min(sel.second, to.second)].type = CHECKER;
							}
							JUMPED = 1;
							ret = 1;
							goto end;
						}
				}
			}
			if (sel.first == to.first + 2) {
				if (sel.first % 2) {
					if (sel.second == to.second - 1 || sel.second == to.second + 1)
						if (_board[sel.first - 1][std::max(sel.second, to.second)].check != _board[sel.first][sel.second].check && _board[sel.first - 1][std::max(sel.second, to.second)].check != NO_CHECKER) {
							if (p) {
								_board[sel.first - 1][std::max(sel.second, to.second)].check = NO_CHECKER;
								_board[sel.first - 1][std::max(sel.second, to.second)].type = CHECKER;
							}
							JUMPED = 1;
							ret = 1;
							goto end;
						}
				}
				else {
					if (sel.second == to.second + 1 || sel.second == to.second - 1)
						if (_board[sel.first - 1][std::min(sel.second, to.second)].check != _board[sel.first][sel.second].check && _board[sel.first - 1][std::min(sel.second, to.second)].check != NO_CHECKER) {
							if (p) {
								_board[sel.first - 1][std::min(sel.second, to.second)].check = NO_CHECKER;
								_board[sel.first - 1][std::min(sel.second, to.second)].type = CHECKER;
							}
							JUMPED = 1;
							ret = 1;
							goto end;
						}
				}
			}


			//miscare simpla
			if (sel.first == to.first - 1) {
				if (_board[sel.first][sel.second].check == WHITE_CHECKER) {
					if (sel.first % 2) {//impar
						if (sel.second == to.second || sel.second == to.second - 1) {
							ret = 1;
							goto end;
						}
					}
					else {
						if (sel.second == to.second || sel.second == to.second + 1) {
							ret = 1;
							goto end;
						}
					}
				}
			}

			if (sel.first == to.first + 1) {
				if (_board[sel.first][sel.second].check == BLACK_CHECKER) {
					if (sel.first % 2) {//impar
						if (sel.second == to.second || sel.second == to.second - 1) {
							ret = 1;
							goto end;
						}
					}
					else {
						if (sel.second == to.second || sel.second == to.second + 1) {
							ret = 1;
							goto end;
						}
					}
				}
			}

		}
	} //end of if piesa is checker


	//begin of if type is King
	if (_board[sel.first][sel.second].type == KING) {
		if (_board[to.first][to.second].check == NO_CHECKER) {
			std::pair<int, int> rem;
			rem.first = rem.second = -1;
			int count = 0, i, j;
			int ver = 0, is = 0;

			if (sel.first % 2) {
				i = sel.first + 1;
				j = sel.second + 1;
			}
			else {
				i = sel.first;
				j = sel.second;
			}

			if (to.first > sel.first && to.second >= j) { //daca ma duc in dreapta sus
				int need = 0;
				for (; j < COLUMNS; i++) {
					++need;

					if (i == sel.first && j == sel.second)
						continue;


					if (_board[i][j].check != NO_CHECKER && _board[i][j].check != _board[sel.first][sel.second].check) {
						if (!count) {
							rem.first = i;
							rem.second = j;
						}
						++count;
					}

					if (_board[i][j].check == _board[sel.first][sel.second].check)
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
						_board[rem.first][rem.second].check = NO_CHECKER;
						_board[rem.first][rem.second].type = CHECKER;
					}
					JUMPED = 1;
					ret = 1;
					goto end;
				}

				if (count > 1)
					return 0;

				if (is) {
					ret = 1;
					goto end;
				}
			}


			if (sel.first % 2) {
				i = sel.first;
				j = sel.second;
			}
			else {
				i = sel.first + 1;
				j = sel.second - 1;
			}

			if (to.first > sel.first && to.second <= j) { //daca ma duc in stinga sus
				int need = 0;


				for (; j >= 0; i++) {
					++need;
					if (i == sel.first && j == sel.second)
						continue;

					if (_board[i][j].check != NO_CHECKER && _board[i][j].check != _board[sel.first][sel.second].check) {
						if (!count) {
							rem.first = i;
							rem.second = j;
						}
						++count;
					}

					if (_board[i][j].check == _board[sel.first][sel.second].check)
						return 0;

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
						_board[rem.first][rem.second].check = NO_CHECKER;
						_board[rem.first][rem.second].type = CHECKER;
					}
					JUMPED = 1;
					ret = 1;
					goto end;
				}

				if (count > 1)
					return 0;

				if (is) {
					ret = 1;
					goto end;
				}

			}



			if (sel.first % 2) {
				i = sel.first - 1;
				j = sel.second + 1;
			}
			else {
				i = sel.first;
				j = sel.second;
			}

			if (to.first < sel.first && to.second >= j) { //daca ma duc in dreapta jos 
				int need = 0;


				for (; j <= 3; i--) {
					++need;

					if (i == sel.first && j == sel.second) {
						continue;
					}

					if (_board[i][j].check != NO_CHECKER && _board[i][j].check != _board[sel.first][sel.second].check) {
						if (!count) {
							rem.first = i;
							rem.second = j;
						}
						++count;
					}

					if (_board[i][j].check == _board[sel.first][sel.second].check)
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
						_board[rem.first][rem.second].check = NO_CHECKER;
						_board[rem.first][rem.second].type = CHECKER;
					}
					JUMPED = 1;
					ret = 1;
					goto end;
				}

				if (count > 1)
					return 0;

				if (is) {
					ret = 1;
					goto end;
				}
			}


			if (sel.first % 2) {
				i = sel.first;
				j = sel.second;
			}
			else {
				i = sel.first - 1;
				j = sel.second - 1;
			}

			count = 0;
			if (to.first < sel.first && to.second <= j) { //daca ma duc in stinga jos 
				int need = 0;


				for (; j >= 0; i--) {
					++need;
					if (i == sel.first && j == sel.second)
						continue;

					if (_board[i][j].check != NO_CHECKER && _board[i][j].check != _board[sel.first][sel.second].check) {
						if (!count) {
							rem.first = i;
							rem.second = j;
						}
						++count;
					}

					if (_board[i][j].check == _board[sel.first][sel.second].check)
						return 0;

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
						_board[rem.first][rem.second].check = NO_CHECKER;
						_board[rem.first][rem.second].type = CHECKER;
					}
					JUMPED = 1;
					ret = 1;
					goto end;
				}

				if (count > 1)
					return 0;

				if (is) {
					ret = 1;
					goto end;
				}

			}


		} //end of NO_CHECKER
	} //end of if piesa is king 
	return 0;

end:
	if (listContainElement<int>(jumpList, sel.first, sel.second) && !JUMPED)
		return 0;
	else
		return ret;

}


//returneaza true daca piesele de tip "checker" nu mai au nici o miscare
bool no_more_moves(const unsigned int checker) {

	for (auto& it : moveList)
	{
		if (_board[it.first][it.second].check == checker) {
			return false;
		}
	}

	for (auto& it : checkList)
	{
		if (_board[it.first][it.second].check == checker) {
			return false;
		}
	}
	return true;
}
