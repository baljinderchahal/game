//
//  Game.cpp
//

#include "Game.h"

#include <cassert>
#include <iostream>
#include <string>

#include "Board.h"
#include "ArtificialIntelligence.h"

using namespace std;

static const float KOMI = 7.5f;



Game :: Game ()
		: white(BOARD_VALUE_WHITE)
{
}

Game :: Game (int board_size_in)
		: board(board_size_in),
		  white(BOARD_VALUE_WHITE)
{
	assert(board_size_in >= BOARD_SIZE_MIN);
	assert(board_size_in <= BOARD_SIZE_MAX);
}

Game :: Game (const std::string& filename_in)
		: white(BOARD_VALUE_WHITE)
{
	assert(filename_in != "");

	board.load(filename_in);
}



void Game :: printBoard () const
{
	board.print();
}

void Game :: printWinner () const
{
	float black_score = board.calculateScore(BOARD_VALUE_BLACK) + 0.0f;
	float white_score = board.calculateScore(BOARD_VALUE_WHITE) + KOMI;
	if(black_score > white_score)
		cout << "Black wins " << black_score << " to " << white_score << " (with komi)" << endl;
	else if(black_score < white_score)
		cout << "White wins " << white_score << " to " << black_score << " (with komi)" << endl;
	else
		cout << "Tie game" << endl;
	cout << endl;
}

const Board& Game :: getBoard () const
{
	return board;
}



void Game :: blackPass ()
{
	cout << "Black passed" << endl;
}

bool Game :: blackPlay (int row_in, int column_in)
{
	if(!board.isOnBoard(row_in, column_in))
	{
		cout << "Forbidden: Point is outside board" << endl;
		return false;
	}
	if(board.getAt(row_in, column_in) != BOARD_VALUE_EMPTY)
	{
		cout << "Forbidden: Point is not empty" << endl;
		return false;
	}

	Board after = board;
	StonesRemoved removed = after.playStone(row_in, column_in, BOARD_VALUE_BLACK);
	if(removed.us > 0)
	{
		cout << "Forbidden: Suicide rule" << endl;
		return false;
	}

	board = after;
	cout << "Black placed a stone at row " << row_in << ", column " << column_in << endl;
	printRemoved(removed);
	return true;
}

bool Game :: whiteAi ()
{
	Move chosen = white.chooseMove(board);
	if(chosen.is_played)
	{
		cout << "White placed a stone at row " << chosen.row
		     << ", column " << chosen.column << endl;
		StonesRemoved removed = board.playStone(chosen.row, chosen.column, BOARD_VALUE_WHITE);
		printRemoved(removed);
		return true;
	}
	else
	{
		cout << "White passed" << endl;
		return false;
	}
}

void Game :: printRemoved (const StonesRemoved& removed) const
{
	if(removed.them == 1)
		cout << "  Captured " << removed.them << " stone" << endl;
	else if(removed.them > 1)
		cout << "  Captured " << removed.them << " stones" << endl;

	if(removed.us == 1)
		cout << "  Lost " << removed.us << " stone to suicide" << endl;
	else if(removed.us > 1)
		cout << "  Lost " << removed.us << " stones to suicide" << endl;
}

