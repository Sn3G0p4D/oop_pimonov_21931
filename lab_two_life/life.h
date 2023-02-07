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
	std::vector <std::string> tokens;					// ������ ���������� cmd
	// game mode
	bool is_online_mode;								// Online ����� ��� ���
	// offline args
	std::string output_filename;						// ��� output �����
	int iterations_cnt;									// ���������� ��������
	// input data
	std::string input_filename;							// ��� input �����
	std::vector <std::string> input_data;				// ��� ������
	std::string name;									// ��� ���������
	std::vector <std::string> init_born_save;			// ������� �������� � ������, [0] - born, [1] - save
	std::vector <std::vector <int>> init_field;			// ����������� ����, ������ � ������

	Input();
	Input(const int& argc, char** argv);
	~Input();

	void get_tokens(const int& argc, char** argv);			// ��������� ��������� cmd �� ������
	void normailize_tokens();								// "�����������", �� ���� �������� "--parameter=x" �� "-p" � "x"
	std::string get_cmd_arg(const std::string& arg_type);	// ���� arg_type �������� ����� ���� ����������
	void get_args_and_parse(const int& argc, char** argv);

	void get_input_data();
	void parse_data();
};

class Universe {
public:
	std::string name;									// ��� ���������
	std::vector <int> born;								// ������� ��������
	std::vector <int> save;								// ������� ������
	std::vector <std::vector <int>> game_field;			// �������� ����
	std::vector <std::vector <int>> temp_field;			// ��������� ����

	Universe();
	Universe(const Input& input);
	~Universe();
};

class Game {
public:
	// game parameters
	bool is_online_mode;								// Online ����� ��� ���
	std::string output_filename;						// ��� output �����
	int iterations_cnt;									// ���������� ��������
	Universe universe;									// ���������
	// user input
	std::string user_input;								// ���� ���������������� ����
	std::vector <std::string> tokens;					// ����, �������� �� ������

	Game();
	Game(const Input& input);
	~Game();

	void iterate(const int& n);
	void write_in_file();
	void print_game_field();
	void parse_user_input();
	void process_the_game();
};