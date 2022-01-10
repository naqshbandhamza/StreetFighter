#include "character.h"
#include <iostream>
#include <fstream>

using namespace std;

character::character()
{
	cout << "c called" << endl;
}

character::~character()
{

}

character::character(string picname, int no_cycles, int max_no_frames, int X, int Y)
{
	r = u = l = d = false;
	this->picname = picname;
	this->no_cycles = no_cycles;
	this->max_no_frames = max_no_frames;
	dimX = X;
	dimY = Y;
	sprite = new sf::Sprite();
	texture = new sf::Texture();
	texture->loadFromFile(picname);
	sprite->setTexture(*texture);
	cur_piece.left = 0;
	cur_piece.top = 0;
	cur_piece.width = X/max_no_frames;
	cur_piece.height = Y/no_cycles;
	sprite->setTextureRect(cur_piece);
	dx_onepiece = dimX / max_no_frames;
	dy_onepiece = dimY / no_cycles;
	no_of_frames_in_cycle = new int[no_cycles];
	top_left = new int* [no_cycles];
	this->moves = new int* [no_cycles];
	for (int i = 0; i < no_cycles; i++)
	{
		this->moves[i] = NULL;
		no_of_frames_in_cycle[i] = max_no_frames;
		top_left[i] = new int[2];
		top_left[i][0] = 0;
		top_left[i][1] = 0;
	}
}


void character::setvalues(int no_cycles, int max_no_frames, int X, int Y)
{
	r = u = l = d = false;
	this->no_cycles = no_cycles;
	this->max_no_frames = max_no_frames;
	dimX = X;
	dimY = Y;
	sprite = new sf::Sprite();
	texture = new sf::Texture();
	texture->loadFromFile(picname);
	sprite->setTexture(*texture);
	cur_piece.left = 0;
	cur_piece.top = 0;
	cur_piece.width = X / max_no_frames;
	cur_piece.height = Y / no_cycles;
	sprite->setTextureRect(cur_piece);
	dx_onepiece = dimX / max_no_frames;
	dy_onepiece = dimY / no_cycles;
	no_of_frames_in_cycle = new int[no_cycles];
	top_left = new int* [no_cycles];
	this->moves = new int* [no_cycles];
	for (int i = 0; i < no_cycles; i++)
	{
		this->moves[i] = NULL;
		no_of_frames_in_cycle[i] = max_no_frames;
		top_left[i] = new int[2];
		top_left[i][0] = 0;
		top_left[i][1] = 0;
	}
}

void character::set_picname(std::string picname)
{
	this->picname = picname;
}


void character::readmovesfromfile(const char* name)
{
	ifstream read;
	string temp = ""; char tempc = '1';
	int index_tracker = 1;
	int count = 0, inner_count = 0; int v1 = 0, v2 = 0, v3 = 0, v4 = 0, k = 0, cs = 0;
	int baorami_index = 0;
	
	read.open(name);
	cout << "file opened" << endl;
	while (!read.eof())
	{
		read >> noskipws >> tempc;
		//cout << tempc;
		if (tempc != ',' && tempc != '\n' && !read.eof())
		{
			temp = temp + tempc;
		}
		else
		{
			if (tempc == ',')
			{
				if (count == 0)
				{
					if (inner_count == 0)
						v1 = stoi(temp);
					else if (inner_count == 1)
						v2 = stoi(temp);
					else if (inner_count == 2)
						v3 = stoi(temp);
				}
				else if (count == index_tracker)
				{
					if (inner_count == 0)
						k = stoi(temp);
				}
				else
				{
					this->moves[k][baorami_index] = stoi(temp);
					baorami_index++;
				}
				inner_count++;
				temp = "";
			}
			else
			{
				if (count == 0)
				{
					v4 = stoi(temp);
					this->setvalues(v1, v2, v3, v4);
				}
				else if (count == index_tracker)
				{
					baorami_index = 0;
					cs = stoi(temp);
					this->set_index_cycle(k, cs);
					index_tracker += (cs + 1);
					int fsize = cs * 4;
					this->moves[k] = new int[fsize];
				}
				else
				{
					this->moves[k][baorami_index] = stoi(temp);
					baorami_index++;
					
				}
				inner_count = 0; count++;
				temp = "";
			}
		}
	}


	if (this->picname == "Ryu.png")
	{
		this->setscale({ 2,2 });
	}
	else if (this->picname == "blanka.png")
	{
		this->setscale({ 1.4,1.4 });
	}
}

void character::setscale(sf::Vector2f scale)
{
	this->sprite->setScale(scale);
}

void character::set_properties(int left,int top)
{
	this->cur_piece.left = left;
	this->cur_piece.top = top;
	this->sprite->setTextureRect(this->cur_piece);
}

void character::set_udrl(int up, int down, int right, int left)
{
	this->up = up;
	this->down = down;
	this->right = right;
	this->left = left;
}

void character::set_index_cycle(int index, int no)
{
	no_of_frames_in_cycle[index] = no;
}


void character::set_position(sf::Vector2f pos)
{
	this->sprite->setPosition(pos);
}


int character::get_nframes(int index)
{
	return this->no_of_frames_in_cycle[index];
}

void character::move_left(sf::RenderWindow& win)
{
	
	if (this->sprite->getPosition().x - 20 > -40)
	   this->set_position({ this->sprite->getPosition().x - 20,this->sprite->getPosition().y });
}

void character::move_right(sf::RenderWindow& win)
{
	if(this->sprite->getPosition().x+20 < win.getSize().x-290)
	    this->set_position({ this->sprite->getPosition().x+20,this->sprite->getPosition().y });
}

void character::move_up()
{
	
}

void character::movement(int index,int c_no,int extra_x)
{
	r = l = u =d = false;
	if (c_no==0)
	{
		this->cur_piece.top = ((this->dimY / this->no_cycles) * index) + top_left[index][0];
		this->cur_piece.left = top_left[index][1];
	}
	if (this->cur_piece.left < top_left[index][1] + dx_onepiece * no_of_frames_in_cycle[index] - dx_onepiece)
		this->cur_piece.left += dx_onepiece+extra_x;
	else
		this->cur_piece.left = top_left[index][1];

	this->sprite->setTextureRect(this->cur_piece);
}

void character::movement_left(int index, int c_no, int extra_x)
{
	r = l = u = d = false;
	if (c_no == 0)
	{
		this->cur_piece.top = ((this->dimY / this->no_cycles) * index) + top_left[index][0];
		this->cur_piece.left = top_left[index][1];
	}
	if (this->cur_piece.left >  dx_onepiece)
		this->cur_piece.left -= dx_onepiece + extra_x;
	else
		this->cur_piece.left = top_left[index][1];

	this->sprite->setTextureRect(this->cur_piece);
}


void character::move_down(sf::RenderWindow& win)
{
	if (this->sprite->getPosition().x + 20 < win.getSize().x-290)
	    this->set_position({ this->sprite->getPosition().x + 20,this->sprite->getPosition().y });
}

void character::set_index_top(int index, int no)
{
	top_left[index][0] = no;
}

void character::set_index_left(int index, int no)
{
	top_left[index][1] = no;
}

sf::Sprite* character::get_sprite()
{
	return this->sprite;
}