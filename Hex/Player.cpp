#include "Player.h"

void checkError()
{
	std::cout << "It's not a number, please try again" << std::endl;
	std::cin.clear();
	std::cin.ignore(10000, '\n');
}

int Player::getBoardSize()
{
	int boardSize{ 0 };

	while (true)
	{
		std::cout << "Enter game board size: ";
		std::cin >> boardSize;
		if (std::cin.fail())
		{
			checkError();
			continue;
		}

		if (boardSize < 40 && boardSize > 2) 
			break; 

		if (boardSize >= 40) 
			std::cout << "Goin' big, huh. Try again" << std::endl;
		if (boardSize <= 2) 
			std::cout << "Minimum board size is 3" << std::endl;
	}
	return boardSize;
}

std::pair<int, int> Player::getPlayerMove(int pN)
{
	int x, y;
	int idx = 0;
	std::cout << "Player " << pN << " turn: " << std::endl;
	while (true)
	{
		if (idx == 0)
		{
			std::cout << "Enter X: " << std::endl;
			std::cin >> x;
		}
		else if (idx == 1)
		{
			std::cout << "Enter Y: " << std::endl;
			std::cin >> y;
		}

		if (std::cin.fail())
		{
			checkError();
			continue;
		}
		else idx++;
	}
	return std::pair<int, int>(x, y);
}

bool Player::getPlayersNumber()
{
	int nP;
	while (true)
	{
		std::cout << "Enter number of players: ";
		std::cin >> nP;
		if (std::cin.fail())
		{
			checkError();
			continue;
		}
		else
			return nP;
		
	}
	return (nP == 1) ? 0 : 1;
}
