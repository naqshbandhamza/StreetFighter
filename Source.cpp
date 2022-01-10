#include <iostream>
#include "game.h"
#include <thread>

using namespace std;

int main()
{
	
	game* gameptr = new game();
	gameptr->start();
	delete gameptr;
}