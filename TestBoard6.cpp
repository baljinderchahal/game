//
//  TestBoard6.cpp
//
//  A test program for the Board module.  It is the TestBoard5B
//    program expanded to also test boards of different sizes.
//
//  This program is to be used with Assignment 6: Part A for
//    CS115, 202310.
//
//  Do not modify this file.
//

#include "Board.h"
#include "Board.h"  // repeated to test for #include errors

#include <cassert>
#include <cctype>
#include <string>
#include <iostream>

#include "TestHelper.h"
#include "BoardValue.h"

using namespace std;



int testBoardCountWithValue (const Board& board,
                             int correct_size,
                             int correct_black,
                             int correct_white,
                             bool is_print_correct);
bool testBoardSetAt (Board& board,
                     int row, int column,
                     char value, const string& value_name,
                     bool is_print_correct);

struct PrintResult
{
	unsigned int basic_print;
	unsigned int star_points;
	unsigned int star_points_max;
	unsigned int row_headers;
	unsigned int column_headers;
};
PrintResult createZeroPrintResult ();
bool isPrintResultAllCorrect (const PrintResult& result);
PrintResult testBoardPrint (const Board& board,
                            int board_size);
string trimRight (const string& str);
void checkPrintedCropped (const Board& board,
                          unsigned int board_size,
                          const vector<string>& captured_lines,
                          unsigned int row_offset,
                          unsigned int column_offset,
                          PrintResult& result);
void checkPrintedCropped (const Board& board,
                          unsigned int board_size,
                          const vector<string>& cropped,
                          PrintResult& result);
bool isCorrectStarPoint (unsigned int board_size,
                         int row, int column);
void checkPrintedRowHeadings (unsigned int board_size,
                              const vector<string>& captured_lines,
                              unsigned int row_offset,
                              PrintResult& result);
bool isAllDigits (const string& str);
void checkPrintedColumnHeadings (unsigned int board_size,
                                 const vector<string>& captured_lines,
                                 unsigned int column_offset,
                                 PrintResult& result);

int testIsANeighbourWithValue (const Board& board,
                               int row,
                               int column,
                               bool correct_black,
                               bool correct_white,
                               bool correct_empty);
bool testFillConnected (const Board& board,
                        unsigned int board_size,
                        int correct_marked,
                        char value_before,
                        int count_before);
unsigned int testCalculateScore (const Board& board,
                                 unsigned int board_size,
                                 char us_value,
                                 char them_value,
                                 int correct_score);
unsigned int testPlayStone (const Board& board,
                            const StonesRemoved& removed,
                            char us_value,
                            char them_value,
                            int correct_score_us,
                            int correct_score_them,
                            int correct_captured_us,
                            int correct_captured_them,
                            const string& name_us,
                            const string& name_them);

bool testGetSize (const Board& board,
                  int correct_size,
                  bool is_print_correct,
                  bool is_print_incorrect);
bool testIsOnBoardGrid (const Board& board,
                        int correct_size,
                        bool is_print_correct,
                        bool is_print_incorrect);
bool testIsOnBoard (const Board& board,
                    int row,
                    int column,
                    bool correct_validity,
                    int correct_size,
                    bool is_print_incorrect);
bool testEqualityTest (const Board& board1,
                       const Board& board2,
                       bool is_equal_correct);

int calculateMark (unsigned int correct_board_size_count,
                   bool correct_board_bytes,
                   bool correct_stones_removed_bytes,
                   unsigned int correct_board_value_constants,
                   double correct_default_constructor_fraction,
                   unsigned int correct_count_with_value,
                   unsigned int correct_set_at,
                   unsigned int correct_count_after_set,
                   unsigned int correct_load,
                   double correct_basic_print_fraction,
                   double correct_star_point_fraction,
                   double correct_row_header_fraction,
                   double correct_column_header_fraction,
                   double correct_replace_all_fraction,
                   double correct_is_neighbour_fraction,
                   double correct_fill_connected_fraction,
                   double correct_calculate_score_fraction,
                   double correct_play_stone_fraction,
                   double correct_copy_to_fraction,
                   double correct_copy_from_fraction,
                   double correct_copy_delete_fraction,
                   double correct_assign_to_fraction,
                   double correct_assign_from_fraction,
                   double correct_assign_delete_fraction,
                   double correct_self_assign_fraction,
                   double correct_chain_assign_fraction,
                   double correct_get_size_fraction,
                   double correct_is_on_board_fraction,
                   double correct_print_by_size_fraction,
                   double correct_load_sizes_fraction,
                   double correct_replace_24_fraction,
                   double correct_score_24_fraction,
                   double correct_play_stone_24_fraction,
                   double correct_equality_self_fraction,
                   double correct_equality_size_fraction,
                   bool correct_equality_different,
                   bool correct_equality_same,
                   bool is_finished);

const int EXISTING_WORK_MARKS         = 1;
const int COMPILE_AND_START_MARKS     = 1 - EXISTING_WORK_MARKS;
const int DID_NOT_CRASH_RUNNING_MARKS = 1;
const int COMPILE_AND_NOT_CRASH_MARKS = COMPILE_AND_START_MARKS + DID_NOT_CRASH_RUNNING_MARKS;
const int COMBINED_TEST_MARKS         = 3 + EXISTING_WORK_MARKS;
const int TOTAL_MARKS                 = COMPILE_AND_NOT_CRASH_MARKS + COMBINED_TEST_MARKS;

const int BOARD_SIZE_MIN_CORRECT     =  1;
const int BOARD_SIZE_MAX_CORRECT     = 24;
const int BOARD_SIZE_DEFAULT_CORRECT = 19;
const int BOARD_CELL_COUNT_DEFAULT   = BOARD_SIZE_DEFAULT_CORRECT * BOARD_SIZE_DEFAULT_CORRECT;
const int BOARD_SIZE_RANGE = BOARD_SIZE_MAX_CORRECT - BOARD_SIZE_MIN_CORRECT + 1;
const int BOARD_SIZE_COUNT = 3;  // how many constants

const char BOARD_VALUE_EMPTY_CORRECT  = '.';
const char BOARD_VALUE_BLACK_CORRECT  = 'O';
const char BOARD_VALUE_WHITE_CORRECT  = '@';
const char BOARD_VALUE_MARKED_CORRECT = '#';
const unsigned int BOARD_VALUE_COUNT  = 3;
const unsigned int BOARD_VALUE_COUNT_WITH_MARKED = BOARD_VALUE_COUNT + 1;
const char BOARD_VALUE_STAR_POINT_CORRECT = '*';

const unsigned int BASIC_PRINT_CORRECT_MAX    = 10;
const unsigned int ROW_HEADING_CORRECT_MAX    =  8;
const unsigned int COLUMN_HEADING_CORRECT_MAX =  8;

const unsigned int LOAD_COUNT = 3;
const unsigned int LOAD_BOARD_VALUE_COUNT          = LOAD_COUNT * BOARD_VALUE_COUNT;
const unsigned int LOAD_BASIC_PRINT_CORRECT_MAX    = LOAD_COUNT * BASIC_PRINT_CORRECT_MAX;
const unsigned int LOAD_ROW_HEADING_CORRECT_MAX    = LOAD_COUNT * ROW_HEADING_CORRECT_MAX;
const unsigned int LOAD_COLUMN_HEADING_CORRECT_MAX = LOAD_COUNT * COLUMN_HEADING_CORRECT_MAX;

const unsigned int BOARD_EQUALITY_COUNT   = 4;
const unsigned int BOARD_INEQUALITY_COUNT = (BOARD_EQUALITY_COUNT * (BOARD_EQUALITY_COUNT - 1)) / 2;



int main ()
{
	TestHelper::startup("TestBoard6", COMPILE_AND_START_MARKS, TOTAL_MARKS);

	//
	//  Test constants and Board struct
	//

	cout << "Testing constants and definitions:" << endl;
	cout << "----------------------------------" << endl;
	cout << endl;

	cout << "Testing BOARD_SIZE_MIN constant" << endl;
	bool correct_board_size_min = TestHelper::testConstant(BOARD_SIZE_MIN, BOARD_SIZE_MIN_CORRECT, "BOARD_SIZE_MIN");
	cout << endl;

	cout << "Testing BOARD_SIZE_MAX constant" << endl;
	bool correct_board_size_max = TestHelper::testConstant(BOARD_SIZE_MAX, BOARD_SIZE_MAX_CORRECT, "BOARD_SIZE_MAX");
	cout << endl;

	cout << "Testing BOARD_SIZE_DEFAULT constant" << endl;
	bool correct_board_size_default = TestHelper::testConstant(BOARD_SIZE_DEFAULT, BOARD_SIZE_DEFAULT_CORRECT, "BOARD_SIZE_DEFAULT");
	cout << endl;

	cout << "Checking Board size in bytes" << endl;
	size_t board_bytes         = sizeof(Board);
	size_t board_bytes_pointer = sizeof(char*);
	size_t board_bytes_int     = sizeof(int);
	size_t board_bytes_array   = sizeof(char) * BOARD_SIZE_DEFAULT_CORRECT * BOARD_SIZE_DEFAULT_CORRECT;
	size_t board_bytes_correct = board_bytes_pointer + board_bytes_int * 2;
	bool correct_board_bytes = false;
	if(board_bytes == board_bytes_pointer)
		cout << "* Incorrect: Board should also contain size and place count" << endl;
	else if(board_bytes >= board_bytes_array * 2/3 &&
	        board_bytes <= board_bytes_array * 4/3)
	{
		cout << "* Incorrect: Board array should be dynamically allocated" << endl;
	}
	else if(board_bytes < board_bytes_correct)
		cout << "* Incorrect: Board should contain more data" << endl;
	else if(board_bytes > board_bytes_correct * 2)  // extra space because padding happens
		cout << "* Incorrect: Board should contain less data" << endl;
	else
	{
		cout << "* Correct" << endl;
		correct_board_bytes = true;
	}
	cout << endl;
/*
	cout << "Creating Board" << endl;
	{
		Board board0;  // will be destroyed at end of scope
		cout << "* Completed without crashing" << endl;
		cout << endl;

		(void)(board0);  // no-op to silence compiler warnings about unused local variable

		cout << "Destroying Board" << endl;
	}  // board0 is destroyed here
	cout << "* Completed without crashing" << endl;
	cout << endl;
*/
	cout << "Checking StonesRemoved size in bytes" << endl;
	size_t removed_bytes         = sizeof(StonesRemoved);
	size_t removed_bytes_single  = sizeof(int);
	size_t removed_bytes_correct = sizeof(int) * 2;
	bool correct_removed_bytes = false;
	if(removed_bytes == removed_bytes_single)
		cout << "* Incorrect: StonesRemoved should contain two values, not one" << endl;
	else if(removed_bytes < removed_bytes_correct)
		cout << "* Incorrect: Board should contain more data" << endl;
	else if(removed_bytes > removed_bytes_correct * 2)  // extra space because padding happens
		cout << "* Incorrect: Board should contain less data" << endl;
	else
	{
		cout << "* Correct" << endl;
		correct_removed_bytes = true;
	}
	cout << endl;

	cout << "Creating StonesRemoved" << endl;
	{
		StonesRemoved removed0;  // will be destroyed at end of scope
		cout << "* Completed without crashing" << endl;
		cout << endl;

		(void)(removed0);  // no-op to silence compiler warnings about unused local variable

		cout << "Destroying StonesRemoved" << endl;
	}  // removed0 is destroyed here
	cout << "* Completed without crashing" << endl;
	cout << endl;

	cout << "Testing BOARD_VALUE_EMPTY constant (from Assignment 2)" << endl;
	bool correct_board_value_empty = TestHelper::testConstant(BOARD_VALUE_EMPTY, BOARD_VALUE_EMPTY_CORRECT, "BOARD_VALUE_EMPTY");
	cout << endl;

	cout << "Testing BOARD_VALUE_BLACK constant (from Assignment 2)" << endl;
	bool correct_board_value_black = TestHelper::testConstant(BOARD_VALUE_BLACK, BOARD_VALUE_BLACK_CORRECT, "BOARD_VALUE_BLACK");
	cout << endl;

	cout << "Testing BOARD_VALUE_WHITE constant (from Assignment 2)" << endl;
	bool correct_board_value_white = TestHelper::testConstant(BOARD_VALUE_WHITE, BOARD_VALUE_WHITE_CORRECT, "BOARD_VALUE_WHITE");
	cout << endl;

	cout << "Testing BOARD_VALUE_MARKED constant (from Part C)" << endl;
	bool correct_board_value_marked = TestHelper::testConstant(BOARD_VALUE_MARKED, BOARD_VALUE_MARKED_CORRECT, "BOARD_VALUE_MARKED");
	cout << endl;

	cout << endl;
	cout << endl;


	//
	//  Print results 1
	//

	unsigned int correct_board_size_count = (correct_board_size_min     ? 1 : 0) +
	                                        (correct_board_size_max     ? 1 : 0) +
	                                        (correct_board_size_default ? 1 : 0);

	unsigned int correct_board_value = (correct_board_value_empty  ? 1 : 0) +
	                                   (correct_board_value_black  ? 1 : 0) +
	                                   (correct_board_value_white  ? 1 : 0) +
	                                   (correct_board_value_marked ? 1 : 0);

	TestHelper::printSummaryHeaderPartial(32, 2, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("BOARD_SIZE_MIN",                 correct_board_size_min);
	TestHelper::printSummaryLine("BOARD_SIZE_MAX",                 correct_board_size_max);
	TestHelper::printSummaryLine("BOARD_SIZE_DEFAULT",             correct_board_size_default);
	TestHelper::printSummaryLine("Board declaration",              correct_board_bytes);
	TestHelper::printSummaryLine("StonesRemoved declaration",      correct_removed_bytes);
	TestHelper::printSummaryLine("BOARD_VALUE_ constants",         correct_board_value,      BOARD_VALUE_COUNT_WITH_MARKED);

	unsigned int highest1 = calculateMark(BOARD_SIZE_COUNT, true, true, BOARD_VALUE_COUNT_WITH_MARKED, // constants and types
	                                      0.0, 0, 0, 0,  // basic board manipulation
	                                      0,             // load
	                                      0.0,           // basic print
	                                      0.0,           // print star points
	                                      0.0, 0.0,      // print indexes
	                                      0.0, 0.0, 0.0, // A3 Part D helper functions
	                                      0.0, 0.0,      // A3 Part D non-helper functions
	                                      0.0, 0.0, 0.0, // copy constructor
	                                      0.0, 0.0, 0.0, // assignment operator (basic)
	                                      0.0, 0.0,      // assignment operator (fancy)
	                                      0.0, 0.0,      // size-related functions
	                                      0.0, 0.0,      // print and load other sizes
	                                      0.0, 0.0, 0.0, // A3 Part D other sizes
	                                      0.0, 0.0,      // equality tests (size)
	                                      false, false,  // equality tests (places)
	                                      false);        // finished
	unsigned int mark1 = calculateMark(correct_board_size_count, correct_board_bytes, correct_removed_bytes, correct_board_value,
	                                   0.0, 0, 0, 0,  // basic board manipulation
	                                   0,             // load
	                                   0.0,           // basic print
	                                   0.0,           // print star points
	                                   0.0, 0.0,      // print indexes
	                                   0.0, 0.0, 0.0, // A3 Part D helper functions
	                                   0.0, 0.0,      // A3 Part D non-helper functions
	                                   0.0, 0.0, 0.0, // copy constructor
	                                   0.0, 0.0, 0.0, // assignment operator (basic)
	                                   0.0, 0.0,      // assignment operator (fancy)
	                                   0.0, 0.0,      // size-related functions
	                                   0.0, 0.0,      // print and load other sizes
	                                   0.0, 0.0, 0.0, // A3 Part D other sizes
	                                   0.0, 0.0,      // equality tests (size)
	                                   false, false,  // equality tests (places)
	                                   false);        // finished
	assert(mark1 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark1, highest1, TOTAL_MARKS);

	cout << endl;
	cout << endl;


	//
	//  Test Board functions
	//

	cout << "Testing basic Board functions:" << endl;
	cout << "------------------------------" << endl;
	cout << endl;

	cout << "Creating board with default constructor" << endl;
	Board board2;
	cout << "* Completed without crashing" << endl;
	cout << endl;

	cout << "Testing board contents (Board::getAt)" << endl;
	unsigned int correct_default_constructor = 0;
	bool is_print_wrong_default_constructor = true;
	for(int r = 0; r < BOARD_SIZE_DEFAULT_CORRECT; r++)
		for(int c = 0; c < BOARD_SIZE_DEFAULT_CORRECT; c++)
		{
			char returned1 = board2.getAt(r, c);
			if(returned1 == BOARD_VALUE_EMPTY_CORRECT)
				correct_default_constructor++;
			else if(is_print_wrong_default_constructor)
			{
				cout << "* Incorrect: row " << r << ", column " << c
				     << " is '" << returned1 << "', should be BOARD_VALUE_EMPTY ('"
				     << BOARD_VALUE_EMPTY_CORRECT << "')" << endl;
				is_print_wrong_default_constructor = false;
			}
		}
	if(correct_default_constructor == BOARD_CELL_COUNT_DEFAULT)
		cout << "* Correct" << endl;
	else
	{
		int wrong_count = BOARD_CELL_COUNT_DEFAULT - correct_default_constructor;
		if(wrong_count > 1)
			cout << "* " << (wrong_count - 1) << " more cells incorrect" << endl;
	}
	cout << endl;

	cout << "Counting cells by type (Board::countWithValue)" << endl;
	unsigned int correct_count_with_value = testBoardCountWithValue(board2, BOARD_SIZE_DEFAULT_CORRECT, 0, 0, true);
	cout << endl;

	bool correct_set_at_1 = testBoardSetAt(board2, 4, 2, BOARD_VALUE_BLACK_CORRECT, "BOARD_VALUE_BLACK", true);
	cout << endl;

	cout << "Setting 9 more cells (Board::setAt)" << endl;
	unsigned int correct_set_at_9 = 0;
	if(testBoardSetAt(board2,  4,  3, BOARD_VALUE_BLACK_CORRECT, "BOARD_VALUE_BLACK", false))
		correct_set_at_9++;
	if(testBoardSetAt(board2, 12, 12, BOARD_VALUE_BLACK_CORRECT, "BOARD_VALUE_BLACK", false))
		correct_set_at_9++;
	if(testBoardSetAt(board2,  5,  0, BOARD_VALUE_BLACK_CORRECT, "BOARD_VALUE_BLACK", false))
		correct_set_at_9++;
	if(testBoardSetAt(board2,  5,  3, BOARD_VALUE_BLACK_CORRECT, "BOARD_VALUE_BLACK", false))
		correct_set_at_9++;
	if(testBoardSetAt(board2,  5,  3, BOARD_VALUE_WHITE_CORRECT, "BOARD_VALUE_WHITE", false))
		correct_set_at_9++;
	if(testBoardSetAt(board2,  6, 18, BOARD_VALUE_WHITE_CORRECT, "BOARD_VALUE_WHITE", false))
		correct_set_at_9++;
	if(testBoardSetAt(board2, 18,  1, BOARD_VALUE_WHITE_CORRECT, "BOARD_VALUE_WHITE", false))
		correct_set_at_9++;
	if(testBoardSetAt(board2,  0, 15, BOARD_VALUE_WHITE_CORRECT, "BOARD_VALUE_WHITE", false))
		correct_set_at_9++;
	if(testBoardSetAt(board2,  5,  3, BOARD_VALUE_EMPTY_CORRECT, "BOARD_VALUE_EMPTY", false))
		correct_set_at_9++;
	if(correct_set_at_9 == 9)
		cout << "* Correct" << endl;
	cout << endl;

	cout << "Counting cells by type (Board::countWithValue)" << endl;
	unsigned int correct_count_after_set = testBoardCountWithValue(board2, BOARD_SIZE_DEFAULT_CORRECT, 4, 3, true);
	cout << endl;

	cout << endl;
	cout << endl;


	//
	//  Print results 2
	//

	double correct_default_constructor_fraction = (double)(correct_default_constructor) / BOARD_CELL_COUNT_DEFAULT;
	unsigned int correct_set_at = correct_set_at_9 + (correct_set_at_1 ? 1 : 0);

	TestHelper::printSummaryHeaderPartial(32, 2, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("BOARD_SIZE_MIN",                 correct_board_size_min);
	TestHelper::printSummaryLine("BOARD_SIZE_MAX",                 correct_board_size_max);
	TestHelper::printSummaryLine("BOARD_SIZE_DEFAULT",             correct_board_size_default);
	TestHelper::printSummaryLine("Board declaration",              correct_board_bytes);
	TestHelper::printSummaryLine("StonesRemoved declaration",      correct_removed_bytes);
	TestHelper::printSummaryLine("BOARD_VALUE_ constants",         correct_board_value,      BOARD_VALUE_COUNT_WITH_MARKED);
	cout << endl;
	TestHelper::printSummaryLine("Default constructor",            correct_default_constructor_fraction);
	TestHelper::printSummaryLine("Board::countWithValue",          correct_count_with_value, BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::setAt",                   correct_set_at,           10);
	TestHelper::printSummaryLine("Board::countWithValue",          correct_count_after_set,  BOARD_VALUE_COUNT);

	unsigned int highest2 = calculateMark(BOARD_SIZE_COUNT, true, true, BOARD_VALUE_COUNT_WITH_MARKED, // constants and types
	                                      1.0, BOARD_VALUE_COUNT, 10, BOARD_VALUE_COUNT,  // basic board manipulation
	                                      0,             // load
	                                      0.0,           // basic print
	                                      0.0,           // print star points
	                                      0.0, 0.0,      // print indexes
	                                      0.0, 0.0, 0.0, // A3 Part D helper functions
	                                      0.0, 0.0,      // A3 Part D non-helper functions
	                                      0.0, 0.0, 0.0, // copy constructor
	                                      0.0, 0.0, 0.0, // assignment operator (basic)
	                                      0.0, 0.0,      // assignment operator (fancy)
	                                      0.0, 0.0,      // size-related functions
	                                      0.0, 0.0,      // print and load other sizes
	                                      0.0, 0.0, 0.0, // A3 Part D other sizes
	                                      0.0, 0.0,      // equality tests (size)
	                                      false, false,  // equality tests (places)
	                                      false);        // finished
	unsigned int mark2 = calculateMark(correct_board_size_count, correct_board_bytes, correct_removed_bytes, correct_board_value,
	                                   correct_default_constructor_fraction, correct_count_with_value,
	                                   correct_set_at, correct_count_after_set,
	                                   0,             // load
	                                   0.0,           // basic print
	                                   0.0,           // print star points
	                                   0.0, 0.0,      // print indexes
	                                   0.0, 0.0, 0.0, // A3 Part D helper functions
	                                   0.0, 0.0,      // A3 Part D non-helper functions
	                                   0.0, 0.0, 0.0, // copy constructor
	                                   0.0, 0.0, 0.0, // assignment operator (basic)
	                                   0.0, 0.0,      // assignment operator (fancy)
	                                   0.0, 0.0,      // size-related functions
	                                   0.0, 0.0,      // print and load other sizes
	                                   0.0, 0.0, 0.0, // A3 Part D other sizes
	                                   0.0, 0.0,      // equality tests (size)
	                                   false, false,  // equality tests (places)
	                                   false);        // finished
	assert(mark2 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark2, highest2, TOTAL_MARKS);

	cout << endl;
	cout << endl;


	//
	//  Test loading Board
	//

	cout << "Testing Board::load:" << endl;
	cout << "--------------------" << endl;
	cout << endl;

	cout << "Loading board \"empty.txt\"" << endl;
	Board board_empty;
	cout << "* Created without crashing" << endl;
	board_empty.load("empty.txt");
	cout << "* Loaded without crashing" << endl;
	unsigned int correct_load_empty = testBoardCountWithValue(board_empty, BOARD_SIZE_DEFAULT_CORRECT, 0, 0, true);
	cout << endl;

	cout << "Loading board \"ear.txt\"" << endl;
	Board board_ear;
	cout << "* Created without crashing" << endl;
	board_ear.load("ear.txt");
	cout << "* Loaded without crashing" << endl;
	unsigned int correct_load_ear = testBoardCountWithValue(board_ear, BOARD_SIZE_DEFAULT_CORRECT, 59, 58, true);
	cout << endl;

	cout << "Loading \"shapes.txt\"" << endl;
	Board board_shapes;
	cout << "* Created without crashing" << endl;
	board_shapes.load("shapes.txt");
	cout << "* Loaded without crashing" << endl;
	unsigned int correct_load_shapes = testBoardCountWithValue(board_shapes, BOARD_SIZE_DEFAULT_CORRECT, 96, 96, true);
	cout << endl;

	cout << endl;
	cout << endl;


	//
	//  Print results 3
	//

	unsigned int correct_load = correct_load_empty +
	                            correct_load_ear   +
	                            correct_load_shapes;

	TestHelper::printSummaryHeaderPartial(32, 2, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("BOARD_SIZE_MIN",                 correct_board_size_min);
	TestHelper::printSummaryLine("BOARD_SIZE_MAX",                 correct_board_size_max);
	TestHelper::printSummaryLine("BOARD_SIZE_DEFAULT",             correct_board_size_default);
	TestHelper::printSummaryLine("Board declaration",              correct_board_bytes);
	TestHelper::printSummaryLine("StonesRemoved declaration",      correct_removed_bytes);
	TestHelper::printSummaryLine("BOARD_VALUE_ constants",         correct_board_value,      BOARD_VALUE_COUNT_WITH_MARKED);
	cout << endl;
	TestHelper::printSummaryLine("Default constructor",            correct_default_constructor_fraction);
	TestHelper::printSummaryLine("Board::countWithValue",          correct_count_with_value, BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::setAt",                   correct_set_at,           10);
	TestHelper::printSummaryLine("Board::countWithValue",          correct_count_after_set,  BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::load",                    correct_load,             LOAD_BOARD_VALUE_COUNT);

	unsigned int highest3 = calculateMark(BOARD_SIZE_COUNT, true, true, BOARD_VALUE_COUNT_WITH_MARKED, // constants and types
	                                      1.0, BOARD_VALUE_COUNT, 10, BOARD_VALUE_COUNT,  // basic board manipulation
	                                      LOAD_BOARD_VALUE_COUNT,  // load
	                                      0.0,           // basic print
	                                      0.0,           // print star points
	                                      0.0, 0.0,      // print indexes
	                                      0.0, 0.0, 0.0, // A3 Part D helper functions
	                                      0.0, 0.0,      // A3 Part D non-helper functions
	                                      0.0, 0.0, 0.0, // copy constructor
	                                      0.0, 0.0, 0.0, // assignment operator (basic)
	                                      0.0, 0.0,      // assignment operator (fancy)
	                                      0.0, 0.0,      // size-related functions
	                                      0.0, 0.0,      // print and load other sizes
	                                      0.0, 0.0, 0.0, // A3 Part D other sizes
	                                      0.0, 0.0,      // equality tests (size)
	                                      false, false,  // equality tests (places)
	                                      false);        // finished
	unsigned int mark3 = calculateMark(correct_board_size_count, correct_board_bytes, correct_removed_bytes, correct_board_value,
	                                   correct_default_constructor_fraction, correct_count_with_value,
	                                   correct_set_at, correct_count_after_set,
	                                   correct_load,
	                                   0.0,           // basic print
	                                   0.0,           // print star points
	                                   0.0, 0.0,      // print indexes
	                                   0.0, 0.0, 0.0, // A3 Part D helper functions
	                                   0.0, 0.0,      // A3 Part D non-helper functions
	                                   0.0, 0.0, 0.0, // copy constructor
	                                   0.0, 0.0, 0.0, // assignment operator (basic)
	                                   0.0, 0.0,      // assignment operator (fancy)
	                                   0.0, 0.0,      // size-related functions
	                                   0.0, 0.0,      // print and load other sizes
	                                   0.0, 0.0, 0.0, // A3 Part D other sizes
	                                   0.0, 0.0,      // equality tests (size)
	                                   false, false,  // equality tests (places)
	                                   false);        // finished
	assert(mark3 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark3, highest3, TOTAL_MARKS);

	cout << endl;
	cout << endl;


	//
	//  Test printing Board
	//

	cout << "Testing Board::print:" << endl;
	cout << "---------------------" << endl;
	cout << endl;

	cout << "Printing board \"empty.txt\"" << endl;
	PrintResult print_result_empty = createZeroPrintResult();
	if(correct_load_empty == BOARD_VALUE_COUNT)
		print_result_empty = testBoardPrint(board_empty, BOARD_SIZE_DEFAULT_CORRECT);
	else
		cout << "* Skipping: Not loaded correctly" << endl;
	cout << endl;

	cout << "Printing board \"ear.txt\"" << endl;
	PrintResult print_result_ear = createZeroPrintResult();
	if(correct_load_ear == BOARD_VALUE_COUNT)
		print_result_ear = testBoardPrint(board_ear, BOARD_SIZE_DEFAULT_CORRECT);
	else
		cout << "* Skipping: Not loaded correctly" << endl;
	cout << endl;

	cout << "Printing board \"shapes.txt\"" << endl;
	PrintResult print_result_shapes = createZeroPrintResult();
	if(correct_load_shapes == BOARD_VALUE_COUNT)
		print_result_shapes = testBoardPrint(board_shapes, BOARD_SIZE_DEFAULT_CORRECT);
	else
		cout << "* Skipping: Not loaded correctly" << endl;
	cout << endl;

	cout << endl;
	cout << endl;


	//
	//  Print results 4
	//

	unsigned int correct_basic_print = print_result_empty .basic_print +
	                                   print_result_ear   .basic_print +
	                                   print_result_shapes.basic_print;
	double correct_basic_print_fraction = (double)(correct_basic_print) / LOAD_BASIC_PRINT_CORRECT_MAX;

	unsigned int correct_star_points = print_result_empty .star_points +
	                                   print_result_ear   .star_points +
	                                   print_result_shapes.star_points;
	unsigned int correct_star_points_max = print_result_empty .star_points_max +
	                                       print_result_ear   .star_points_max +
	                                       print_result_shapes.star_points_max;

	double correct_star_points_fraction = 0.0;
	if(correct_star_points_max > 0)
		correct_star_points_fraction = (double)(correct_star_points) / correct_star_points_max;

	unsigned int correct_row_headers = print_result_empty .row_headers +
	                                   print_result_ear   .row_headers +
	                                   print_result_shapes.row_headers;
	double correct_row_headers_fraction = (double)(correct_row_headers) / LOAD_ROW_HEADING_CORRECT_MAX;
	unsigned int correct_column_headers = print_result_empty .column_headers +
	                                      print_result_ear   .column_headers +
	                                      print_result_shapes.column_headers;
	double correct_column_headers_fraction = (double)(correct_column_headers) / LOAD_COLUMN_HEADING_CORRECT_MAX;

	TestHelper::printSummaryHeaderPartial(32, 2, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("BOARD_SIZE_MIN",                 correct_board_size_min);
	TestHelper::printSummaryLine("BOARD_SIZE_MAX",                 correct_board_size_max);
	TestHelper::printSummaryLine("BOARD_SIZE_DEFAULT",             correct_board_size_default);
	TestHelper::printSummaryLine("Board declaration",              correct_board_bytes);
	TestHelper::printSummaryLine("StonesRemoved declaration",      correct_removed_bytes);
	TestHelper::printSummaryLine("BOARD_VALUE_ constants",         correct_board_value,      BOARD_VALUE_COUNT_WITH_MARKED);
	cout << endl;
	TestHelper::printSummaryLine("Default constructor",            correct_default_constructor_fraction);
	TestHelper::printSummaryLine("Board::countWithValue",          correct_count_with_value, BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::setAt",                   correct_set_at,           10);
	TestHelper::printSummaryLine("Board::countWithValue",          correct_count_after_set,  BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::load",                    correct_load,             LOAD_BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::print (basic)",           correct_basic_print_fraction);
	TestHelper::printSummaryLine("Board::print (star points)",     correct_star_points_fraction);
	TestHelper::printSummaryLine("Board::print (row headers)",     correct_row_headers_fraction);
	TestHelper::printSummaryLine("Board::print (column heads)",    correct_column_headers_fraction);

	unsigned int highest4 = calculateMark(BOARD_SIZE_COUNT, true, true, BOARD_VALUE_COUNT_WITH_MARKED, // constants and types
	                                      1.0, BOARD_VALUE_COUNT, 10, BOARD_VALUE_COUNT,  // basic board manipulation
	                                      LOAD_BOARD_VALUE_COUNT,  // load
	                                      1.0,           // basic print
	                                      1.0,           // print star points
	                                      1.0, 1.0,      // print indexes
	                                      0.0, 0.0, 0.0, // A3 Part D helper functions
	                                      0.0, 0.0,      // A3 Part D non-helper functions
	                                      0.0, 0.0, 0.0, // copy constructor
	                                      0.0, 0.0, 0.0, // assignment operator (basic)
	                                      0.0, 0.0,      // assignment operator (fancy)
	                                      0.0, 0.0,      // size-related functions
	                                      0.0, 0.0,      // print and load other sizes
	                                      0.0, 0.0, 0.0, // A3 Part D other sizes
	                                      0.0, 0.0,      // equality tests (size)
	                                      false, false,  // equality tests (places)
	                                      false);        // finished
	unsigned int mark4 = calculateMark(correct_board_size_count, correct_board_bytes, correct_removed_bytes, correct_board_value,
	                                   correct_default_constructor_fraction, correct_count_with_value,
	                                   correct_set_at, correct_count_after_set,
	                                   correct_load,
	                                   correct_basic_print_fraction,
	                                   correct_star_points_fraction,
	                                   correct_row_headers_fraction, correct_column_headers_fraction,
	                                   0.0, 0.0, 0.0, // A3 Part D helper functions
	                                   0.0, 0.0,      // A3 Part D non-helper functions
	                                   0.0, 0.0, 0.0, // copy constructor
	                                   0.0, 0.0, 0.0, // assignment operator (basic)
	                                   0.0, 0.0,      // assignment operator (fancy)
	                                   0.0, 0.0,      // size-related functions
	                                   0.0, 0.0,      // print and load other sizes
	                                   0.0, 0.0, 0.0, // A3 Part D other sizes
	                                   0.0, 0.0,      // equality tests (size)
	                                   false, false,  // equality tests (places)
	                                   false);        // finished
	assert(mark4 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark4, highest4, TOTAL_MARKS);

	cout << endl;
	cout << endl;


	//
	//  Test copy and destroy, helper functions, and fill-rated functions
	//    -> all of these require the board to have loaded correctly
	//

	unsigned int correct_replace_all    = 0;
	double correct_replace_all_fraction = 0.0;

	unsigned int correct_is_neighbour    = 0;
	double correct_is_neighbour_fraction = 0.0;

	unsigned int correct_fill_connected    = 0;
	double correct_fill_connected_fraction = 0.0;

	unsigned int correct_calculate_score = 0;
	double correct_calculate_score_fraction = 0.0;

	unsigned int correct_play_stone = 0;
	double correct_play_stone_fraction = 0.0;

	unsigned int correct_copy_to       = 0;
	unsigned int correct_copy_from     = 0;
	unsigned int correct_copy_delete   = 0;
	unsigned int correct_assign_to     = 0;
	unsigned int correct_assign_from   = 0;
	unsigned int correct_self_assign   = 0;
	unsigned int correct_chain_assign  = 0;
	unsigned int correct_assign_delete = 0;
	double correct_copy_to_fraction       = 0.0;
	double correct_copy_from_fraction     = 0.0;
	double correct_copy_delete_fraction   = 0.0;
	double correct_assign_to_fraction     = 0.0;
	double correct_assign_from_fraction   = 0.0;
	double correct_self_assign_fraction   = 0.0;
	double correct_chain_assign_fraction  = 0.0;
	double correct_assign_delete_fraction = 0.0;

	cout << "Loading board \"simple.txt\"" << endl;
	static const int SHAPES_STONE_COUNT =  96;
	static const int SHAPES_SCORE       = 122;
	static const int SIMPLE_STONES_BLACK = 16;
	static const int SIMPLE_STONES_WHITE = 13;
	static const int SIMPLE_SCORE_BLACK  = SIMPLE_STONES_BLACK + 2;
	static const int SIMPLE_SCORE_WHITE  = SIMPLE_STONES_WHITE;
	Board board_simple;
	board_simple.load("simple.txt");

	unsigned int correct_load_simple = testBoardCountWithValue(board_simple, BOARD_SIZE_DEFAULT_CORRECT,
	                                                           SIMPLE_STONES_BLACK, SIMPLE_STONES_WHITE, false);
	if(correct_load_shapes < BOARD_VALUE_COUNT)
		cout << "* Skipping ahead: \"shapes.txt\" not loaded correctly" << endl;
	else if(correct_load_simple < BOARD_VALUE_COUNT)
		cout << "* Skipping ahead: \"simple.txt\" not loaded correctly" << endl;
	else if(!isBoardValueValid(BOARD_VALUE_EMPTY_CORRECT))
		cout << "* Skipping ahead: isBoardValueValid returns false for empty" << endl;
	else if(!isBoardValueValid(BOARD_VALUE_BLACK_CORRECT))
		cout << "* Skipping ahead: isBoardValueValid returns false for black" << endl;
	else if(!isBoardValueValid(BOARD_VALUE_WHITE_CORRECT))
		cout << "* Skipping ahead: isBoardValueValid returns false for white" << endl;
	else if(!isBoardValueValid(BOARD_VALUE_MARKED_CORRECT))
		cout << "* Skipping ahead: isBoardValueValid returns false for marked" << endl;
	else if(!isBoardValuePlayer(BOARD_VALUE_BLACK_CORRECT))
		cout << "* Skipping ahead: isBoardValuePlayer returns false for black" << endl;
	else if(!isBoardValuePlayer(BOARD_VALUE_WHITE_CORRECT))
		cout << "* Skipping ahead: isBoardValuePlayer returns false for white" << endl;
	else
	{
		cout << "* Loaded correctly" << endl;
		board_shapes.print();
		cout << endl;

		cout << endl;
		cout << endl;


		//
		//  Test copying and destroying Board
		//

		{
			cout << "Testing Board copy constructor:" << endl;
			cout << "-------------------------------" << endl;
			cout << endl;

			cout << "Copying board \"shapes.txt\"" << endl;
			Board board_shapes2 = board_shapes;
			cout << "* Completed without crashing" << endl;
			correct_copy_to += testBoardCountWithValue(board_shapes2, BOARD_SIZE_DEFAULT_CORRECT,
			                                           SHAPES_STONE_COUNT, SHAPES_STONE_COUNT, true);
			cout << endl;

			cout << "Checking original \"shapes.txt\"" << endl;
			correct_copy_from += testBoardCountWithValue(board_shapes, BOARD_SIZE_DEFAULT_CORRECT,
			                                             SHAPES_STONE_COUNT, SHAPES_STONE_COUNT, true);
			cout << endl;

			cout << "Copying board \"simple.txt\"" << endl;
			Board board_simple2 = board_simple;
			cout << "* Completed without crashing" << endl;
			correct_copy_to += testBoardCountWithValue(board_simple2, BOARD_SIZE_DEFAULT_CORRECT,
			                                           SIMPLE_STONES_BLACK, SIMPLE_STONES_WHITE, true);
			cout << endl;

			cout << "Checking original \"simple.txt\"" << endl;
			correct_copy_from += testBoardCountWithValue(board_simple, BOARD_SIZE_DEFAULT_CORRECT,
			                                             SIMPLE_STONES_BLACK, SIMPLE_STONES_WHITE, true);
			cout << endl;

			cout << endl;
			cout << endl;


			cout << "Testing Board destructor:" << endl;
			cout << "-------------------------" << endl;
			cout << endl;

			cout << "About to destroy copies" << endl;
		}  // boards go out of scope here
		cout << "* Destroyed copies without crashing" << endl;
		cout << endl;

		cout << "Checking original \"shapes.txt\" board" << endl;
		correct_copy_delete += testBoardCountWithValue(board_shapes, BOARD_SIZE_DEFAULT_CORRECT,
			                                           SHAPES_STONE_COUNT, SHAPES_STONE_COUNT, true);
		cout << endl;

		cout << "Checking original \"simple.txt\" board" << endl;
		correct_copy_delete += testBoardCountWithValue(board_simple, BOARD_SIZE_DEFAULT_CORRECT,
			                                           SIMPLE_STONES_BLACK, SIMPLE_STONES_WHITE, true);
		cout << endl;

		cout << endl;
		cout << endl;


		//
		//  Print results 5
		//

		correct_copy_to_fraction     = correct_copy_to       / (BOARD_VALUE_COUNT * 2.0);
		correct_copy_from_fraction   = correct_copy_from     / (BOARD_VALUE_COUNT * 2.0);
		correct_copy_delete_fraction = correct_copy_delete   / (BOARD_VALUE_COUNT * 2.0);

		TestHelper::printSummaryHeaderPartial(32, 2, COMPILE_AND_START_MARKS);
		TestHelper::printSummaryLine("BOARD_SIZE_MIN",                 correct_board_size_min);
		TestHelper::printSummaryLine("BOARD_SIZE_MAX",                 correct_board_size_max);
		TestHelper::printSummaryLine("BOARD_SIZE_DEFAULT",             correct_board_size_default);
		TestHelper::printSummaryLine("Board declaration",              correct_board_bytes);
		TestHelper::printSummaryLine("StonesRemoved declaration",      correct_removed_bytes);
		TestHelper::printSummaryLine("BOARD_VALUE_ constants",         correct_board_value,      BOARD_VALUE_COUNT_WITH_MARKED);
		cout << endl;
		TestHelper::printSummaryLine("Default constructor",            correct_default_constructor_fraction);
		TestHelper::printSummaryLine("Board::countWithValue",          correct_count_with_value, BOARD_VALUE_COUNT);
		TestHelper::printSummaryLine("Board::setAt",                   correct_set_at,           10);
		TestHelper::printSummaryLine("Board::countWithValue",          correct_count_after_set,  BOARD_VALUE_COUNT);
		TestHelper::printSummaryLine("Board::load",                    correct_load,             LOAD_BOARD_VALUE_COUNT);
		TestHelper::printSummaryLine("Board::print (basic)",           correct_basic_print_fraction);
		TestHelper::printSummaryLine("Board::print (star points)",     correct_star_points_fraction);
		TestHelper::printSummaryLine("Board::print (row headers)",     correct_row_headers_fraction);
		TestHelper::printSummaryLine("Board::print (column heads)",    correct_column_headers_fraction);
		cout << endl;
		TestHelper::printSummaryLine("Copy constructor",               correct_copy_to,          2 * BOARD_VALUE_COUNT);
		TestHelper::printSummaryLine("Copied board after constructor", correct_copy_from,        2 * BOARD_VALUE_COUNT);
		TestHelper::printSummaryLine("Copied board after destroy",     correct_copy_delete,      2 * BOARD_VALUE_COUNT);

		unsigned int highest5 = calculateMark(BOARD_SIZE_COUNT, true, true, BOARD_VALUE_COUNT_WITH_MARKED, // constants and types
											  1.0, BOARD_VALUE_COUNT, 10, BOARD_VALUE_COUNT,  // basic board manipulation
											  LOAD_BOARD_VALUE_COUNT,  // load
											  1.0,           // basic print
											  1.0,           // print star points
											  1.0, 1.0,      // print indexes
											  0.0, 0.0, 0.0, // A3 Part D helper functions
		                                      0.0, 0.0,      // A3 Part D non-helper functions
		                                      1.0, 1.0, 1.0, // copy constructor
		                                      0.0, 0.0, 0.0, // assignment operator (basic)
		                                      0.0, 0.0,      // assignment operator (fancy)
		                                      0.0, 0.0,      // size-related functions
		                                      0.0, 0.0,      // print and load other sizes
		                                      0.0, 0.0, 0.0, // A3 Part D other sizes
		                                      0.0, 0.0,      // equality tests (size)
		                                      false, false,  // equality tests (places)
											  false);        // finished
		unsigned int mark5 = calculateMark(correct_board_size_count, correct_board_bytes, correct_removed_bytes, correct_board_value,
										   correct_default_constructor_fraction, correct_count_with_value,
										   correct_set_at, correct_count_after_set,
										   correct_load,
										   correct_basic_print_fraction,
										   correct_star_points_fraction,
										   correct_row_headers_fraction, correct_column_headers_fraction,
		                                   0.0, 0.0, 0.0, // A3 Part D helper functions
		                                   0.0, 0.0,      // A3 Part D non-helper functions
		                                   correct_copy_to_fraction, correct_copy_from_fraction, correct_copy_delete_fraction,
		                                   0.0, 0.0, 0.0, // assignment operator (basic)
		                                   0.0, 0.0,      // assignment operator (fancy)
		                                   0.0, 0.0,      // size-related functions
		                                   0.0, 0.0,      // print and load other sizes
		                                   0.0, 0.0, 0.0, // A3 Part D other sizes
		                                   0.0, 0.0,      // equality tests (size)
		                                   false, false,  // equality tests (places)
										   false);        // finished
		assert(mark5 <= TOTAL_MARKS);
		TestHelper::printMarkPartial(mark5, highest5, TOTAL_MARKS);

		cout << endl;
		cout << endl;


		//
		//  Test helper functions
		//

		cout << "Testing replaceAll:" << endl;
		cout << "-------------------" << endl;
		cout << endl;

		cout << "Replacing black with empty" << endl;
		Board board_replace_b_e = board_shapes;
		board_replace_b_e.replaceAll(BOARD_VALUE_BLACK_CORRECT, BOARD_VALUE_EMPTY_CORRECT);
		board_replace_b_e.print();
		unsigned int correct_replace_all_b_e = testBoardCountWithValue(board_replace_b_e, BOARD_SIZE_DEFAULT_CORRECT,
		                                                               0, SHAPES_STONE_COUNT, true);
		cout << endl;

		cout << "Replacing white with empty" << endl;
		Board board_replace_w_e = board_shapes;
		board_replace_w_e.replaceAll(BOARD_VALUE_WHITE_CORRECT, BOARD_VALUE_EMPTY_CORRECT);
		unsigned int correct_replace_all_w_e = testBoardCountWithValue(board_replace_w_e, BOARD_SIZE_DEFAULT_CORRECT,
		                                                               SHAPES_STONE_COUNT, 0, true);
		cout << endl;

		cout << "Replacing black with white" << endl;
		Board board_replace_b_w = board_shapes;
		board_replace_b_w.replaceAll(BOARD_VALUE_BLACK_CORRECT, BOARD_VALUE_WHITE_CORRECT);
		unsigned int correct_replace_all_b_w = testBoardCountWithValue(board_replace_b_w, BOARD_SIZE_DEFAULT_CORRECT,
		                                                               0, SHAPES_STONE_COUNT * 2, true);
		cout << endl;

		cout << "Replacing white with black" << endl;
		Board board_replace_w_b = board_shapes;
		board_replace_w_b.replaceAll(BOARD_VALUE_WHITE_CORRECT, BOARD_VALUE_BLACK_CORRECT);
		unsigned int correct_replace_all_w_b = testBoardCountWithValue(board_replace_w_b, BOARD_SIZE_DEFAULT_CORRECT,
		                                                               SHAPES_STONE_COUNT * 2, 0, true);
		cout << endl;

		cout << "Replacing empty with black" << endl;
		Board board_replace_e_b = board_shapes;
		board_replace_e_b.replaceAll(BOARD_VALUE_EMPTY_CORRECT, BOARD_VALUE_BLACK_CORRECT);
		unsigned int correct_replace_all_e_b = testBoardCountWithValue(board_replace_e_b, BOARD_SIZE_DEFAULT_CORRECT,
		                                                               BOARD_CELL_COUNT_DEFAULT - SHAPES_STONE_COUNT, SHAPES_STONE_COUNT, true);
		cout << endl;

		cout << "Replacing empty with white" << endl;
		Board board_replace_e_w = board_shapes;
		board_replace_e_w.replaceAll(BOARD_VALUE_EMPTY_CORRECT, BOARD_VALUE_WHITE_CORRECT);
		unsigned int correct_replace_all_e_w = testBoardCountWithValue(board_replace_e_w, BOARD_SIZE_DEFAULT_CORRECT,
		                                                               SHAPES_STONE_COUNT, BOARD_CELL_COUNT_DEFAULT - SHAPES_STONE_COUNT, true);
		cout << endl;

		cout << endl;
		cout << endl;


		cout << "Testing isANeighbourWithValue:" << endl;
		cout << "------------------------------" << endl;
		cout << endl;

		cout << "Testing G2 (row 2, column 6)" << endl;
		unsigned int correct_is_neighbour_1 = testIsANeighbourWithValue(board_shapes, 2, 6, true, false, true);
		cout << endl;

		cout << "Testing K11 (row 11, column 9)" << endl;
		unsigned int correct_is_neighbour_2 = testIsANeighbourWithValue(board_shapes, 11, 9, false, true, false);
		cout << endl;

		cout << "Testing P0 (row 0, column 13) - borders top" << endl;
		unsigned int correct_is_neighbour_3 = testIsANeighbourWithValue(board_shapes, 0, 13, true, true, true);
		cout << endl;

		cout << "Testing B18 (row 18, column 1) - borders bottom" << endl;
		unsigned int correct_is_neighbour_4 = testIsANeighbourWithValue(board_shapes, 18, 1, false, false, true);
		cout << endl;

		cout << "Testing A3 (row 3, column 0) - borders left" << endl;
		unsigned int correct_is_neighbour_5 = testIsANeighbourWithValue(board_shapes, 3, 0, true, false, false);
		cout << endl;

		cout << "Testing U9 (row 9, column 18) - borders right" << endl;
		unsigned int correct_is_neighbour_6 = testIsANeighbourWithValue(board_shapes, 9, 18, true, true, false);
		cout << endl;

		cout << endl;
		cout << endl;


		cout << "Testing fillConnected:" << endl;
		cout << "----------------------" << endl;
		cout << endl;

		cout << "Marking black from empty" << endl;
		Board board_fill_b_e = board_shapes;
		board_fill_b_e.fillConnected(BOARD_VALUE_BLACK_CORRECT, BOARD_VALUE_MARKED_CORRECT, BOARD_VALUE_EMPTY_CORRECT);
		board_fill_b_e.print();
		bool correct_fill_connected_b_e = testFillConnected(board_fill_b_e, BOARD_SIZE_DEFAULT_CORRECT,
		                                                    SHAPES_STONE_COUNT - 5,
		                                                    BOARD_VALUE_BLACK_CORRECT, SHAPES_STONE_COUNT);
		cout << endl;

		cout << "Marking white from empty" << endl;
		Board board_fill_w_e = board_shapes;
		board_fill_w_e.fillConnected(BOARD_VALUE_WHITE_CORRECT, BOARD_VALUE_MARKED_CORRECT, BOARD_VALUE_EMPTY_CORRECT);
		bool correct_fill_connected_w_e = testFillConnected(board_fill_w_e, BOARD_SIZE_DEFAULT_CORRECT,
		                                                    SHAPES_STONE_COUNT - 5,
		                                                    BOARD_VALUE_WHITE_CORRECT, SHAPES_STONE_COUNT);
		cout << endl;

		cout << "Marking black from white" << endl;
		Board board_fill_b_w = board_shapes;
		board_fill_b_w.fillConnected(BOARD_VALUE_BLACK_CORRECT, BOARD_VALUE_MARKED_CORRECT, BOARD_VALUE_WHITE_CORRECT);
		bool correct_fill_connected_b_w = testFillConnected(board_fill_b_w, BOARD_SIZE_DEFAULT_CORRECT,
		                                                    83,
		                                                    BOARD_VALUE_BLACK_CORRECT, SHAPES_STONE_COUNT);
		cout << endl;

		cout << "Marking white from black" << endl;
		Board board_fill_w_b = board_shapes;
		board_fill_w_b.fillConnected(BOARD_VALUE_WHITE_CORRECT, BOARD_VALUE_MARKED_CORRECT, BOARD_VALUE_BLACK_CORRECT);
		bool correct_fill_connected_w_b = testFillConnected(board_fill_w_b, BOARD_SIZE_DEFAULT_CORRECT,
		                                                    83,
		                                                    BOARD_VALUE_WHITE_CORRECT, SHAPES_STONE_COUNT);
		cout << endl;

		cout << "Marking empty from black" << endl;
		Board board_fill_e_b = board_shapes;
		board_fill_e_b.fillConnected(BOARD_VALUE_EMPTY_CORRECT, BOARD_VALUE_MARKED_CORRECT, BOARD_VALUE_BLACK_CORRECT);
		bool correct_fill_connected_e_b = testFillConnected(board_fill_e_b, BOARD_SIZE_DEFAULT_CORRECT,
		                                                    143,
		                                                    BOARD_VALUE_EMPTY_CORRECT, SHAPES_STONE_COUNT);
		cout << endl;

		cout << "Marking empty from white" << endl;
		Board board_fill_e_w = board_shapes;
		board_fill_e_w.fillConnected(BOARD_VALUE_EMPTY_CORRECT, BOARD_VALUE_MARKED_CORRECT, BOARD_VALUE_WHITE_CORRECT);
		bool correct_fill_connected_e_w = testFillConnected(board_fill_e_w, BOARD_SIZE_DEFAULT_CORRECT,
		                                                    143,
		                                                    BOARD_VALUE_EMPTY_CORRECT, SHAPES_STONE_COUNT);
		cout << endl;

		cout << endl;
		cout << endl;


		//
		//  Print results 6
		//

		correct_replace_all = correct_replace_all_b_e +
		                      correct_replace_all_w_e +
		                      correct_replace_all_b_w +
		                      correct_replace_all_w_b +
		                      correct_replace_all_e_b +
		                      correct_replace_all_e_w;
		correct_replace_all_fraction = correct_replace_all / (BOARD_VALUE_COUNT * 6.0);

		correct_is_neighbour = correct_is_neighbour_1 +
		                       correct_is_neighbour_2 +
		                       correct_is_neighbour_3 +
		                       correct_is_neighbour_4 +
		                       correct_is_neighbour_5 +
		                       correct_is_neighbour_6;
		correct_is_neighbour_fraction = correct_is_neighbour / (BOARD_VALUE_COUNT * 6.0);

		correct_fill_connected = (correct_fill_connected_b_e ? 1 : 0) +
		                         (correct_fill_connected_w_e ? 1 : 0) +
		                         (correct_fill_connected_b_w ? 1 : 0) +
		                         (correct_fill_connected_w_b ? 1 : 0) +
		                         (correct_fill_connected_e_b ? 1 : 0) +
		                         (correct_fill_connected_e_w ? 1 : 0);
		correct_fill_connected_fraction = correct_fill_connected / 6.0;

		TestHelper::printSummaryHeaderPartial(32, 2, COMPILE_AND_START_MARKS);
		TestHelper::printSummaryLine("BOARD_SIZE_MIN",                 correct_board_size_min);
		TestHelper::printSummaryLine("BOARD_SIZE_MAX",                 correct_board_size_max);
		TestHelper::printSummaryLine("BOARD_SIZE_DEFAULT",             correct_board_size_default);
		TestHelper::printSummaryLine("Board declaration",              correct_board_bytes);
		TestHelper::printSummaryLine("StonesRemoved declaration",      correct_removed_bytes);
		TestHelper::printSummaryLine("BOARD_VALUE_ constants",         correct_board_value,      BOARD_VALUE_COUNT_WITH_MARKED);
		cout << endl;
		TestHelper::printSummaryLine("Default constructor",            correct_default_constructor_fraction);
		TestHelper::printSummaryLine("Board::countWithValue",          correct_count_with_value, BOARD_VALUE_COUNT);
		TestHelper::printSummaryLine("Board::setAt",                   correct_set_at,           10);
		TestHelper::printSummaryLine("Board::countWithValue",          correct_count_after_set,  BOARD_VALUE_COUNT);
		TestHelper::printSummaryLine("Board::load",                    correct_load,             LOAD_BOARD_VALUE_COUNT);
		TestHelper::printSummaryLine("Board::print (basic)",           correct_basic_print_fraction);
		TestHelper::printSummaryLine("Board::print (star points)",     correct_star_points_fraction);
		TestHelper::printSummaryLine("Board::print (row headers)",     correct_row_headers_fraction);
		TestHelper::printSummaryLine("Board::print (column heads)",    correct_column_headers_fraction);
		cout << endl;
		TestHelper::printSummaryLine("Board::replaceAll",              correct_replace_all,      6 * BOARD_VALUE_COUNT);
		TestHelper::printSummaryLine("Board::isANeighbourWithValue",   correct_is_neighbour,     6 * BOARD_VALUE_COUNT);
		TestHelper::printSummaryLine("Board::fillConnected",           correct_fill_connected,   6);
		cout << endl;
		TestHelper::printSummaryLine("Copy constructor",               correct_copy_to,          2 * BOARD_VALUE_COUNT);
		TestHelper::printSummaryLine("Copied board after constructor", correct_copy_from,        2 * BOARD_VALUE_COUNT);
		TestHelper::printSummaryLine("Copied board after destroy",     correct_copy_delete,      2 * BOARD_VALUE_COUNT);

		unsigned int highest6 = calculateMark(BOARD_SIZE_COUNT, true, true, BOARD_VALUE_COUNT_WITH_MARKED, // constants and types
											  1.0, BOARD_VALUE_COUNT, 10, BOARD_VALUE_COUNT,  // basic board manipulation
											  LOAD_BOARD_VALUE_COUNT,  // load
											  1.0,           // basic print
											  1.0,           // print star points
											  1.0, 1.0,      // print indexes
											  1.0, 1.0, 1.0, // A3 Part D helper functions
		                                      0.0, 0.0,      // A3 Part D non-helper functions
		                                      1.0, 1.0, 1.0, // copy constructor
		                                      0.0, 0.0, 0.0, // assignment operator (basic)
		                                      0.0, 0.0,      // assignment operator (fancy)
		                                      0.0, 0.0,      // size-related functions
		                                      0.0, 0.0,      // print and load other sizes
		                                      0.0, 0.0, 0.0, // A3 Part D other sizes
		                                      0.0, 0.0,      // equality tests (size)
		                                      false, false,  // equality tests (places)
											  false);        // finished
		unsigned int mark6 = calculateMark(correct_board_size_count, correct_board_bytes, correct_removed_bytes, correct_board_value,
										   correct_default_constructor_fraction, correct_count_with_value,
										   correct_set_at, correct_count_after_set,
										   correct_load,
										   correct_basic_print_fraction,
										   correct_star_points_fraction,
										   correct_row_headers_fraction, correct_column_headers_fraction,
		                                   correct_replace_all_fraction, correct_is_neighbour_fraction, correct_fill_connected_fraction,
		                                   0.0, 0.0,      // A3 Part D non-helper functions
		                                   correct_copy_to_fraction, correct_copy_from_fraction, correct_copy_delete_fraction,
		                                   0.0, 0.0, 0.0, // assignment operator (basic)
		                                   0.0, 0.0,      // assignment operator (fancy)
		                                   0.0, 0.0,      // size-related functions
		                                   0.0, 0.0,      // print and load other sizes
		                                   0.0, 0.0, 0.0, // A3 Part D other sizes
		                                   0.0, 0.0,      // equality tests (size)
		                                   false, false,  // equality tests (places)
										   false);        // finished
		assert(mark6 <= TOTAL_MARKS);
		TestHelper::printMarkPartial(mark6, highest6, TOTAL_MARKS);

		cout << endl;
		cout << endl;


		//
		//  Test non-helper functions
		//

		cout << "Testing calculateScore:" << endl;
		cout << "-----------------------" << endl;
		cout << endl;

		cout << "Calculating score for black: ";
		unsigned int correct_score_black = testCalculateScore(board_shapes, BOARD_SIZE_DEFAULT_CORRECT,
		                                                      BOARD_VALUE_BLACK_CORRECT, BOARD_VALUE_WHITE_CORRECT, SHAPES_SCORE);
		cout << endl;

		cout << "Calculating score for white: ";
		unsigned int correct_score_white = testCalculateScore(board_shapes, BOARD_SIZE_DEFAULT_CORRECT,
		                                                      BOARD_VALUE_WHITE_CORRECT, BOARD_VALUE_BLACK_CORRECT, SHAPES_SCORE);
		cout << endl;

		cout << endl;
		cout << endl;


		cout << "Testing playStone:" << endl;
		cout << "------------------" << endl;
		cout << endl;

		cout << "Starting with \"simple.txt\"" << endl;
		board_simple.print();
		cout << "Score is " << SIMPLE_SCORE_BLACK << " (black) to " << SIMPLE_SCORE_WHITE << " (white)" << endl;
		cout << endl;

		cout << "Playing black at C14 (row 14, column 2) - far away" << endl;
		Board board_replace = board_simple;
		StonesRemoved removed_replace = board_replace.playStone(14, 2, BOARD_VALUE_BLACK_CORRECT);
		board_replace.print();
		unsigned int correct_play_1 = testPlayStone(board_replace, removed_replace,
		                                            BOARD_VALUE_BLACK_CORRECT, BOARD_VALUE_WHITE_CORRECT,
		                                            SIMPLE_SCORE_BLACK + 1, SIMPLE_SCORE_WHITE, 0, 0,
		                                            "black", "white");
		cout << endl;

		cout << "Playing white at J18 (row 18, column 8) - claim territory" << endl;
		Board board_play_safe = board_simple;
		StonesRemoved removed_safe = board_play_safe.playStone(18, 8, BOARD_VALUE_WHITE_CORRECT);
		board_play_safe.print();
		unsigned int correct_play_2 = testPlayStone(board_play_safe, removed_safe,
		                                            BOARD_VALUE_WHITE_CORRECT, BOARD_VALUE_BLACK_CORRECT,
		                                            SIMPLE_SCORE_WHITE + 25, SIMPLE_SCORE_BLACK, 0, 0,
		                                            "white", "black");
		cout << endl;

		cout << "Playing black at G15 (row 15, column 6) - fill eye" << endl;
		Board board_play_fill_eye = board_simple;
		StonesRemoved removed_fill_eye = board_play_fill_eye.playStone(15, 6, BOARD_VALUE_BLACK_CORRECT);
		board_play_fill_eye.print();
		unsigned int correct_play_3 = testPlayStone(board_play_fill_eye, removed_fill_eye,
		                                            BOARD_VALUE_BLACK_CORRECT, BOARD_VALUE_WHITE_CORRECT,
		                                            SIMPLE_SCORE_BLACK, SIMPLE_SCORE_WHITE, 0, 0,
		                                            "black", "white");
		cout << endl;

		cout << "Playing white at G15 (row 15, column 6) - suicide" << endl;
		Board board_play_suicide = board_simple;
		StonesRemoved removed_suicide = board_play_suicide.playStone(15, 6, BOARD_VALUE_WHITE_CORRECT);
		board_play_suicide.print();
		unsigned int correct_play_4 = testPlayStone(board_play_suicide, removed_suicide,
		                                            BOARD_VALUE_WHITE_CORRECT, BOARD_VALUE_BLACK_CORRECT,
		                                            SIMPLE_SCORE_WHITE, SIMPLE_SCORE_BLACK, 1, 0,
		                                            "white", "black");
		cout << endl;

		cout << "Playing black at J16 (row 16, column 8) - capture" << endl;
		Board board_play_capture = board_simple;
		StonesRemoved removed_capture = board_play_capture.playStone(16, 8, BOARD_VALUE_BLACK_CORRECT);
		board_play_capture.print();
		unsigned int correct_play_5 = testPlayStone(board_play_capture, removed_capture,
		                                            BOARD_VALUE_BLACK_CORRECT, BOARD_VALUE_WHITE_CORRECT,
		                                            SIMPLE_SCORE_BLACK + 2, SIMPLE_SCORE_WHITE - 1, 0, 1,
		                                            "black", "white");
		cout << endl;

		cout << "Playing white at L15 (row 15, column 10) - capture (order matters)" << endl;
		Board board_play_ordered = board_simple;
		StonesRemoved removed_ordered = board_play_ordered.playStone(15, 10, BOARD_VALUE_WHITE_CORRECT);
		board_play_ordered.print();
		unsigned int correct_play_6 = testPlayStone(board_play_ordered, removed_ordered,
		                                            BOARD_VALUE_WHITE_CORRECT, BOARD_VALUE_BLACK_CORRECT,
		                                            SIMPLE_SCORE_WHITE + 2, SIMPLE_SCORE_BLACK - 2, 0, 1,
		                                            "white", "black");
		cout << endl;

		cout << endl;
		cout << endl;


		//
		//  Print results 7
		//

		correct_calculate_score = correct_score_black +
		                          correct_score_white;
		correct_calculate_score_fraction = correct_calculate_score / 4.0;

		correct_play_stone = correct_play_1 +
		                     correct_play_2 +
		                     correct_play_3 +
		                     correct_play_4 +
		                     correct_play_5 +
		                     correct_play_6;
		correct_play_stone_fraction = correct_play_stone / 24.0;

		TestHelper::printSummaryHeaderPartial(32, 2, COMPILE_AND_START_MARKS);
		TestHelper::printSummaryLine("BOARD_SIZE_MIN",                 correct_board_size_min);
		TestHelper::printSummaryLine("BOARD_SIZE_MAX",                 correct_board_size_max);
		TestHelper::printSummaryLine("BOARD_SIZE_DEFAULT",             correct_board_size_default);
		TestHelper::printSummaryLine("Board declaration",              correct_board_bytes);
		TestHelper::printSummaryLine("StonesRemoved declaration",      correct_removed_bytes);
		TestHelper::printSummaryLine("BOARD_VALUE_ constants",         correct_board_value,      BOARD_VALUE_COUNT_WITH_MARKED);
		cout << endl;
		TestHelper::printSummaryLine("Default constructor",            correct_default_constructor_fraction);
		TestHelper::printSummaryLine("Board::countWithValue",          correct_count_with_value, BOARD_VALUE_COUNT);
		TestHelper::printSummaryLine("Board::setAt",                   correct_set_at,           10);
		TestHelper::printSummaryLine("Board::countWithValue",          correct_count_after_set,  BOARD_VALUE_COUNT);
		TestHelper::printSummaryLine("Board::load",                    correct_load,             LOAD_BOARD_VALUE_COUNT);
		TestHelper::printSummaryLine("Board::print (basic)",           correct_basic_print_fraction);
		TestHelper::printSummaryLine("Board::print (star points)",     correct_star_points_fraction);
		TestHelper::printSummaryLine("Board::print (row headers)",     correct_row_headers_fraction);
		TestHelper::printSummaryLine("Board::print (column heads)",    correct_column_headers_fraction);
		cout << endl;
		TestHelper::printSummaryLine("Board::replaceAll",              correct_replace_all,      6 * BOARD_VALUE_COUNT);
		TestHelper::printSummaryLine("Board::isANeighbourWithValue",   correct_is_neighbour,     6 * BOARD_VALUE_COUNT);
		TestHelper::printSummaryLine("Board::fillConnected",           correct_fill_connected,   6);
		TestHelper::printSummaryLine("Board::calculateScore",          correct_calculate_score,  2 * 2);
		TestHelper::printSummaryLine("Board::playStone",               correct_play_stone,       6 * 4);
		cout << endl;
		TestHelper::printSummaryLine("Copy constructor",               correct_copy_to,          2 * BOARD_VALUE_COUNT);
		TestHelper::printSummaryLine("Copied board after constructor", correct_copy_from,        2 * BOARD_VALUE_COUNT);
		TestHelper::printSummaryLine("Copied board after destroy",     correct_copy_delete,      2 * BOARD_VALUE_COUNT);

		unsigned int highest7 = calculateMark(BOARD_SIZE_COUNT, true, true, BOARD_VALUE_COUNT_WITH_MARKED, // constants and types
											  1.0, BOARD_VALUE_COUNT, 10, BOARD_VALUE_COUNT,  // basic board manipulation
											  LOAD_BOARD_VALUE_COUNT,  // load
											  1.0,           // basic print
											  1.0,           // print star points
											  1.0, 1.0,      // print indexes
											  1.0, 1.0, 1.0, // A3 Part D helper functions
		                                      1.0, 1.0,      // A3 Part D non-helper functions
		                                      1.0, 1.0, 1.0, // copy constructor
		                                      0.0, 0.0, 0.0, // assignment operator (basic)
		                                      0.0, 0.0,      // assignment operator (fancy)
		                                      0.0, 0.0,      // size-related functions
		                                      0.0, 0.0,      // print and load other sizes
		                                      0.0, 0.0, 0.0, // A3 Part D other sizes
		                                      0.0, 0.0,      // equality tests (size)
		                                      false, false,  // equality tests (places)
											  false);        // finished
		unsigned int mark7 = calculateMark(correct_board_size_count, correct_board_bytes, correct_removed_bytes, correct_board_value,
										   correct_default_constructor_fraction, correct_count_with_value,
										   correct_set_at, correct_count_after_set,
										   correct_load,
										   correct_basic_print_fraction,
										   correct_star_points_fraction,
										   correct_row_headers_fraction, correct_column_headers_fraction,
		                                   correct_replace_all_fraction, correct_is_neighbour_fraction, correct_fill_connected_fraction,
		                                   correct_calculate_score_fraction, correct_play_stone_fraction,
		                                   correct_copy_to_fraction, correct_copy_from_fraction, correct_copy_delete_fraction,
		                                   0.0, 0.0, 0.0, // assignment operator (basic)
		                                   0.0, 0.0,      // assignment operator (fancy)
		                                   0.0, 0.0,      // size-related functions
		                                   0.0, 0.0,      // print and load other sizes
		                                   0.0, 0.0, 0.0, // A3 Part D other sizes
		                                   0.0, 0.0,      // equality tests (size)
		                                   false, false,  // equality tests (places)
										   false);        // finished
		assert(mark7 <= TOTAL_MARKS);
		TestHelper::printMarkPartial(mark7, highest7, TOTAL_MARKS);

		cout << endl;
		cout << endl;


		//
		//  Test assignment operator
		//

		{
			cout << "Testing Board assignment operator (operator=):" << endl;
			cout << "----------------------------------------------" << endl;
			cout << endl;

			cout << "Creating an empty board" << endl;
			Board board_assign;
			cout << "* Completed without crashing" << endl;
			cout << endl;

			cout << "Assigning board to \"shapes.txt\"" << endl;
			board_assign = board_shapes;
			cout << "* Completed without crashing" << endl;
			correct_assign_to += testBoardCountWithValue(board_assign, BOARD_SIZE_DEFAULT_CORRECT,
			                                             SHAPES_STONE_COUNT, SHAPES_STONE_COUNT, true);
			cout << endl;

			cout << "Checking original \"shapes.txt\"" << endl;
			correct_assign_from += testBoardCountWithValue(board_shapes, BOARD_SIZE_DEFAULT_CORRECT,
			                                               SHAPES_STONE_COUNT, SHAPES_STONE_COUNT, true);
			cout << endl;

			cout << "Assigning board to \"simple.txt\"" << endl;
			board_assign = board_simple;
			cout << "* Completed without crashing" << endl;
			correct_assign_to += testBoardCountWithValue(board_assign, BOARD_SIZE_DEFAULT_CORRECT,
			                                             SIMPLE_STONES_BLACK, SIMPLE_STONES_WHITE, true);
			cout << endl;

			cout << "Checking original \"shapes.txt\"" << endl;
			correct_assign_from += testBoardCountWithValue(board_shapes, BOARD_SIZE_DEFAULT_CORRECT,
			                                               SHAPES_STONE_COUNT, SHAPES_STONE_COUNT, true);
			cout << endl;

			cout << "Checking original \"simple.txt\"" << endl;
			correct_assign_from += testBoardCountWithValue(board_simple, BOARD_SIZE_DEFAULT_CORRECT,
			                                               SIMPLE_STONES_BLACK, SIMPLE_STONES_WHITE, true);
			cout << endl;

			cout << endl;
			cout << endl;


			cout << "Testing self-assignment:" << endl;
			cout << "------------------------" << endl;
			cout << endl;

			cout << "Assigning board to itself" << endl;
			board_assign = board_assign;
			cout << "* Completed once without crashing" << endl;
			board_assign = board_assign;
			cout << "* Completed twice without crashing" << endl;
			correct_self_assign += testBoardCountWithValue(board_assign, BOARD_SIZE_DEFAULT_CORRECT,
			                                               SIMPLE_STONES_BLACK, SIMPLE_STONES_WHITE, true);
			cout << endl;

			cout << "Checking original \"shapes.txt\"" << endl;
			correct_assign_from += testBoardCountWithValue(board_shapes, BOARD_SIZE_DEFAULT_CORRECT,
			                                               SHAPES_STONE_COUNT, SHAPES_STONE_COUNT, true);
			cout << endl;

			cout << "Checking original \"simple.txt\"" << endl;
			correct_assign_from += testBoardCountWithValue(board_simple, BOARD_SIZE_DEFAULT_CORRECT,
			                                               SIMPLE_STONES_BLACK, SIMPLE_STONES_WHITE, true);
			cout << endl;

			cout << endl;
			cout << endl;


			cout << "Testing chained assignment:" << endl;
			cout << "---------------------------" << endl;
			cout << endl;

			cout << "Chain-assigning board (in middle)" << endl;
			Board board_extra;
			cout << "* Created another board without crashing" << endl;
			board_extra = board_assign = board_shapes;
			cout << "* Completed without crashing" << endl;
			correct_chain_assign += testBoardCountWithValue(board_assign, BOARD_SIZE_DEFAULT_CORRECT,
			                                                SHAPES_STONE_COUNT, SHAPES_STONE_COUNT, true);
			cout << endl;

			cout << "Checking original \"shapes.txt\"" << endl;
			correct_assign_from += testBoardCountWithValue(board_shapes, BOARD_SIZE_DEFAULT_CORRECT,
			                                               SHAPES_STONE_COUNT, SHAPES_STONE_COUNT, true);
			cout << endl;

			cout << "Checking original \"simple.txt\"" << endl;
			correct_assign_from += testBoardCountWithValue(board_simple, BOARD_SIZE_DEFAULT_CORRECT,
			                                               SIMPLE_STONES_BLACK, SIMPLE_STONES_WHITE, true);
			cout << endl;

			cout << "Chain-assigning board (as leftmost)" << endl;
			cout << "* Created another board without crashing" << endl;
			board_assign = board_extra = board_simple;
			cout << "* Completed without crashing" << endl;
			correct_chain_assign += testBoardCountWithValue(board_assign, BOARD_SIZE_DEFAULT_CORRECT,
			                                                SIMPLE_STONES_BLACK, SIMPLE_STONES_WHITE, true);
			cout << endl;

			cout << "Checking original \"shapes.txt\"" << endl;
			correct_assign_from += testBoardCountWithValue(board_shapes, BOARD_SIZE_DEFAULT_CORRECT,
			                                               SHAPES_STONE_COUNT, SHAPES_STONE_COUNT, true);
			cout << endl;

			cout << "Checking original \"simple.txt\"" << endl;
			correct_assign_from += testBoardCountWithValue(board_simple, BOARD_SIZE_DEFAULT_CORRECT,
			                                               SIMPLE_STONES_BLACK, SIMPLE_STONES_WHITE, true);
			cout << endl;

			cout << "About to destroy extra boards" << endl;
		}  // boards go out of scope here
		cout << "* Destroyed copies without crashing" << endl;
		cout << endl;

		cout << "Checking original \"shapes.txt\" board" << endl;
		correct_assign_delete += testBoardCountWithValue(board_shapes, BOARD_SIZE_DEFAULT_CORRECT,
			                                             SHAPES_STONE_COUNT, SHAPES_STONE_COUNT, true);
		cout << endl;

		cout << "Checking original \"simple.txt\" board" << endl;
		correct_assign_delete += testBoardCountWithValue(board_simple, BOARD_SIZE_DEFAULT_CORRECT,
			                                             SIMPLE_STONES_BLACK, SIMPLE_STONES_WHITE, true);
		cout << endl;
	}

	cout << endl;
	cout << endl;


	//
	//  Print results 8
	//

	correct_assign_to_fraction     = correct_assign_to     / (BOARD_VALUE_COUNT * 2.0);
	correct_assign_from_fraction   = correct_assign_from   / (BOARD_VALUE_COUNT * 9.0);
	correct_assign_delete_fraction = correct_assign_delete / (BOARD_VALUE_COUNT * 2.0);

	correct_self_assign_fraction  = correct_self_assign   / (BOARD_VALUE_COUNT * 1.0);
	correct_chain_assign_fraction = correct_chain_assign  / (BOARD_VALUE_COUNT * 2.0);

	TestHelper::printSummaryHeaderPartial(32, 2, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("BOARD_SIZE_MIN",                 correct_board_size_min);
	TestHelper::printSummaryLine("BOARD_SIZE_MAX",                 correct_board_size_max);
	TestHelper::printSummaryLine("BOARD_SIZE_DEFAULT",             correct_board_size_default);
	TestHelper::printSummaryLine("Board declaration",              correct_board_bytes);
	TestHelper::printSummaryLine("StonesRemoved declaration",      correct_removed_bytes);
	TestHelper::printSummaryLine("BOARD_VALUE_ constants",         correct_board_value,      BOARD_VALUE_COUNT_WITH_MARKED);
	cout << endl;
	TestHelper::printSummaryLine("Default constructor",            correct_default_constructor_fraction);
	TestHelper::printSummaryLine("Board::countWithValue",          correct_count_with_value, BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::setAt",                   correct_set_at,           10);
	TestHelper::printSummaryLine("Board::countWithValue",          correct_count_after_set,  BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::load",                    correct_load,             LOAD_BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::print (basic)",           correct_basic_print_fraction);
	TestHelper::printSummaryLine("Board::print (star points)",     correct_star_points_fraction);
	TestHelper::printSummaryLine("Board::print (row headers)",     correct_row_headers_fraction);
	TestHelper::printSummaryLine("Board::print (column heads)",    correct_column_headers_fraction);
	cout << endl;
	TestHelper::printSummaryLine("Board::replaceAll",              correct_replace_all,      6 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::isANeighbourWithValue",   correct_is_neighbour,     6 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::fillConnected",           correct_fill_connected,   6);
	TestHelper::printSummaryLine("Board::calculateScore",          correct_calculate_score,  2 * 2);
	TestHelper::printSummaryLine("Board::playStone",               correct_play_stone,       6 * 4);
	cout << endl;
	TestHelper::printSummaryLine("Copy constructor",               correct_copy_to,          2 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Copied board after constructor", correct_copy_from,        2 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Copied board after destroy",     correct_copy_delete,      2 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Assignment operator",            correct_assign_to,        2 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Copied board after assignment",  correct_assign_from,      9 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Self-assignment",                correct_self_assign,      1 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Chained assignment",             correct_chain_assign,     2 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Copied board after destroy",     correct_assign_delete,    2 * BOARD_VALUE_COUNT);

	unsigned int highest8 = calculateMark(BOARD_SIZE_COUNT, true, true, BOARD_VALUE_COUNT_WITH_MARKED, // constants and types
	                                      1.0, BOARD_VALUE_COUNT, 10, BOARD_VALUE_COUNT,  // basic board manipulation
	                                      LOAD_BOARD_VALUE_COUNT,  // load
	                                      1.0,           // basic print
	                                      1.0,           // print star points
	                                      1.0, 1.0,      // print indexes
	                                      1.0, 1.0, 1.0, // A3 Part D helper functions
	                                      1.0, 1.0,      // A3 Part D non-helper functions
	                                      1.0, 1.0, 1.0, // copy constructor
	                                      1.0, 1.0, 1.0, // assignment operator (basic)
	                                      1.0, 1.0,      // assignment operator (fancy)
	                                      0.0, 0.0,      // size-related functions
	                                      0.0, 0.0,      // print and load other sizes
	                                      0.0, 0.0, 0.0, // A3 Part D other sizes
	                                      0.0, 0.0,      // equality tests (size)
	                                      false, false,  // equality tests (places)
	                                      false);        // finished
	unsigned int mark8 = calculateMark(correct_board_size_count, correct_board_bytes, correct_removed_bytes, correct_board_value,
	                                   correct_default_constructor_fraction, correct_count_with_value,
	                                   correct_set_at, correct_count_after_set,
	                                   correct_load,
	                                   correct_basic_print_fraction,
	                                   correct_star_points_fraction,
	                                   correct_row_headers_fraction, correct_column_headers_fraction,
	                                   correct_replace_all_fraction, correct_is_neighbour_fraction, correct_fill_connected_fraction,
	                                   correct_calculate_score_fraction, correct_play_stone_fraction,
	                                   correct_copy_to_fraction, correct_copy_from_fraction, correct_copy_delete_fraction,
	                                   correct_assign_to_fraction, correct_assign_from_fraction, correct_assign_delete_fraction,
	                                   correct_self_assign_fraction, correct_chain_assign_fraction,
	                                   0.0, 0.0,      // size-related functions
	                                   0.0, 0.0,      // print and load other sizes
	                                   0.0, 0.0, 0.0, // A3 Part D other sizes
	                                   0.0, 0.0,      // equality tests (size)
	                                   false, false,  // equality tests (places)
	                                   false);        // finished
	assert(mark8 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark8, highest8, TOTAL_MARKS);

	cout << endl;
	cout << endl;


	//
	//  Test all sizes of boards
	//

	cout << "Testing a 9x9 board:" << endl;
	cout << "--------------------" << endl;
	cout << endl;

	cout << "Creating 9x9 board with size constructor" << endl;
	Board board_9(9);
	cout << "* Completed without crashing" << endl;
	cout << endl;

	cout << "Testing Board::getSize for 9x9 board" << endl;
	bool correct_get_size_9x9 = testGetSize(board_9, 9, true, true);
	cout << endl;

	cout << "Testing Board::isOnBoard for 9x9 board" << endl;
	bool correct_is_on_board_9x9 = testIsOnBoardGrid(board_9, 9, true, true);
	cout << endl;

	cout << "Printing a 9x9 board" << endl;
	PrintResult print_result_9x9 = testBoardPrint(board_9, 9);
	bool correct_print_9x9 = isPrintResultAllCorrect(print_result_9x9);
	cout << endl;

	cout << endl;
	cout << endl;


	cout << "Testing boards of all sizes:" << endl;
	cout << "----------------------------" << endl;
	cout << endl;

	cout << "Creating board array: one of each size" << endl;
	Board board_array[BOARD_SIZE_RANGE];
	cout << "* Created array without crashing" << endl;
	for(unsigned int i = 0; i < BOARD_SIZE_RANGE; i++)
		board_array[i] = Board(BOARD_SIZE_MIN + i);
	cout << "* Initialized all boards without crashing" << endl;
	cout << endl;

	cout << "Testing Board::getSize for all board sizes" << endl;
	unsigned int correct_get_size = 0;
	for(unsigned int i = 0; i < BOARD_SIZE_RANGE; i++)
	{
		bool is_print_wrong = correct_get_size == i;
		bool result = testGetSize(board_array[i], BOARD_SIZE_MIN + i, false, is_print_wrong);
		if(result == true)
			correct_get_size++;
	}
	if(correct_get_size == BOARD_SIZE_RANGE)
		cout << "* Correct" << endl;
	cout << endl;

	cout << "Testing Board::isOnBoard for all board sizes" << endl;
	unsigned int correct_is_on_board = 0;
	for(unsigned int i = 0; i < BOARD_SIZE_RANGE; i++)
	{
		bool is_print_wrong = correct_is_on_board == i;
		bool result = testIsOnBoardGrid(board_array[i], BOARD_SIZE_MIN + i, false, is_print_wrong);
		if(result == true)
			correct_is_on_board++;
	}
	if(correct_is_on_board == BOARD_SIZE_RANGE)
		cout << "* Correct" << endl;
	cout << endl;

	cout << endl;
	cout << endl;


	cout << "Printing boards of all sizes:" << endl;
	cout << "-----------------------------" << endl;
	cout << endl;

	unsigned int correct_print_by_size = 0;
	for(unsigned int i = 0; i < BOARD_SIZE_RANGE; i++)
	{
		unsigned int board_size = BOARD_SIZE_MIN + i;
		cout << "Printing " << board_size << "x" << board_size << " board" << endl;
		PrintResult print_result = testBoardPrint(board_array[i], board_size);
		if(isPrintResultAllCorrect(print_result))
			correct_print_by_size++;
		cout << endl;
	}

	cout << endl;
	cout << endl;


	//
	//  Print results 9
	//

	double correct_get_size_fraction      = (double)(correct_get_size     ) / BOARD_SIZE_RANGE;
	double correct_is_on_board_fraction   = (double)(correct_is_on_board  ) / BOARD_SIZE_RANGE;
	double correct_print_by_size_fraction = (double)(correct_print_by_size) / BOARD_SIZE_RANGE;

	TestHelper::printSummaryHeaderPartial(32, 2, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("BOARD_SIZE_MIN",                 correct_board_size_min);
	TestHelper::printSummaryLine("BOARD_SIZE_MAX",                 correct_board_size_max);
	TestHelper::printSummaryLine("BOARD_SIZE_DEFAULT",             correct_board_size_default);
	TestHelper::printSummaryLine("Board declaration",              correct_board_bytes);
	TestHelper::printSummaryLine("StonesRemoved declaration",      correct_removed_bytes);
	TestHelper::printSummaryLine("BOARD_VALUE_ constants",         correct_board_value,      BOARD_VALUE_COUNT_WITH_MARKED);
	cout << endl;
	TestHelper::printSummaryLine("Default constructor",            correct_default_constructor_fraction);
	TestHelper::printSummaryLine("Board::countWithValue",          correct_count_with_value, BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::setAt",                   correct_set_at,           10);
	TestHelper::printSummaryLine("Board::countWithValue",          correct_count_after_set,  BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::load",                    correct_load,             LOAD_BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::print (basic)",           correct_basic_print_fraction);
	TestHelper::printSummaryLine("Board::print (star points)",     correct_star_points_fraction);
	TestHelper::printSummaryLine("Board::print (row headers)",     correct_row_headers_fraction);
	TestHelper::printSummaryLine("Board::print (column heads)",    correct_column_headers_fraction);
	cout << endl;
	TestHelper::printSummaryLine("Board::replaceAll",              correct_replace_all,      6 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::isANeighbourWithValue",   correct_is_neighbour,     6 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::fillConnected",           correct_fill_connected,   6);
	TestHelper::printSummaryLine("Board::calculateScore",          correct_calculate_score,  2 * 2);
	TestHelper::printSummaryLine("Board::playStone",               correct_play_stone,       6 * 4);
	cout << endl;
	TestHelper::printSummaryLine("Copy constructor",               correct_copy_to,          2 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Copied board after constructor", correct_copy_from,        2 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Copied board after destroy",     correct_copy_delete,      2 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Assignment operator",            correct_assign_to,        2 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Copied board after assignment",  correct_assign_from,      9 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Self-assignment",                correct_self_assign,      1 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Chained assignment",             correct_chain_assign,     2 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Copied board after destroy",     correct_assign_delete,    2 * BOARD_VALUE_COUNT);
	cout << endl;
	TestHelper::printSummaryLine("Board::getSize   (9x9)",         correct_get_size_9x9);
	TestHelper::printSummaryLine("Board::isOnBoard (9x9)",         correct_is_on_board_9x9);
	TestHelper::printSummaryLine("Board::print     (9x9)",         correct_print_9x9);
	TestHelper::printSummaryLine("Board::getSize   (all sizes)",   correct_get_size,         BOARD_SIZE_RANGE);
	TestHelper::printSummaryLine("Board::isOnBoard (all sizes)",   correct_is_on_board,      BOARD_SIZE_RANGE);
	TestHelper::printSummaryLine("Board::print     (all sizes)",   correct_print_by_size,    BOARD_SIZE_RANGE);

	unsigned int highest9 = calculateMark(BOARD_SIZE_COUNT, true, true, BOARD_VALUE_COUNT_WITH_MARKED, // constants and types
	                                      1.0, BOARD_VALUE_COUNT, 10, BOARD_VALUE_COUNT,  // basic board manipulation
	                                      LOAD_BOARD_VALUE_COUNT,  // load
	                                      1.0,           // basic print
	                                      1.0,           // print star points
	                                      1.0, 1.0,      // print indexes
	                                      1.0, 1.0, 1.0, // A3 Part D helper functions
	                                      1.0, 1.0,      // A3 Part D non-helper functions
	                                      1.0, 1.0, 1.0, // copy constructor
	                                      1.0, 1.0, 1.0, // assignment operator (basic)
	                                      1.0, 1.0,      // assignment operator (fancy)
	                                      1.0, 1.0,      // size-related functions
	                                      1.0, 0.0,      // print and load other sizes
	                                      0.0, 0.0, 0.0, // A3 Part D other sizes
	                                      0.0, 0.0,      // equality tests (size)
	                                      false, false,  // equality tests (places)
	                                      false);        // finished
	unsigned int mark9 = calculateMark(correct_board_size_count, correct_board_bytes, correct_removed_bytes, correct_board_value,
	                                   correct_default_constructor_fraction, correct_count_with_value,
	                                   correct_set_at, correct_count_after_set,
	                                   correct_load,
	                                   correct_basic_print_fraction,
	                                   correct_star_points_fraction,
	                                   correct_row_headers_fraction, correct_column_headers_fraction,
	                                   correct_replace_all_fraction, correct_is_neighbour_fraction, correct_fill_connected_fraction,
	                                   correct_calculate_score_fraction, correct_play_stone_fraction,
	                                   correct_copy_to_fraction, correct_copy_from_fraction, correct_copy_delete_fraction,
	                                   correct_assign_to_fraction, correct_assign_from_fraction, correct_assign_delete_fraction,
	                                   correct_self_assign_fraction, correct_chain_assign_fraction,
	                                   correct_get_size_fraction, correct_is_on_board_fraction,
	                                   correct_print_by_size_fraction, 0.0, // load other sizes
	                                   0.0, 0.0, 0.0, // A3 Part D other sizes
	                                   0.0, 0.0,      // equality tests (size)
	                                   false, false,  // equality tests (places)
	                                   false);        // finished
	assert(mark9 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark9, highest9, TOTAL_MARKS);

	cout << endl;
	cout << endl;


	//
	//  Test loading boards
	//

	cout << "Testing loading other-sized boards:" << endl;
	cout << "-----------------------------------" << endl;
	cout << endl;

	cout << "Loading board \"1x1.txt\"" << endl;
	Board board_1x1;
	cout << "* Created without crashing" << endl;
	board_1x1.load("1x1.txt");
	cout << "* Loaded without crashing" << endl;
	unsigned int correct_load_1x1 = testBoardCountWithValue(board_1x1, 1, 0, 0, true);
	if(correct_load_1x1 == BOARD_VALUE_COUNT)
		board_1x1.print();
	cout << endl;

	cout << "Loading board \"24full.txt\"" << endl;
	Board board_24full;
	cout << "* Created without crashing" << endl;
	board_24full.load("24full.txt");
	cout << "* Loaded without crashing" << endl;
	unsigned int correct_load_24full = testBoardCountWithValue(board_24full, 24, 540, 0, true);
	if(correct_load_24full == BOARD_VALUE_COUNT)
		board_24full.print();
	cout << endl;

	unsigned int correct_replace_24     = 0;
	unsigned int correct_score_24_black = 0;
	unsigned int correct_score_24_white = 0;
	unsigned int correct_play_stone_24  = 0;

	if(correct_load_24full < BOARD_VALUE_COUNT)
		cout << "* Skipping ahead: \"24full.txt\" not loaded correctly" << endl;
	else
	{
		cout << "Replacing black with white" << endl;
		Board board_24_replace = board_24full;
		board_24_replace.replaceAll(BOARD_VALUE_BLACK_CORRECT, BOARD_VALUE_WHITE_CORRECT);
		cout << "* Completed without crashing" << endl;
		correct_replace_24 = testBoardCountWithValue(board_24_replace, 24, 0, 540, true);
		cout << endl;

		cout << "Add some black stones" << endl;
		Board board_24_mixed = board_24_replace;
		board_24_mixed.setAt( 1,  1, BOARD_VALUE_BLACK_CORRECT);
		board_24_mixed.setAt( 1, 22, BOARD_VALUE_BLACK_CORRECT);
		board_24_mixed.setAt(22,  1, BOARD_VALUE_BLACK_CORRECT);
		for(int r = 21; r < 24; r++)
			for(int c = 21; c < 24; c++)
				if(r != 22 || c != 22)
					board_24_mixed.setAt(r, c, BOARD_VALUE_BLACK_CORRECT);
		cout << "* Completed without crashing" << endl;
		board_24_mixed.print();
		cout << endl;

		cout << "Calculating score for black: ";
		correct_score_24_black = testCalculateScore(board_24_mixed, 24,
		                                            BOARD_VALUE_BLACK_CORRECT, BOARD_VALUE_WHITE_CORRECT, 12);
		cout << endl;

		cout << "Calculating score for white: ";
		correct_score_24_white = testCalculateScore(board_24_mixed, 24,
		                                            BOARD_VALUE_WHITE_CORRECT, BOARD_VALUE_BLACK_CORRECT, 540);
		cout << endl;

		cout << "Playing suicidally in bottom right" << endl;
		Board board_24_suicide = board_24_mixed;
		StonesRemoved removed_24 = board_24_suicide.playStone(22, 22, BOARD_VALUE_BLACK_CORRECT);
		cout << "* Played white stone at Y22 without crashing" << endl;
		board_24_suicide.print();
		correct_play_stone_24 = testPlayStone(board_24_suicide, removed_24,
		                                      BOARD_VALUE_BLACK_CORRECT, BOARD_VALUE_WHITE_CORRECT,
		                                      3, 549, 9, 0,
		                                      "black", "white");
		cout << endl;
	}

	cout << endl;
	cout << endl;


	//
	//  Print results 10
	//

	unsigned int correct_load_sizes_count = (correct_load_1x1    == BOARD_VALUE_COUNT ? 1 : 0) +
	                                        (correct_load_24full == BOARD_VALUE_COUNT ? 1 : 0);
	unsigned int correct_score_24 = correct_score_24_black +
	                                correct_score_24_white;

	double correct_load_sizes_fraction    = correct_load_sizes_count /  2.0;
	double correct_replace_24_fraction    = correct_replace_24       / (double)(BOARD_VALUE_COUNT);
	double correct_score_24_fraction      = correct_score_24         / (2.0 * 2.0);
	double correct_play_stone_24_fraction = correct_play_stone_24    /  4.0;

	TestHelper::printSummaryHeaderPartial(32, 2, COMPILE_AND_START_MARKS);
	TestHelper::printSummaryLine("BOARD_SIZE_MIN",                 correct_board_size_min);
	TestHelper::printSummaryLine("BOARD_SIZE_MAX",                 correct_board_size_max);
	TestHelper::printSummaryLine("BOARD_SIZE_DEFAULT",             correct_board_size_default);
	TestHelper::printSummaryLine("Board declaration",              correct_board_bytes);
	TestHelper::printSummaryLine("StonesRemoved declaration",      correct_removed_bytes);
	TestHelper::printSummaryLine("BOARD_VALUE_ constants",         correct_board_value,      BOARD_VALUE_COUNT_WITH_MARKED);
	cout << endl;
	TestHelper::printSummaryLine("Default constructor",            correct_default_constructor_fraction);
	TestHelper::printSummaryLine("Board::countWithValue",          correct_count_with_value, BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::setAt",                   correct_set_at,           10);
	TestHelper::printSummaryLine("Board::countWithValue",          correct_count_after_set,  BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::load",                    correct_load,             LOAD_BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::print (basic)",           correct_basic_print_fraction);
	TestHelper::printSummaryLine("Board::print (star points)",     correct_star_points_fraction);
	TestHelper::printSummaryLine("Board::print (row headers)",     correct_row_headers_fraction);
	TestHelper::printSummaryLine("Board::print (column heads)",    correct_column_headers_fraction);
	cout << endl;
	TestHelper::printSummaryLine("Board::replaceAll",              correct_replace_all,      6 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::isANeighbourWithValue",   correct_is_neighbour,     6 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::fillConnected",           correct_fill_connected,   6);
	TestHelper::printSummaryLine("Board::calculateScore",          correct_calculate_score,  2 * 2);
	TestHelper::printSummaryLine("Board::playStone",               correct_play_stone,       6 * 4);
	cout << endl;
	TestHelper::printSummaryLine("Copy constructor",               correct_copy_to,          2 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Copied board after constructor", correct_copy_from,        2 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Copied board after destroy",     correct_copy_delete,      2 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Assignment operator",            correct_assign_to,        2 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Copied board after assignment",  correct_assign_from,      9 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Self-assignment",                correct_self_assign,      1 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Chained assignment",             correct_chain_assign,     2 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Copied board after destroy",     correct_assign_delete,    2 * BOARD_VALUE_COUNT);
	cout << endl;
	TestHelper::printSummaryLine("Board::getSize   (9x9)",         correct_get_size_9x9);
	TestHelper::printSummaryLine("Board::isOnBoard (9x9)",         correct_is_on_board_9x9);
	TestHelper::printSummaryLine("Board::print     (9x9)",         correct_print_9x9);
	TestHelper::printSummaryLine("Board::getSize   (all sizes)",   correct_get_size,         BOARD_SIZE_RANGE);
	TestHelper::printSummaryLine("Board::isOnBoard (all sizes)",   correct_is_on_board,      BOARD_SIZE_RANGE);
	TestHelper::printSummaryLine("Board::print     (all sizes)",   correct_print_by_size,    BOARD_SIZE_RANGE);

	unsigned int highest10 = calculateMark(BOARD_SIZE_COUNT, true, true, BOARD_VALUE_COUNT_WITH_MARKED, // constants and types
	                                       1.0, BOARD_VALUE_COUNT, 10, BOARD_VALUE_COUNT,  // basic board manipulation
	                                       LOAD_BOARD_VALUE_COUNT,  // load
	                                       1.0,           // basic print
	                                       1.0,           // print star points
	                                       1.0, 1.0,      // print indexes
	                                       1.0, 1.0, 1.0, // A3 Part D helper functions
	                                       1.0, 1.0,      // A3 Part D non-helper functions
	                                       1.0, 1.0, 1.0, // copy constructor
	                                       1.0, 1.0, 1.0, // assignment operator (basic)
	                                       1.0, 1.0,      // assignment operator (fancy)
	                                       1.0, 1.0,      // size-related functions
	                                       1.0, 0.0,      // print and load other sizes
	                                       1.0, 1.0, 1.0, // A3 Part D other sizes
	                                       0.0, 0.0,      // equality tests (size)
	                                       false, false,  // equality tests (places)
	                                       false);        // finished
	unsigned int mark10 = calculateMark(correct_board_size_count, correct_board_bytes, correct_removed_bytes, correct_board_value,
	                                    correct_default_constructor_fraction, correct_count_with_value,
	                                    correct_set_at, correct_count_after_set,
	                                    correct_load,
	                                    correct_basic_print_fraction,
	                                    correct_star_points_fraction,
	                                    correct_row_headers_fraction, correct_column_headers_fraction,
	                                    correct_replace_all_fraction, correct_is_neighbour_fraction, correct_fill_connected_fraction,
	                                    correct_calculate_score_fraction, correct_play_stone_fraction,
	                                    correct_copy_to_fraction, correct_copy_from_fraction, correct_copy_delete_fraction,
	                                    correct_assign_to_fraction, correct_assign_from_fraction, correct_assign_delete_fraction,
	                                    correct_self_assign_fraction, correct_chain_assign_fraction,
	                                    correct_get_size_fraction, correct_is_on_board_fraction,
	                                    correct_print_by_size_fraction, 0.0, // load other sizes
	                                    correct_replace_24_fraction, correct_score_24_fraction, correct_play_stone_24_fraction,
	                                    0.0, 0.0,      // equality tests (size)
	                                    false, false,  // equality tests (places)
	                                    false);        // finished
	assert(mark10 <= TOTAL_MARKS);
	TestHelper::printMarkPartial(mark10, highest10, TOTAL_MARKS);

	cout << endl;
	cout << endl;


	//
	//  Test loading boards
	//

	cout << "Testing operator== for different board sizes:" << endl;
	cout << "---------------------------------------------" << endl;
	cout << endl;

	cout << "Creating boards" << endl;
	Board equality_boards[BOARD_EQUALITY_COUNT];
	assert(BOARD_EQUALITY_COUNT == 4);
	equality_boards[0] = Board(BOARD_SIZE_MIN_CORRECT);
	equality_boards[1] = Board(9);
	equality_boards[2] = Board(BOARD_SIZE_DEFAULT_CORRECT);
	equality_boards[3] = Board(BOARD_SIZE_MAX_CORRECT);
	cout << "* Completed without crashing" << endl;
	cout << endl;

	unsigned int correct_equality_self = 0;
	for(unsigned int i = 0; i < BOARD_EQUALITY_COUNT; i++)
	{
		int size = equality_boards[i].getSize();
		cout << "Comparing " << size << "x" << size << " board to itself: ";
		bool is_correct = testEqualityTest(equality_boards[i], equality_boards[i], true);
		if(is_correct)
			correct_equality_self++;
		cout << endl;
	}

	unsigned int correct_equality_size = 0;
	for(unsigned int i = 0; i < BOARD_EQUALITY_COUNT; i++)
	{
		for(unsigned int j = i + 1; j < BOARD_EQUALITY_COUNT; j++)
		{
			int size1 = equality_boards[i].getSize();
			int size2 = equality_boards[j].getSize();
			cout << "Comparing " << size1 << "x" << size1
			     << " board to " << size2 << "x" << size2 << " board: ";
			if(size1 == size2)
				cout << endl << "Skipping: Both boards are the same size" << endl;
			else
			{
				bool is_correct = testEqualityTest(equality_boards[i], equality_boards[j], false);
				if(is_correct)
					correct_equality_size++;
			}
			cout << endl;
		}
	}

	cout << "Testing operator== for same-size boards:" << endl;
	cout << "----------------------------------------" << endl;
	cout << endl;

	cout << "Comparing boards with one place changed: ";
	Board board_9b = board_9;
	board_9b.setAt(2, 4, BOARD_VALUE_BLACK);
	bool correct_equality_different = testEqualityTest(board_9, board_9b, false);
	cout << endl;

	cout << "Comparing boards with place changed back: ";
	Board board_9e = board_9b;
	board_9e.setAt(2, 4, BOARD_VALUE_EMPTY);
	bool correct_equality_same = testEqualityTest(board_9, board_9e, true);
	cout << endl;

	cout << endl;
	cout << endl;


	//
	//  Print results 11
	//

	double correct_equality_self_fraction = (double)(correct_equality_self) / BOARD_EQUALITY_COUNT;
	double correct_equality_size_fraction = (double)(correct_equality_size) / BOARD_INEQUALITY_COUNT;

	TestHelper::printSummaryHeaderFinal(32, 2, DID_NOT_CRASH_RUNNING_MARKS);
	TestHelper::printSummaryLine("BOARD_SIZE_MIN",                 correct_board_size_min);
	TestHelper::printSummaryLine("BOARD_SIZE_MAX",                 correct_board_size_max);
	TestHelper::printSummaryLine("BOARD_SIZE_DEFAULT",             correct_board_size_default);
	TestHelper::printSummaryLine("Board declaration",              correct_board_bytes);
	TestHelper::printSummaryLine("StonesRemoved declaration",      correct_removed_bytes);
	TestHelper::printSummaryLine("BOARD_VALUE_ constants",         correct_board_value,      BOARD_VALUE_COUNT_WITH_MARKED);
	cout << endl;
	TestHelper::printSummaryLine("Default constructor",            correct_default_constructor_fraction);
	TestHelper::printSummaryLine("Board::countWithValue",          correct_count_with_value, BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::setAt",                   correct_set_at,           10);
	TestHelper::printSummaryLine("Board::countWithValue",          correct_count_after_set,  BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::load",                    correct_load,             LOAD_BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::print (basic)",           correct_basic_print_fraction);
	TestHelper::printSummaryLine("Board::print (star points)",     correct_star_points_fraction);
	TestHelper::printSummaryLine("Board::print (row headers)",     correct_row_headers_fraction);
	TestHelper::printSummaryLine("Board::print (column heads)",    correct_column_headers_fraction);
	cout << endl;
	TestHelper::printSummaryLine("Board::replaceAll",              correct_replace_all,      6 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::isANeighbourWithValue",   correct_is_neighbour,     6 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::fillConnected",           correct_fill_connected,   6);
	TestHelper::printSummaryLine("Board::calculateScore",          correct_calculate_score,  2 * 2);
	TestHelper::printSummaryLine("Board::playStone",               correct_play_stone,       6 * 4);
	cout << endl;
	TestHelper::printSummaryLine("Copy constructor",               correct_copy_to,          2 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Copied board after constructor", correct_copy_from,        2 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Copied board after destroy",     correct_copy_delete,      2 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Assignment operator",            correct_assign_to,        2 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Copied board after assignment",  correct_assign_from,      9 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Self-assignment",                correct_self_assign,      1 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Chained assignment",             correct_chain_assign,     2 * BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Copied board after destroy",     correct_assign_delete,    2 * BOARD_VALUE_COUNT);
	cout << endl;
	TestHelper::printSummaryLine("Board::getSize   (9x9)",         correct_get_size_9x9);
	TestHelper::printSummaryLine("Board::isOnBoard (9x9)",         correct_is_on_board_9x9);
	TestHelper::printSummaryLine("Board::print     (9x9)",         correct_print_9x9);
	TestHelper::printSummaryLine("Board::getSize   (all sizes)",   correct_get_size,         BOARD_SIZE_RANGE);
	TestHelper::printSummaryLine("Board::isOnBoard (all sizes)",   correct_is_on_board,      BOARD_SIZE_RANGE);
	TestHelper::printSummaryLine("Board::print     (all sizes)",   correct_print_by_size,    BOARD_SIZE_RANGE);
	TestHelper::printSummaryLine("Board::load      (other sizes)", correct_load_sizes_count, 2);
	TestHelper::printSummaryLine("Board::replaceAll    (24x24)",   correct_replace_24,       BOARD_VALUE_COUNT);
	TestHelper::printSummaryLine("Board::calculateScore(24x24)",   correct_score_24,         2 * 2);
	TestHelper::printSummaryLine("Board::playStone     (24x24)",   correct_play_stone_24,    4);
	cout << endl;
	TestHelper::printSummaryLine("Board::operator== (self)",       correct_equality_self,    BOARD_EQUALITY_COUNT);
	TestHelper::printSummaryLine("Board::operator== (diff. size)", correct_equality_size,    BOARD_INEQUALITY_COUNT);
	TestHelper::printSummaryLine("Board::operator== (different)",  correct_equality_different);
	TestHelper::printSummaryLine("Board::operator== (same)",       correct_equality_same);

	unsigned int mark11 = calculateMark(correct_board_size_count, correct_board_bytes, correct_removed_bytes, correct_board_value,
	                                   correct_default_constructor_fraction, correct_count_with_value,
	                                   correct_set_at, correct_count_after_set,
	                                   correct_load,
	                                   correct_basic_print_fraction,
	                                   correct_star_points_fraction,
	                                   correct_row_headers_fraction, correct_column_headers_fraction,
	                                   correct_replace_all_fraction, correct_is_neighbour_fraction, correct_fill_connected_fraction,
	                                   correct_calculate_score_fraction, correct_play_stone_fraction,
	                                   correct_copy_to_fraction, correct_copy_from_fraction, correct_copy_delete_fraction,
	                                   correct_assign_to_fraction, correct_assign_from_fraction, correct_assign_delete_fraction,
	                                   correct_self_assign_fraction, correct_chain_assign_fraction,
	                                   correct_get_size_fraction, correct_is_on_board_fraction,
	                                   correct_print_by_size_fraction, correct_load_sizes_fraction,
	                                   correct_replace_24_fraction, correct_score_24_fraction, correct_play_stone_24_fraction,
	                                   correct_equality_self_fraction, correct_equality_size_fraction,
	                                   correct_equality_different, correct_equality_same,
	                                   true);  // finished
	assert(mark11 <= TOTAL_MARKS);
	TestHelper::printMark(mark11, TOTAL_MARKS);

	TestHelper::waitForEnter();
	return 0;
}



int testBoardCountWithValue (const Board& board,
                             int correct_size,
                             int correct_black,
                             int correct_white,
                             bool is_print_correct)
{
	assert(correct_black <= correct_size * correct_size);
	assert(correct_white <= correct_size * correct_size);

	int cell_count = correct_size * correct_size;

	assert(correct_black + correct_white <= cell_count);
	int correct_empty = cell_count - correct_black - correct_white;
	assert(correct_empty >= 0);
	assert(correct_empty <= cell_count);

	int empty_count = board.countWithValue(BOARD_VALUE_EMPTY_CORRECT);
	int black_count = board.countWithValue(BOARD_VALUE_BLACK_CORRECT);
	int white_count = board.countWithValue(BOARD_VALUE_WHITE_CORRECT);

	unsigned int result = 0;

	if(empty_count == correct_empty)
		result++;
	else
	{
		cout << "* Incorrect: Counted " << empty_count << " BOARD_VALUE_EMPTY ('"
		     << BOARD_VALUE_EMPTY_CORRECT << "'), should be " << correct_empty << endl;
	}

	if(black_count == correct_black)
		result++;
	else
	{
		cout << "* Incorrect: Counted " << black_count << " BOARD_VALUE_BLACK ('"
		     << BOARD_VALUE_BLACK_CORRECT << "'), should be " << correct_black << endl;
	}

	if(white_count == correct_white)
		result++;
	else
	{
		cout << "* Incorrect: Counted " << white_count << " BOARD_VALUE_WHITE ('"
		     << BOARD_VALUE_WHITE_CORRECT << "'), should be " << correct_white << endl;
	}

	if(result == BOARD_VALUE_COUNT)
		if(is_print_correct)
			cout << "* Correct" << endl;
	return result;
}

bool testBoardSetAt (Board& board,
                     int row, int column,
                     char value, const string& value_name,
                     bool is_print_test_and_correct)
{
	assert(row    < BOARD_SIZE_DEFAULT_CORRECT);
	assert(column < BOARD_SIZE_DEFAULT_CORRECT);
	assert(value_name != "");

	if(is_print_test_and_correct)
	{
		cout << "Setting place " << (char)(column + 'A') << row
		     << " (row " << row << ", column " << column << ") to "
		     << value_name << " ('" << value << "')" << endl;
	}

	board.setAt(row, column, value);
	if(is_print_test_and_correct)
		cout << "* Completed without crashing" << endl;

	char returned2 = board.getAt(row, column);
	if(returned2 == value)
	{
		if(is_print_test_and_correct)
			cout << "* Correct" << endl;
		return true;
	}
	else
	{
		cout << "* Incorrect: Value at " << (char)(column + 'A') << row
		     << " (row " << row << ", column " << column << ") is now '"
		     << returned2 << "', should be " << value_name << " ('"
		     << value << "')" << endl;
		return false;
	}
}



PrintResult createZeroPrintResult ()
{
	PrintResult result;
	result.basic_print     = 0;
	result.star_points     = 0;
	result.star_points_max = 0;
	result.row_headers     = 0;
	result.column_headers  = 0;
	return result;
}

bool isPrintResultAllCorrect (const PrintResult& result)
{
	if(result.basic_print    == BASIC_PRINT_CORRECT_MAX &&
	   result.star_points    == result.star_points_max &&
	   result.row_headers    == ROW_HEADING_CORRECT_MAX &&
	   result.column_headers == COLUMN_HEADING_CORRECT_MAX)
	{
		return true;
	}
	else
		return false;
}

PrintResult testBoardPrint (const Board& board,
                            int board_size)
{
	static const unsigned int ROW_BORDER    = 1;
	static const unsigned int COLUMN_BORDER = 3;

	unsigned int height_no_borders = board_size;
	unsigned int  width_no_borders = board_size * 2 - 1;

	unsigned int height_correct    = height_no_borders + 2 * ROW_BORDER;
	unsigned int width_correct_max =  width_no_borders + 2 * COLUMN_BORDER;
	unsigned int width_correct_min = width_correct_max - 1;
	// width can vary if numbers are printed left-aligned

	// Step 1: Capture board printing

	TestHelper::printCaptureBlockLine();
	TestHelper::startCapture();
	board.print();
	string captured = TestHelper::endCapture(true);
	TestHelper::printCaptureBlockLine();
	vector<string> captured_lines = TestHelper::splitWithoutEmpty(captured, '\n');

	PrintResult result = createZeroPrintResult();
	if(captured_lines.empty())
	{
		cout << "* Incorrect: Nothing printed" << endl;
		return result;
	}
	result.basic_print++;

	// Step 2: Check output height

	assert(height_no_borders < height_correct);
	unsigned int crop_line_start = 0;
	unsigned int crop_line_end   = captured_lines.size();
	if(captured_lines.size() == height_no_borders)
	{
		assert(crop_line_start <= crop_line_end);
		assert(crop_line_end - crop_line_start == board_size);

		result.basic_print++;
	}
	else if(captured_lines.size() == height_correct)
	{
		crop_line_start++;
		crop_line_end  --;
		assert(crop_line_start <= crop_line_end);
		assert(crop_line_end - crop_line_start == board_size);

		result.basic_print++;
	}
	else
	{
		cout << "* Incorrect: Should print " << height_correct << " lines" << endl;
		return result;
	}

	// Step 3: Trim spaces at end of lines

	for(unsigned int r = 0; r < captured_lines.size(); r++)
	{
		captured_lines[r] = trimRight(captured_lines[r]);
	}

	// Step 4: Check output width
	//  -> more complex because lines might be different

	assert(crop_line_start < captured_lines.size());
	unsigned int shortest_line_width = captured_lines[crop_line_start].length();
	unsigned int  longest_line_width = shortest_line_width;
	unsigned int shortest_line_index = 0;
	unsigned int  longest_line_index = 0;

	for(unsigned int r = crop_line_start; r < crop_line_end; r++)
	{
		unsigned int line_width = captured_lines[r].length();
		if(line_width < shortest_line_width)
		{
			shortest_line_width = line_width;
			shortest_line_index = r;
		}
		if(line_width > longest_line_width)
		{
			longest_line_width = line_width;
			longest_line_index = r;
		}
	}

	if(shortest_line_width == board_size &&
	    longest_line_width == board_size)
	{
		cout << "* Incorrect: All lines " << longest_line_index << " have "
		     << longest_line_width << " / " << width_no_borders << " characters"
		     << " - missing spaces?" << endl;
	}
	else if(longest_line_width < width_no_borders)
	{
		cout << "* Incorrect: Longest line " << longest_line_index << " has only "
		     << longest_line_width << " / " << width_correct_min << " characters" << endl;
	}
	else if(shortest_line_width < width_no_borders)
	{
		cout << "* Incorrect: Shortest line " << shortest_line_index << " has only "
		     << shortest_line_width << " / " << width_correct_min << " characters" << endl;
	}
	else if(shortest_line_width > width_correct_max)
	{
		cout << "* Incorrect: Shortest line " << shortest_line_index << " has "
		     << shortest_line_width << " / " << width_correct_max << " characters" << endl;
	}
	else if(longest_line_width > width_correct_max)
	{
		cout << "* Incorrect: Longest line " << longest_line_index << " has "
		     << longest_line_width << " / " << width_correct_max << " characters" << endl;
	}
	else if(shortest_line_width == width_no_borders &&
	         longest_line_width == width_no_borders)
	{
		result.basic_print++;

		if(captured_lines.size() == height_no_borders)
			checkPrintedCropped(board, board_size, captured_lines, 0, 0, result);
		else
		{
			cout << "* Incorrect: Headings printed for columns but not rows" << endl;
			assert(captured_lines.size() == height_correct);
			checkPrintedCropped(board, board_size, captured_lines, ROW_BORDER, 0, result);
			checkPrintedColumnHeadings(board_size, captured_lines, 0, result);
		}
	}
	else if(shortest_line_width >= width_correct_min &&
	         longest_line_width <= width_correct_max)
	{
		result.basic_print++;

		if(captured_lines.size() == height_no_borders)
		{
			cout << "* Incorrect: Headings printed for rows but not columns" << endl;
			checkPrintedCropped(board, board_size, captured_lines, 0, COLUMN_BORDER, result);
			checkPrintedRowHeadings(board_size, captured_lines, 0, result);
		}
		else
		{
			assert(captured_lines.size() == height_correct);
			checkPrintedCropped(board, board_size, captured_lines, ROW_BORDER, COLUMN_BORDER, result);
			checkPrintedRowHeadings   (board_size, captured_lines, ROW_BORDER, result);
			checkPrintedColumnHeadings(board_size, captured_lines, COLUMN_BORDER, result);
		}
	}
	else
	{
		cout << "* Incorrect: All lines should have " << width_correct_max << " characters" << endl;
	}

	if(isPrintResultAllCorrect(result))
		cout << "* Correct" << endl;
	return result;
}

string trimRight (const string& str)
{
	if(str.empty())
		return "";

	string::size_type end;
	for(end = str.size(); end > 0 && isspace(str[end - 1]); end--)
		;  // do nothing

	return str.substr(0, end);
}

void checkPrintedCropped (const Board& board,
                          unsigned int board_size,
                          const vector<string>& captured_lines,
                          unsigned int row_offset,
                          unsigned int column_offset,
                          PrintResult& result)
{
	assert(!captured_lines.empty());
	assert(captured_lines.size() >= row_offset);
	assert(captured_lines.size() + row_offset >= board_size);
	for(unsigned int r = 0; r < board_size; r++)
	{
		assert(r + row_offset < captured_lines.size());
		assert(captured_lines[r + row_offset].length() >= column_offset);
		assert(captured_lines[r + row_offset].length() - column_offset >= board_size * 2 - 1);
	}

	unsigned int width_no_borders = board_size * 2 - 1;

	//  Step 1: Crop captured output

	vector<string> cropped(board_size, "");
	for(unsigned int r = 0; r < board_size; r++)
	{
		assert(r < cropped.size());
		assert(r + row_offset < captured_lines.size());
		assert(captured_lines[r + row_offset].length() >= column_offset);
		assert(captured_lines[r + row_offset].length() - column_offset >= width_no_borders);
		cropped[r] = captured_lines[r + row_offset].substr(column_offset, width_no_borders);
	}

	//  Step 2: Check if the spaces are in the correct places

	for(unsigned int r = 0; r < board_size; r++)
	{
		for(unsigned int c = 0; c < width_no_borders; c++)
		{
			bool is_space        = cropped[r][c] == ' ';
			bool should_be_space = c % 2 != 0;
			if(is_space && !should_be_space)
			{
				cout << "* Incorrect: Should not be a space at row " << r << " column " << (c / 2) << endl;
				return;  // end function immediately
			}
		}
	}
	result.basic_print++;

	for(unsigned int r = 0; r < board_size; r++)
	{
		for(unsigned int c = 0; c < width_no_borders; c++)
		{
			bool is_space        = cropped[r][c] == ' ';
			bool should_be_space = c % 2 != 0;
			if(!is_space && should_be_space)
			{
				cout << "* Incorrect: After row " << r << " column " << (c / 2) << " should be a space" << endl;
				return;  // end function immediately
			}
		}
	}
	result.basic_print++;

	//  Step 3: Check if non-space characters are valid

	for(unsigned int r = 0; r < board_size; r++)
	{
		for(unsigned int c = 0; c < width_no_borders; c += 2)
		{
			// ignore star points for now
			char here_value = cropped[r][c];
			if(here_value == BOARD_VALUE_STAR_POINT_CORRECT)
				here_value = BOARD_VALUE_EMPTY_CORRECT;

			switch(here_value)
			{
			case BOARD_VALUE_EMPTY_CORRECT:
			case BOARD_VALUE_BLACK_CORRECT:
			case BOARD_VALUE_WHITE_CORRECT:
				// do nothing
				break;
			default:
				if(isCorrectStarPoint(board_size, r, c / 2))
				{
					cout << "* Incorrect: Row " << r << " column " << (c / 2)
					     << " is bad character '" << here_value << "'" << endl;
					cout << "             Star points should be '*'" << endl;
				}
				else
				{
					cout << "* Incorrect: Row " << r << " column " << (c / 2)
					     << " is bad character '" << here_value << "'" << endl;
				}
				return;  // end function immediately
			}
		}
	}
	result.basic_print += 2;

	//  Step 4: Check if non-space characters are correct

	for(unsigned int r = 0; r < board_size; r++)
	{
		for(unsigned int c = 0; c < width_no_borders; c += 2)
		{
			char correct_value = board.getAt(r, c / 2);

			// ignore star points for now
			char here_value = cropped[r][c];
			if(here_value == BOARD_VALUE_STAR_POINT_CORRECT)
				here_value = BOARD_VALUE_EMPTY_CORRECT;

			if(here_value != correct_value)
			{
				cout << "* Incorrect: Row " << r << " column " << (c / 2) << " is '"
				     << here_value << "', should be '" << correct_value << "'" << endl;
				return;  // end function immediately
			}
		}
	}
	result.basic_print += 3;

	//  Step 5: Check star points

	checkPrintedCropped(board, board_size, cropped, result);
}

void checkPrintedCropped (const Board& board,
                          unsigned int board_size,
                          const vector<string>& cropped,
                          PrintResult& result)
{
	assert(!cropped.empty());
	assert(cropped.size() == board_size);
	for(unsigned int r = 0; r < board_size; r++)
	{
		assert(cropped[r].length() == board_size * 2 - 1);
	}

	unsigned int width_no_borders = board_size * 2 - 1;

	//  Step 1: Stop if all star points are missing

	bool is_any_printed = false;
	for(unsigned int r = 0; r < board_size; r++)
	{
		for(unsigned int c = 0; c < width_no_borders; c += 2)
		{
			if(isCorrectStarPoint(board_size, r, c / 2))
				if(board.getAt(r, c / 2) == BOARD_VALUE_EMPTY_CORRECT)
					result.star_points_max++;

			if(cropped[r][c] == BOARD_VALUE_STAR_POINT_CORRECT)
				is_any_printed = true;
		}
	}
	if(result.star_points_max > 0 && !is_any_printed)
	{
		cout << "* Incorrect: Star points not printed" << endl;
		return;  // no marks for star points
	}

	//  Step 2: Check star points

	int missed_count = 0;
	int extra_count  = 0;
	for(unsigned int r = 0; r < board_size; r++)
	{
		for(unsigned int c = 0; c < width_no_borders; c += 2)
		{
			if(isCorrectStarPoint(board_size, r, c / 2))
			{
				if(board.getAt(r, c / 2) == BOARD_VALUE_EMPTY_CORRECT)
				{
					if(cropped[r][c] != BOARD_VALUE_STAR_POINT_CORRECT)
					{
						if(missed_count == 0)
						{
							cout << "* Incorrect: Star point at row " << r << " column " << (c / 2)
							     << " is missing" << endl;
						}
						missed_count++;
					}
				}
			}
			else
			{
				if(cropped[r][c] == BOARD_VALUE_STAR_POINT_CORRECT)
				{
					if(extra_count == 0)
					{
						cout << "* Incorrect: Row " << r << " column " << (c / 2)
						     << " should not be a star point" << endl;
					}
					extra_count++;
				}
			}
		}
	}

	// star points are right minus wrong
	int wrong_count = missed_count;
	if(wrong_count < extra_count)
		wrong_count = extra_count;
	if(wrong_count < (int)(result.star_points_max))
		result.star_points = result.star_points_max - wrong_count;
	else
		result.star_points = 0;

	//  Step 3: Messages for additional wrong star points

	int misplaced_count = missed_count;
	if(misplaced_count > extra_count)
		misplaced_count = extra_count;
	if(misplaced_count > 1)
	{
		cout << "* Incorrect: " << (misplaced_count - 1) << " more star points misplaced" << endl;
		assert(misplaced_count <= missed_count);
		missed_count -= misplaced_count;
		assert(misplaced_count <= extra_count);
		extra_count -= misplaced_count;
	}
	if(extra_count > 1)
		cout << "* Incorrect: " << (extra_count - 1) << " more extra star points printed" << endl;
	if(missed_count > 1)
		cout << "* Incorrect: " << (missed_count - 1) << " more star points missing" << endl;
}

bool isCorrectStarPoint (unsigned int board_size,
                         int row, int column)
{
	static const int STAR_POINT_SPACING_CORRECT = 6;

	int center = board_size / 2;

	vector<int> star_point_indexes;
	star_point_indexes.push_back(center);
	for(int i = center - STAR_POINT_SPACING_CORRECT; i >= 0; i -= STAR_POINT_SPACING_CORRECT)
		star_point_indexes.push_back(i);
	for(int i = center + STAR_POINT_SPACING_CORRECT; i < (int)(board_size); i += STAR_POINT_SPACING_CORRECT)
		star_point_indexes.push_back(i);

	bool is_star_row = false;
	for(unsigned int i = 0; i < star_point_indexes.size(); i++)
		if(row == star_point_indexes[i])
			is_star_row = true;
	if(!is_star_row)
		return false;

	bool is_star_column = false;
	for(unsigned int i = 0; i < star_point_indexes.size(); i++)
		if(column == star_point_indexes[i])
			is_star_column = true;
	if(!is_star_column)
		return false;

	return true;
}

void checkPrintedRowHeadings (unsigned int board_size,
                              const vector<string>& captured_lines,
                              unsigned int row_offset,
                              PrintResult& result)
{
	static const unsigned int NUMBER_WIDTH = 2;
	static const unsigned int  LEFT_SPACE  = NUMBER_WIDTH;

	unsigned int right_space = LEFT_SPACE + board_size * 2;

	assert(!captured_lines.empty());
	assert(captured_lines.size() >= row_offset);
	assert(captured_lines.size() + row_offset >= board_size);
	for(unsigned int r = 0; r < board_size; r++)
	{
		assert(r + row_offset < captured_lines.size());
		assert(captured_lines[r + row_offset].length() > right_space);
	}

	//  Step 1: Check if there are spoaces between indexes and board

	for(unsigned int r = 0; r < board_size; r++)
	{
		assert(r + row_offset < captured_lines.size());
		assert(captured_lines[r + row_offset].length() >= LEFT_SPACE);
		if(captured_lines[r + row_offset][LEFT_SPACE] != ' ')
		{
			cout << "* Incorrect: Should be a space after row " << r << " left index" << endl;
			return;  // end function immediately
		}
	}
	result.row_headers++;

	for(unsigned int r = 0; r < board_size; r++)
	{
		assert(r + row_offset < captured_lines.size());
		assert(captured_lines[r + row_offset].length() >= right_space);
		if(captured_lines[r + row_offset][right_space] != ' ')
		{
			cout << "* Incorrect: Should be a space before row " << r << " right index" << endl;
			return;  // end function immediately
		}
	}
	result.row_headers++;

	//  Step 2: Crop out row indexes

	vector<string> left (board_size, "");
	vector<string> right(board_size, "");
	for(unsigned int r = 0; r < board_size; r++)
	{
		assert(r < left .size());
		assert(r < right.size());
		assert(r + row_offset < captured_lines.size());
		const string& line = captured_lines[r + row_offset];
		assert(line.length() > right_space);
		left [r] = TestHelper::trim(line.substr(0,               NUMBER_WIDTH));
		right[r] = TestHelper::trim(line.substr(right_space + 1, NUMBER_WIDTH));
	}

	//  Step 3: Check if row indexes are present

	for(unsigned int r = 0; r < board_size; r++)
	{
		assert(r < left.size());
		if(left[r] == "")
		{
			cout << "* Incorrect: No left index for row " << r << endl;
			return;  // end function immediately
		}
	}
	result.row_headers++;

	for(unsigned int r = 0; r < board_size; r++)
	{
		assert(r < right.size());
		if(right[r] == "")
		{
			cout << "* Incorrect: No right index for row " << r << endl;
			return;  // end function immediately
		}
	}
	result.row_headers++;

	//  Step 4: Check if row indexes are numbers

	for(unsigned int r = 0; r < board_size; r++)
	{
		assert(r < left.size());
		if(!isAllDigits(left[r]))
		{
			cout << "* Incorrect: left index for row " << r << " should be a number" << endl;
			return;  // end function immediately
		}
	}
	result.row_headers++;

	for(unsigned int r = 0; r < board_size; r++)
	{
		assert(r < right.size());
		if(!isAllDigits(right[r]))
		{
			cout << "* Incorrect: No right index for row " << r << " should be a number" << endl;
			return;  // end function immediately
		}
	}
	result.row_headers++;

	//  Step 5: Check if row indexes are correct

	for(unsigned int r = 0; r < board_size; r++)
	{
		string correct = to_string(r);
		assert(r < left.size());
		if(left[r] != correct)
		{
			cout << "* Incorrect: Left index for row " << r << " is \"" << left[r]
			     << "\", but should be \"" << correct << "\"" << endl;
			return;  // end function immediately
		}
	}
	result.row_headers++;

	for(unsigned int r = 0; r < board_size; r++)
	{
		string correct = to_string(r);
		assert(r < right.size());
		if(right[r] != correct)
		{
			cout << "* Incorrect: Right index for row " << r << " is \"" << right[r]
			     << "\", but should be \"" << correct << "\"" << endl;
			return;  // end function immediately
		}
	}
	result.row_headers++;
}

bool isAllDigits (const std::string& str)
{
	for(unsigned int i = 0; i < str.length(); i++)
		if(!isdigit(str[i]))
			return false;
	return true;
}

void checkPrintedColumnHeadings (unsigned int board_size,
                                 const vector<string>& captured_lines,
                                 unsigned int column_offset,
                                 PrintResult& result)
{
	assert(!captured_lines.empty());
	assert(captured_lines.front().length() >= column_offset);
	assert(captured_lines.back ().length() >= column_offset);

	static const string CORRECT_LETTERS_ALL = "ABCDEFGHJKLMNPQRSTUVWXYZ";
	unsigned int correct_width   = board_size * 2 - 1;
	string       correct_letters = CORRECT_LETTERS_ALL.substr(0, board_size);
	assert(correct_letters.length() == board_size);

	//  Step 1: Check if the right amount is printed

	unsigned int top_width = correct_width;
	if(top_width > captured_lines.front().length() - column_offset)
		top_width = captured_lines.front().length() - column_offset;
	string top = captured_lines.front().substr(column_offset, top_width);
	assert(top.length() == top_width);

	unsigned int bottom_width = correct_width;
	if(bottom_width > captured_lines.back().length() - column_offset)
		bottom_width = captured_lines.back().length() - column_offset;
	string bottom = captured_lines.front().substr(column_offset, top_width);
	assert(bottom.length() == bottom_width);

	if(top_width < correct_width)
	{
		cout
			<< "* Incorrect: Top headings have only " << top_width
			<< " / " << correct_width << " characters" << endl;
		return;  // end function immediately
	}
	result.column_headers++;

	if(bottom_width < correct_width)
	{
		cout
			<< "* Incorrect: Bottom headings have only " << top_width
			<< " / " << correct_width << " characters" << endl;
		return;  // end function immediately
	}
	result.column_headers++;

	//  Step 2: Check if the spaces are in the correct places

	for(unsigned int c = 0; c < top.length(); c++)
	{
		bool is_space        = top[c] == ' ';
		bool should_be_space = c % 2 != 0;
		if(is_space != should_be_space)
		{
			if(is_space)
				cout << "* Incorrect: Should not be a space above column " << (c / 2) << endl;
			else
				cout << "* Incorrect: After above column " << (c / 2) << " should be a space" << endl;
			return;  // end function immediately
		}
	}
	result.column_headers++;

	for(unsigned int c = 0; c < bottom.length(); c++)
	{
		bool is_space        = bottom[c] == ' ';
		bool should_be_space = c % 2 != 0;
		if(is_space != should_be_space)
		{
			if(is_space)
				cout << "* Incorrect: Should not be a space below column " << (c / 2) << endl;
			else
				cout << "* Incorrect: After below column " << (c / 2) << " should be a space" << endl;
			return;  // end function immediately
		}
	}
	result.column_headers++;

	//  Step 3: Check if the column headings are uppercase letters

	for(unsigned int c = 0; c < top.length(); c += 2)
	{
		if(!isupper(top[c]))
		{
			if(isalpha(top[c]))
				cout << "* Incorrect: Above column " << (c / 2) << " should be a uppercase" << endl;
			else
				cout << "* Incorrect: Above column " << (c / 2) << " should be a letter" << endl;
			return;  // end function immediately
		}
	}
	result.column_headers++;

	for(unsigned int c = 0; c < bottom.length(); c += 2)
	{
		if(!isupper(bottom[c]))
		{
			if(isalpha(bottom[c]))
				cout << "* Incorrect: Below column " << (c / 2) << " should be a uppercase" << endl;
			else
				cout << "* Incorrect: Below column " << (c / 2) << " should be a letter" << endl;
			return;  // end function immediately
		}
	}
	result.column_headers++;

	//  Step 4: Check if the column headings are correct letters

	for(unsigned int c = 0; c < top.length(); c += 2)
	{
		assert(c / 2 < correct_letters.length());
		char correct_letter = correct_letters[c / 2];
		if(top[c] != correct_letter)
		{
			cout << "* Incorrect: Above column " << (c / 2) << " is '"
			     << top[c] << "', should be '" << correct_letter << "'" << endl;
			return;  // end function immediately
		}
	}
	result.column_headers++;

	for(unsigned int c = 0; c < bottom.length(); c += 2)
	{
		assert(c / 2 < correct_letters.length());
		char correct_letter = correct_letters[c / 2];
		if(bottom[c] != correct_letter)
		{
			cout << "* Incorrect: Below column " << (c / 2) << " is '"
			     << bottom[c] << "', should be '" << correct_letter << "'" << endl;
			return;  // end function immediately
		}
	}
	result.column_headers++;
}



int testIsANeighbourWithValue (const Board& board,
                               int row,
                               int column,
                               bool correct_black,
                               bool correct_white,
                               bool correct_empty)
{
	bool neighbour_black = board.isANeighbourWithValue(row, column, BOARD_VALUE_BLACK_CORRECT);
	bool neighbour_white = board.isANeighbourWithValue(row, column, BOARD_VALUE_WHITE_CORRECT);
	bool neighbour_empty = board.isANeighbourWithValue(row, column, BOARD_VALUE_EMPTY_CORRECT);

	unsigned int result = 0;

	if(neighbour_black == correct_black)
		result++;
	else if(neighbour_black == true)
		cout << "* Incorrect: Reported a black neighbour but shouldn't" << endl;
	else
		cout << "* Incorrect: Should have reported a black neighbour" << endl;

	if(neighbour_white == correct_white)
		result++;
	else if(neighbour_white == true)
		cout << "* Incorrect: Reported a white neighbour but shouldn't" << endl;
	else
		cout << "* Incorrect: Should have reported a white neighbour" << endl;

	if(neighbour_empty == correct_empty)
		result++;
	else if(neighbour_empty == true)
		cout << "* Incorrect: Reported an empty neighbour but shouldn't" << endl;
	else
		cout << "* Incorrect: Should have reported an empty neighbour" << endl;

	if(result == BOARD_VALUE_COUNT)
		cout << "* Correct" << endl;
	return result;
}

bool testFillConnected (const Board& board,
                        unsigned int board_size,
                        int correct_marked,
                        char value_before,
                        int count_before)
{
	int count = board.countWithValue(BOARD_VALUE_MARKED_CORRECT);
	if(count == correct_marked)
	{
		cout << "* Correct" << endl;
		return true;
	}
	
	if(count == 0)
		cout << "* Incorrect: No board places were filled" << endl;
	else if(count == board_size * board_size)
		cout << "* Incorrect: All board places were filled" << endl;
	else if(count == count_before)
		cout << "* Incorrect: All '" << value_before << "' places were filled" << endl;
	else
		cout << "* Incorrect: Filled " << count << " board places, should be " << correct_marked << endl;
	return false;
}

unsigned int testCalculateScore (const Board& board,
                                 unsigned int board_size,
                                 char us_value,
                                 char them_value,
                                 int correct_score)
{
	assert(isBoardValuePlayer(us_value));
	assert(isBoardValuePlayer(them_value));

	int score = board.calculateScore(us_value);
	cout << score << endl;

	if(score == correct_score)
	{
		cout << "* Correct" << endl;
		return 2;
	}

	int stone_count = board.countWithValue(us_value);
	if(score == stone_count)
	{
		cout << "* Incorrect: Did not include controlled territory" << endl;
		return 1;
	}
	
	if(score == 0)
		cout << "* Incorrect: No board places were counted" << endl;
	else if(score == board_size * board_size)
		cout << "* Incorrect: All board places were counted" << endl;
	else
		cout << "* Incorrect: Should be " << correct_score << endl;
	return 0;
}

unsigned int testPlayStone (const Board& board,
                            const StonesRemoved& removed,
                            char us_value,
                            char them_value,
                            int correct_score_us,
                            int correct_score_them,
                            int correct_captured_us,
                            int correct_captured_them,
                            const string& name_us,
                            const string& name_them)
{
	assert(isBoardValuePlayer(us_value));
	assert(isBoardValuePlayer(them_value));

	unsigned int result = 0;

	string them_stones_word = (removed.them == 1) ? " stone" : " stones";
	string   us_stones_word = (removed.us   == 1) ? " stone" : " stones";
	cout << "Captured " << removed.them << them_stones_word
	     << ", lost "     << removed.us   <<   us_stones_word << " to suicide" << endl;

	int score_us   = board.calculateScore(  us_value);
	int score_them = board.calculateScore(them_value);
	if(us_value == BOARD_VALUE_BLACK_CORRECT)
		cout << "Score is " << score_us   << " (black) to " << score_them << " (white)" << endl;
	else
		cout << "Score is " << score_them << " (black) to " << score_us   << " (white)" << endl;

	if(removed.them == correct_captured_them)
		result++;
	else
		cout << "* Incorrect: Should have capture " << correct_captured_them << endl;
	if(removed.us == correct_captured_us)
		result++;
	else
		cout << "* Incorrect: Should have lost " << correct_captured_us << " to suicide" << endl;

	if(score_us == correct_score_us)
		result++;
	else
	{
		cout << "* Incorrect: Score for " << name_us << " is " << score_us
		     << ", should be " << correct_score_us << endl;
	}
	if(score_them == correct_score_them)
		result++;
	else
	{
		cout << "* Incorrect: Score for " << name_them << " is " << score_them
		     << ", should be " << correct_score_them << endl;
	}
	if(score_us != score_them &&
	   score_us   == correct_score_them &&
	   score_them == correct_score_us)
	{
		result++;
	}

	if(result == 4)
		cout << "* Correct" << endl;
	return result;
}



bool testGetSize (const Board& board,
                  int correct_size,
                  bool is_print_correct,
                  bool is_print_incorrect)
{
	int result = board.getSize();

	if(result == correct_size)
	{
		if(is_print_correct)
			cout << "* Correct" << endl;
		return true;
	}
	else
	{
		if(is_print_incorrect)
			cout << "* Incorrect: Should have size " << correct_size << endl;
		return false;
	}
}

bool testIsOnBoardGrid (const Board& board,
                        int correct_size,
                        bool is_print_correct,
                        bool is_print_incorrect)
{
	static const int EDGE_PADDING = 5;

	unsigned int grid_yes_trials  = 0;
	unsigned int grid_no_trials   = 0;
	unsigned int grid_yes_correct = 0;
	unsigned int grid_no_correct  = 0;
	bool grid_all_correct = true;

	for(int r = -EDGE_PADDING; r <= correct_size + EDGE_PADDING; r++)
	{
		for(int c = -EDGE_PADDING; c <= correct_size + EDGE_PADDING; c++)
		{
			bool is_valid = r >= 0 && r < correct_size &&
			                c >= 0 && c < correct_size;
			if(is_valid)
				grid_yes_trials++;
			else
				grid_no_trials++;

			bool is_correct = testIsOnBoard(board, r, c, is_valid, correct_size, grid_all_correct);
			if(is_correct)
			{
				if(is_valid)
					grid_yes_correct++;
				else
					grid_no_correct++;
			}
			else if(grid_all_correct)
			{
				if(is_print_incorrect)
					cout << "             For row = " << r << ", column = " << c << endl;
				grid_all_correct = false;
			}
		}
	}

	if(grid_all_correct && is_print_correct)
		cout << "* Correct" << endl;
	else if(is_print_incorrect)
	{
		unsigned int   trial_count = grid_yes_trials  + grid_no_trials;
		unsigned int correct_count = grid_yes_correct + grid_no_correct;
		assert(correct_count <= trial_count);
		unsigned int incorrect_count = trial_count - correct_count;
		if(incorrect_count > 1)
			cout << "* " << incorrect_count << " more incorrect" << endl;
	}
	return grid_all_correct;
}

bool testIsOnBoard (const Board& board,
                    int row,
                    int column,
                    bool correct_validity,
                    int correct_size,
                    bool is_print_incorrect)
{
	bool reported_validity = board.isOnBoard(row, column);

	if(reported_validity == correct_validity)
		return true;

	if(reported_validity == false && correct_validity == true)
	{
		if(is_print_incorrect)
			cout << "* Incorrect: Should be valid" << endl;
		return false;
	}

	assert(reported_validity == true);
	assert(correct_validity == false);
	if(is_print_incorrect)
	{
		if(row < 0)
			cout << "* Incorrect: Should be invalid if row is negative" << endl;
		else if(row == correct_size)
			cout << "* Incorrect: Should be invalid if row == " << correct_size << endl;
		else if(row > correct_size)
			cout << "* Incorrect: Should be invalid if row > " << correct_size << endl;
		else if(column < 0)
			cout << "* Incorrect: Should be invalid if column is negative" << endl;
		else if(column == correct_size)
			cout << "* Incorrect: Should be invalid if column == " << correct_size << endl;
		else if(column > correct_size)
			cout << "* Incorrect: Should be invalid if column > " << correct_size << endl;
		else
			cout << "* Incorrect: But why?  This may be a test program error" << endl;
	}
	return false;
}

bool testEqualityTest (const Board& board1,
                       const Board& board2,
                       bool is_equal_correct)
{
	bool is_equal1 = board1 == board2;
	bool is_equal2 = board2 == board1;

	if(is_equal1 && is_equal2)
		cout << "Equal" << endl;
	else if(!is_equal1 && !is_equal2)
		cout << "Not equal" << endl;
	else
		cout << "Depends on order" << endl;

	if(is_equal1 != is_equal2)
	{
		cout << "* Incorrect: Result should be the same in either order" << endl;
		return false;
	}

	if(is_equal1 == is_equal_correct)
	{
		cout << "* Correct" << endl;
		return true;
	}
	else
	{
		if(is_equal_correct)
			cout << "* Incorrect: Should be reported as equal" << endl;
		else if(board1.getSize() != board2.getSize())
			cout << "* Incorrect: Boards of different sizes are always unequal" << endl;
		else
			cout << "* Incorrect: Should be reported as unequal" << endl;
		return false;
	}
}



int calculateMark (unsigned int correct_board_size_count,
                   bool correct_board_bytes,
                   bool correct_stones_removed_bytes,
                   unsigned int correct_board_value_constants,
                   double correct_default_constructor_fraction,
                   unsigned int correct_count_with_value,
                   unsigned int correct_set_at,
                   unsigned int correct_count_after_set,
                   unsigned int correct_load,
                   double correct_basic_print_fraction,
                   double correct_star_point_fraction,
                   double correct_row_header_fraction,
                   double correct_column_header_fraction,
                   double correct_replace_all_fraction,
                   double correct_is_neighbour_fraction,
                   double correct_fill_connected_fraction,
                   double correct_calculate_score_fraction,
                   double correct_play_stone_fraction,
                   double correct_copy_to_fraction,
                   double correct_copy_from_fraction,
                   double correct_copy_delete_fraction,
                   double correct_assign_to_fraction,
                   double correct_assign_from_fraction,
                   double correct_assign_delete_fraction,
                   double correct_self_assign_fraction,
                   double correct_chain_assign_fraction,
                   double correct_get_size_fraction,
                   double correct_is_on_board_fraction,
                   double correct_print_by_size_fraction,
                   double correct_load_sizes_fraction,
                   double correct_replace_24_fraction,
                   double correct_score_24_fraction,
                   double correct_play_stone_24_fraction,
                   double correct_equality_self_fraction,
                   double correct_equality_size_fraction,
                   bool correct_equality_different,
                   bool correct_equality_same,
                   bool is_finished)
{
	unsigned int mark = 0;
	if(is_finished)
		mark = COMPILE_AND_NOT_CRASH_MARKS;
	else
		mark = COMPILE_AND_START_MARKS;

	// from earlier assignments
	assert(EXISTING_WORK_MARKS == 1);

	if(correct_stones_removed_bytes &&
	   correct_board_value_constants == BOARD_VALUE_COUNT_WITH_MARKED &&
	   correct_default_constructor_fraction == 1.0 &&
	   correct_count_with_value == BOARD_VALUE_COUNT &&
	   correct_count_after_set  == BOARD_VALUE_COUNT &&
	   correct_set_at == 10 &&
	   correct_load &&
	   correct_basic_print_fraction   == 1.0 &&
	   correct_star_point_fraction    == 1.0 &&
	   correct_row_header_fraction    == 1.0 &&
	   correct_column_header_fraction == 1.0 &&
	   correct_replace_all_fraction     == 1.0 &&
	   correct_is_neighbour_fraction    == 1.0 &&
	   correct_fill_connected_fraction  == 1.0 &&
	   correct_calculate_score_fraction == 1.0 &&
	   correct_play_stone_fraction      == 1.0 &&
	   correct_copy_to_fraction       == 1.0 &&
	   correct_copy_from_fraction     == 1.0 &&
	   correct_copy_delete_fraction   == 1.0 &&
	   correct_assign_to_fraction     == 1.0 &&
	   correct_assign_from_fraction   == 1.0 &&
	   correct_assign_delete_fraction == 1.0 &&
	   correct_self_assign_fraction   == 1.0 &&
	   correct_chain_assign_fraction  == 1.0 &&
	   correct_board_size_count == BOARD_SIZE_COUNT &&
	   correct_get_size_fraction      == 1.0 &&
	   correct_is_on_board_fraction   == 1.0 &&
	   correct_print_by_size_fraction == 1.0 &&
	   correct_load_sizes_fraction    == 1.0 &&
	   correct_replace_24_fraction    == 1.0 &&
	   correct_score_24_fraction      == 1.0 &&
	   correct_play_stone_24_fraction == 1.0)
	{
		mark += 1;
	}

	// from Assignment 6

	bool any_true  = correct_equality_self_fraction > 0.0 || correct_equality_same;
	bool any_false = correct_equality_size_fraction > 0.0 || correct_equality_different;
	if(any_true && any_false)
		mark++;

	if(correct_equality_self_fraction == 1.0 &&
	   correct_equality_size_fraction == 1.0)
	{
		mark++;
	}

	if(correct_equality_different && correct_equality_same)
		mark++;

	assert(mark <= TOTAL_MARKS);
	return mark;
}
