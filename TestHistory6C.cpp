//
//  TestHistory6C.cpp
//
//  A test program for the first part of the History module.
//
//  This program is to be used with Assignment 6: Part C for
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

// this doesn't work because superclass destructor is not virtual
class CountedHistoryNode : public HistoryNode
{
public:
	static int s_allocated;
	CountedHistoryNode ()
			: HistoryNode()
	{
		s_allocated++;
	}
	CountedHistoryNode (const CountedHistoryNode& to_copy)
			: HistoryNode(to_copy)
	{
		s_allocated++;
	}
	~CountedHistoryNode ()
	{
		s_allocated--;
	}
};
int CountedHistoryNode :: s_allocated = 0;

bool testDestroyLinkedList (HistoryNode* p_head,
                            int total_length);
int testCopyLinkedList (const HistoryNode* p_old_head,
                        int correct_length);
bool testLength (const History& history,
                 int correct_length,
                 bool is_print_correct);

int calculateMark (bool correct_node_bytes,
                   bool correct_history_bytes,
                   double correct_at_least_N_fraction,
                   double correct_is_in_list_fraction,
                   double correct_first_fraction,
                   double correct_second_fraction,
                   double correct_copy_fraction,
                   double correct_copy_con_fraction,
                   double correct_remove_all_fraction,
                   double correct_assign_fraction,
                   double correct_self_fraction,
                   double correct_chain_fraction,
                   bool is_finished);

const int COMPILE_AND_START_MARKS     =  7;
const int DID_NOT_CRASH_RUNNING_MARKS = 11;
const int COMPILE_AND_NOT_CRASH_MARKS = COMPILE_AND_START_MARKS + DID_NOT_CRASH_RUNNING_MARKS;
const int COMBINED_TEST_MARKS         = 37;
const int TOTAL_MARKS                 = COMPILE_AND_NOT_CRASH_MARKS + COMBINED_TEST_MARKS;

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
	TestHelper::startup("TestHistory6C", COMPILE_AND_START_MARKS, TOTAL_MARKS);


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

	unsigned int highest1 = calculateMark(true, true,     // type definitions
	                                      0.0, 0.0,       // list length and is in list
	                                      0.0, 0.0,       // retrieving list elments
	                                      0.0, 0.0,       // copy list, copy constructor
	                                      0.0,            // removeAll
	                                      0.0, 0.0, 0.0,  // assignment operator
	                                      false);         // finished
	unsigned int mark1 = calculateMark(correct_node_bytes, correct_history_bytes,
	                                   0.0, 0.0,       // list length and is in list
	                                   0.0, 0.0,       // retrieving list elments
	                                   0.0, 0.0,       // copy list, copy constructor
	                                   0.0,            // removeAll
	                                   0.0, 0.0, 0.0,  // assignment operator
	                                   false);         // finished
	assert(mark1 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark1, highest1, TOTAL_MARKS);

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
	TestHelper::printSummaryLine("isAtLeast?Long",     totals.at_least_N,  totals.at_least_N_max);
	TestHelper::printSummaryLine("isInList",           totals.is_in_list,  totals.is_in_list_max);

	unsigned int highest2 = calculateMark(true, true,     // type definitions
	                                      totals.at_least_N_highest, totals.is_in_list_highest,
	                                      totals.first_highest, totals.second_highest,
	                                      0.0, 0.0,       // copy list, copy constructor
	                                      0.0,            // removeAll
	                                      0.0, 0.0, 0.0,  // assignment operator
	                                      false);         // finished
	unsigned int mark2 = calculateMark(correct_node_bytes, correct_history_bytes,
	                                   totals.at_least_N_fraction, totals.is_in_list_fraction,
	                                   totals.first_fraction, totals.second_fraction,
	                                   0.0, 0.0,       // copy list, copy constructor
	                                   0.0,            // removeAll
	                                   0.0, 0.0, 0.0,  // assignment operator
	                                   false);         // finished
	assert(mark2 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark2, highest2, TOTAL_MARKS);

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
	TestHelper::printSummaryLine("isAtLeast?Long",     totals.at_least_N,  totals.at_least_N_max);
	TestHelper::printSummaryLine("isInList",           totals.is_in_list,  totals.is_in_list_max);
	TestHelper::printSummaryLine("getFirst",           totals.first,       totals.first_max);

	unsigned int highest3 = calculateMark(true, true,     // type definitions
	                                      totals.at_least_N_highest, totals.is_in_list_highest,
	                                      totals.first_highest, totals.second_highest,
	                                      0.0, 0.0,       // copy list, copy constructor
	                                      0.0,            // removeAll
	                                      0.0, 0.0, 0.0,  // assignment operator
	                                      false);         // finished
	unsigned int mark3 = calculateMark(correct_node_bytes, correct_history_bytes,
	                                   totals.at_least_N_fraction, totals.is_in_list_fraction,
	                                   totals.first_fraction, totals.second_fraction,
	                                   0.0, 0.0,       // copy list, copy constructor
	                                   0.0,            // removeAll
	                                   0.0, 0.0, 0.0,  // assignment operator
	                                   false);         // finished
	assert(mark3 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark3, highest3, TOTAL_MARKS);

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
	TestHelper::printSummaryLine("isAtLeast?Long",     totals.at_least_N,  totals.at_least_N_max);
	TestHelper::printSummaryLine("isInList",           totals.is_in_list,  totals.is_in_list_max);
	TestHelper::printSummaryLine("getFirst",           totals.first,       totals.first_max);
	TestHelper::printSummaryLine("getSecond",          totals.second,      totals.second_max);

	unsigned int highest4 = calculateMark(true, true,     // type definitions
	                                      totals.at_least_N_highest, totals.is_in_list_highest,
	                                      totals.first_highest, totals.second_highest,
	                                      0.0, 0.0,       // copy list, copy constructor
	                                      0.0,            // removeAll
	                                      0.0, 0.0, 0.0,  // assignment operator
	                                      false);         // finished
	unsigned int mark4 = calculateMark(correct_node_bytes, correct_history_bytes,
	                                   totals.at_least_N_fraction, totals.is_in_list_fraction,
	                                   totals.first_fraction, totals.second_fraction,
	                                   0.0, 0.0,       // copy list, copy constructor
	                                   0.0,            // removeAll
	                                   0.0, 0.0, 0.0,  // assignment operator
	                                   false);         // finished
	assert(mark4 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark4, highest4, TOTAL_MARKS);

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
	TestHelper::printSummaryLine("isAtLeast?Long",     totals.at_least_N,  totals.at_least_N_max);
	TestHelper::printSummaryLine("isInList",           totals.is_in_list,  totals.is_in_list_max);
	TestHelper::printSummaryLine("getFirst",           totals.first,       totals.first_max);
	TestHelper::printSummaryLine("getSecond",          totals.second,      totals.second_max);

	unsigned int highest5 = calculateMark(true, true,     // type definitions
	                                      totals.at_least_N_highest, totals.is_in_list_highest,
	                                      totals.first_highest, totals.second_highest,
	                                      0.0, 0.0,       // copy list, copy constructor
	                                      0.0,            // removeAll
	                                      0.0, 0.0, 0.0,  // assignment operator
	                                      false);         // finished
	unsigned int mark5 = calculateMark(correct_node_bytes, correct_history_bytes,
	                                   totals.at_least_N_fraction, totals.is_in_list_fraction,
	                                   totals.first_fraction, totals.second_fraction,
	                                   0.0, 0.0,       // copy list, copy constructor
	                                   0.0,            // removeAll
	                                   0.0, 0.0, 0.0,  // assignment operator
	                                   false);         // finished
	assert(mark5 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark5, highest5, TOTAL_MARKS);

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
	TestHelper::printSummaryLine("isAtLeast?Long",     totals.at_least_N,  totals.at_least_N_max);
	TestHelper::printSummaryLine("isInList",           totals.is_in_list,  totals.is_in_list_max);
	TestHelper::printSummaryLine("getFirst",           totals.first,       totals.first_max);
	TestHelper::printSummaryLine("getSecond",          totals.second,      totals.second_max);

	unsigned int highest6 = calculateMark(true, true,     // type definitions
	                                      totals.at_least_N_highest, totals.is_in_list_highest,
	                                      totals.first_highest, totals.second_highest,
	                                      0.0, 0.0,       // copy list, copy constructor
	                                      0.0,            // removeAll
	                                      0.0, 0.0, 0.0,  // assignment operator
	                                      false);         // finished
	unsigned int mark6 = calculateMark(correct_node_bytes, correct_history_bytes,
	                                   totals.at_least_N_fraction, totals.is_in_list_fraction,
	                                   totals.first_fraction, totals.second_fraction,
	                                   0.0, 0.0,       // copy list, copy constructor
	                                   0.0,            // removeAll
	                                   0.0, 0.0, 0.0,  // assignment operator
	                                   false);         // finished
	assert(mark6 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark6, highest6, TOTAL_MARKS);

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

	TestHelper::printSummaryHeaderPartial(24, 2, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("HistoryNode struct", correct_node_bytes);
	TestHelper::printSummaryLine("History definition", correct_history_bytes);
	TestHelper::printSummaryLine("isAtLeast?Long",     totals.at_least_N,  totals.at_least_N_max);
	TestHelper::printSummaryLine("isInList",           totals.is_in_list,  totals.is_in_list_max);
	TestHelper::printSummaryLine("getFirst",           totals.first,       totals.first_max);
	TestHelper::printSummaryLine("getSecond",          totals.second,      totals.second_max);

	unsigned int highest7 = calculateMark(true, true,     // type definitions
	                                      totals.at_least_N_highest, totals.is_in_list_highest,
	                                      totals.first_highest, totals.second_highest,
	                                      0.0, 0.0,       // copy list, copy constructor
	                                      0.0,            // removeAll
	                                      0.0, 0.0, 0.0,  // assignment operator
	                                      false);         // finished
	unsigned int mark7 = calculateMark(correct_node_bytes, correct_history_bytes,
	                                   totals.at_least_N_fraction, totals.is_in_list_fraction,
	                                   totals.first_fraction, totals.second_fraction,
	                                   0.0, 0.0,       // copy list, copy constructor
	                                   0.0,            // removeAll
	                                   0.0, 0.0, 0.0,  // assignment operator
	                                   false);         // finished
	assert(mark7 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark7, highest7, TOTAL_MARKS);

	cout << endl;
	cout << endl;


	//
	//  Test destroying linked lists
	//

	cout << "Testing destroyLinkedList:" << endl;
	cout << "--------------------------" << endl;
	cout << endl;

	cout << "Calling destroyLinkedList on nullptr" << endl;
	assert(CountedHistoryNode::s_allocated == 0);
	bool correct_destroy0 = testDestroyLinkedList(nullptr, 0);
	cout << endl;

	cout << "Calling destroyLinkedList on a single node" << endl;
	CountedHistoryNode::s_allocated = 0;
	HistoryNode* p_destroy_list1 = new CountedHistoryNode;
	p_destroy_list1->p_next = nullptr;
	cout << "* Created list without crashing" << endl;
	bool correct_destroy1 = testDestroyLinkedList(p_destroy_list1, 1);
	cout << endl;

	cout << "Calling destroyLinkedList on a 2-node linked list" << endl;
	CountedHistoryNode::s_allocated = 0;
	HistoryNode* p_destroy_list2 = new CountedHistoryNode;
	p_destroy_list2->p_next = new CountedHistoryNode;
	p_destroy_list2->p_next
	               ->p_next = nullptr;
	cout << "* Created list without crashing" << endl;
	bool correct_destroy2 = testDestroyLinkedList(p_destroy_list2, 2);
	cout << endl;

	cout << "Calling destroyLinkedList on a 3-node linked list" << endl;
	CountedHistoryNode::s_allocated = 0;
	HistoryNode* p_destroy_list3 = new CountedHistoryNode;
	p_destroy_list3->p_next = new CountedHistoryNode;
	p_destroy_list3->p_next
	               ->p_next = new CountedHistoryNode;
	p_destroy_list3->p_next
	               ->p_next
	               ->p_next = nullptr;
	cout << "* Created list without crashing" << endl;
	bool correct_destroy3 = testDestroyLinkedList(p_destroy_list3, 3);
	cout << endl;

	cout << endl;
	cout << endl;


	//
	//  Print results 8
	//

	// these are always true
	unsigned int correct_destroy = (correct_destroy0 ? 1 : 0) +
	                               (correct_destroy1 ? 1 : 0) +
	                               (correct_destroy2 ? 1 : 0) +
	                               (correct_destroy3 ? 1 : 0);
	double correct_destroy_fraction = correct_destroy / 4.0;

	TestHelper::printSummaryHeaderPartial(24, 2, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("HistoryNode struct", correct_node_bytes);
	TestHelper::printSummaryLine("History definition", correct_history_bytes);
	TestHelper::printSummaryLine("isAtLeast?Long",     totals.at_least_N,  totals.at_least_N_max);
	TestHelper::printSummaryLine("isInList",           totals.is_in_list,  totals.is_in_list_max);
	TestHelper::printSummaryLine("getFirst",           totals.first,       totals.first_max);
	TestHelper::printSummaryLine("getSecond",          totals.second,      totals.second_max);
	//TestHelper::printSummaryLine("destroyLinkedList",  correct_destroy,    4);
	TestHelper::printSummaryLine("destroyLinkedList",  true);

	unsigned int highest8 = calculateMark(true, true,     // type definitions
	                                      totals.at_least_N_highest, totals.is_in_list_highest,
	                                      totals.first_highest, totals.second_highest,
	                                      0.0, 0.0,       // copy list, copy constructor
	                                      0.0,            // removeAll
	                                      0.0, 0.0, 0.0,  // assignment operator
	                                      false);         // finished
	unsigned int mark8 = calculateMark(correct_node_bytes, correct_history_bytes,
	                                   totals.at_least_N_fraction, totals.is_in_list_fraction,
	                                   totals.first_fraction, totals.second_fraction,
	                                   0.0, 0.0,       // copy list, copy constructor
	                                   0.0,            // removeAll
	                                   0.0, 0.0, 0.0,  // assignment operator
	                                   false);         // finished
	assert(mark8 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark8, highest8, TOTAL_MARKS);

	cout << endl;
	cout << endl;


	//
	//  Test copying linked lists
	//

	cout << "Testing copyLinkedList:" << endl;
	cout << "-----------------------" << endl;
	cout << endl;

	cout << "Calling copyLinkedList on nullptr" << endl;
	int correct_copy0 = testCopyLinkedList(nullptr, 0);
	cout << endl;

	cout << "Calling copyLinkedList on a single node" << endl;
	HistoryNode* p_copy_list1 = new CountedHistoryNode;
	p_copy_list1->p_next = nullptr;
	cout << "* Created list without crashing" << endl;
	int correct_copy1 = testCopyLinkedList(p_copy_list1, 1);
	cout << endl;

	cout << "Calling copyLinkedList on a 2-node linked list" << endl;
	HistoryNode* p_copy_list2 = new CountedHistoryNode;
	p_copy_list2->p_next = new CountedHistoryNode;
	p_copy_list2->p_next
	            ->p_next = nullptr;
	cout << "* Created list without crashing" << endl;
	int correct_copy2 = testCopyLinkedList(p_copy_list2, 2);
	cout << endl;

	cout << "Calling copyLinkedList on a 3-node linked list" << endl;
	HistoryNode* p_copy_list3 = new CountedHistoryNode;
	p_copy_list3->p_next = new CountedHistoryNode;
	p_copy_list3->p_next
	            ->p_next = new CountedHistoryNode;
	p_copy_list3->p_next
	            ->p_next
	            ->p_next = nullptr;
	cout << "* Created list without crashing" << endl;
	int correct_copy3 = testCopyLinkedList(p_copy_list3, 3);
	cout << endl;

	cout << endl;
	cout << endl;


	//
	//  Print results 9
	//

	unsigned int correct_copy = correct_copy0 +
	                            correct_copy1 +
	                            correct_copy2 +
	                            correct_copy3;
	double correct_copy_fraction = correct_copy / 10.0;

	TestHelper::printSummaryHeaderPartial(24, 2, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("HistoryNode struct", correct_node_bytes);
	TestHelper::printSummaryLine("History definition", correct_history_bytes);
	TestHelper::printSummaryLine("isAtLeast?Long",     totals.at_least_N,  totals.at_least_N_max);
	TestHelper::printSummaryLine("isInList",           totals.is_in_list,  totals.is_in_list_max);
	TestHelper::printSummaryLine("getFirst",           totals.first,       totals.first_max);
	TestHelper::printSummaryLine("getSecond",          totals.second,      totals.second_max);
	//TestHelper::printSummaryLine("destroyLinkedList",  correct_destroy,    4);
	TestHelper::printSummaryLine("destroyLinkedList",  true);
	TestHelper::printSummaryLine("copyLinkedList",     correct_copy,       10);

	unsigned int highest9 = calculateMark(true, true,     // type definitions
	                                      totals.at_least_N_highest, totals.is_in_list_highest,
	                                      totals.first_highest, totals.second_highest,
	                                      1.0, 0.0,       // copy list, copy constructor
	                                      0.0,            // removeAll
	                                      0.0, 0.0, 0.0,  // assignment operator
	                                      false);         // finished
	unsigned int mark9 = calculateMark(correct_node_bytes, correct_history_bytes,
	                                   totals.at_least_N_fraction, totals.is_in_list_fraction,
	                                   totals.first_fraction, totals.second_fraction,
	                                   correct_copy_fraction, 0.0,  // copy constructor
	                                   0.0,            // removeAll
	                                   0.0, 0.0, 0.0,  // assignment operator
	                                   false);         // finished
	assert(mark9 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark9, highest9, TOTAL_MARKS);

	cout << endl;
	cout << endl;


	//
	//  Test copying History
	//

	cout << "Testing copy constructor:" << endl;
	cout << "-------------------------" << endl;
	cout << endl;

	cout << "Copying History with 0 nodes" << endl;
	History* p_history0 = new History();
	cout << "* Created empty history without crashing" << endl;
	History* p_copy0 = new History(*p_history0);
	cout << "* Called copy constructor without crashing" << endl;
	cout << endl;

	cout << "Testing length of copy" << endl;
	bool correct_copy_con_0 = testLength(*p_copy0, 0, true);
	cout << endl;

	cout << "Copying History with 3 nodes" << endl;
	History* p_history3 = new History();
	cout << "* Created empty history without crashing" << endl;
	p_history3->addAtHead(board_marked);
	p_history3->addAtHead(board_white);
	p_history3->addAtHead(board_black);
	cout << "* Added nodes without crashing" << endl;
	History* p_copy3 = new History(*p_history3);
	cout << "* Called copy constructor without crashing" << endl;
	cout << endl;

	cout << "Testing length of copy" << endl;
	bool correct_copy_con_3 = testLength(*p_copy3, 3, true);
	cout << endl;

	cout << endl;
	cout << endl;


	//
	//  Print results 10
	//

	unsigned int correct_copy_con = (correct_copy_con_0 ? 1 : 0) +
	                                (correct_copy_con_3 ? 1 : 0);
	double correct_copy_con_fraction = correct_copy_con / 2.0;

	TestHelper::printSummaryHeaderPartial(24, 2, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("HistoryNode struct", correct_node_bytes);
	TestHelper::printSummaryLine("History definition", correct_history_bytes);
	TestHelper::printSummaryLine("isAtLeast?Long",     totals.at_least_N,  totals.at_least_N_max);
	TestHelper::printSummaryLine("isInList",           totals.is_in_list,  totals.is_in_list_max);
	TestHelper::printSummaryLine("getFirst",           totals.first,       totals.first_max);
	TestHelper::printSummaryLine("getSecond",          totals.second,      totals.second_max);
	//TestHelper::printSummaryLine("destroyLinkedList",  correct_destroy,    4);
	TestHelper::printSummaryLine("destroyLinkedList",  true);
	TestHelper::printSummaryLine("copyLinkedList",     correct_copy,       10);
	TestHelper::printSummaryLine("Copy constructor",   correct_copy_con,   2);

	unsigned int highest10 = calculateMark(true, true,     // type definitions
	                                       totals.at_least_N_highest, totals.is_in_list_highest,
	                                       totals.first_highest, totals.second_highest,
	                                       1.0, 1.0,       // copy list, copy constructor
	                                       0.0,            // removeAll
	                                       0.0, 0.0, 0.0,  // assignment operator
	                                       false);         // finished
	unsigned int mark10 = calculateMark(correct_node_bytes, correct_history_bytes,
	                                    totals.at_least_N_fraction, totals.is_in_list_fraction,
	                                    totals.first_fraction, totals.second_fraction,
	                                    correct_copy_fraction, correct_copy_con_fraction,
	                                    0.0,            // removeAll
	                                    0.0, 0.0, 0.0,  // assignment operator
	                                    false);         // finished
	assert(mark10 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark10, highest10, TOTAL_MARKS);

	cout << endl;
	cout << endl;


	//
	//  Test destroying History
	//

	cout << "Testing destructors:" << endl;
	cout << "--------------------" << endl;
	cout << endl;

	cout << "Destroying History with 0 nodes" << endl;
	delete p_history0;
	p_history0 = nullptr;
	cout << "* Destroyed original without crashing" << endl;
	delete p_copy0;
	p_copy0 = nullptr;
	cout << "* Destroyed copy without crashing" << endl;
	cout << endl;

	cout << "Destroying History with 3 nodes" << endl;
	delete p_history3;
	p_history3 = nullptr;
	cout << "* Destroyed original without crashing" << endl;
	delete p_copy3;
	p_copy3 = nullptr;
	cout << "* Destroyed copy without crashing" << endl;
	cout << endl;

	cout << endl;
	cout << endl;


	cout << "Testing removeAll:" << endl;
	cout << "------------------" << endl;
	cout << endl;

	cout << "Creating Historys" << endl;
	History* p_remove0 = new History();
	History* p_remove1 = new History();
	p_remove1->addAtHead(board_black);
	History* p_remove2 = new History();
	p_remove2->addAtHead(board_black);
	p_remove2->addAtHead(board_white);
	cout << "* Completed without crashing" << endl;
	cout << endl;

	cout << "Removing all from empty History" << endl;
	p_remove0->removeAll();
	cout << "* Completed without crashing" << endl;
	bool correct_remove_all_0 = testLength(*p_remove0, 0, true);
	cout << endl;

	cout << "Removing all from 1-node History" << endl;
	p_remove1->removeAll();
	cout << "* Completed without crashing" << endl;
	bool correct_remove_all_1 = testLength(*p_remove1, 0, true);
	cout << endl;

	cout << "Removing all from 2-node History" << endl;
	p_remove2->removeAll();
	cout << "* Completed without crashing" << endl;
	bool correct_remove_all_2 = testLength(*p_remove2, 0, true);
	cout << endl;

	cout << "Destroying Historys" << endl;
	delete p_remove0;
	delete p_remove1;
	delete p_remove2;
	p_remove0 = nullptr;
	p_remove1 = nullptr;
	p_remove2 = nullptr;
	cout << "* Completed without crashing" << endl;
	cout << endl;

	cout << endl;
	cout << endl;


	//
	//  Print results 11
	//

	unsigned int correct_remove_all = (correct_remove_all_0 ? 1 : 0) +
	                                  (correct_remove_all_1 ? 1 : 0) +
	                                  (correct_remove_all_2 ? 1 : 0);
	double correct_remove_all_fraction = correct_remove_all / 3.0;

	TestHelper::printSummaryHeaderPartial(24, 2, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("HistoryNode struct", correct_node_bytes);
	TestHelper::printSummaryLine("History definition", correct_history_bytes);
	TestHelper::printSummaryLine("isAtLeast?Long",     totals.at_least_N,  totals.at_least_N_max);
	TestHelper::printSummaryLine("isInList",           totals.is_in_list,  totals.is_in_list_max);
	TestHelper::printSummaryLine("getFirst",           totals.first,       totals.first_max);
	TestHelper::printSummaryLine("getSecond",          totals.second,      totals.second_max);
	//TestHelper::printSummaryLine("destroyLinkedList",  correct_destroy,    4);
	TestHelper::printSummaryLine("destroyLinkedList",  true);
	TestHelper::printSummaryLine("copyLinkedList",     correct_copy,       10);
	TestHelper::printSummaryLine("Copy constructor",   correct_copy_con,   2);
	TestHelper::printSummaryLine("Destructor",         true);
	TestHelper::printSummaryLine("removeAll" ,         correct_remove_all, 3);

	unsigned int highest11 = calculateMark(true, true,     // type definitions
	                                       totals.at_least_N_highest, totals.is_in_list_highest,
	                                       totals.first_highest, totals.second_highest,
	                                       1.0, 1.0,       // copy list, copy constructor
	                                       1.0,            // removeAll
	                                       0.0, 0.0, 0.0,  // assignment operator
	                                       false);         // finished
	unsigned int mark11 = calculateMark(correct_node_bytes, correct_history_bytes,
	                                    totals.at_least_N_fraction, totals.is_in_list_fraction,
	                                    totals.first_fraction, totals.second_fraction,
	                                    correct_copy_fraction, correct_copy_con_fraction,
	                                    correct_remove_all_fraction,
	                                    0.0, 0.0, 0.0,  // assignment operator
	                                    false);         // finished
	assert(mark11 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark11, highest11, TOTAL_MARKS);

	cout << endl;
	cout << endl;


	//
	//  Test History assignment operator (basic)
	//

	cout << "Testing basic assignment:" << endl;
	cout << "-------------------------" << endl;
	cout << endl;

	cout << "Creating Historys" << endl;
	History* p_assign0 = new History();
	History* p_assign1 = new History();
	p_assign1->addAtHead(board_black);
	History* p_assign2 = new History();
	p_assign2->addAtHead(board_black);
	p_assign2->addAtHead(board_white);
	History* p_assignX = new History();
	cout << "* Completed without crashing" << endl;
	cout << endl;

	cout << "Assigning empty History to empty History" << endl;
	*p_assignX = *p_assign0;
	cout << "* Completed without crashing" << endl;
	bool correct_assign00 = testLength(*p_assignX, 0, true);
	cout << endl;

	cout << "Assigning empty History to 1-node History" << endl;
	*p_assignX = *p_assign1;
	cout << "* Completed without crashing" << endl;
	bool correct_assign01 = testLength(*p_assignX, 1, true);
	cout << endl;

	cout << "Assigning 1-node History to 1-node History" << endl;
	*p_assignX = *p_assign1;
	cout << "* Completed without crashing" << endl;
	bool correct_assign11 = testLength(*p_assignX, 1, true);
	cout << endl;

	cout << "Assigning 1-node History to empty History" << endl;
	*p_assignX = *p_assign0;
	cout << "* Completed without crashing" << endl;
	bool correct_assign10 = testLength(*p_assignX, 0, true);
	cout << endl;

	cout << "Assigning empty History to 2-node History" << endl;
	*p_assignX = *p_assign2;
	cout << "* Completed without crashing" << endl;
	bool correct_assign02 = testLength(*p_assignX, 2, true);
	cout << endl;

	cout << "Assigning 2-node History to 2-node History" << endl;
	*p_assignX = *p_assign2;
	cout << "* Completed without crashing" << endl;
	bool correct_assign22 = testLength(*p_assignX, 2, true);
	cout << endl;

	cout << "Assigning 2-node History to empty History" << endl;
	*p_assignX = *p_assign0;
	cout << "* Completed without crashing" << endl;
	bool correct_assign20 = testLength(*p_assignX, 0, true);
	cout << endl;

	cout << "Destroying Historys" << endl;
	delete p_assign0;
	delete p_assign1;
	delete p_assign2;
	delete p_assignX;
	p_assign0 = nullptr;
	p_assign1 = nullptr;
	p_assign2 = nullptr;
	p_assignX = nullptr;
	cout << "* Completed without crashing" << endl;
	cout << endl;

	cout << endl;
	cout << endl;


	//
	//  Print results 12
	//

	unsigned int correct_assign = (correct_assign00 ? 1 : 0) +
	                              (correct_assign01 ? 1 : 0) +
	                              (correct_assign11 ? 1 : 0) +
	                              (correct_assign10 ? 1 : 0) +
	                              (correct_assign02 ? 1 : 0) +
	                              (correct_assign22 ? 1 : 0) +
	                              (correct_assign20 ? 1 : 0);
	double correct_assign_fraction = correct_assign / 7.0;

	TestHelper::printSummaryHeaderPartial(24, 2, DID_NOT_CRASH_RUNNING_MARKS);
	TestHelper::printSummaryLine("HistoryNode struct", correct_node_bytes);
	TestHelper::printSummaryLine("History definition", correct_history_bytes);
	TestHelper::printSummaryLine("isAtLeast?Long",     totals.at_least_N,  totals.at_least_N_max);
	TestHelper::printSummaryLine("isInList",           totals.is_in_list,  totals.is_in_list_max);
	TestHelper::printSummaryLine("getFirst",           totals.first,       totals.first_max);
	TestHelper::printSummaryLine("getSecond",          totals.second,      totals.second_max);
	//TestHelper::printSummaryLine("destroyLinkedList",  correct_destroy,    4);
	TestHelper::printSummaryLine("destroyLinkedList",  true);
	TestHelper::printSummaryLine("copyLinkedList",     correct_copy,       10);
	TestHelper::printSummaryLine("Copy constructor",   correct_copy_con,   2);
	TestHelper::printSummaryLine("Destructor",         true);
	TestHelper::printSummaryLine("removeAll" ,         correct_remove_all, 3);
	TestHelper::printSummaryLine("operator= (basic)",  correct_assign,     7);

	unsigned int highest12 = calculateMark(true, true,     // type definitions
	                                       totals.at_least_N_highest, totals.is_in_list_highest,
	                                       totals.first_highest, totals.second_highest,
	                                       1.0, 1.0,       // copy list, copy constructor
	                                       1.0,            // removeAll
	                                       1.0, 0.0, 0.0,  // assignment operator
	                                       false);         // finished
	unsigned int mark12 = calculateMark(correct_node_bytes, correct_history_bytes,
	                                    totals.at_least_N_fraction, totals.is_in_list_fraction,
	                                    totals.first_fraction, totals.second_fraction,
	                                    correct_copy_fraction, correct_copy_con_fraction,
	                                    correct_remove_all_fraction,
	                                    correct_assign_fraction, 0.0, 0.0,
	                                    false);         // finished
	assert(mark12 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark12, highest12, TOTAL_MARKS);

	cout << endl;
	cout << endl;


	//
	//  Test History assignment operator (special)
	//

	cout << "Testing self-assignment:" << endl;
	cout << "------------------------" << endl;
	cout << endl;

	cout << "Creating Historys" << endl;
	p_assign0 = new History();
	p_assign1 = new History();
	p_assign1->addAtHead(board_black);
	p_assign2 = new History();
	p_assign2->addAtHead(board_black);
	p_assign2->addAtHead(board_white);
	cout << "* Completed without crashing" << endl;
	cout << endl;

	cout << "Assigning empty History to itself" << endl;
	*p_assign0 = *p_assign0;
	cout << "* Completed without crashing" << endl;
	bool correct_self0 = testLength(*p_assign0, 0, true);
	cout << endl;

	cout << "Assigning 1-node History to itself" << endl;
	*p_assign1 = *p_assign1;
	cout << "* Completed without crashing" << endl;
	bool correct_self1 = testLength(*p_assign1, 1, true);
	cout << endl;

	cout << "Assigning 2-node History to itself" << endl;
	*p_assign2 = *p_assign2;
	cout << "* Completed without crashing" << endl;
	bool correct_self2 = testLength(*p_assign2, 2, true);
	cout << endl;

	cout << "Destroying Historys" << endl;
	delete p_assign0;
	delete p_assign1;
	delete p_assign2;
	p_assign0 = nullptr;
	p_assign1 = nullptr;
	p_assign2 = nullptr;
	cout << "* Completed without crashing" << endl;
	cout << endl;

	cout << endl;
	cout << endl;


	cout << "Testing chained assignment:" << endl;
	cout << "---------------------------" << endl;
	cout << endl;

	cout << "Creating Historys" << endl;
	p_assign0 = new History();
	p_assign2 = new History();
	p_assign2->addAtHead(board_black);
	p_assign2->addAtHead(board_white);
	History* p_assignA = new History();
	History* p_assignB = new History();
	cout << "* Completed without crashing" << endl;
	cout << endl;

	cout << "Assigning empty History to empty to empty" << endl;
	*p_assignA = *p_assignB = *p_assign0;
	cout << "* Completed without crashing" << endl;
	bool correct_chain00A = testLength(*p_assignA, 0, false);
	bool correct_chain00B = testLength(*p_assignB, 0, false);
	if(correct_chain00A && correct_chain00B)
		cout << "* Correct" << endl;
	cout << endl;

	cout << "Assigning empty History to empty to 2-node" << endl;
	*p_assignA = *p_assignB = *p_assign2;
	cout << "* Completed without crashing" << endl;
	bool correct_chain02A = testLength(*p_assignA, 2, false);
	bool correct_chain02B = testLength(*p_assignB, 2, false);
	if(correct_chain02A && correct_chain02B)
		cout << "* Correct" << endl;
	cout << endl;

	cout << "Assigning 2-node History to 2-node to 2-node" << endl;
	*p_assignA = *p_assignB = *p_assign2;
	cout << "* Completed without crashing" << endl;
	bool correct_chain22A = testLength(*p_assignA, 2, false);
	bool correct_chain22B = testLength(*p_assignB, 2, false);
	if(correct_chain22A && correct_chain22B)
		cout << "* Correct" << endl;
	cout << endl;

	cout << "Assigning 2-node History to 2-node to empty" << endl;
	*p_assignA = *p_assignB = *p_assign0;
	cout << "* Completed without crashing" << endl;
	bool correct_chain20A = testLength(*p_assignA, 0, false);
	bool correct_chain20B = testLength(*p_assignB, 0, false);
	if(correct_chain20A && correct_chain20B)
		cout << "* Correct" << endl;
	cout << endl;

	cout << "Destroying Historys" << endl;
	delete p_assign0;
	delete p_assign2;
	delete p_assignA;
	delete p_assignB;
	p_assign0 = nullptr;
	p_assign2 = nullptr;
	p_assignA = nullptr;
	p_assignB = nullptr;
	cout << "* Completed without crashing" << endl;
	cout << endl;

	cout << endl;
	cout << endl;


	//
	//  Print results 13
	//

	unsigned int correct_self = (correct_self0 ? 1 : 0) +
	                            (correct_self1 ? 1 : 0) +
	                            (correct_self2 ? 1 : 0);
	double correct_self_fraction = correct_self / 3.0;

	unsigned int correct_chain = (correct_chain00A ? 1 : 0) +
	                             (correct_chain00B ? 1 : 0) +
	                             (correct_chain02A ? 1 : 0) +
	                             (correct_chain02B ? 1 : 0) +
	                             (correct_chain20A ? 1 : 0) +
	                             (correct_chain20B ? 1 : 0) +
	                             (correct_chain22A ? 1 : 0) +
	                             (correct_chain22B ? 1 : 0);
	double correct_chain_fraction = correct_chain / 8.0;

	TestHelper::printSummaryHeaderFinal(24, 2, DID_NOT_CRASH_RUNNING_MARKS);
	TestHelper::printSummaryLine("HistoryNode struct", correct_node_bytes);
	TestHelper::printSummaryLine("History definition", correct_history_bytes);
	TestHelper::printSummaryLine("isAtLeast?Long",     totals.at_least_N,  totals.at_least_N_max);
	TestHelper::printSummaryLine("isInList",           totals.is_in_list,  totals.is_in_list_max);
	TestHelper::printSummaryLine("getFirst",           totals.first,       totals.first_max);
	TestHelper::printSummaryLine("getSecond",          totals.second,      totals.second_max);
	//TestHelper::printSummaryLine("destroyLinkedList",  correct_destroy,    4);
	TestHelper::printSummaryLine("destroyLinkedList",  true);
	TestHelper::printSummaryLine("copyLinkedList",     correct_copy,       10);
	TestHelper::printSummaryLine("Copy constructor",   correct_copy_con,   2);
	TestHelper::printSummaryLine("Destructor",         true);
	TestHelper::printSummaryLine("removeAll" ,         correct_remove_all, 3);
	TestHelper::printSummaryLine("operator= (basic)",  correct_assign,     7);
	TestHelper::printSummaryLine("operator= (self)",   correct_self,       3);
	TestHelper::printSummaryLine("operator= (chain)",  correct_chain,      8);

	unsigned int mark13 = calculateMark(correct_node_bytes, correct_history_bytes,
	                                    totals.at_least_N_fraction, totals.is_in_list_fraction,
	                                    totals.first_fraction, totals.second_fraction,
	                                    correct_copy_fraction, correct_copy_con_fraction,
	                                    correct_remove_all_fraction,
	                                    correct_assign_fraction, correct_self_fraction, correct_chain_fraction,
	                                    true);         // finished
	assert(mark13 <= TOTAL_MARKS);
	TestHelper::printMark(mark13, TOTAL_MARKS);


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

bool testDestroyLinkedList (HistoryNode* p_head,
                            int total_length)
{
	destroyLinkedList(p_head);
	cout << "* Called function without crashing" << endl;
	return true;
/*
	int destroy_count = total_length - CountedHistoryNode::s_allocated;
	if(destroy_count == total_length)
	{
		cout << "* Correct" << endl;
		return true;
	}
	else
	{
		cout << "* Incorrect: Deallocated " << destroy_count << " / " << total_length << " nodes" << endl;
		return false;
	}
*/
}

int testCopyLinkedList (const HistoryNode* p_old_head,
                        int correct_length)
{
	HistoryNode* p_new_head = copyLinkedList(p_old_head);
	cout << "* Called function without crashing" << endl;

	const HistoryNode* p_old_current = p_old_head;
	HistoryNode*       p_new_current = p_new_head;

	if(p_new_current == nullptr && p_old_current != nullptr)
	{
		cout << "* Incorrect: Returned nullptr for non-empty linked list" << endl;
		return 0;
	}

	for(int i = 0; i < correct_length; i++) // loop also returns out below
	{
		if(p_old_current == nullptr)
		{
			cout << "* Incorrect: Old linked list is damaged on node " << i << " / " << correct_length << endl;
			return i;
		}
		if(p_new_current == nullptr)
		{
			cout << "* Incorrect: Node " << i << " / " << correct_length << " was not copied" << endl;
			return i;
		}
		if(p_new_current == p_old_current)
		{
			cout << "* Incorrect: New node " << i << " / " << correct_length << " points to old linked list" << endl;
			return i;
		}
		if(p_new_current->p_next == p_new_current)
		{
			cout << "* Incorrect: New node " << i << " / " << correct_length << " links to itself" << endl;
			return i;
		}

		// check for link back into lists
		const HistoryNode* p_old_third = p_old_head;
		      HistoryNode* p_new_third = p_new_head;
		for(int j = 0; j < i; j++)
		{
			if(p_new_current == p_new_third)
			{
				cout << "* Incorrect: New node " << i << " / " << correct_length
				     << " links back to node " << j << " of new list" << endl;
				return i;
			}
			if(p_new_current == p_old_third)
			{
				cout << "* Incorrect: New node " << i << " / " << correct_length
				     << " links back to node " << j << " of old list" << endl;
				return i;
			}
			p_new_third = p_new_third->p_next;
			p_old_third = p_old_third->p_next;
		}

		// advance pointers for next loop iteration
		p_old_current = p_old_current->p_next;
		p_new_current = p_new_current->p_next;
	}

	if(p_old_current != nullptr)
	{
		cout << "* Incorrect: Old linked list is damaged on last node (" << correct_length << ")" << endl;
		return correct_length;
	}
	if(p_new_current == nullptr)
	{
		cout << "* Correct" << endl;
		return correct_length + 1;  // +1 for correct nullptr
	}
	// list should end here

	// count extra nodes
	p_new_current = p_new_current->p_next;
	for(int i = 1; i < correct_length; i++) // loop also returns out below
	{
		if(p_new_current == nullptr)
		{
			cout << "* Incorrect: " << i << " extra nodes in new linked list" << endl;
			return correct_length - i;
		}
		p_new_current = p_new_current->p_next;
	}

	cout << "* Incorrect: Far too many nodes in new linked list" << endl;
	return 0;
}

bool testLength (const History& history,
                 int correct_length,
                 bool is_print_correct)
{
	assert(correct_length >= 0);
	assert(correct_length <= 3);

	bool is_1 = history.isAtLeast1Long();
	bool is_2 = history.isAtLeast2Long();
	bool is_3 = history.isAtLeast3Long();

	int length = is_3 ? 3 : (is_2 ? 2 : (is_1 ? 1 : 0));

	if(length == correct_length)
	{
		if(is_print_correct)
			cout << "* Correct" << endl;
		return true;
	}
	else
	{
		cout << "* Incorrect: History contains " << length << " nodes, should contain " << correct_length << endl;
		return false;
	}
}



int calculateMark (bool correct_node_bytes,
                   bool correct_history_bytes,
                   double correct_at_least_N_fraction,
                   double correct_is_in_list_fraction,
                   double correct_first_fraction,
                   double correct_second_fraction,
                   double correct_copy_fraction,
                   double correct_copy_con_fraction,
                   double correct_remove_all_fraction,
                   double correct_assign_fraction,
                   double correct_self_fraction,
                   double correct_chain_fraction,
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

	// Part C marks
	mark += (int)(5.0 * correct_copy_fraction);
	mark += (int)(2.0 * correct_copy_con_fraction);
	mark += (int)(2.0 * correct_remove_all_fraction);
	mark += (int)(4.0 * correct_assign_fraction);
	mark += (int)(2.0 * correct_self_fraction);
	mark += (int)(2.0 * correct_chain_fraction);

	assert(mark <= TOTAL_MARKS);
	return mark;
}
