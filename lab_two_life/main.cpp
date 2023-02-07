#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <conio.h>
#include "life.h"

int main(int argc, char** argv) {
	setlocale(LC_ALL, "Rus");

	Input input(argc, argv);
	Game game(input);

	game.process_the_game();

	return 0;
 }