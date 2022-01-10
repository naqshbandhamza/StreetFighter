#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/Audio.hpp"
#include <string>

class character
{
	friend class game;
private:
	std::string picname;
	sf::Texture* texture;
	bool r, l, u, d;
	int no_cycles;
	int max_no_frames;
	int dimX;
	int dimY;
	int* no_of_frames_in_cycle;
	int dx_onepiece;
	int dy_onepiece;
	int** top_left;
	sf::Sprite* sprite;
	sf::IntRect cur_piece;
	int up, down, right, left;
	int** moves;
public:
	character();
	void set_picname(std::string picname);
	void setvalues(int no_cycles, int max_no_frames, int X, int Y);
	void set_index_cycle(int index, int no);
	void set_index_top(int index, int no);
	void set_index_left(int index, int no);
	int get_nframes(int index);
	void movement(int index,int c_no = 1,int extra_x = 0);
	void movement_left(int index, int c_no = 1, int extra_x = 0);
	void readmovesfromfile(const char* name);
	void set_position(sf::Vector2f pos);
	character(std::string picname,int no_cycles,int max_no_frames,int X,int Y);
	void set_properties(int left, int top);
	void set_udrl(int up,int down,int right,int left);
	void move_left(sf::RenderWindow& win);
	void setscale(sf::Vector2f scale);
	void move_right(sf::RenderWindow & win);
	void move_up();
	void move_down(sf::RenderWindow& win);
	sf::Sprite* get_sprite();
	~character();
};

