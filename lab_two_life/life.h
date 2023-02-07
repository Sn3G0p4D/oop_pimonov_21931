#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <conio.h>

class Input {
public:
	// cmd args
	std::vector <std::string> tokens;					// Токены аргументов cmd
	// game mode
	bool is_online_mode;								// Online режим или нет
	// offline args
	std::string output_filename;						// Имя output файла
	int iterations_cnt;									// Количество итераций
	// input data
	std::string input_filename;							// Имя input файла
	std::vector <std::string> input_data;				// Все данные
	std::string name;									// Имя вселенной
	std::vector <std::string> init_born_save;			// Условия рождения и смерти, [0] - born, [1] - save
	std::vector <std::vector <int>> init_field;			// Изначальное поле, размер и ячейки

	Input();
	Input(const int& argc, char** argv);
	~Input();

	void get_tokens(const int& argc, char** argv);			// Разбиваем аргументы cmd на токены
	void normailize_tokens();								// "Нормализуем", то есть заменяем "--parameter=x" на "-p" и "x"
	std::string get_cmd_arg(const std::string& arg_type);	// Ищем arg_type аргумент среди всех аргументов
	void get_args_and_parse(const int& argc, char** argv);

	void get_input_data();
	void parse_data();
};

class Universe {
public:
	std::string name;									// Имя вселенной
	std::vector <int> born;								// Условие рождения
	std::vector <int> save;								// Условие смерти
	std::vector <std::vector <int>> game_field;			// Основное поле
	std::vector <std::vector <int>> temp_field;			// Временное поле

	Universe();
	Universe(const Input& input);
	~Universe();
};

class Game {
public:
	// game parameters
	bool is_online_mode;								// Online режим или нет
	std::string output_filename;						// Имя output файла
	int iterations_cnt;									// Количество итераций
	Universe universe;									// Вселенная
	// user input
	std::string user_input;								// Весь пользовательский ввод
	std::vector <std::string> tokens;					// Ввод, разбитый на токены

	Game();
	Game(const Input& input);
	~Game();

	void iterate(const int& n);
	void write_in_file();
	void print_game_field();
	void parse_user_input();
	void process_the_game();
};