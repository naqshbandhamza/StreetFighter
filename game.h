#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/Audio.hpp"
#include <vector>
#include "character.h"

class game
{
	char p1_last;
	char p2_last;
	bool p1_turn_running;
	bool p2_turn_running;
	int p1_count,pluser1;
	int p2_count,pluser2;
	bool game_flag;
	sf::RectangleShape player1power;
	sf::RectangleShape player2power;
	sf::RectangleShape player2powerr;
	sf::RectangleShape player1powerr;
	int powerp1ow;
	int powerp2ow;
	int powerp2xp;
	int powerp1xp;
	sf::RectangleShape player2xp;
	sf::RectangleShape player1xp;
	sf::RectangleShape player1xppower;
	sf::RectangleShape player2xppower;
	sf::RenderWindow win;
	sf::Texture bg_texture;
	sf::Sprite bg;
	character* players[2];
	int last;
	sf::Clock clock;
	character c1;
	character c3;
	character* c2;
	//helper for player 1
	int o_w,o_he;
	bool p1_innerflag;
	sf::Vector2f o_pos_p1;
	//helper for player 2
	int o_w2, o_he2;
	sf::Vector2f o_pos_p2;
	std::vector<int>* v1;
	bool p2_innerflag;
	bool c2flag;
	//to find who moved first
	int ffirst;
	bool collision_check_flag1;
	bool collision_check_flag2;
	int c1_current_frame;
	int c3_current_frame;
	char lastp1,lastp2;
	bool punchflag;
	sf::CircleShape cp1[4];
	
	sf::CircleShape cp2[4];
	bool efxflag;
	sf::Texture speffects;
	sf::Sprite specialeffects;
	
	int upcount1, upcount2, upluser1, upluser2;
	bool upturnrunnig1, upturnrunnig2;


	sf::SoundBuffer buffer2;
	sf::Sound sound2;
	sf::SoundBuffer* buffer31;
	sf::Sound* sound31;
	sf::SoundBuffer* buffer312;
	sf::Sound* sound312;
	sf::SoundBuffer buffer1;
	sf::Sound sound1;
	sf::SoundBuffer buffer21;
	sf::Sound sound21;
	sf::SoundBuffer buffer3;
	sf::Sound sound3;

public:
	game();
	~game();
	void start();
	void check_collision_and_do_stuff();
	void player_1_turn();
	int get_p1_current_frame();
	int get_p2_current_frame();
	int get_p1_current_cycle();
	int get_p2_current_cycle();
	void get_punched(int player= 0,int choice = 0);
	void player_2_turn();
	void player1relay();
	void player2relay();
	void checkp1isntnegscale();
	void checkp2isntnegscale();
	void helper();
};

