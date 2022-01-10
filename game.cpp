#include "game.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <SFML/System.hpp>
#include <future>

using namespace std;

static sf::Mutex mu;

game::game():win(sf::VideoMode(1200, 700), "Street fighter")
{	
	win.setFramerateLimit(100);
	//initializing important variables
	this->lastp1 = lastp2 = '0';
	this->upcount1 = upcount2 = upluser1 = upluser2 = 0;
	this->upturnrunnig1 = upturnrunnig2 = false;
	this->efxflag = false;
	speffects.loadFromFile("efx.png");
	this->specialeffects.setTexture(speffects);
	sf::IntRect effectrect;
	effectrect.left = 471;
	effectrect.top = 139;
	effectrect.width = 90;
	effectrect.height = 93;
	this->specialeffects.setTextureRect(effectrect);
	this->specialeffects.setPosition({ 400,10 });
	this->specialeffects.setScale({ 0.5,0.5 });
	sf::Font xpfont;
	if (xpfont.loadFromFile("arial.ttf"))
		cout << "font  loaded" << endl;
	bg_texture.loadFromFile("SuzakuCastle.jpg");
	bg.setScale({ 1.5,2 });
	bg.setTexture(bg_texture);
	this->game_flag = false;
	players[0] = nullptr;
	players[1] = nullptr;
	this->last = 1;
	this->p1_last = '0';
	this->p2_last = '0';
	this->p1_turn_running = false;
	this->p2_turn_running = false;
	this->p1_count = 0;
	this->p2_count = 0;
	this->pluser1 = 0;
	this->pluser2 = 0;
	this->p1_innerflag = false;
	this->p2_innerflag = false;
	this->c2flag = false;
	this->ffirst = 0;
	this->collision_check_flag1 = false;
	this->collision_check_flag2 = false;
	this->punchflag = false;
	this->player1power.setSize({ 400,15 });
	this->player1power.setPosition({ 10,30 });
	this->player1power.setFillColor(sf::Color::Transparent);
	this->player1power.setOutlineThickness(1);
	this->player1power.setOutlineColor(sf::Color::White);
	this->player1powerr.setSize({ 400,15 });
	this->player1powerr.setPosition({ 10,30 });
	this->player1powerr.setFillColor(sf::Color::Blue);
	powerp1ow = powerp2ow = 400;
	powerp1xp = powerp2xp = 0;
	this->player1xp.setSize({ 150,15 });
	this->player1xp.setPosition({ 10,50 });
	this->player1xp.setFillColor(sf::Color::Transparent);
	this->player1xp.setOutlineThickness(1);
	this->player1xp.setOutlineColor(sf::Color::White);
	this->player1xppower.setSize({ 0,15 });
	this->player1xppower.setPosition({ 10,50 });
	this->player1xppower.setFillColor(sf::Color::Yellow);
	this->player2power.setSize({ 400,15 });
	this->player2power.setPosition({ 790,30 });
	this->player2power.setFillColor(sf::Color::Transparent);
	this->player2power.setOutlineThickness(1);
	this->player2power.setOutlineColor(sf::Color::White);
	this->player2powerr.setSize({ 400,15 });
	this->player2powerr.setPosition({ 1190,30 });
	this->player2powerr.setFillColor(sf::Color::Green);
	this->player2powerr.setScale({player2power.getScale().x*-1,player2powerr.getScale().y});
	this->player2xp.setSize({ 150,15 });
	this->player2xp.setPosition({ 1190,50 });
	this->player2xp.setFillColor(sf::Color::Transparent);
	this->player2xp.setOutlineThickness(1);
	this->player2xp.setOutlineColor(sf::Color::White);
	this->player2xp.setScale({ player2xp.getScale().x * -1,player2xp.getScale().y });
	this->player2xppower.setSize({ 0,15 });
	this->player2xppower.setPosition({ 1190,50 });
	this->player2xppower.setFillColor(sf::Color::Yellow);
	this->player2xppower.setScale({ player2xppower.getScale().x * -1,player2xppower.getScale().y });
	float cxx1, cyy1, cxx2, cyy2;
	cxx1 = 10, cyy1 = 70;
	cxx2 = 790; cyy2 = 70;
	for (int i = 0; i < 4; i++)
	{
		cp1[i].setRadius(10);
		cp1[i].setFillColor(sf::Color::Red);
		cp1[i].setOutlineThickness(1);
		cp1[i].setOutlineColor(sf::Color::Black);
		cp1[i].setPosition({ cxx1,cyy1 });
		cxx1 += 30;
		cp2[i].setRadius(10);
		cp2[i].setFillColor(sf::Color::Red);
		cp2[i].setOutlineThickness(1);
		cp2[i].setOutlineColor(sf::Color::Black);
		cp2[i].setPosition({ cxx2,cyy2 });
		cxx2 += 30;
	}


	//sf::SoundBuffer buffer2;
	buffer2.loadFromFile("PUNCH.wav");

	//sf::Sound sound2;
	sound2.setBuffer(buffer2);
	sound2.setVolume(50);
	sound2.setPitch(1.1);

	buffer31 = new sf::SoundBuffer();
	buffer31->loadFromFile("ryuken-uggh.wav");

	sound31 = new sf::Sound();
	sound31->setBuffer(*buffer31);
	sound31->setVolume(50);
	sound31->setPitch(1.1);

	buffer312 = new sf::SoundBuffer();
	buffer312->loadFromFile("blanka-growl.wav");

	sound312 = new sf::Sound();
	sound312->setBuffer(*buffer312);
	sound312->setVolume(50);
	sound312->setPitch(1.1);


	//sf::SoundBuffer buffer1;
	buffer1.loadFromFile("hadouken.wav");

	//sf::Sound sound1;
	sound1.setBuffer(buffer1);
	sound1.setVolume(100);
	sound1.setPitch(1.1);


	//sf::SoundBuffer buffer21;
	buffer21.loadFromFile("shoryuken.wav");

	//sf::Sound sound21;
	sound21.setBuffer(buffer21);
	sound21.setVolume(50);
	sound21.setPitch(1.1);

	//sf::SoundBuffer buffer3;
	buffer3.loadFromFile("ryuken-kick.wav");

	//sf::Sound sound3;
	sound3.setBuffer(buffer3);
	sound3.setVolume(50);
	sound3.setPitch(1.1);

	this->win.setKeyRepeatEnabled(false);
	
}

int game::get_p1_current_frame()
{
	return this->p1_count;
}

int game::get_p2_current_frame()
{
	return this->p2_count;
}

int game::get_p1_current_cycle()
{
	return this->c1_current_frame;
}

int game::get_p2_current_cycle()
{
	return this->c3_current_frame;
}

void game::get_punched(int player, int choice)
{
	


	if (player == 1)
	{
		if (choice == 0)
		{
			if (powerp1ow != 0)
			{
				powerp1ow -= 1;
				this->player1powerr.setSize({ player1powerr.getSize().x - 1,15 });
			}
			if (powerp2xp != 150)
			{
				powerp2xp += 25;
				this->player2xppower.setSize({ player2xppower.getSize().x + 25,15 });
				if (powerp1xp != 0)
				{
					powerp1xp -= 25;
					this->player1xppower.setSize({ player1xppower.getSize().x - 25,15 });
				}
			}
			
			int incount = 0;
			sound2.play();
			int templuser = 0;
			efxflag = true;
			this->specialeffects.setPosition({ c1.sprite->getPosition().x+40,c1.sprite->getPosition().y+40 });
			while (incount < this->c1.get_nframes(22))
			{
				c1.cur_piece.left = c1.moves[22][0 + templuser];
				c1.cur_piece.top = c1.moves[22][1 + templuser];
				c1.cur_piece.width = c1.moves[22][2 + templuser];
				c1.cur_piece.height = c1.moves[22][3 + templuser];
				c1.sprite->setTextureRect(c1.cur_piece);
				templuser += 4;

				Sleep(80);
				incount++;
			}
			efxflag = false;
			c1.cur_piece.top = c1.moves[2][1];
			c1.cur_piece.left = c1.moves[2][0];
			c1.cur_piece.width = c1.moves[2][2];
			c1.cur_piece.height = c1.moves[2][3];
			c1.sprite->setTextureRect(c1.cur_piece);
			if(this->get_p2_current_cycle()!=12)
			    c1.set_position({ c1.sprite->getPosition().x,485 });
			p1_turn_running = false;
			pluser1 = 0;
			p1_count = 0;
			p1_last = 'c';
			p1_innerflag = false;
			
		}
		else if (choice == 1)
		{
			sound31->play();
			this->o_he = c1.cur_piece.height;
			this->o_w = c1.cur_piece.width;
			this->o_pos_p1 = c1.sprite->getPosition();
			o_pos_p1.y = 485;
			int incount = 0;
			int templuser = 0;
			c1.sprite->setScale({ c1.sprite->getScale().x*-1,c1.sprite->getScale().y });
			c1.sprite->setPosition(c1.sprite->getPosition().x, c1.sprite->getPosition().y - 60);
			
			while (incount < this->c1.get_nframes(23)+5)
			{
				if (incount < this->c1.get_nframes(23))
				{
					c1.cur_piece.left = c1.moves[23][0 + templuser];
					c1.cur_piece.top = c1.moves[23][1 + templuser];
					c1.cur_piece.width = c1.moves[23][2 + templuser];
					c1.cur_piece.height = c1.moves[23][3 + templuser];
					c1.sprite->setTextureRect(c1.cur_piece);
				}
				else
				{
					c1.set_position({ c1.sprite->getPosition().x-30,c1.sprite->getPosition().y + 30+0 });
				}
				templuser += 4;

				/*if (incount == 0)
					Sleep(250);
				else
				    Sleep(70);*/
				Sleep(70);
				incount++;
			}
			while (c1.sprite->getPosition().y < 575)
			    c1.set_position({ c1.sprite->getPosition().x,c1.sprite->getPosition().y + 1 });
			//c1.sprite->setScale({ c1.sprite->getScale().x * -1,c1.sprite->getScale().y });
			Sleep(3000);
			lastp1= 'z';
			pluser1 = 0;
			p1_turn_running = false;
			p1_count = 0;
			p1_last = '0';
			this->powerp2xp = 0;
			this->player2xppower.setSize({ 0,15 });
			if (powerp1ow != 0)
			{
				powerp1ow -= 20;
				this->player1powerr.setSize({ player1powerr.getSize().x - 20,15 });
			}
		}
	}
	else if (player == 2)
	{
		if (choice == 0 || choice==2)
		{
			if (powerp2ow != 0)
			{
				powerp2ow -= 1;
				this->player2powerr.setSize({ player2powerr.getSize().x - 1,15 });
			}
			if (powerp1xp != 150)
			{
				powerp1xp += 25;
				this->player1xppower.setSize({ player1xppower.getSize().x + 25,15 });
				if (powerp2xp != 0)
				{
					powerp2xp -= 25;
					this->player2xppower.setSize({ player2xppower.getSize().x - 25,15 });
				}
			}
			int incount = 0;
			int temppluser = 0;
			sound2.play();
			efxflag = true;
			this->specialeffects.setPosition({ c3.sprite->getPosition().x+40,c3.sprite->getPosition().y+10 });
			while (incount < this->c3.get_nframes(23))
			{
				c3.cur_piece.left = c3.moves[23][0 + temppluser];
				c3.cur_piece.top = c3.moves[23][1 + temppluser];
				c3.cur_piece.width = c3.moves[23][2 + temppluser];
				c3.cur_piece.height = c3.moves[23][3 + temppluser];
				c3.sprite->setTextureRect(c3.cur_piece);
				temppluser += 4;

				
				    Sleep(80);
				incount++;
			}
			efxflag = false;
			c3.cur_piece.width = c3.moves[2][2];
			c3.cur_piece.height = c3.moves[2][3];
			c3.cur_piece.left = c3.moves[2][0];
			c3.cur_piece.top = c3.moves[2][1];
			if(choice==0)
			    c3.sprite->setTextureRect(c3.cur_piece);
			
			if(this->get_p1_current_cycle()!=18)
			    c3.set_position({ c3.sprite->getPosition().x,520 });

			p2_turn_running = false;
			pluser2 = 0;
			p2_count = 0;
			p2_last = 'c';
			p2_innerflag = false;
		}
		else if (choice == 1)
		{
			
			sound312->play();
			cout << "here" << endl;
			this->o_he2 = c3.cur_piece.height;
			this->o_w2 = c3.cur_piece.width;
			this->o_pos_p2 = c3.sprite->getPosition();
			o_pos_p2.y = 520;
			int incount = 0;
			int templuser = 0;
			c3.sprite->setScale({ c3.sprite->getScale().x * -1,c3.sprite->getScale().y });
			c3.set_position({ c3.sprite->getPosition().x + 100,c3.sprite->getPosition().y });
			
			while (incount < this->c3.get_nframes(24)+1)
			{
				if (incount < this->c3.get_nframes(24))
				{
					c3.cur_piece.left = c3.moves[24][0 + templuser];
					c3.cur_piece.top = c3.moves[24][1 + templuser];
					c3.cur_piece.width = c3.moves[24][2 + templuser];
					c3.cur_piece.height = c3.moves[24][3 + templuser];
					c3.sprite->setTextureRect(c3.cur_piece);
				}
			if(incount==0)
					c3.set_position({ c3.sprite->getPosition().x + 100,c3.sprite->getPosition().y-100 });
			else
				c3.set_position({ c3.sprite->getPosition().x + 100,c3.sprite->getPosition().y + 60 });
				
				templuser += 4;

				
					Sleep(80);
				
				incount++;
			}
			while(c3.sprite->getPosition().y<600)
				c3.set_position({ c3.sprite->getPosition().x,c3.sprite->getPosition().y + 1 });
			//c1.sprite->setScale({ c1.sprite->getScale().x * -1,c1.sprite->getScale().y });
			Sleep(3000);
			lastp2 = 'z';
			pluser2 = 0;
			p2_turn_running = false;
			p2_count = 0;
			p2_last = '0';
			this->powerp1xp = 0;
			this->player1xppower.setSize({ 0,15 });
			if (powerp2ow != 0)
			{
				powerp2ow -= 20;
				this->player2powerr.setSize({ player2powerr.getSize().x - 20,15 });
			}
		}
	}

	
}

void game::checkp1isntnegscale()
{
	//p1_last != '0' && lastp1 == 'z'
	if (lastp1 == 'z')
	{
		c1.cur_piece.width = o_w;
		c1.cur_piece.height = o_he;
		c1.set_position(o_pos_p1);
		c1.set_position({ c1.sprite->getPosition().x - 200,c1.sprite->getPosition().y });
		c1.sprite->setTextureRect(c1.cur_piece);
		c1.sprite->setScale({ c1.sprite->getScale().x * -1,c1.sprite->getScale().y });
		lastp1 = '0';
	}
}

void game::checkp2isntnegscale()
{
	//p2_last != '0' && lastp2 == 'z'
	if (lastp2 == 'z')
	{
		c3.cur_piece.width = o_w2;
		c3.cur_piece.height = o_he2;
		c3.set_position(o_pos_p2);
		c3.set_position({ c3.sprite->getPosition().x + 150,c3.sprite->getPosition().y });
		c3.sprite->setTextureRect(c1.cur_piece);
		c3.sprite->setScale({ c3.sprite->getScale().x * -1,c3.sprite->getScale().y });
		lastp2 = '0';
	}
}

void game::helper()
{
	this->get_punched(2, 2);

	this->get_punched(2, 1);
}

void game::player_1_turn()
{
	
	
	/*sf::SoundBuffer buffer1;
	buffer1.loadFromFile("hadouken.wav");

	sf::Sound sound1;
	sound1.setBuffer(buffer1);
	sound1.setVolume(100);
	sound1.setPitch(1.1);


	sf::SoundBuffer buffer21;
	buffer21.loadFromFile("shoryuken.wav");

	sf::Sound sound21;
	sound21.setBuffer(buffer21);
	sound21.setVolume(50);
	sound21.setPitch(1.1);

	sf::SoundBuffer buffer3;
	buffer3.loadFromFile("ryuken-kick.wav");

	sf::Sound sound3;
	sound3.setBuffer(buffer3);
	sound3.setVolume(50);
	sound3.setPitch(1.1);*/

	while (game_flag)
	{
		if (collision_check_flag1 == false)
		{
			
			if (p1_last == 'R')
			{
				this->checkp1isntnegscale();
				this->c1_current_frame = 2;
				if (c1.picname == "Ryu.png")
				{
					last = 1;
					if (p1_count == 0)
					{
						c1.set_position({ c1.sprite->getPosition().x,465 });
						pluser1 = 0;
					}


					if (p1_turn_running)
					{

						c1.move_right(win);
						c1.cur_piece.left = c1.moves[2][0 + pluser1];
						c1.cur_piece.top = c1.moves[2][1 + pluser1];
						c1.cur_piece.width = c1.moves[2][2 + pluser1];
						c1.cur_piece.height = c1.moves[2][3 + pluser1];


						c1.sprite->setTextureRect(c1.cur_piece);

						
						pluser1 += 4;

						Sleep(20);
						p1_count++;
						/*win.clear();
						win.draw(bg);
						win.draw(*c3.get_sprite());
						win.draw(*c1.get_sprite());
						win.display();*/
						if (!(p1_count < c1.get_nframes(2)))
						{
							pluser1 = 0;
							p1_turn_running = false;
							p1_count = 0;
							p1_last = '0';
						}


					}

				}
			}
			if (p1_last == 'L')
			{
				this->checkp1isntnegscale();
				this->c1_current_frame = 3;
				last = 1;
				//int count = 0;
				//int pluser = 0;
				if (p1_count == 0)
				{
					c1.set_position({ c1.sprite->getPosition().x,465 });
					pluser1 = 0;
				}
				if (p1_turn_running)
				{
					c1.move_left(win);
					c1.cur_piece.left = c1.moves[3][0 + pluser1];
					c1.cur_piece.top = c1.moves[3][1 + pluser1];
					c1.cur_piece.width = c1.moves[3][2 + pluser1];
					c1.cur_piece.height = c1.moves[3][3 + pluser1];
					c1.sprite->setTextureRect(c1.cur_piece);
					
					pluser1 += 4;

					Sleep(20);
					p1_count++;
					/*win.clear();
					win.draw(bg);
					win.draw(*c3.get_sprite());
					win.draw(*c1.get_sprite());
					win.display();*/
					if (!(p1_count < c1.get_nframes(3)))
					{
						pluser1 = 0;
						p1_turn_running = false;
						p1_count = 0;
						p1_last = '0';
					}
				}
			}
			if (p1_last == 'U')
			{
				this->checkp1isntnegscale();
				this->c1_current_frame = 4;
				last = 1;
				sf::Vector2f pos; int half = 0;
				if (upcount1 == 0)
				{
					pos.x = c1.sprite->getPosition().x;
					pos.y = c1.sprite->getPosition().y;
					half = c1.get_nframes(4) / 2;
				}
				if (this->upturnrunnig1)
				{
					c1.move_up();

					c1.cur_piece.left = c1.moves[4][0 + upluser1];
					c1.cur_piece.top = c1.moves[4][1 + upluser1];
					c1.cur_piece.width = c1.moves[4][2 + upluser1];
					c1.cur_piece.height = c1.moves[4][3 + upluser1];
					c1.sprite->setTextureRect(c1.cur_piece);

					if (p1_count < half)
						c1.set_position({ pos.x,pos.y - 250 });
					else
					{
						if (c1.sprite->getPosition().y + 5 < win.getSize().y - 500)
							c1.set_position({ pos.x,pos.y + 5 });
					}
					
					upluser1 += 4;
					if (upcount1 == 6)
						c1.set_position({ c1.sprite->getPosition().x,485 });
					/*int wait = 35;
					if (p2_last != '0')
						wait = 10;*/
					Sleep(50);
					upcount1++;
					/*win.clear();
					win.draw(bg);
					win.draw(*c3.get_sprite());
					win.draw(*c1.get_sprite());
					win.display();*/
					if (!(upcount1 < c1.get_nframes(4)))
					{
						upluser1 = 0;
						this->upturnrunnig1 = false;
						upcount1 = 0;
						p1_last = '0';
					}
				}

			}
			if (p1_last == 'D')
			{
				this->checkp1isntnegscale();
				this->c1_current_frame = 5;
				last = 1;
				//int count = 0;
				//int pluser = 0;

				if (p1_count == 0)
				{
					o_w = c1.cur_piece.width;
				}
				if (p1_turn_running)
				{

					c1.move_down(win);
					c1.cur_piece.left = c1.moves[5][0 + pluser1];
					c1.cur_piece.top = c1.moves[5][1 + pluser1];
					c1.cur_piece.width = c1.moves[5][2 + pluser1];
					c1.cur_piece.height = c1.moves[5][3 + pluser1];
					c1.sprite->setTextureRect(c1.cur_piece);
					
					pluser1 += 4;
					if (p1_count == 6)
					{
						c1.cur_piece.width = o_w;
						c1.sprite->setTextureRect(c1.cur_piece);
						if (c1.sprite->getPosition().x + 20 < win.getSize().x - 290)
							c1.sprite->setPosition({ c1.sprite->getPosition().x + 20,c1.sprite->getPosition().y });

					}
					/*int wait = 50;
					if (p2_last != '0')
						wait = 10;*/
					Sleep(50);
					p1_count++;
					/*win.clear();
					win.draw(bg);
					win.draw(*c3.get_sprite());
					win.draw(*c1.get_sprite());
					win.display();*/

					if (!(p1_count < c1.get_nframes(5)))
					{
						pluser1 = 0;
						p1_turn_running = false;
						p1_count = 0;
						p1_last = '0';
					}
				}

				c1.set_position({ c1.sprite->getPosition().x,485 });

			}
			if (p1_last == 'A')
			{
				//this->checkp1isntnegscale();
				this->c1_current_frame = 7;
				last = 1;
				//int count = 0, pluser = 0;
				if (p1_count == 0)
				{
					
					c1.sprite->setTextureRect(c1.cur_piece);
				}
				if (p1_turn_running)
				{

					c1.cur_piece.left = c1.moves[7][0 + pluser1];
					c1.cur_piece.top = c1.moves[7][1 + pluser1];
					c1.cur_piece.width = c1.moves[7][2 + pluser1];
					c1.cur_piece.height = c1.moves[7][3 + pluser1];
					c1.sprite->setTextureRect(c1.cur_piece);

					
					pluser1 += 4;
					/*int wait = 50;
					if (p2_last != '0')
						wait = 10;*/
					Sleep(50);
					p1_count++;
					/*win.clear();
					win.draw(bg);
					win.draw(*c3.get_sprite());
					win.draw(*c1.get_sprite());
					win.display();*/
					if (!(p1_count < c1.get_nframes(7)))
					{
						pluser1 = 0;
						p1_turn_running = false;
						p1_count = 0;
						p1_last = lastp1;
						lastp1 = '0';
						c1.cur_piece.left = c1.moves[2][0];
						c1.cur_piece.top = c1.moves[2][1];
						c1.cur_piece.width = c1.moves[2][2];
						c1.cur_piece.height = c1.moves[2][3];
						c1.sprite->setTextureRect(c1.cur_piece);
					}
				}
			}
			if (p1_last == 'S')
			{
				//this->checkp1isntnegscale();
				this->c1_current_frame = 9;
				last = 1;
				//int count = 0;
				//int pluser = 0;
				if (p1_count == 0)
				{
					int o_w = c1.cur_piece.width;
				}
				if (p1_turn_running)
				{
					c1.cur_piece.left = c1.moves[9][0 + pluser1];
					c1.cur_piece.top = c1.moves[9][1 + pluser1];
					c1.cur_piece.width = c1.moves[9][2 + pluser1];
					c1.cur_piece.height = c1.moves[9][3 + pluser1];
					c1.sprite->setTextureRect(c1.cur_piece);
					/*int wait = 50;
					if (p2_last != '0')
						wait = 10;*/
					Sleep(50);
					p1_count++;
					pluser1 += 4;
					/*win.clear();
					win.draw(bg);
					win.draw(*c3.get_sprite());
					win.draw(*c1.get_sprite());
					win.display();*/
					if (!(p1_count < c1.get_nframes(9)))
					{
						pluser1 = 0;
						p1_turn_running = false;
						p1_count = 0;
						p1_last = lastp1;
						lastp1 = '0';
						c1.cur_piece.top = c1.moves[2][1];
						c1.cur_piece.left = c1.moves[2][0];
						c1.cur_piece.width = c1.moves[2][2];
						c1.cur_piece.height = c1.moves[2][3];
						c1.sprite->setTextureRect(c1.cur_piece);
					}
				}
			}
			if (p1_last == 'd')
			{
				this->checkp1isntnegscale();
				this->c1_current_frame = 17;
				last = 1;
				if (p1_count == 0)
				{
					sound21.play();
					o_w = c1.cur_piece.width;
					o_he = c1.cur_piece.height;
				}
				//int count = 0, pluser = 0;
				if (p1_turn_running)
				{
					if (p1_count < 3)
						c1.set_position({ c1.sprite->getPosition().x,c1.sprite->getPosition().y - 50 });

					c1.cur_piece.left = c1.moves[17][0 + pluser1];
					c1.cur_piece.top = c1.moves[17][1 + pluser1];
					c1.cur_piece.width = c1.moves[17][2 + pluser1];
					c1.cur_piece.height = c1.moves[17][3 + pluser1];
					c1.sprite->setTextureRect(c1.cur_piece);

					
					pluser1 += 4;
					/*int wait = 50;
					if (p2_last != '0')
						wait = 10;*/
					Sleep(50);
					p1_count++;
					/*win.clear();
					win.draw(bg);
					win.draw(*c3.get_sprite());
					win.draw(*c1.get_sprite());
					win.display();*/
					if (!(p1_count < c1.get_nframes(17)))
					{
						pluser1 = 0;
						p1_turn_running = false;
						p1_count = 0;
						//p1_last = '0';
						p1_innerflag = true;
					}
				}
				if (p1_innerflag)
				{
					c1.set_position({ c1.sprite->getPosition().x,c1.sprite->getPosition().y + 50 });
					/*int wait = 50;
					if (p2_last != '0')
						wait = 10;*/
					Sleep(50);
					p1_count++;
					/*win.clear();
					win.draw(bg);
					win.draw(*c3.get_sprite());
					win.draw(*c1.get_sprite());
					win.display();*/
					if (!(p1_count < 3))
					{
						p1_count = 0;
						p1_last = '0';
						p1_innerflag = false;
						c1.cur_piece.top = c1.moves[2][1];
						c1.cur_piece.left = c1.moves[2][0];
						c1.cur_piece.width = c1.moves[2][2];
						c1.cur_piece.height = c1.moves[2][3];
						c1.sprite->setTextureRect(c1.cur_piece);
					}
				}
			}
			if (p1_last == 'F')
			{
				this->checkp1isntnegscale();
				this->c1_current_frame = 18;
				last = 1;
				if (p1_count == 0)
				{
					sound3.play();
					if (c1.sprite->getPosition().x + 50 < win.getSize().x - 290)
						c1.sprite->setPosition({ c1.sprite->getPosition().x + 50,c1.sprite->getPosition().y });
				}
				//int count = 0, pluser = 0;

				if (p1_turn_running)
				{
					c1.movement(18, p1_count);

					if (p1_count < 3)
						c1.set_position({ c1.sprite->getPosition().x,c1.sprite->getPosition().y - 40 });

					c1.cur_piece.left = c1.moves[18][0 + pluser1];
					c1.cur_piece.top = c1.moves[18][1 + pluser1];
					c1.cur_piece.width = c1.moves[18][2 + pluser1];
					c1.cur_piece.height = c1.moves[18][3 + pluser1];
					c1.sprite->setTextureRect(c1.cur_piece);

					/*int wait = 50;
					if (p2_last != '0')
						wait = 10;*/
					Sleep(50);
					p1_count++;
					pluser1 += 4;
					/*win.clear();
					win.draw(bg);
					win.draw(*c3.get_sprite());
					win.draw(*c1.get_sprite());
					win.display();*/
					if (!(p1_count < c1.get_nframes(18)))
					{
						pluser1 = 0;
						p1_turn_running = false;
						p1_count = 0;
						//p1_last = '0';
						p1_innerflag = true;
					}
				}
				if (p1_innerflag)
				{
					c1.set_position({ c1.sprite->getPosition().x,c1.sprite->getPosition().y + 40 });

					/*int wait = 50;
					if (p2_last != '0')
						wait = 10;*/
					Sleep(50);
					p1_count++;
					/*win.clear();
					win.draw(bg);
					win.draw(*c3.get_sprite());
					win.draw(*c1.get_sprite());
					win.display();*/
					if (!(p1_count < 3))
					{
						p1_count = 0;
						p1_last = '0';
						p1_innerflag = false;
						c1.cur_piece.top = c1.moves[2][1];
						c1.cur_piece.left = c1.moves[2][0];
						c1.cur_piece.width = c1.moves[2][2];
						c1.cur_piece.height = c1.moves[2][3];
						c1.sprite->setTextureRect(c1.cur_piece);
					}
				}
			}
			if (p1_last == 'G')
			{
				
				sf::Thread tg(&game::helper, this);
				bool tgflag = true;
				//std::thread* t2punched=0;

				this->checkp1isntnegscale();
				this->c1_current_frame = 16;
				last = 1;
				//int count = 0, pluser = 0;
				if (p1_count == 0)
				{
					o_w = c1.cur_piece.width;
					o_he = c1.cur_piece.height;
					c1.cur_piece.width = o_w + 20;
					c1.cur_piece.left += 8;
					c1.sprite->setTextureRect(c1.cur_piece);
				}


				if (p1_turn_running)
				{
					c1.cur_piece.left = c1.moves[16][0 + pluser1];
					c1.cur_piece.top = c1.moves[16][1 + pluser1];
					c1.cur_piece.width = c1.moves[16][2 + pluser1];
					c1.cur_piece.height = c1.moves[16][3 + pluser1];
					c1.sprite->setTextureRect(c1.cur_piece);
					
					pluser1 += 4;
					/*win.clear();
					win.draw(bg);
					win.draw(*c3.get_sprite());
					win.draw(*c1.get_sprite());
					win.display();*/
					/*int wait = 100;
					if (p2_last != '0')
						wait = 10;*/
					Sleep(100);
					p1_count++;
					if (!(p1_count < c1.get_nframes(16)))
					{
						pluser1 = 0;
						p1_turn_running = false;
						p1_count = 0;
						p1_last = '0';
						p1_innerflag = true;
					}
				}
				if (p1_innerflag)
				{

					sound1.play();
					c1.cur_piece.top = c1.moves[2][1];
					c1.cur_piece.left = c1.moves[2][0];
					c1.cur_piece.width = c1.moves[2][2];
					c1.cur_piece.height = c1.moves[2][3];
					c1.sprite->setTextureRect(c1.cur_piece);
					int out_count = 0;
					while (out_count < 2)
					{
						c2 = new character("Ryu.png", 33, 8, 637, 3494);
						c2->set_index_cycle(27, 8);
						c2->set_index_left(27, -3);
						c2->set_index_top(27, 268);

						if (out_count == 0)
							c2->set_position({ c1.sprite->getPosition().x + 120,c1.sprite->getPosition().y + 30 });
						else
							c2->set_position({ c1.sprite->getPosition().x + 120 + 280,c1.sprite->getPosition().y + 30 });

						c2->setscale({ 3,3 });
						c2->cur_piece.height = 58;
						c2->cur_piece.width += 6;
						c2->sprite->setTextureRect(c2->cur_piece);
						int count = 0;
						while (count < c2->get_nframes(27))
						{
							c2->movement(27, count);

							c2->set_position({ c2->sprite->getPosition().x + 30,c2->sprite->getPosition().y });
							if (count == 1)
							{
								c2->cur_piece.left += 4;
								c2->cur_piece.width -= 10;
								c2->sprite->setTextureRect(c2->cur_piece);
							}
							else if (count == 2)
							{
								c2->cur_piece.width += 10;
								c2->cur_piece.left -= 4;
								c2->cur_piece.width -= 6;
								c2->sprite->setTextureRect(c2->cur_piece);
							}
							else if (count == 3)
							{
								c2->cur_piece.width += 6;
								c2->cur_piece.left -= 6;
								c2->sprite->setTextureRect(c2->cur_piece);
							}
							else if (count == 4)
							{
								c2->cur_piece.width -= 6;
								c2->cur_piece.left += 6;
								c2->sprite->setTextureRect(c2->cur_piece);
							}
							c2flag = true;
							/*int wait = 50;
							if (p2_last != '0')
								wait = 10;*/

							Sleep(40);
							count++;
							/*win.clear();
							win.draw(bg);
							win.draw(*c3.get_sprite());
							win.draw(*c2.get_sprite());
							win.draw(*c1.get_sprite());
							win.display();*/

							if (
								((c3.sprite->getPosition().x - c2->sprite->getPosition().x >= 0 && c3.sprite->getPosition().x - c2->sprite->getPosition().x <= 140) ||
								(c2->sprite->getPosition().x - c3.sprite->getPosition().x >= 0 && c2->sprite->getPosition().x - c3.sprite->getPosition().x <= 140))
								//( c3.sprite->getPosition().x <= (c1.sprite->getPosition().x + c1.cur_piece.width) && c3.sprite->getPosition().x > c1.sprite->getPosition().x
								// || c1.sprite->getPosition().x <= (c3.sprite->getPosition().x + c3.cur_piece.width) && c1.sprite->getPosition().x > c3.sprite->getPosition().x ) 
								&&
								(
								(c3.sprite->getPosition().y - c2->sprite->getPosition().y <= 100 && c3.sprite->getPosition().y - c2->sprite->getPosition().y >= 0) ||
									(c2->sprite->getPosition().y - c3.sprite->getPosition().y <= 100 && c2->sprite->getPosition().y - c3.sprite->getPosition().y >= 0)
									)
								)
							{
								if (lastp2 != 'z')
								{
									if (tgflag)
									{
										tgflag = false;
										//this->get_punched(2, 2);
										//this->get_punched(2, 1);
										tg.launch();
									}
									
								}
							}

						}
						out_count++;
						count = 0;
						c2flag = false;
						
						delete c2;

					}
					p1_innerflag = false;
				}

				tg.wait();
				tg.terminate();
			}
			if (p1_last == 'Q')
			{
				this->checkp1isntnegscale();
				this->c1_current_frame = 10;
				last = 1;
				//int count = 0, pluser = 0;
				if (p1_count == 0)
				{
					sound2.play();
					o_w = c1.cur_piece.width;
					o_he = c1.cur_piece.height;
				}
				if (p1_turn_running)
				{
					c1.cur_piece.left = c1.moves[10][0 + pluser1];
					c1.cur_piece.top = c1.moves[10][1 + pluser1];
					c1.cur_piece.width = c1.moves[10][2 + pluser1];
					c1.cur_piece.height = c1.moves[10][3 + pluser1];
					c1.sprite->setTextureRect(c1.cur_piece);
					if (p1_count == 4)
					{
						int val = 100;
						if (c1.sprite->getPosition().x + 100 < win.getSize().x - 290)
							val = 100;
						else
							val = 0;
						c1.set_position({ c1.sprite->getPosition().x + val,c1.sprite->getPosition().y + 100 });
						c1.sprite->setTextureRect(c1.cur_piece);
					}
					else if (p1_count == 3)
					{
						c1.set_position({ c1.sprite->getPosition().x,c1.sprite->getPosition().y - 250 });
					}
					else if (p1_count == 5)
					{
						int val = 100;
						if (c1.sprite->getPosition().x + 100 < win.getSize().x - 290)
							val = 100;
						else
							val = 0;
						c1.set_position({ c1.sprite->getPosition().x + val,c1.sprite->getPosition().y + 100 });
						c1.sprite->setTextureRect(c1.cur_piece);
					}


					if (p1_count != 2 && p1_count != 5)
					{

						Sleep(50);
					}
					else
					{

						Sleep(100);
					}

					p1_count++;
					pluser1 += 4;
					/*win.clear();
					win.draw(bg);
					win.draw(*c3.get_sprite());
					win.draw(*c1.get_sprite());
					win.display();*/



				}

				if (!(p1_count < c1.get_nframes(10)))
				{
					int val = 50;
					if (c1.sprite->getPosition().x + 50 < win.getSize().x - 290)
						val = 50;
					else
						val = 0;
					c1.set_position({ c1.sprite->getPosition().x + val,c1.sprite->getPosition().y + 40 });

					pluser1 = 0;
					p1_turn_running = false;
					p1_count = 0;
					p1_last = '0';
					c1.cur_piece.top = c1.moves[2][1];
					c1.cur_piece.left = c1.moves[2][0];
					c1.cur_piece.width = c1.moves[2][2];
					c1.cur_piece.height = c1.moves[2][3];
					c1.sprite->setTextureRect(c1.cur_piece);
					c1.set_position({ c1.sprite->getPosition().x,485 });

					/*win.clear();
					win.draw(bg);
					win.draw(*c3.get_sprite());
					win.draw(*c1.get_sprite());
					win.display();*/

				}
			}
			if (p1_last == 'W')
			{
				this->checkp1isntnegscale();
				this->c1_current_frame = 11;
				last = 1;
				//int count = 0, pluser = 0;
				if (p1_count == 0)
				{
					sound3.play();
					o_w = c1.cur_piece.width;
					o_he = c1.cur_piece.height;
				}
				if (p1_turn_running)
				{
					c1.cur_piece.left = c1.moves[11][0 + pluser1];
					c1.cur_piece.top = c1.moves[11][1 + pluser1];
					c1.cur_piece.width = c1.moves[11][2 + pluser1];
					c1.cur_piece.height = c1.moves[11][3 + pluser1];
					c1.sprite->setTextureRect(c1.cur_piece);
					if (p1_count == 0)
					{
						c1.set_position({ c1.sprite->getPosition().x,c1.sprite->getPosition().y - 250 });
						c1.sprite->setTextureRect(c1.cur_piece);
					}
					else if (p1_count == 1)
					{

						int val = 50;
						if (c1.sprite->getPosition().x + 50 < win.getSize().x - 290)
							val = 50;
						else
							val = 0;
						c1.set_position({ c1.sprite->getPosition().x + val,c1.sprite->getPosition().y + 60 });
					}
					else if (p1_count == 2)
					{
						int val = 50;
						if (c1.sprite->getPosition().x + 50 < win.getSize().x - 290)
							val = 50;
						else
							val = 0;
						c1.set_position({ c1.sprite->getPosition().x + val,c1.sprite->getPosition().y + 60 });
					}
					else if (p1_count == 4)
					{

						int val = 50;
						if (c1.sprite->getPosition().x + 50 < win.getSize().x - 290)
							val = 50;
						else
							val = 0;
						c1.set_position({ c1.sprite->getPosition().x + val,c1.sprite->getPosition().y + 60 });
						c1.sprite->setTextureRect(c1.cur_piece);
					}
					else if (p1_count == 3)
					{
						int val = 50;
						if (c1.sprite->getPosition().x + 50 < win.getSize().x - 290)
							val = 50;
						else
							val = 0;
						c1.set_position({ c1.sprite->getPosition().x + val,c1.sprite->getPosition().y + 60 });
						c1.sprite->setTextureRect(c1.cur_piece);
					}
					else if (p1_count == 5)
					{

						int val = 50;
						if (c1.sprite->getPosition().x + 50 < win.getSize().x - 290)
							val = 50;
						else
							val = 0;
						c1.set_position({ c1.sprite->getPosition().x + val,c1.sprite->getPosition().y + 60 });
						c1.sprite->setTextureRect(c1.cur_piece);
					}


					if (p1_count != 0 && p1_count != 4)
					{

						Sleep(70);
					}
					else
					{
						if (p1_count == 0)
						{

							Sleep(100);
						}
						else
						{

							Sleep(100);
						}
					}

					p1_count++;
					pluser1 += 4;
					/*win.clear();
					win.draw(bg);
					win.draw(*c3.get_sprite());
					win.draw(*c1.get_sprite());
					win.display();*/



					



					if (!(p1_count < c1.get_nframes(11)))
					{
						pluser1 = 0;
						p1_turn_running = false;
						p1_count = 0;
						p1_last = '0';
						c1.cur_piece.top = c1.moves[2][1];
						c1.cur_piece.left = c1.moves[2][0];
						c1.cur_piece.width = c1.moves[2][2];
						c1.cur_piece.height = c1.moves[2][3];
						c1.sprite->setTextureRect(c1.cur_piece);
						c1.set_position({ c1.sprite->getPosition().x,485 });
					}
				}
			}
			if (p1_last == 'E')
			{
				this->checkp1isntnegscale();
				this->c1_current_frame = 12;
				last = 1;
				//int count = 0, pluser = 0;
				if (p1_count == 0)
				{
					int o_w = c1.cur_piece.width;
					int o_he = c1.cur_piece.height;
					o_pos_p1 = c1.sprite->getPosition();
				}

				if (p1_turn_running)
				{
					c1.cur_piece.left = c1.moves[12][0 + pluser1];
					c1.cur_piece.top = c1.moves[12][1 + pluser1];
					c1.cur_piece.width = c1.moves[12][2 + pluser1];
					c1.cur_piece.height = c1.moves[12][3 + pluser1];
					c1.sprite->setTextureRect(c1.cur_piece);
					if (p1_count == 0)
					{
						c1.sprite->setPosition({ c1.sprite->getPosition().x,c1.sprite->getPosition().y + 24 });
					}
					
					pluser1 += 4;
					/*win.clear();
					win.draw(bg);
					win.draw(*c3.get_sprite());
					win.draw(*c1.get_sprite());
					win.display();*/

					/*int wait = 10;
					if (p2_last != '0')
						Sleep(wait);
					else
						Sleep(60);*/

					Sleep(40);
					p1_count++;
					if (!(p1_count < c1.get_nframes(11)))
					{
						pluser1 = 0;
						p1_turn_running = false;
						p1_count = 0;
						p1_last = '0';
						c1.cur_piece.top = c1.moves[2][1];
						c1.cur_piece.left = c1.moves[2][0];
						c1.cur_piece.width = c1.moves[2][2];
						c1.cur_piece.height = c1.moves[2][3];
						c1.sprite->setTextureRect(c1.cur_piece);
						c1.sprite->setPosition(o_pos_p1);
						c1.set_position({ c1.sprite->getPosition().x,485 });
					}
				}
			}
			
		}
		

	}
	
	
}

void game::player_2_turn()
{
	cout << "player 2 thread works" << endl;
	sf::SoundBuffer buffer2;
	buffer2.loadFromFile("blanka-roar.wav");

	sf::Sound sound2;
	sound2.setBuffer(buffer2);
	sound2.setVolume(25);
	sound2.setPitch(1);

	while (game_flag)
	{
		if (collision_check_flag2 == false)
		{
			if (p2_last == 'J')
			{
				this->checkp2isntnegscale();
				this->c3_current_frame = 2;
				last = 2;
				if (p2_turn_running)
				{
					if (c3.sprite->getPosition().x - 20 > 0)
						c3.set_position({ c3.sprite->getPosition().x - 20,c3.sprite->getPosition().y });

					c3.cur_piece.left = c3.moves[2][0 + pluser2];
					c3.cur_piece.top = c3.moves[2][1 + pluser2];
					c3.cur_piece.width = c3.moves[2][2 + pluser2];
					c3.cur_piece.height = c3.moves[2][3 + pluser2];
					c3.sprite->setTextureRect(c3.cur_piece);


					 pluser2 += 4;

					Sleep(25);
					p2_count++;
					/*win.clear();
					win.draw(bg);
					win.draw(*c1.get_sprite());
					win.draw(*c3.get_sprite());
					win.display();*/
					if (!(p2_count < c3.get_nframes(2)))
					{
						p2_turn_running = false;
						pluser2 = 0;
						p2_count = 0;
						p2_last = '0';
					}
				}
			}
			if (p2_last == 'L')
			{
				this->checkp2isntnegscale();
				this->c3_current_frame = 3;
				last = 2;

				if (p2_turn_running)
				{
					if (c3.sprite->getPosition().x + 20 < win.getSize().x - 290)
						c3.set_position({ c3.sprite->getPosition().x + 20,c3.sprite->getPosition().y });
					c3.cur_piece.left = c3.moves[3][0 + pluser2];
					c3.cur_piece.top = c3.moves[3][1 + pluser2];
					c3.cur_piece.width = c3.moves[3][2 + pluser2];
					c3.cur_piece.height = c3.moves[3][3 + pluser2];
					c3.sprite->setTextureRect(c3.cur_piece);

					
					pluser2 += 4;

					Sleep(25);
					p2_count++;
					/*win.clear();
					win.draw(bg);
					win.draw(*c1.get_sprite());
					win.draw(*c3.get_sprite());
					win.display();*/

					if (!(p2_count < c3.get_nframes(3)))
					{
						p2_turn_running = false;
						pluser2 = 0;
						p2_count = 0;
						p2_last = '0';
					}

				}
			}
			if (p2_last == 'I')
			{
				this->checkp2isntnegscale();
				this->c3_current_frame = 5;
				last = 2;
				//int count = 0, pluser = 0;
				if (upcount2 == 0)
				{
					o_w2 = c3.cur_piece.width;
					o_he2 = c3.cur_piece.height;
				}
				if (this->upturnrunnig2)
				{

					c3.cur_piece.left = c3.moves[5][0 + upluser2];
					c3.cur_piece.top = c3.moves[5][1 + upluser2];
					c3.cur_piece.width = c3.moves[5][2 + upluser2];
					c3.cur_piece.height = c3.moves[5][3 + upluser2];
					c3.sprite->setTextureRect(c3.cur_piece);

					if (upcount2 == 0)
					{
						c3.set_position({ c3.sprite->getPosition().x,c3.sprite->getPosition().y - 280 });
					}
					else if (upcount2 == 3)
					{
						c3.set_position({ c3.sprite->getPosition().x,c3.sprite->getPosition().y + 80 });
					}
					else if (upcount2 == 4)
					{
						c3.set_position({ c3.sprite->getPosition().x,c3.sprite->getPosition().y + 80 });
					}

					
					upluser2 += 4;

					if (upcount2 != 4)
					{
						Sleep(50);

					}
					else
					{
						//Sleep(50);

						/*win.clear();
						win.draw(bg);
						win.draw(*c1.get_sprite());
						win.draw(*c3.get_sprite());
						win.display();*/



						c3.cur_piece.width = o_w2;
						c3.cur_piece.height = o_he2;
						c3.cur_piece.left = c3.dimX - (c3.dx_onepiece + 10);
						c3.cur_piece.top = c3.dimY / c3.no_cycles * 2;
						c3.sprite->setTextureRect(c3.cur_piece);
						c3.set_position({ c3.sprite->getPosition().x,520 });
						Sleep(50);
					}


					upcount2++;

					/*win.clear();
					win.draw(bg);
					win.draw(*c1.get_sprite());
					win.draw(*c3.get_sprite());
					win.display();*/

					if (!(upcount2 < c3.get_nframes(5)))
					{
						this->upturnrunnig2 = false;
						upluser2 = 0;
						upcount2 = 0;
						p2_last = '0';
					}

				}

			}
			if (p2_last == 'B')
			{
				this->checkp2isntnegscale();
				this->c3_current_frame = 6;
				last = 2;
				//int count = 0, pluser = 0;
				if (p2_count == 0)
				{
					o_w2 = c3.cur_piece.width;
					o_he2 = c3.cur_piece.height;
				}
				if (p2_turn_running)
				{
					c3.cur_piece.left = c3.moves[6][0 + pluser2];
					c3.cur_piece.top = c3.moves[6][1 + pluser2];
					c3.cur_piece.width = c3.moves[6][2 + pluser2];
					c3.cur_piece.height = c3.moves[6][3 + pluser2];
					c3.sprite->setTextureRect(c3.cur_piece);

					 pluser2 += 4;

					/*int wait = 45;
					if (p1_last != '0')
						wait = 10;*/

					if (p2_count != 2)
					{

						Sleep(40);
					}
					else
					{

						Sleep(40);
						/*win.clear();
						win.draw(bg);
						win.draw(*c1.get_sprite());
						win.draw(*c3.get_sprite());
						win.display();*/
						
						Sleep(40);
						c3.cur_piece.width = o_w2;
						c3.cur_piece.height = o_he2;
						c3.cur_piece.left = c3.dimX - (2 * c3.dx_onepiece + 10);
						c3.cur_piece.top = c3.dimY / c3.no_cycles * 2;
						c3.sprite->setTextureRect(c3.cur_piece);
						if (c3.sprite->getPosition().x + 10 < win.getSize().x - 290)
							c3.set_position({ c3.sprite->getPosition().x + 10,520 });
						//break;
					}
					p2_count++;
					/*win.clear();
					win.draw(bg);
					win.draw(*c1.get_sprite());
					win.draw(*c3.get_sprite());
					win.display();*/

					if (!(p2_count < c3.get_nframes(6)))
					{
						p2_turn_running = false;
						pluser2 = 0;
						p2_count = 0;
						p2_last = '0';
					}

				}

			}
			if (p2_last == 'K')
			{
				//this->checkp2isntnegscale();
				this->c3_current_frame = 10;
				last = 2;
				//int count = 0, pluser = 0;
				if (p2_count == 0)
				{
					o_w2 = c3.cur_piece.width;
					o_he2 = c3.cur_piece.height;
					o_pos_p2 = c3.sprite->getPosition();
					//c3.set_position({ c3.sprite->getPosition().x - 50,530 });
					c3.set_position({ c3.sprite->getPosition().x - 50,c3.sprite->getPosition().y });
					v1 = new vector<int>[c3.get_nframes(10)];
				}


				if (p2_turn_running)
				{
					c3.cur_piece.left = c3.moves[10][0 + pluser2];
					c3.cur_piece.top = c3.moves[10][1 + pluser2];
					c3.cur_piece.width = c3.moves[10][2 + pluser2];
					c3.cur_piece.height = c3.moves[10][3 + pluser2];
					c3.sprite->setTextureRect(c3.cur_piece);

					v1[p2_count].push_back(c3.cur_piece.left);
					v1[p2_count].push_back(c3.cur_piece.top);
					v1[p2_count].push_back(c3.cur_piece.width);
					v1[p2_count].push_back(c3.cur_piece.height);

					 pluser2 += 4;

					Sleep(30);
					p2_count++;
					/*win.clear();
					win.draw(bg);
					win.draw(*c1.get_sprite());
					win.draw(*c3.get_sprite());
					win.display();*/

					if (!(p2_count < c3.get_nframes(10)))
					{
						p2_turn_running = false;
						pluser2 = 0;
						p2_count = c3.get_nframes(10) - 1;
						p2_innerflag = true;
						//p2_last = '0';
					}

				}

				if (p2_innerflag)
				{

					/*c3.cur_piece.left = v1[p2_count][0];
					c3.cur_piece.top = v1[p2_count][1];
					c3.cur_piece.width = v1[p2_count][2];
					c3.cur_piece.height = v1[p2_count][3];

					c3.sprite->setTextureRect(c3.cur_piece);
					

					Sleep(30);
					p2_count--;*/
					
					/*win.clear();
					win.draw(bg);
					win.draw(*c1.get_sprite());
					win.draw(*c3.get_sprite());
					win.display();*/

					if (true/*!(p2_count > 0)*/)
					{
						p2_turn_running = false;
						pluser2 = 0;
						p2_count = 0;
						p2_innerflag = false;
						

						c3.cur_piece.width = o_w2;
						c3.cur_piece.height = o_he2;
						c3.cur_piece.left = c3.dimX - (2 * c3.dx_onepiece + 10);
						c3.cur_piece.top = c3.dimY / c3.no_cycles * 2;
						c3.sprite->setTextureRect(c3.cur_piece);
						//c3.set_position({ o_pos_p2.x,520 });
						c3.set_position(o_pos_p2);
						Sleep(126);
						p2_last = lastp2;
						lastp2 = '0';
					}

				}
			}
			if (p2_last == 'N')
			{
				this->checkp2isntnegscale();
				this->c3_current_frame = 17;
				last = 2;
				//int count = 0, pluser = 0;
				if (p2_count == 0)
				{
					sound2.play();
					o_w2 = c3.cur_piece.width;
					o_he2 = c3.cur_piece.height;
					o_pos_p2 = c3.sprite->getPosition();
					c3.set_position({ c3.sprite->getPosition().x - 70,o_pos_p2.y });
					v1 = new vector<int>[c3.get_nframes(17)];
				}
				if (p2_turn_running)
				{
					c3.cur_piece.left = c3.moves[17][0 + pluser2];
					c3.cur_piece.top = c3.moves[17][1 + pluser2];
					c3.cur_piece.width = c3.moves[17][2 + pluser2];
					c3.cur_piece.height = c3.moves[17][3 + pluser2];
					c3.sprite->setTextureRect(c3.cur_piece);

					v1[p2_count].push_back(c3.cur_piece.left);
					v1[p2_count].push_back(c3.cur_piece.top);
					v1[p2_count].push_back(c3.cur_piece.width);
					v1[p2_count].push_back(c3.cur_piece.height);

					 pluser2 += 4;
					/*int wait = 80;
					if (p1_last != '0')
						wait = 10;*/

					Sleep(100);
					p2_count++;
					/*win.clear();
					win.draw(bg);
					win.draw(*c1.get_sprite());
					win.draw(*c3.get_sprite());
					win.display();*/

					if (!(p2_count < c3.get_nframes(17)))
					{
						p2_turn_running = false;
						pluser2 = 0;
						p2_count = c3.get_nframes(17) - 1;
						p2_innerflag = true;
						//p2_last = '0';
						
					}

				}

				if (p2_innerflag)
				{

					c3.cur_piece.left = v1[p2_count][0];
					c3.cur_piece.top = v1[p2_count][1];
					c3.cur_piece.width = v1[p2_count][2];
					c3.cur_piece.height = v1[p2_count][3];

					c3.sprite->setTextureRect(c3.cur_piece);
					
					/*int wait = 80;
					if (p1_last != '0')
						wait = 10;*/
					Sleep(100);
					p2_count--;
					/*win.clear();
					win.draw(bg);
					win.draw(*c1.get_sprite());
					win.draw(*c3.get_sprite());
					win.display();*/

					if (!(p2_count > 0))
					{
						p2_turn_running = false;
						pluser2 = 0;
						p2_count = 0;
						p2_innerflag = false;
						
						/*int wait = 40;
						if (p1_last != '0')
							wait = 10;*/
						Sleep(100);
						c3.cur_piece.width = o_w2;
						c3.cur_piece.height = o_he2;
						c3.cur_piece.left = c3.dimX - (2 * c3.dx_onepiece + 10);
						c3.cur_piece.top = c3.dimY / c3.no_cycles * 2;
						c3.sprite->setTextureRect(c3.cur_piece);
						c3.set_position({ o_pos_p2.x,520 });
						p2_last = '0';

					}
				}
			}
			if (p2_last == 'M')
			{
				this->checkp2isntnegscale();
				this->c3_current_frame = 18;
				last = 2;
				//int count = 0, pluser = 0;
				if (p2_count == 0)
				{
					sound2.play();
					o_w2 = c3.cur_piece.width;
					o_he2 = c3.cur_piece.height;
					o_pos_p2 = c3.sprite->getPosition();
					v1 = new vector<int>[c3.get_nframes(18)];
				}
				if (p2_turn_running)
				{
					if (p2_count > 0)
					{
						if (c3.sprite->getPosition().x - 40 > 0)
							c3.set_position({ c3.sprite->getPosition().x - 40,c3.sprite->getPosition().y });
					}

					c3.cur_piece.left = c3.moves[18][0 + pluser2];
					c3.cur_piece.top = c3.moves[18][1 + pluser2];
					c3.cur_piece.width = c3.moves[18][2 + pluser2];
					c3.cur_piece.height = c3.moves[18][3 + pluser2];
					c3.sprite->setTextureRect(c3.cur_piece);

					v1[p2_count].push_back(c3.cur_piece.left);
					v1[p2_count].push_back(c3.cur_piece.top);
					v1[p2_count].push_back(c3.cur_piece.width);
					v1[p2_count].push_back(c3.cur_piece.height);

				 pluser2 += 4;
					/*int wait = 30;
					if (p1_last != '0')
						wait = 10;*/
					Sleep(30);
					p2_count++;
					/*win.clear();
					win.draw(bg);
					win.draw(*c1.get_sprite());
					win.draw(*c3.get_sprite());
					win.display();*/

					if (!(p2_count < c3.get_nframes(18)))
					{
						p2_turn_running = false;
						pluser2 = 0;
						p2_count = 1;
						p2_innerflag = true;
						//p2_last = '0';
						
					}

				}

				if (p2_innerflag)
				{
					if (p2_count > 0)
					{
						if (c3.sprite->getPosition().x - 40 > 0)
							c3.set_position({ c3.sprite->getPosition().x - 40,c3.sprite->getPosition().y });
					}
					c3.cur_piece.left = v1[p2_count][0];
					c3.cur_piece.top = v1[p2_count][1];
					c3.cur_piece.width = v1[p2_count][2];
					c3.cur_piece.height = v1[p2_count][3];

					c3.sprite->setTextureRect(c3.cur_piece);

					
					/*int wait = 30;
					if (p1_last != '0')
						wait = 10;*/
					Sleep(30);
					p2_count++;
					/*win.clear();
					win.draw(bg);
					win.draw(*c1.get_sprite());
					win.draw(*c3.get_sprite());
					win.display();*/

					if (!(p2_count < c3.get_nframes(18)))
					{
						p2_turn_running = false;
						pluser2 = 0;
						p2_count = 0;
						p2_innerflag = false;
						
						/*int wait = 120;
						if (p1_last != '0')
							wait = 10;*/
						Sleep(30);
						c3.cur_piece.width = o_w2;
						c3.cur_piece.height = o_he2;
						c3.cur_piece.left = c3.dimX - (2 * c3.dx_onepiece + 10);
						c3.cur_piece.top = c3.dimY / c3.no_cycles * 2;
						c3.sprite->setTextureRect(c3.cur_piece);
						c3.set_position({ c3.sprite->getPosition().x,520 });
						p2_last = '0';
					}

				}
			}
			if (p2_last == 'O')
			{
				this->checkp2isntnegscale();
				this->c3_current_frame = 12;
				last = 2;
				//int count = 0; int pluser = 0;
				if (p2_count == 0)
				{
					sound2.play();
					o_w2 = c3.cur_piece.width;
					o_he2 = c3.cur_piece.height;
					o_pos_p2 = c3.sprite->getPosition();
				}
				if (p2_turn_running)
				{
					c3.cur_piece.left = c3.moves[12][0 + pluser2];
					c3.cur_piece.top = c3.moves[12][1 + pluser2];
					c3.cur_piece.width = c3.moves[12][2 + pluser2];
					c3.cur_piece.height = c3.moves[12][3 + pluser2];
					c3.sprite->setTextureRect(c3.cur_piece);

					if (p2_count == 2)
					{
						c3.set_position({ c3.sprite->getPosition().x ,c3.sprite->getPosition().y - 200 });
					}
					else if (p2_count == 3)
					{
						if (c3.sprite->getPosition().x - 150 > 0)
							c3.set_position({ c3.sprite->getPosition().x - 150 ,c3.sprite->getPosition().y + 150 });
						else
							c3.set_position({ c3.sprite->getPosition().x ,c3.sprite->getPosition().y + 150 });
					}

					if (p2_count == 0 || p2_count == 1)
					{
						/*int wait = 200;
						if (p1_last != '0')
							wait = 60;*/
						Sleep(100);
					}
					else
					{
						/*int wait = 100;
						if (p1_last != '0')
							wait = 60;*/
						Sleep(100);
					}

					p2_count++; pluser2 += 4;
					/*win.clear();
					win.draw(bg);
					win.draw(*c1.get_sprite());
					win.draw(*c3.get_sprite());
					win.display();*/

					if (!(p2_count < c3.get_nframes(12)))
					{
						p2_turn_running = false;
						pluser2 = 0;
						p2_count = 0;
						
						/*int wait = 500;
						if (p1_last != '0')
							wait = 60;*/
						Sleep(100);
						c3.cur_piece.width = o_w2;
						c3.cur_piece.height = o_he2;
						c3.cur_piece.left = c3.dimX - (2 * c3.dx_onepiece + 10);
						c3.cur_piece.top = c3.dimY / c3.no_cycles * 2;
						c3.sprite->setTextureRect(c3.cur_piece);
						c3.set_position({ c3.sprite->getPosition().x,520 });
						p2_last = '0';
					}

				}
			}
			if (p2_last == 'P')
			{
				this->checkp2isntnegscale();
				this->c3_current_frame = 11;
				last = 2;
				//int count = 0; int pluser = 0;
				if (p2_count == 0)
				{
					o_w2 = c3.cur_piece.width;
					o_he2 = c3.cur_piece.height;
				}

				if (p2_turn_running)
				{

					c3.cur_piece.left = c3.moves[11][0 + pluser2];
					c3.cur_piece.top = c3.moves[11][1 + pluser2];
					c3.cur_piece.width = c3.moves[11][2 + pluser2];
					c3.cur_piece.height = c3.moves[11][3 + pluser2];
					c3.sprite->setTextureRect(c3.cur_piece);

					if (p2_count == 1)
					{
						c3.set_position({ c3.sprite->getPosition().x - 60,c3.sprite->getPosition().y });
					}
					else if (p2_count == 2)
					{
						c3.set_position({ c3.sprite->getPosition().x + 60,c3.sprite->getPosition().y });
					}

					/*int wait = 50;
					if (p1_last != '0')
						wait = 10;*/
					Sleep(40);

					p2_count++; pluser2 += 4;
					/*win.clear();
					win.draw(bg);
					win.draw(*c1.get_sprite());
					win.draw(*c3.get_sprite());
					win.display();*/

					if (!(p2_count < c3.get_nframes(11)))
					{
						p2_turn_running = false;
						pluser2 = 0;
						p2_count = 0;
						
						/*int wait = 50;
						if (p1_last != '0')
							wait = 10;*/
						Sleep(40);
						c3.cur_piece.width = o_w2;
						c3.cur_piece.height = o_he2;
						c3.cur_piece.left = c3.dimX - (2 * c3.dx_onepiece + 10);
						c3.cur_piece.top = c3.dimY / c3.no_cycles * 2;
						c3.sprite->setTextureRect(c3.cur_piece);
						c3.set_position({ c3.sprite->getPosition().x,520 });
						p2_last = '0';
					}

				}
			}
			if (p2_last == 'U')
			{
				this->checkp2isntnegscale();
				this->c3_current_frame = 8;
				last = 2;
				//int count = 0; int pluser = 0;
				if (p2_count == 0)
				{
					sound2.play();
					o_w2 = c3.cur_piece.width;
					o_he2 = c3.cur_piece.height;
					o_pos_p2 = c3.sprite->getPosition();
				}
				if (p2_turn_running)
				{

					c3.cur_piece.left = c3.moves[8][0 + pluser2];
					c3.cur_piece.top = c3.moves[8][1 + pluser2];
					c3.cur_piece.width = c3.moves[8][2 + pluser2];
					c3.cur_piece.height = c3.moves[8][3 + pluser2];
					c3.sprite->setTextureRect(c3.cur_piece);
					if (p2_count == 0)
					{
						c3.set_position({ c3.sprite->getPosition().x - 60 ,c3.sprite->getPosition().y });
					}
					else if (p2_count == 1)
					{
						c3.set_position({ c3.sprite->getPosition().x + 60 ,c3.sprite->getPosition().y });
					}
					else if (p2_count == 2)
					{
						if (c3.sprite->getPosition().x - 80 > 0)
							c3.set_position({ c3.sprite->getPosition().x - 80 ,c3.sprite->getPosition().y - 260 });
						else
							c3.set_position({ c3.sprite->getPosition().x ,c3.sprite->getPosition().y - 260 });
					}
					else if (p2_count == 3)
					{
						if (c3.sprite->getPosition().x - 250 > 0)
							c3.set_position({ c3.sprite->getPosition().x - 250 ,c3.sprite->getPosition().y + 140 });
						else
							c3.set_position({ c3.sprite->getPosition().x ,c3.sprite->getPosition().y + 140 });
					}
					 pluser2 += 4;
					/*win.clear();
					win.draw(bg);
					win.draw(*c1.get_sprite());
					win.draw(*c3.get_sprite());
					win.display();*/

					if (p2_count != 2 && p2_count != 0)
					{
						if (p2_count == 3)
						{
							/*int wait = 50;
							if (p1_last != '0')
								wait = 60;*/
							Sleep(50);
						}
						else
						{
							/*int wait = 80;
							if (p1_last != '0')
								wait = 60;*/
							Sleep(80);
						}
					}
					else
					{
						if (p2_count == 0)
						{
							/*int wait = 300;
							if (p1_last != '0')
								wait = 60;*/
							Sleep(300);
						}
						else
						{
							/*int wait = 500;
							if (p1_last != '0')
								wait = 60;*/
							Sleep(500);
						}
					}
					p2_count++;

					if (!(p2_count < c3.get_nframes(8)))
					{
						p2_turn_running = false;
						pluser2 = 0;
						p2_count = 0;
						
						if (c3.sprite->getPosition().x - 80 > 0)
							c3.set_position({ c3.sprite->getPosition().x - 80 ,c3.sprite->getPosition().y + 50 });
						else
							c3.set_position({ c3.sprite->getPosition().x ,c3.sprite->getPosition().y + 50 });
						/*win.clear();
						win.draw(bg);
						win.draw(*c1.get_sprite());
						win.draw(*c3.get_sprite());
						win.display();*/
						/*int wait = 500;
						if (p1_last != '0')
							wait = 60;*/
						Sleep(500);
						c3.cur_piece.width = o_w2;
						c3.cur_piece.height = o_he2;
						c3.cur_piece.left = c3.dimX - (2 * c3.dx_onepiece + 10);
						c3.cur_piece.top = c3.dimY / c3.no_cycles * 2;
						c3.sprite->setTextureRect(c3.cur_piece);

						if (c3.sprite->getPosition().x - 60 > 0)
							c3.set_position({ c3.sprite->getPosition().x - 60 ,520 });
						else
							c3.set_position({ c3.sprite->getPosition().x ,520 });
						p2_last = '0';
					}
				}
			}
			if (p2_last == 'Y')
			{
				//this->checkp2isntnegscale();
				this->c3_current_frame = 9;
				last = 2;
				//int count = 0; int pluser = 0;
				if (p2_count == 0)
				{
					o_w2 = c3.cur_piece.width;
					o_he2 = c3.cur_piece.height;
					o_pos_p2 = c3.sprite->getPosition();
				}
				if (p2_turn_running)
				{
					c3.cur_piece.left = c3.moves[9][0 + pluser2];
					c3.cur_piece.top = c3.moves[9][1 + pluser2];
					c3.cur_piece.width = c3.moves[9][2 + pluser2];
					c3.cur_piece.height = c3.moves[9][3 + pluser2];
					c3.sprite->setTextureRect(c3.cur_piece);
					if (p2_count == 0)
					{
						c3.set_position({ c3.sprite->getPosition().x - 100,c3.sprite->getPosition().y });
					}
					else if (p2_count == 1)
					{
						c3.set_position({ c3.sprite->getPosition().x + 80,c3.sprite->getPosition().y - 34 });
					}
					 pluser2 += 4;
					/*win.clear();
					win.draw(bg);
					win.draw(*c1.get_sprite());
					win.draw(*c3.get_sprite());
					win.display();*/
					/*int wait = 50;
					if (p1_last != '0')
						wait = 10;*/
					Sleep(100);
					p2_count++;
					if (!(p2_count < c3.get_nframes(9)))
					{
						p2_turn_running = false;
						pluser2 = 0;
						p2_count = 0;
						

						c3.set_position({ c3.sprite->getPosition().x + 20,c3.sprite->getPosition().y + 34 });
						c3.cur_piece.width = o_w2;
						c3.cur_piece.height = o_he2;
						c3.cur_piece.left = c3.dimX - (2 * c3.dx_onepiece + 10);
						c3.cur_piece.top = c3.dimY / c3.no_cycles * 2;
						c3.sprite->setTextureRect(c3.cur_piece);
						p2_last = lastp2;
						lastp2 = '0';
					}
				}
			}
			if (p2_last == 'T')
			{
				this->checkp2isntnegscale();
				this->c3_current_frame = 14;
				last = 2;
				//int count = 0, pluser = 0;
				if (p2_count == 0)
				{
					o_w2 = c3.cur_piece.width;
					o_he2 = c3.cur_piece.height;
					o_pos_p2 = c3.sprite->getPosition();
				}
				if (p2_turn_running)
				{
					c3.cur_piece.left = c3.moves[14][0 + pluser2];
					c3.cur_piece.top = c3.moves[14][1 + pluser2];
					c3.cur_piece.width = c3.moves[14][2 + pluser2];
					c3.cur_piece.height = c3.moves[14][3 + pluser2];
					c3.sprite->setTextureRect(c3.cur_piece);
					if (p2_count == 2)
					{
						c3.set_position({ c3.sprite->getPosition().x,c3.sprite->getPosition().y - 56 });
					}
					else if (p2_count == 3)
					{
						c3.set_position({ c3.sprite->getPosition().x,c3.sprite->getPosition().y + 50 });
					}
					pluser2 += 4;
					/*win.clear();
					win.draw(bg);
					win.draw(*c1.get_sprite());
					win.draw(*c3.get_sprite());
					win.display();*/

					/*int wait = 500;
					if (p1_last != '0')
						wait = 10;*/

					Sleep(300);

					p2_count++;
					if (!(p2_count < c3.get_nframes(9)))
					{
						p2_turn_running = false;
						pluser2 = 0;
						p2_count = 0;
						

						c3.set_position({ c3.sprite->getPosition().x,c3.sprite->getPosition().y + (56 - 50) });
						c3.cur_piece.width = o_w2;
						c3.cur_piece.height = o_he2;
						c3.cur_piece.left = c3.dimX - (2 * c3.dx_onepiece + 10);
						c3.cur_piece.top = c3.dimY / c3.no_cycles * 2;
						c3.sprite->setTextureRect(c3.cur_piece);
						p2_last = '0';
					}
				}
			}
			
		}
	}
}

void game::player1relay()
{
	cout << "player 1 thread works" << endl;
	while (game_flag)
	{
		
		//cout << "player 1 relauy  ";
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			if (p1_last == '0')
			{
				
					mu.lock();
					this->ffirst = 1;
					mu.unlock();
					p1_turn_running = true;
					p1_last = 'R';
				
			}
			
		}
		//for left
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
		{
			if (p1_last == '0')
			{
				
					mu.lock();
					this->ffirst = 1;
					mu.unlock();
					p1_turn_running = true;
					p1_last = 'L';
				
			}
			
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
		{
			if (p1_last == '0')
			{
				
					mu.lock();
					this->ffirst = 1;
					mu.unlock();
					this->upturnrunnig1 = true;
					p1_last = 'U';
				
			}
			
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
		{
			if (p1_last == '0')
			{
				
					mu.lock();
					this->ffirst = 1;
					mu.unlock();
					p1_turn_running = true;
					p1_last = 'D';
				
			}
			
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			if (p1_last == '0' || p1_last == 'U')
			{
				this->checkp1isntnegscale();
				lastp1 = p1_last;
					mu.lock();
					this->ffirst = 1;
					mu.unlock();
					p1_turn_running = true;
					p1_last = 'A';
				
			}
			
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			if (p1_last == '0' || p1_last=='U')
			{
				this->checkp1isntnegscale();
				lastp1 = p1_last;
					mu.lock();
					this->ffirst = 1;
					mu.unlock();
					p1_turn_running = true;
					p1_last = 'S';
				
			}
			
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{

			if (p1_last == '0')
			{
				mu.lock();
				this->ffirst = 1;
				mu.unlock();
				p1_turn_running = true;
				p1_last = 'd';
			}
			

		}
		if (powerp1xp == 150)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
			{
				if (p1_last == '0')
				{
					mu.lock();
					this->ffirst = 1;
					mu.unlock();
					p1_turn_running = true;
					p1_last = 'F';
				}

			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
			{
				if (p1_last == '0')
				{
					mu.lock();
					this->ffirst = 1;
					mu.unlock();
					p1_turn_running = true;
					p1_last = 'G';
				}

			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			{
				if (p1_last == '0')
				{
					mu.lock();
					this->ffirst = 1;
					mu.unlock();
					p1_turn_running = true;
					p1_last = 'Q';
				}

			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				if (p1_last == '0')
				{
					mu.lock();
					this->ffirst = 1;
					mu.unlock();
					p1_turn_running = true;
					p1_last = 'W';
				}

			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		{
			if (p1_last == '0')
			{
				mu.lock();
				this->ffirst = 1;
				mu.unlock();
				p1_turn_running = true;
				p1_last = 'E';
			}
			
		}
		
		if (p1_last == '0')
		{
			this->c1_current_frame = -998;
		}
		
	}
}

void game::player2relay()
{
	while (game_flag)
	{
		
		//cout << "player 2 relay" << endl;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
		{
			if (p2_last == '0')
			{
				mu.lock();
				this->ffirst = 2;
				mu.unlock();
				p2_turn_running = true;
				p2_last = 'J';
			}

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
		{
			if (p2_last == '0')
			{
				mu.lock();
				this->ffirst = 2;
				mu.unlock();
				p2_turn_running = true;
				p2_last = 'L';
			}

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
		{
			if (p2_last == '0')
			{
				mu.lock();
				this->ffirst = 2;
				mu.unlock();
				this->upturnrunnig2 = true;
				p2_last = 'I';
			}

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
		{
			if (p2_last == '0')
			{
				mu.lock();
				this->ffirst = 2;
				mu.unlock();
				p2_turn_running = true;
				p2_last = 'B';
			}

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
		{
			if (p2_last == '0' || p2_last=='I')
			{
				this->checkp2isntnegscale();
				this->lastp2 = p2_last;
				mu.lock();
				this->ffirst = 2;
				mu.unlock();
				p2_turn_running = true;
				p2_last = 'K';
			}

		}
		if (powerp2xp == 150)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
			{
				if (p2_last == '0')
				{
					mu.lock();
					this->ffirst = 2;
					mu.unlock();
					p2_turn_running = true;
					p2_last = 'N';
				}

			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
			{
				if (p2_last == '0')
				{
					mu.lock();
					this->ffirst = 2;
					mu.unlock();
					p2_turn_running = true;
					p2_last = 'M';
				}

			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
			{
				if (p2_last == '0')
				{
					mu.lock();
					this->ffirst = 2;
					mu.unlock();
					p2_turn_running = true;
					p2_last = 'O';
				}

			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
			{
				if (p2_last == '0')
				{
					mu.lock();
					this->ffirst = 2;
					mu.unlock();
					p2_turn_running = true;
					p2_last = 'U';
				}

			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
		{
			if (p2_last == '0')
			{
				mu.lock();
				this->ffirst = 2;
				mu.unlock();
				p2_turn_running = true;
				p2_last = 'P';
			}

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
		{
			if (p2_last == '0' || p2_last=='I')
			{
				this->checkp2isntnegscale();
				this->lastp2 = p2_last;
				mu.lock();
				this->ffirst = 2;
				mu.unlock();
				p2_turn_running = true;
				p2_last = 'Y';
			}

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
		{
			if (p2_last == '0')
			{
				mu.lock();
				this->ffirst = 2;
				mu.unlock();
				p2_turn_running = true;
				p2_last = 'T';
			}

		}
		
		if (p2_last == '0')
		{
			this->c3_current_frame = -998;
		}

	}
}


void game::check_collision_and_do_stuff()
{
	bool p1inpos=false;
	bool p2inpos=false;
	bool heavierlift = false;
	
	float xdiff = 140;

	
	while (game_flag)
	{
		this->collision_check_flag2 = false;
		this->collision_check_flag1 = false;
		
		float xdiff = 140;
		//if (p1_last == 'Q')
			//xdiff = 240;
		

			if (
				((c3.sprite->getPosition().x - c1.sprite->getPosition().x >= 0 && c3.sprite->getPosition().x - c1.sprite->getPosition().x <= xdiff) ||
				(c1.sprite->getPosition().x - c3.sprite->getPosition().x >= 0 && c1.sprite->getPosition().x - c3.sprite->getPosition().x <= xdiff))
				//( c3.sprite->getPosition().x <= (c1.sprite->getPosition().x + c1.cur_piece.width) && c3.sprite->getPosition().x > c1.sprite->getPosition().x
				// || c1.sprite->getPosition().x <= (c3.sprite->getPosition().x + c3.cur_piece.width) && c1.sprite->getPosition().x > c3.sprite->getPosition().x ) 
				&&
				(
				(c3.sprite->getPosition().y - c1.sprite->getPosition().y <= 140 && c3.sprite->getPosition().y - c1.sprite->getPosition().y >= 0) ||
					(c1.sprite->getPosition().y - c3.sprite->getPosition().y <= 140 && c1.sprite->getPosition().y - c3.sprite->getPosition().y >= 0)
					)
				)
			{

				//cout << "collison occured" << endl;

				if (
					(this->get_p1_current_cycle() == 7 && (p1_count == 2 || p1_count == 4)) ||
					(this->get_p1_current_cycle() == 9 && (p1_count == 2 || p1_count == 5)) ||
					(this->get_p1_current_cycle() == 10 && (p1_count == 0 || p1_count == 5)) ||
					(this->get_p1_current_cycle() == 11 && p1_count == 4) ||
					(this->get_p1_current_cycle() == 12 && (p1_count == 0 || p1_count == 2)) ||
					(this->get_p1_current_cycle() == 17 && p1_count == 3) ||
					(this->get_p1_current_cycle() == 18)
					)
				{
					p1inpos = true;
					int temp = this->get_p1_current_cycle();
					if (temp == 10 || temp == 11 || temp == 18)
						heavierlift = true;
					temp = 786;
				}


				if (
					(this->get_p2_current_cycle() == 6 && (p2_count == 2 || p2_count == 3)) ||
					(this->get_p2_current_cycle() == 8 && (p2_count == 0 || p2_count == 3)) ||
					(this->get_p2_current_cycle() == 9 && p2_count == 0) ||
					(this->get_p2_current_cycle() == 10 && p2_count == 1) ||
					(this->get_p2_current_cycle() == 11 && p2_count == 2) ||
					(this->get_p2_current_cycle() == 12 && p2_count == 3) ||
					(this->get_p2_current_cycle() == 17) ||
					(this->get_p2_current_cycle() == 18)
					)
				{
					p2inpos = true;
					int temp = this->get_p2_current_cycle();
					if (temp == 8 || temp == 12 || temp == 17 || temp == 18)
						heavierlift = true;
					temp = 786;
				}

				if (ffirst == 1)
				{
					if (p1inpos && !p2inpos)
					{
						this->collision_check_flag2 = true;
						//this->collision_check_flag1 = true;
						if (lastp2 != 'z')
						{
							
							    
							if (heavierlift)
							{
								this->get_punched(2,2);
								this->get_punched(2, 1);
							}else
								this->get_punched(2);
							heavierlift = false;
						}
						p2_last = '0';

						p1inpos = false;
						p2inpos = false;
					}
					else if (p2inpos && !p1inpos)
					{
						this->collision_check_flag1 = true;
						//this->collision_check_flag2 = true;

						if (lastp1 != 'z')
						{
							this->get_punched(1);
							if (heavierlift)
								this->get_punched(1, 1);
							heavierlift = false;
						}
						p1_last = '0';
						p1inpos = false;
						p2inpos = false;
					}
					else
					{
						p1inpos = false;
						p2inpos = false;
						//cout << "blocked" << endl;
					}
				}
				else if (ffirst == 2)
				{
					if (p2inpos && !p1inpos)
					{
						this->collision_check_flag1 = true;
						//this->collision_check_flag2 = true;


						if (lastp1 != 'z')
						{
							this->get_punched(1);
							if (heavierlift)
								this->get_punched(1, 1);
							heavierlift = false;
						}

						p1_last = '0';
						p1inpos = false;
						p2inpos = false;
					}
					else if (p1inpos && !p2inpos)
					{
						this->collision_check_flag2 = true;
						//this->collision_check_flag1 = true;

						if (lastp2 != 'z')
						{
							this->get_punched(2);
							if (heavierlift)
								this->get_punched(2, 1);
							heavierlift = false;
						}
						p2_last = '0';

						p1inpos = false;
						p2inpos = false;
					}
					else
					{
						p1inpos = false;
						p2inpos = false;
						//cout << "blocked" << endl;
					}
				}
				this->collision_check_flag2 = false;
				this->collision_check_flag1 = false;
				heavierlift = false;

			}
		
        
		
        
	}
	
}

void game::start()
{
	sf::Font player1font;
	sf::Font player2font;
	player1font.loadFromFile("ghostclanlaserital.ttf");
	player2font.loadFromFile("ghostclanlaserital.ttf");
	
	sf::Text player1text;
	sf::Text player2text;
	sf::Text winner;
	
	winner.setFont(player1font);
	winner.setCharacterSize(100);
	winner.setFillColor(sf::Color::White);
	winner.setPosition({ 300,250 });
	winner.setString("wow");
	

	player1text.setFont(player1font);
	player1text.setCharacterSize(20);
	player1text.setFillColor(sf::Color::White);
	player1text.setPosition({ 10,5 });
	player1text.setString("Ryu");

	player2text.setFont(player2font);
	player2text.setCharacterSize(20);
	player2text.setFillColor(sf::Color::White);
	player2text.setPosition({ 1100,5 });
	player2text.setString("Bazuka");

	c1.set_picname("Ryu.png");
	
	if (c1.picname == "Ryu.png")
		c1.readmovesfromfile("ryudata.txt");
	cout << "data successfully read" << endl;
	this->players[0] = &c1;
	c1.set_position({ 100,465 });
	c1.set_udrl(4, 5, 2, 3);
	bool down_flag = false;
	bool first_time = true;

	sf::SoundBuffer buffer;
	buffer.loadFromFile("ryu.wav");

	sf::Sound sound;
	sound.setBuffer(buffer);
	sound.setVolume(10);
	sound.setPitch(1);
	sound.play();
	sound.setLoop(true);

	bool running = true;

	c3.set_picname("blanka.png");
	if (c3.picname == "blanka.png")
		c3.readmovesfromfile("blankadata.txt");
	cout << "data successfully read" << endl;
	c3.cur_piece.left = c3.dimX - c3.dx_onepiece;
	c3.sprite->setTextureRect(c3.cur_piece);
	this->players[1] = &c3;
	c3.set_position({ 900,520 });


	last = 1;

	sf::Thread t1(&game::player1relay, this);
	sf::Thread t2(&game::player2relay, this);
	sf::Thread t3(&game::player_1_turn, this);
	sf::Thread t4(&game::player_2_turn, this);
	sf::Thread t5(&game::check_collision_and_do_stuff, this);
	t1.launch();
	t2.launch();
	t3.launch();
	t4.launch();
	t5.launch();
	this->game_flag = true;
	int flip = 0;

	
	int starting = 0;

	win.setKeyRepeatEnabled(false);

	while (win.isOpen())
	{

		if (this->powerp1ow <= 0 || this->powerp2ow <= 0)
		{
			if (this->powerp1ow <= 0)
			{
				winner.setString("Bazuka wins");

			}
			else if (this->powerp2ow <= 0)
			{

				winner.setString("Ryu wins");
			}

			while (p1_last != '0' || p2_last != '0')
			{

			}

			game_flag = false;


			/*t1.wait();
			t2.wait();
			t3.wait();
			t4.wait();
			t5.wait();*/

			t1.terminate();
			t2.terminate();
			t3.terminate();
			t4.terminate();
			t5.terminate();

			

		}
		sf::Event event;
		while (win.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				if (game_flag)
				{
					/*t1.wait();
					t2.wait();
					t3.wait();
					t4.wait();
					t5.wait();*/

					t1.terminate();
					t2.terminate();
					t3.terminate();
					t4.terminate();
					t5.terminate();

				}

				win.close();
			}
		}
		
		win.clear();
		win.draw(bg);
		
		win.draw(this->player1powerr);
		win.draw(this->player1power);
		win.draw(this->player2powerr);
		win.draw(this->player2power);
		win.draw(player1xp);
		win.draw(player2xp);
		win.draw(player2xppower);
		win.draw(player1xppower);
		win.draw(player1text);
		win.draw(player2text);
		if (this->powerp1xp == 150)
		{
			
			for (int i = 0; i < 4; i++)
			{
				if (flip < 100)
					cp1[i].setFillColor(sf::Color::Red);
				else
					cp1[i].setFillColor(sf::Color::Yellow);
				win.draw(cp1[i]);
				
			}
		}
		if (this->powerp2xp == 150)
		{
			for (int i = 0; i < 4; i++)
			{
				if (flip < 100)
					cp2[i].setFillColor(sf::Color::Red);
				else
					cp2[i].setFillColor(sf::Color::Yellow);
				win.draw(cp2[i]);
				
			}
		}
		if (last == 1)
		{
			win.draw(*c3.get_sprite());
			
			win.draw(*c1.get_sprite());
		}
		else if (last == 2)
		{
			win.draw(*c1.get_sprite());
			
			win.draw(*c3.get_sprite());
		}
		if (this->c2flag)
		{
			win.draw(*c2->get_sprite());
		}
		
		if (efxflag)
			win.draw(this->specialeffects);

		if (game_flag == false)
			win.draw(winner);
		win.display();
		
		if (flip != 200)
			flip++;
		else
			flip = 0;

	}
}

game::~game()
{

}