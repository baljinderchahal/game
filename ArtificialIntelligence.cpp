//
//  ArtificialIntelligence.cpp
//

#include "ArtificialIntelligence.h"

#include <cassert>
#include <cstdlib>
#include <iostream>

#include "Move.h"
#include "Search.h"
#include "Board.h"
#include "Game.h"

using namespace std;

// set to true for extra output
static const bool DEBUGGING_SHOW_MOVE_SCORES = false;



ArtificialIntelligence :: ArtificialIntelligence (char us_value_in)
{
	assert(isBoardValuePlayer(us_value_in));

	us_value = us_value_in;

	assert(isInvariantTrue());
}

Move ArtificialIntelligence :: chooseMove (const Board& board_in) const
{
	assert(isInvariantTrue());

	static const int LEGAL_MOVE_COUNT_MAX = BOARD_SIZE_MAX * BOARD_SIZE_MAX + 1;
	SortableMove legal_moves[LEGAL_MOVE_COUNT_MAX];
	int legal_move_count = 0;

	assert(legal_move_count < LEGAL_MOVE_COUNT_MAX);
	legal_moves[legal_move_count].is_played = false;
	legal_moves[legal_move_count].row       = 0;  // these don't matter
	legal_moves[legal_move_count].column    = 0;  //   when passing
	legal_moves[legal_move_count].net_score = calculateNetScore(board_in) + 0.5f;
	legal_move_count++;

	for(int r = 0; r < board_in.getSize(); r++)
	{
		for(int c = 0; c < board_in.getSize(); c++)
		{
			MoveResult result = getPlayResult(board_in, r, c);
			if(result.is_legal)
			{
				assert(legal_move_count < LEGAL_MOVE_COUNT_MAX);
				legal_moves[legal_move_count].is_played = true;
				legal_moves[legal_move_count].row       = r;
				legal_moves[legal_move_count].column    = c;
				legal_moves[legal_move_count].net_score = result.net_score;
				legal_move_count++;
			}
		}
	}

	assert(legal_move_count > 0);
	assert(legal_move_count <= LEGAL_MOVE_COUNT_MAX);
	mySort(legal_moves, legal_move_count);

	if(DEBUGGING_SHOW_MOVE_SCORES)
	{
		for(int i = 0; i < legal_move_count; i++)
		{
			if(legal_moves[i].is_played)
				cout << (char)(legal_moves[i].column + 'A') << legal_moves[i].row << ":";
			else
				cout << "pass:";
			cout << legal_moves[i].net_score << " ";
		}
		cout << endl;
	}

	SortableMove best_move = legal_moves[legal_move_count - 1];  // highest score is at end
	int first_good_move = binarySearchFirst(legal_moves, legal_move_count, best_move);
	assert(first_good_move != VALUE_NOT_FOUND);
	assert(first_good_move <  legal_move_count);
	int good_move_range = legal_move_count - first_good_move;

	assert(good_move_range != 0);
	int chosen = first_good_move + rand() % good_move_range;
	assert(chosen >= 0);
	assert(chosen <  legal_move_count);
	return legal_moves[chosen];
}



float ArtificialIntelligence :: calculateNetScore (const Board& board_in) const
{
	char them_value = getOtherPlayer(us_value);

	int us_score_play   = board_in.calculateScore(us_value);
	int them_score_play = board_in.calculateScore(them_value);
	return (float)(us_score_play - them_score_play);
}

MoveResult ArtificialIntelligence :: getPlayResult (const Board& board_in,
                                                    int row_in, int column_in) const
{
	MoveResult result;
	result.is_legal = false;

	if(!board_in.isOnBoard(row_in, column_in))
		return result;  // place is not on board
	if(board_in.getAt(row_in, column_in) != BOARD_VALUE_EMPTY)
		return result;  // place is not empty

	Board copy = board_in;
	StonesRemoved removed = copy.playStone(row_in, column_in, us_value);
	if(removed.us > 0)
		return result;  // suicide rule

	result.is_legal  = true;
	result.net_score = calculateNetScore(copy);
	return result;
}

bool ArtificialIntelligence :: isInvariantTrue () const
{
	return isBoardValuePlayer(us_value);
}
