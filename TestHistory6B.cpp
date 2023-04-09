//
//  TestHistory6B.cpp
//
//  A test program for the first part of the History module.
//
//  This program is to be used with Assignment 6: Part B for
//    CS115, 202310.
//
//  Do not modify this file.
//

#include "History.h"
#include "History.h"  // repeated to test for #include errors

#include <cassert>
#include <string>
#include <iostream>

#include "TestHelper.h"
#include "BoardValue.h"
#include "Board.h"

using namespace std;



struct BasicResults
{
	bool at_least_1;
	bool at_least_2;
	bool at_least_3;
	unsigned int is_in_list;
	bool newest;
	bool second;
};
BasicResults basicTests (History& r_history,
                         unsigned int count,
                         const Board& board_first,
                         const Board& board_second,
                         const Board& board_third,
                         const Board& board_empty);

struct BasicTotals
{
	unsigned int at_least_N;
	unsigned int at_least_N_max;
	double at_least_N_highest;
	double at_least_N_fraction;

	unsigned int is_in_list;
	unsigned int is_in_list_max;
	double is_in_list_highest;
	double is_in_list_fraction;

	unsigned int first;
	unsigned int first_max;
	double first_highest;
	double first_fraction;

	unsigned int second;
	unsigned int second_max;
	double second_highest;
	double second_fraction;
};
void initBasicTotals (BasicTotals& r_totals);
void updateBasicTotals (BasicTotals& r_totals,
                        const BasicResults& results,
                        unsigned int history_size,
                        unsigned int trials0,
                        unsigned int trials1,
                        unsigned int trials2);

bool testAtLeastNLong (const History& history,
                       unsigned int how_long,
                       bool correct_result);
unsigned int testIsInList (const History& history,
                           const Board& board_empty,
                           const Board& board_black,  bool correct_black,
                           const Board& board_white,  bool correct_white,
                           const Board& board_marked, bool correct_marked);
unsigned int testIsInList (const History& history,
                           const Board& board,
                           bool correct_result,
                           const string& board_name);
bool testGetFirst (const History& history,
                   const Board& correct_board,
                   const Board& empty_board,
                   const Board* p_2nd_board,
                   const Board* p_3rd_board);
bool testGetSecond (const History& history,
                    const Board& correct_board,
                    const Board& head_board,
                    const Board& empty_board,
                    const Board* p_3rd_board);

int calculateMark (bool correct_node_bytes,
                   bool correct_history_bytes,
                   double correct_at_least_N_fraction,
                   double correct_is_in_list_fraction,
                   double correct_first_fraction,
                   double correct_second_fraction,
                   bool is_finished);

const int COMPILE_AND_START_MARKS     = 5;
const int DID_NOT_CRASH_RUNNING_MARKS = 5;
const int COMPILE_AND_NOT_CRASH_MARKS = COMPILE_AND_START_MARKS + DID_NOT_CRASH_RUNNING_MARKS;
const int COMBINED_TEST_MARKS         = 20;
const int TOTAL_MARKS                 = COMPILE_AND_NOT_CRASH_MARKS + COMBINED_TEST_MARKS;
const int TOTAL_MARKS_WITH_PART_C     = TOTAL_MARKS + 25;

const char BOARD_VALUE_EMPTY_CORRECT  = '.';
const char BOARD_VALUE_BLACK_CORRECT  = 'O';
const char BOARD_VALUE_WHITE_CORRECT  = '@';
const char BOARD_VALUE_MARKED_CORRECT = '#';
const unsigned int BOARD_VALUE_COUNT = 4;

const unsigned int CORRECT_AT_LEAST_N_LONG_COUNT = 3;

const unsigned int BASIC_TEST_COUNT   = 6;
const unsigned int BASIC_TEST_1_COUNT = 4;
const unsigned int BASIC_TEST_2_COUNT = 3;



int main ()
{
	TestHelper::startup("TestHistory6B", COMPILE_AND_START_MARKS, TOTAL_MARKS_WITH_PART_C);


	//
	//  Test Move and SortableMove structs
	//

	cout << "Testing definitions:" << endl;
	cout << "--------------------" << endl;
	cout << endl;

	cout << "Checking HistoryNode size in bytes" << endl;
	size_t node_bytes         = sizeof(HistoryNode);
	size_t node_bytes_pointer = sizeof(HistoryNode*);
	size_t node_bytes_board   = sizeof(Board);
	size_t node_bytes_correct = node_bytes_pointer + node_bytes_board;
	bool correct_node_bytes = false;
	if(node_bytes == node_bytes_pointer)
		cout << "* Incorrect: Move should also contain Board" << endl;
	else if(node_bytes == node_bytes_board)
		cout << "* Incorrect: Move should also contain next pointer" << endl;
	else if(node_bytes < node_bytes_correct)
		cout << "* Incorrect: Move should contain more data" << endl;
	else if(node_bytes > node_bytes_correct * 2)  // extra space because padding happens
		cout << "* Incorrect: Move should contain less data" << endl;
	else
	{
		cout << "* Correct" << endl;
		correct_node_bytes = true;
	}
	cout << endl;

	cout << "Creating HistoryNode" << endl;
	{
		HistoryNode node0;  // will be destroyed at end of scope
		cout << "* Completed without crashing" << endl;
		cout << endl;

		(void)(node0);  // no-op to silence compiler warnings about unused local variable

		cout << "Destroying HistoryNode" << endl;
	}  // node0 is destroyed here
	cout << "* Completed without crashing" << endl;
	cout << endl;

	cout << "Checking History size in bytes" << endl;
	size_t history_bytes         = sizeof(History);
	size_t history_bytes_correct = sizeof(HistoryNode*);
	bool correct_history_bytes = false;
	if(history_bytes < history_bytes_correct)
		cout << "* Incorrect: History should contain head pointer" << endl;
	else if(history_bytes > history_bytes_correct * 2)  // extra space because padding happens
		cout << "* Incorrect: History should only contain head pointer" << endl;
	else
	{
		cout << "* Correct" << endl;
		correct_history_bytes = true;
	}
	cout << endl;

	cout << endl;
	cout << endl;


	//
	//  Print results 1
	//

	TestHelper::printSummaryHeaderPartial(24, 2, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("HistoryNode struct", correct_node_bytes);
	TestHelper::printSummaryLine("History definition", correct_history_bytes);

	unsigned int highest1 = calculateMark(true, true,  // type definitions
	                                      0.0, 0.0,    // list length and is in list
	                                      0.0, 0.0,    // retrieving list elments
	                                      false);      // finished
	unsigned int mark1 = calculateMark(correct_node_bytes, correct_history_bytes,
	                                   0.0, 0.0,  // list length and is in list
	                                   0.0, 0.0,  // retrieving list elments
	                                   false);    // finished
	assert(mark1 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark1, highest1, TOTAL_MARKS_WITH_PART_C);

	cout << endl;
	cout << endl;


	//
	//  Test empty list
	//

	cout << "Testing empty History:" << endl;
	cout << "----------------------" << endl;
	cout << endl;

	cout << "Creating History" << endl;
	History history1;
	cout << "* Created without crashing" << endl;
	cout << endl;

	cout << "Creating some Boards for testing" << endl;
	cout << "* empty, black, white, marked" << endl;
	Board board_empty(3);
	Board board_black(3);
	board_black.replaceAll (BOARD_VALUE_EMPTY_CORRECT, BOARD_VALUE_BLACK_CORRECT);
	Board board_white(3);
	board_white.replaceAll (BOARD_VALUE_EMPTY_CORRECT, BOARD_VALUE_WHITE_CORRECT);
	Board board_marked(3);
	board_marked.replaceAll(BOARD_VALUE_EMPTY_CORRECT, BOARD_VALUE_MARKED_CORRECT);
	cout << "* Completed without crashing" << endl;
	cout << endl;

	BasicResults correct_empty = basicTests(history1, 0, board_black, board_white, board_marked, board_empty);

	cout << endl;
	cout << endl;


	//
	//  Print results 2
	//

	BasicTotals totals;
	initBasicTotals(totals);
	updateBasicTotals(totals, correct_empty, 0, 1, 0, 0);

	TestHelper::printSummaryHeaderPartial(24, 2, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("HistoryNode struct", correct_node_bytes);
	TestHelper::printSummaryLine("History definition", correct_history_bytes);
	TestHelper::printSummaryLine("isAtLeast?Long",     totals.at_least_N, totals.at_least_N_max);
	TestHelper::printSummaryLine("isInList",           totals.is_in_list, totals.is_in_list_max);

	unsigned int highest2 = calculateMark(true, true,  // type definitions
	                                      totals.at_least_N_highest, totals.is_in_list_highest,
	                                      totals.first_highest, totals.second_highest,
	                                      false);      // finished
	unsigned int mark2 = calculateMark(correct_node_bytes, correct_history_bytes,
	                                   totals.at_least_N_fraction, totals.is_in_list_fraction,
	                                   totals.first_fraction, totals.second_fraction,
	                                   false);    // finished
	assert(mark2 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark2, highest2, TOTAL_MARKS_WITH_PART_C);

	cout << endl;
	cout << endl;


	//
	//  Test list of 1 node
	//

	cout << "Testing History with one Node:" << endl;
	cout << "------------------------------" << endl;
	cout << endl;

	cout << "Adding all-black board to History" << endl;
	history1.addAtHead(board_black);
	cout << "* Completed without crashing" << endl;
	cout << endl;

	BasicResults correct_single = basicTests(history1, 1, board_black, board_white, board_marked, board_empty);

	cout << endl;
	cout << endl;


	//
	//  Print results 3
	//

	updateBasicTotals(totals, correct_single, 1, 2, 1, 0);

	TestHelper::printSummaryHeaderPartial(24, 2, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("HistoryNode struct", correct_node_bytes);
	TestHelper::printSummaryLine("History definition", correct_history_bytes);
	TestHelper::printSummaryLine("isAtLeast?Long",     totals.at_least_N, totals.at_least_N_max);
	TestHelper::printSummaryLine("isInList",           totals.is_in_list, totals.is_in_list_max);
	TestHelper::printSummaryLine("getFirst",           totals.first,      totals.first_max);

	unsigned int highest3 = calculateMark(true, true,  // type definitions
	                                      totals.at_least_N_highest, totals.is_in_list_highest,
	                                      totals.first_highest, totals.second_highest,
	                                      false);      // finished
	unsigned int mark3 = calculateMark(correct_node_bytes, correct_history_bytes,
	                                   totals.at_least_N_fraction, totals.is_in_list_fraction,
	                                   totals.first_fraction, totals.second_fraction,
	                                   false);    // finished
	assert(mark3 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark3, highest3, TOTAL_MARKS_WITH_PART_C);

	cout << endl;
	cout << endl;


	//
	//  Test list of 2 nodes
	//

	cout << "Testing History with two Nodes:" << endl;
	cout << "-------------------------------" << endl;
	cout << endl;

	cout << "Adding all-white board to History" << endl;
	history1.addAtHead(board_white);
	cout << "* Completed without crashing" << endl;
	cout << endl;

	BasicResults correct_double = basicTests(history1, 2, board_white, board_black, board_marked, board_empty);

	cout << endl;
	cout << endl;


	//
	//  Print results 4
	//

	updateBasicTotals(totals, correct_double, 2, 3, 2, 1);

	TestHelper::printSummaryHeaderPartial(24, 2, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("HistoryNode struct", correct_node_bytes);
	TestHelper::printSummaryLine("History definition", correct_history_bytes);
	TestHelper::printSummaryLine("isAtLeast?Long",     totals.at_least_N, totals.at_least_N_max);
	TestHelper::printSummaryLine("isInList",           totals.is_in_list, totals.is_in_list_max);
	TestHelper::printSummaryLine("getFirst",           totals.first,      totals.first_max);
	TestHelper::printSummaryLine("getSecond",          totals.second,     totals.second_max);

	unsigned int highest4 = calculateMark(true, true,  // type definitions
	                                      totals.at_least_N_highest, totals.is_in_list_highest,
	                                      totals.first_highest, totals.second_highest,
	                                      false);      // finished
	unsigned int mark4 = calculateMark(correct_node_bytes, correct_history_bytes,
	                                   totals.at_least_N_fraction, totals.is_in_list_fraction,
	                                   totals.first_fraction, totals.second_fraction,
	                                   false);    // finished
	assert(mark4 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark4, highest4, TOTAL_MARKS_WITH_PART_C);

	cout << endl;
	cout << endl;


	//
	//  Test list of 3 nodes
	//

	cout << "Testing History with three Nodes:" << endl;
	cout << "---------------------------------" << endl;
	cout << endl;

	cout << "Adding all-marked board to History" << endl;
	history1.addAtHead(board_marked);
	cout << "* Completed without crashing" << endl;
	cout << endl;

	BasicResults correct_triple = basicTests(history1, 3, board_marked, board_white, board_black, board_empty);

	cout << endl;
	cout << endl;


	//
	//  Print results 5
	//

	updateBasicTotals(totals, correct_triple, 3, 4, 3, 2);

	TestHelper::printSummaryHeaderPartial(24, 2, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("HistoryNode struct", correct_node_bytes);
	TestHelper::printSummaryLine("History definition", correct_history_bytes);
	TestHelper::printSummaryLine("isAtLeast?Long",     totals.at_least_N, totals.at_least_N_max);
	TestHelper::printSummaryLine("isInList",           totals.is_in_list, totals.is_in_list_max);
	TestHelper::printSummaryLine("getFirst",           totals.first,      totals.first_max);
	TestHelper::printSummaryLine("getSecond",          totals.second,     totals.second_max);

	unsigned int highest5 = calculateMark(true, true,  // type definitions
	                                      totals.at_least_N_highest, totals.is_in_list_highest,
	                                      totals.first_highest, totals.second_highest,
	                                      false);      // finished
	unsigned int mark5 = calculateMark(correct_node_bytes, correct_history_bytes,
	                                   totals.at_least_N_fraction, totals.is_in_list_fraction,
	                                   totals.first_fraction, totals.second_fraction,
	                                   false);    // finished
	assert(mark5 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark5, highest5, TOTAL_MARKS_WITH_PART_C);

	cout << endl;
	cout << endl;


	//
	//  Test after removing a node (2 left)
	//

	cout << "Testing History after removing head Node:" << endl;
	cout << "-----------------------------------------" << endl;
	cout << endl;

	cout << "Removing head node from History" << endl;
	history1.removeAtHead();
	cout << "* Completed without crashing" << endl;
	cout << endl;

	BasicResults correct_remove = basicTests(history1, 2, board_white, board_black, board_marked, board_empty);

	cout << endl;
	cout << endl;


	//
	//  Print results 6
	//

	updateBasicTotals(totals, correct_triple, 3, 5, 4, 3);

	TestHelper::printSummaryHeaderPartial(24, 2, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("HistoryNode struct", correct_node_bytes);
	TestHelper::printSummaryLine("History definition", correct_history_bytes);
	TestHelper::printSummaryLine("isAtLeast?Long",     totals.at_least_N, totals.at_least_N_max);
	TestHelper::printSummaryLine("isInList",           totals.is_in_list, totals.is_in_list_max);
	TestHelper::printSummaryLine("getFirst",           totals.first,      totals.first_max);
	TestHelper::printSummaryLine("getSecond",          totals.second,     totals.second_max);

	unsigned int highest6 = calculateMark(true, true,  // type definitions
	                                      totals.at_least_N_highest, totals.is_in_list_highest,
	                                      totals.first_highest, totals.second_highest,
	                                      false);      // finished
	unsigned int mark6 = calculateMark(correct_node_bytes, correct_history_bytes,
	                                   totals.at_least_N_fraction, totals.is_in_list_fraction,
	                                   totals.first_fraction, totals.second_fraction,
	                                   false);    // finished
	assert(mark6 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark6, highest6, TOTAL_MARKS_WITH_PART_C);

	cout << endl;
	cout << endl;


	//
	//  Test after removing all nodes
	//

	cout << "Testing History after removing all Nodes:" << endl;
	cout << "-----------------------------------------" << endl;
	cout << endl;

	cout << "Removing remaining nodes from History" << endl;
	history1.removeAtHead();
	cout << "* Removed one node without crashing" << endl;
	history1.removeAtHead();
	cout << "* Removed other node without crashing" << endl;
	cout << endl;

	BasicResults correct_clear = basicTests(history1, 0, board_black, board_white, board_marked, board_empty);

	cout << endl;
	cout << endl;


	//
	//  Print results 7
	//

	updateBasicTotals(totals, correct_clear, 0, 6, 4, 3);

	TestHelper::printSummaryHeaderEndOfFile(24, 2, DID_NOT_CRASH_RUNNING_MARKS);
	TestHelper::printSummaryLine("HistoryNode struct", correct_node_bytes);
	TestHelper::printSummaryLine("History definition", correct_history_bytes);
	TestHelper::printSummaryLine("isAtLeast?Long",     totals.at_least_N, totals.at_least_N_max);
	TestHelper::printSummaryLine("isInList",           totals.is_in_list, totals.is_in_list_max);
	TestHelper::printSummaryLine("getFirst",           totals.first,      totals.first_max);
	TestHelper::printSummaryLine("getSecond",          totals.second,     totals.second_max);

	unsigned int mark7 = calculateMark(correct_node_bytes, correct_history_bytes,
	                                   totals.at_least_N_fraction, totals.is_in_list_fraction,
	                                   totals.first_fraction, totals.second_fraction,
	                                   true);  // finished
	assert(mark7 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark7, TOTAL_MARKS, TOTAL_MARKS_WITH_PART_C);

	TestHelper::waitForEnter();
	return 0;
}



BasicResults basicTests (History& r_history,
                         unsigned int count,
                         const Board& board_first,
                         const Board& board_second,
                         const Board& board_third,
                         const Board& board_empty)
{
	BasicResults result;

	result.at_least_1 = testAtLeastNLong(r_history, 1, (count >= 1));
	result.at_least_2 = testAtLeastNLong(r_history, 2, (count >= 2));
	result.at_least_3 = testAtLeastNLong(r_history, 3, (count >= 3));
	cout << endl;

	cout << "Testing which boards are in list" << endl;
	result.is_in_list = testIsInList(r_history, board_empty,
	                                 board_first,  (count >= 1),
	                                 board_second, (count >= 2),
	                                 board_third,  (count >= 3));
	cout << endl;

	if(count >= 1)
	{
		cout << "Retrieving board at head of list" << endl;
		result.newest = testGetFirst(r_history, board_first, board_empty,
		                             (count >= 2) ? &board_second : nullptr,
		                             (count >= 3) ? &board_third  : nullptr);
		cout << endl;
	}
	else
		result.newest = false;

	if(count >= 2)
	{
		cout << "Retrieving 2nd board in list (after head)" << endl;
		result.second = testGetSecond(r_history, board_second, board_first, board_empty,
		                              (count >= 3) ? &board_third  : nullptr);
		cout << endl;
	}
	else
		result.second = false;

	return result;
}

void initBasicTotals (BasicTotals& r_totals)
{
	r_totals.at_least_N          = 0;
	r_totals.at_least_N_max      = 0;
	r_totals.at_least_N_highest  = 0.0;
	r_totals.at_least_N_fraction = 0.0;

	r_totals.is_in_list          = 0;
	r_totals.is_in_list_max      = 0;
	r_totals.is_in_list_highest  = 0.0;
	r_totals.is_in_list_fraction = 0.0;

	r_totals.first          = 0;
	r_totals.first_max      = 0;
	r_totals.first_highest  = 0.0;
	r_totals.first_fraction = 0.0;

	r_totals.second          = 0;
	r_totals.second_max      = 0;
	r_totals.second_highest  = 0.0;
	r_totals.second_fraction = 0.0;
}

void updateBasicTotals (BasicTotals& r_totals,
                        const BasicResults& results,
                        unsigned int history_size,
                        unsigned int trials0,
                        unsigned int trials1,
                        unsigned int trials2)
{
	r_totals.at_least_N     += (results.at_least_1 ? 1 : 0) +
	                           (results.at_least_2 ? 1 : 0) +
	                           (results.at_least_3 ? 1 : 0);
	r_totals.at_least_N_max += CORRECT_AT_LEAST_N_LONG_COUNT;
	r_totals.at_least_N_highest  = (double)(trials0) / BASIC_TEST_COUNT;
	r_totals.at_least_N_fraction = (double)(r_totals.at_least_N) /
	                               (CORRECT_AT_LEAST_N_LONG_COUNT * BASIC_TEST_COUNT);

	r_totals.is_in_list     += results.is_in_list;
	r_totals.is_in_list_max += BOARD_VALUE_COUNT;
	r_totals.is_in_list_highest  = (double)(trials0) / BASIC_TEST_COUNT;
	r_totals.is_in_list_fraction = (double)(r_totals.is_in_list) /
	                               (BOARD_VALUE_COUNT * BASIC_TEST_COUNT);

	if(history_size >= 1)
	{
		r_totals.first     += (results.newest ? 1 : 0);
		r_totals.first_max += 1;
		r_totals.first_highest  = (double)(trials1        ) / BASIC_TEST_1_COUNT;
		r_totals.first_fraction = (double)(r_totals.first) / BASIC_TEST_1_COUNT;
	}

	if(history_size >= 2)
	{
		r_totals.second     += (results.second ? 1 : 0);
		r_totals.second_max += 1;
		r_totals.second_highest  = (double)(trials2        ) / BASIC_TEST_2_COUNT;
		r_totals.second_fraction = (double)(r_totals.second) / BASIC_TEST_2_COUNT;
	}
}



bool testAtLeastNLong (const History& history,
                       unsigned int how_long,
                       bool correct_result)
{
	assert(how_long >= 1);
	assert(how_long <= 3);

	bool result = false;
	switch(how_long)
	{
	case 1:
		cout << "Testing isAtLeast1Long: ";
		result = history.isAtLeast1Long();
		break;
	case 2:
		cout << "Testing isAtLeast2Long: ";
		result = history.isAtLeast2Long();
		break;
	case 3:
		cout << "Testing isAtLeast3Long: ";
		result = history.isAtLeast3Long();
		break;
	}
	cout << result << endl;

	if(result == correct_result)
	{
		cout << "* Correct" << endl;
		return true;
	}
	else
	{
		cout << "* Incorrect: Should return " << boolalpha << correct_result << endl;
		return false;
	}
}

unsigned int testIsInList (const History& history,
                           const Board& board_empty,
                           const Board& board_black,  bool correct_black,
                           const Board& board_white,  bool correct_white,
                           const Board& board_marked, bool correct_marked)
{
	unsigned int correct_count = 0;
	correct_count += testIsInList(history, board_empty,  false,          "empty");
	correct_count += testIsInList(history, board_black,  correct_black,  "black");
	correct_count += testIsInList(history, board_white,  correct_white,  "white");
	correct_count += testIsInList(history, board_marked, correct_marked, "marked");

	if(correct_count == BOARD_VALUE_COUNT)
		cout << "* Correct" << endl;
	return correct_count;
}

unsigned int testIsInList (const History& history,
                           const Board& board,
                           bool correct_result,
                           const string& board_name)
{
	bool result = history.isInList(board);
	if(result == correct_result)
		return 1;
	if(result = false)
		cout << "* Incorrect: Should not say " << board_name << " board is in list" << endl;
	else
		cout << "* Incorrect: Should say " << board_name << " board is in list" << endl;
	return 0;
}

bool testGetFirst (const History& history,
                   const Board& correct_board,
                   const Board& empty_board,
                   const Board* p_2nd_board,
                   const Board* p_3rd_board)
{
	if(!history.isAtLeast1Long())
	{
		cout << "* Skipping: History contains no nodes" << endl;
		return false;
	}

	const Board& result = history.getFirst();

	if(result == correct_board)
	{
		cout << "* Correct" << endl;
		return true;
	}
	else if(result == empty_board)
	{
		cout << "* Incorrect: Returned empty board" << endl;
		return false;
	}
	else if(p_2nd_board != nullptr && result == *p_2nd_board)
	{
		cout << "* Incorrect: Returned 2nd-newest board" << endl;
		return false;
	}
	else if(p_3rd_board != nullptr && result == *p_3rd_board)
	{
		cout << "* Incorrect: Returned 3rd-newest board" << endl;
		return false;
	}
	else
	{
		cout << "* Incorrect: Returned incorrect board" << endl;
		return false;
	}
}

bool testGetSecond (const History& history,
                    const Board& correct_board,
                    const Board& head_board,
                    const Board& empty_board,
                    const Board* p_3rd_board)
{
	if(!history.isAtLeast1Long())
	{
		cout << "* Skipping: History contains no nodes" << endl;
		return false;
	}
	if(!history.isAtLeast2Long())
	{
		cout << "* Skipping: History contains only one node" << endl;
		return false;
	}

	const Board& result = history.getSecond();

	if(result == correct_board)
	{
		cout << "* Correct" << endl;
		return true;
	}
	else if(result == head_board)
	{
		cout << "* Incorrect: Returned board at head of list" << endl;
		return false;
	}
	else if(result == empty_board)
	{
		cout << "* Incorrect: Returned empty board" << endl;
		return false;
	}
	else if(p_3rd_board != nullptr && result == *p_3rd_board)
	{
		cout << "* Incorrect: Returned 3rd-newest board" << endl;
		return false;
	}
	else
	{
		cout << "* Incorrect: Returned incorrect board" << endl;
		return false;
	}
}



int calculateMark (bool correct_node_bytes,
                   bool correct_history_bytes,
                   double correct_at_least_N_fraction,
                   double correct_is_in_list_fraction,
                   double correct_first_fraction,
                   double correct_second_fraction,
                   bool is_finished)
{
	unsigned int mark = 0;
	if(is_finished)
		mark = COMPILE_AND_NOT_CRASH_MARKS;
	else
		mark = COMPILE_AND_START_MARKS;

	// Part B marks
	if(correct_node_bytes)
		mark += 1;
	if(correct_history_bytes)
		mark += 1;
	mark += (int)(6.0 * correct_at_least_N_fraction +
	              6.0 * correct_is_in_list_fraction +
	              3.0 * correct_first_fraction +
	              3.0 * correct_second_fraction);

	assert(mark <= TOTAL_MARKS);
	return mark;
}
