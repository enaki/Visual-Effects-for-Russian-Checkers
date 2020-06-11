#pragma once

#include <list>

//prototipurile functiilor
void boardInit();
void display();
void timer(int);
void createMenu();
void actionMenu(int);
void passiveMotion(int, int);
void putChecker();
void undo();

int moveIsLegal(int);
void listOfJumpes(std::list<std::pair<int, int>>& jumpList, std::list<std::pair<int, int>>& checkList);
void listOfMoves(std::list<std::pair<int, int>>& moveList);

bool noMoreMoves(int);
int countCheckers(int);
void drawPossibleMoves();

