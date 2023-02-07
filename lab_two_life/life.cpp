#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <conio.h>
#include "life.h"

Input::Input() = default;
Input::Input(const int& argc, char** argv) {
	get_args_and_parse(argc, argv);
	get_input_data();
	parse_data();
}
Input::~Input() = default;

void Input::get_tokens(const int& argc, char** argv) {
	for (int i = 0; i < argc; i++) {
		this->tokens.push_back(argv[i]);
	}
}
void Input::normailize_tokens() {
	std::string tmp_token;							// Буффер для токена
	std::stringstream s_tmp_token;					// Поток строковый для разбиения по '='
	std::vector<std::string> tmp_tokens;			// Два токена, разбитые по '='
	std::vector <std::string> new_tokens;			// Переработанные токены

	for (int i = 0; i < this->tokens.size(); ++i) {
		if (this->tokens[i][0] == '-' && this->tokens[i][1] == '-') {
			// Разбиение на токены по '='
			s_tmp_token = std::stringstream(this->tokens[i]);
			while (std::getline(s_tmp_token, tmp_token, '=')) {
				tmp_tokens.push_back(tmp_token);
			}
			// Меняем на сокращения
			if (tmp_tokens[0].find("iterations")) {
				tmp_tokens[0] = "-i";
			}
			else if (tmp_tokens[0].find("output")) {
				tmp_tokens[0] = "-o";
			}

			new_tokens.push_back(tmp_tokens[0]);
			new_tokens.push_back(tmp_tokens[1]);
		}
		else {
			new_tokens.push_back(this->tokens[i]);
		}
	}

	this->tokens = new_tokens;
}
std::string Input::get_cmd_arg(const std::string& arg_type) {

	std::vector<std::string>::iterator iter = std::find(this->tokens.begin(), this->tokens.end(), arg_type);
	if (iter != this->tokens.end() && ++iter != this->tokens.end()) {
		return *iter;
	}
	else {
		return std::string("");
	}
}
void Input::get_args_and_parse(const int& argc, char** argv) {
	argc > 1 ? this->input_filename = argv[1] : this->input_filename = "default.txt";
	this->is_online_mode = argc < 3;
	this->iterations_cnt = 0;

	if (!this->is_online_mode) {
		this->get_tokens(argc, argv);
		this->normailize_tokens();

		int iterations_cnt = std::stoi(this->get_cmd_arg("-i"));
		std::string output_filename = this->get_cmd_arg("-o");

		this->iterations_cnt = iterations_cnt;
		this->output_filename = output_filename;
	}
}

void Input::get_input_data() {
	std::string line;
	std::ifstream in(this->input_filename);
	if (in.is_open()) {
		while (getline(in, line)) {
			this->input_data.push_back(line);
		}
	}
	in.close();
}
void Input::parse_data() {
	std::string buff, token;						// Буфер для строки и токен для "слов"
	std::stringstream s_buff;						// Поток чтения из строки

	// name
	buff = this->input_data[0];
	this->name = buff.erase(buff.find("#N "), 3);

	// born save rules
	buff = this->input_data[1];
	buff = buff.erase(buff.find("#R "), 3);

	s_buff = std::stringstream(buff);
	while (getline(s_buff, token, '/')) {
		this->init_born_save.push_back(token);
	}
	this->init_born_save[0] = this->init_born_save[0].erase(this->init_born_save[0].find("B"), 1);
	this->init_born_save[1] = this->init_born_save[1].erase(this->init_born_save[1].find("S"), 1);

	// init field
	buff = this->input_data[2];
	buff = buff.erase(buff.find("#S "), 3);

	int tmp_num;
	std::vector <int> sizes;						// Размеры поля
	std::vector <int> row;							// Строка для заполнения init_field

	s_buff = std::stringstream(buff);
	while (s_buff >> tmp_num) {
		sizes.push_back(tmp_num);
	}

	for (int i = 0; i < sizes[0]; i++) {
		row.push_back(0);
	}
	for (int i = 0; i < sizes[1]; i++) {
		this->init_field.push_back(row);
	}

	// fill field
	std::vector <int> coordinates;					// Координаты точки

	int i = 3;
	while (i < this->input_data.size()) {
		buff = this->input_data[i];
		s_buff = std::stringstream(buff);
		while (s_buff >> tmp_num) {
			coordinates.push_back(tmp_num);
		}
		this->init_field[coordinates[1]][coordinates[0]] = 1;
		coordinates.clear();
		++i;
	}
}

Universe::Universe() = default;
Universe::Universe(const Input& input) {
	// get born save rules
	for (int i = 0; i < input.init_born_save[0].size(); ++i) {
		this->born.push_back(input.init_born_save[0][i] - '0');
	}
	for (int i = 0; i < input.init_born_save[1].size(); ++i) {
		this->save.push_back(input.init_born_save[1][i] - '0');
	}

	// get init fields
	this->game_field = input.init_field;
	this->temp_field = input.init_field;

	// get name
	this->name = input.name;
}
Universe::~Universe() = default;

Game::Game() = default;
Game::Game(const Input& input) {
	// set game mode features
	this->is_online_mode = input.is_online_mode;
	this->output_filename = input.output_filename;
	this->iterations_cnt = input.iterations_cnt;
	// set universe
	this->universe = Universe(input);
}
Game::~Game() = default;

void Game::iterate(const int& n) {
	int cells_around_count;								// Количество клеток вокруг выбранной ячейки
	int x_max = this->universe.game_field[0].size();	// Максимум по X
	int y_max = this->universe.game_field.size();		// Максимум по Y

	for (int iter = 0; iter < n; ++iter) {
		for (int x = 0; x < this->universe.game_field[0].size(); ++x) {
			for (int y = 0; y < this->universe.game_field.size(); ++y) {
				cells_around_count = 0;

				// (max + _ % max) % max

				if (this->universe.temp_field[(y_max + (y - 1) % y_max) % y_max][(x_max + (x - 1) % x_max) % x_max]) ++cells_around_count;
				if (this->universe.temp_field[(y_max + (y - 1) % y_max) % y_max][x]) ++cells_around_count;
				if (this->universe.temp_field[(y_max + (y - 1) % y_max) % y_max][(x_max + (x + 1) % x_max) % x_max]) ++cells_around_count;
				if (this->universe.temp_field[y][(x_max + (x - 1) % x_max) % x_max]) ++cells_around_count;
				if (this->universe.temp_field[y][(x_max + (x + 1) % x_max) % x_max]) ++cells_around_count;
				if (this->universe.temp_field[(y_max + (y + 1) % y_max) % y_max][(x_max + (x - 1) % x_max) % x_max]) ++cells_around_count;
				if (this->universe.temp_field[(y_max + (y + 1) % y_max) % y_max][x]) ++cells_around_count;
				if (this->universe.temp_field[(y_max + (y + 1) % y_max) % y_max][(x_max + (x + 1) % x_max) % x_max]) ++cells_around_count;

				if (this->universe.temp_field[y][x]) {
					this->universe.game_field[y][x] = 0;
					for (int i = 0; i < this->universe.save.size(); ++i) {
						if (cells_around_count == this->universe.save[i]) {
							this->universe.game_field[y][x] = 1;
							break;
						}
					}
				}
				else {
					for (int i = 0; i < this->universe.born.size(); ++i) {
						if (cells_around_count == this->universe.born[i]) {
							this->universe.game_field[y][x] = 1;
							break;
						}
					}
				}
			}
		}
		this->universe.temp_field = this->universe.game_field;
	}
}
void Game::write_in_file() {
	std::ofstream out(this->output_filename);

	out << "#N " << this->universe.name << "\n";

	out << "#R B";
	for (int i = 0; i < this->universe.born.size(); ++i) {
		out << this->universe.born[i];
	}
	out << "/S";
	for (int i = 0; i < this->universe.save.size(); ++i) {
		out << this->universe.save[i];
	}

	out << "\n#S " << this->universe.game_field[0].size() << " " << this->universe.game_field.size() << "\n";
	for (int i = 0; i < this->universe.game_field[0].size(); ++i) {
		for (int j = 0; j < this->universe.game_field.size(); ++j) {
			if (this->universe.game_field[j][i]) {
				out << i << " " << j << "\n";
			}
		}
	}
	out.close();
}
void Game::print_game_field() {
	for (int i = 0; i < this->universe.game_field.size(); i++) {
		for (int j = 0; j < this->universe.game_field[0].size(); j++) {
			if (this->universe.game_field[i][j]) {
				std::cout << "# ";
			}
			else {
				std::cout << "_ ";
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}
void Game::parse_user_input() {
	std::string token;
	std::stringstream s_user_input(this->user_input);

	this->tokens.clear();
	while (std::getline(s_user_input, token, ' ')) {
		this->tokens.push_back(token);
	}
}
void Game::process_the_game() {
	if (this->is_online_mode) {
		std::string head;							// Заголовок игры
		std::string help;							// Список команд

		head = "Name: " + this->universe.name + "\n\n";
		head += "Born: ";
		for (int i = 0; i < this->universe.born.size(); ++i) {
			head += std::to_string(this->universe.born[i]) + " ";
		}
		head += "\n\nSave: ";
		for (int i = 0; i < this->universe.save.size(); ++i) {
			head += std::to_string(this->universe.save[i]) + " ";
		}
		head += "\n\n";

		help = "help                --     list of commands\n";
		help += "dump <filename>     --     save universe in <filename> file\n";
		help += "tick <n=1>          --     iterate <n> times (1 by default) and print result\n";
		help += "exit                --     close the game\n\n";
		help += "Press Enter to continue...";

		while (true) {
			system("cls");
			std::cout << head;
			std::cout << "Count of iterations: " << std::to_string(this->iterations_cnt) << "\n\n";
			this->print_game_field();

			std::cout << "If you need list of commands, type in <help> (without <>, ofc)\n";
			std::cout << "Waiting for your command: ";
			std::getline(std::cin, this->user_input);
			this->parse_user_input();

			if (this->tokens[0] == "help") {
				system("cls");
				std::cout << help;
				char dummy = _getch();
			}
			else if (this->tokens[0] == "exit") {
				system("cls");
				std::cout << "Thank you for using this programm\nI hope you enjoyed\n\n";
				break;
			}
			else if (this->tokens[0] == "tick" || this->tokens[0] == "t") {
				if (this->tokens.size() == 1) {
					this->iterate(1);
					this->iterations_cnt++;
				}
				else {
					this->iterate(std::stoi(this->tokens[1]));
					this->iterations_cnt += std::stoi(this->tokens[1]);
				}
			}
			else if (this->tokens[0] == "dump") {
				this->output_filename = this->tokens[1];
				this->write_in_file();
			}
			else {
				std::cout << "Sorry, something wrong in your input, press Enter to type again: ";
				std::cin.ignore();
			}
		}
	}
	else {
		this->iterate(this->iterations_cnt);
		this->write_in_file();
		std::cout << "Universe successfully written to file.\n\n";
	}
}