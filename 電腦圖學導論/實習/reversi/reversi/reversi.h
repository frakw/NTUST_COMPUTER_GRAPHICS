#ifndef _REVERSI_H_
#define _REVERSI_H_
#include <vector>
#include <utility>
enum class Reversi_Color : bool {
	BLACK,
	WHITE
};
class Grid {
public:
	Grid();
	friend class Reversi;
	void set(Reversi_Color c);
	bool is_empty();
	bool not_empty();
	bool available();
	bool is_black();
	bool is_white();
	Reversi_Color color();
	void flip();
	void reset();
	int flip_count;//available時可翻轉的棋數，外部顯示用
private:
	bool _empty = true;
	bool _available = false;
	Reversi_Color _color;	
};
class Reversi {
public:
	Reversi(int = 8,int = 8);
	~Reversi();
	int W() { return this->_W; }
	int H() { return this->_H; }
	void set_chess_texture(const char*, const char*,const char*);
	Grid* operator[](const int index);
	bool legal_coord(const int x, const int y);
	void find_available_grid();
	std::vector<std::pair<int,int> > add_chess(int, int);
	void next_round();
	int black_amount();
	int white_amount();
	bool game_end();
	Reversi_Color* winner();
	void reset();
	int tex_black_id, tex_white_id, tex_empty_id;
	Reversi_Color this_round = Reversi_Color::BLACK;//黑棋先下
private:
	int _W = 8, _H = 8,pass_count = 0,available_count = 0;
	Grid** grid;
};
#endif // _REVERSI_H_