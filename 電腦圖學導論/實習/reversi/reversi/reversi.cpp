#include <glad/glad.h>
#include <ctime>
#include "reversi.h"
#include "texture.h"
Grid::Grid() {}
void Grid::set(Reversi_Color c) {
	_empty = false;
	_color = c;
}
bool Grid::is_empty() { return _empty; }
bool Grid::not_empty() { return !_empty; }
bool Grid::available() { return _available; }
Reversi_Color Grid::color() { return _color; }
bool Grid::is_black() { return _color == Reversi_Color::BLACK && !_empty; }
bool Grid::is_white() { return _color == Reversi_Color::WHITE && !_empty; }
void Grid::flip() {
	if (_color == Reversi_Color::BLACK) _color = Reversi_Color::WHITE;
	else _color = Reversi_Color::BLACK;
}
void Grid::reset() {
	_empty = true;
	_available = false;
}
Reversi::Reversi(int _w, int _h) : _W(_w),_H(_h){
	grid = new Grid * [_H];
	for (int i = 0;i < _H;i++) {
		grid[i] = new Grid[_W];
	}
	grid[_H / 2 - 1][_W / 2 - 1].set(Reversi_Color::WHITE);
	grid[_H / 2][_W / 2].set(Reversi_Color::WHITE);
	grid[_H / 2 - 1][_W / 2].set(Reversi_Color::BLACK);
	grid[_H / 2][_W / 2 - 1].set(Reversi_Color::BLACK);
}
Reversi::~Reversi() {
	for (int i = 0;i < _H;i++) {
		delete[] grid[i];
	}
	delete[] grid;
}

void Reversi::set_chess_texture(const char* black_path, const char* white_path,const char* empty_path) {
	tex_black_id = TextureFromFile(black_path);
	tex_white_id = TextureFromFile(white_path);
	tex_empty_id = TextureFromFile(empty_path);
}
Grid* Reversi::operator[](const int index) {
	return grid[index];
}

inline bool Reversi::legal_coord(const int x, const int y) {
	return (x >= 0 && x < _W) && (y >= 0 && y < _H);
}

void Reversi::find_available_grid() {
	int direction[8][2] = {
		{-1,	-1},
		{0 ,	-1},
		{+1,	-1},
		{+1,	 0},
		{+1,	+1},
		{0,		+1},
		{-1,	+1},
		{-1,	 0}
	};
	available_count = 0;
	for (int i = 0;i < _H;i++) {
		for (int j = 0;j < _W;j++) {
			grid[i][j]._available = false;
			grid[i][j].flip_count = 0;
			if (grid[i][j].not_empty()) continue;
			for (int k = 0;k < 8;k++) {
				int x = j + direction[k][0], y = i + direction[k][1];
				if (!legal_coord(x, y)) continue;
				if (grid[y][x].is_empty() || grid[y][x].color() == this_round) continue;
				while (grid[y][x].color() != this_round && grid[y][x].not_empty()) {
					x += direction[k][0];
					y += direction[k][1];
					if (!legal_coord(x, y))break;
				}
				if (legal_coord(x, y)) {
					if (grid[y][x].not_empty()) {
						if (!grid[i][j]._available) {
							grid[i][j]._available = true;
							if (j == x) {//vertical
								grid[i][j].flip_count = abs(y - i) - 1;
							}
							else{//horizontal or slope
								grid[i][j].flip_count = abs(x - j) - 1;
							}
							++available_count;
						}
						else {
							if (j == x) {//vertical
								grid[i][j].flip_count += abs(y - i) - 1;
							}
							else {//horizontal or slope
								grid[i][j].flip_count += abs(x - j) - 1;
							}
						}
					}
				}
			}
		}
	}
}

std::vector<std::pair<int, int> > Reversi::add_chess(int x, int y) {
	grid[y][x].set(this_round);
	int direction[8][2] = {
		{-1,	-1},
		{0 ,	-1},
		{+1,	-1},
		{+1,	 0},
		{+1,	+1},
		{0,		+1},
		{-1,	+1},
		{-1,	 0}
	};
	std::vector<std::pair<int, int> > all_flip;
	for (int i = 0;i < 8;i++) {
		int _x = x + direction[i][0], _y = y + direction[i][1];
		if (!legal_coord(_x, _y)) continue;
		if (grid[_y][_x].is_empty() || grid[_y][_x].color() == this_round) continue;
		while (grid[_y][_x].color() != this_round && grid[_y][_x].not_empty()) {
			_x += direction[i][0];
			_y += direction[i][1];
			if (!legal_coord(_x, _y))break;
		}
		if (legal_coord(_x, _y)) {
			if (grid[_y][_x].not_empty()) {
				_x -= direction[i][0];
				_y -= direction[i][1];
				while (!(_x==x && _y==y)){
					grid[_y][_x].flip();
					all_flip.push_back(std::make_pair(_x, _y));
					_x -= direction[i][0];
					_y -= direction[i][1];
				}
			}
		}
	}
	next_round();
	return all_flip;
}

void Reversi::next_round() {
	if (this_round == Reversi_Color::BLACK) this_round = Reversi_Color::WHITE;
	else this_round = Reversi_Color::BLACK;
}

int Reversi::black_amount() {
	int count = 0;
	for (int i = 0;i < _H;i++) {
		for (int j = 0;j < _W;j++) {
			if (grid[i][j].not_empty() && grid[i][j].color() == Reversi_Color::BLACK) count++;
		}
	}
	return count;
}
int Reversi::white_amount() {
	int count = 0;
	for (int i = 0;i < _H;i++) {
		for (int j = 0;j < _W;j++) {
			if (grid[i][j].not_empty() && grid[i][j].color() == Reversi_Color::WHITE) count++;
		}
	}
	return count;
}


bool Reversi::game_end() {
	if (available_count == 0) {
		++pass_count;
	}
	else {
		pass_count = 0;
	}

	if (pass_count == 2) {
		return true;//game end winner comes
	}
	return false;
}

Reversi_Color* Reversi::winner() {
	int b_count = black_amount(), w_count = white_amount();
	if (b_count > w_count) {
		this_round = Reversi_Color::BLACK;
		return &this_round;
	}
	else if (b_count < w_count) {
		this_round = Reversi_Color::WHITE;
		return &this_round;
	}
	return nullptr;//ендт
}

void Reversi::reset() {
	for (int i = 0;i < _H;i++) {
		for (int j = 0;j < _W;j++) {
			grid[i][j].reset();
		}
	}
	pass_count = 0;
	available_count = 0;
	this_round = Reversi_Color::BLACK;
	grid[_H / 2 - 1][_W / 2 - 1].set(Reversi_Color::WHITE);
	grid[_H / 2][_W / 2].set(Reversi_Color::WHITE);
	grid[_H / 2 - 1][_W / 2].set(Reversi_Color::BLACK);
	grid[_H / 2][_W / 2 - 1].set(Reversi_Color::BLACK);
}
