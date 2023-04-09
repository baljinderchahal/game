//
//  TestHelper.h
//
//  A module containing helper functions for test programs.
//
//  Do not modify this file.
//
//  2023-01-05: Added a test to testConstant for checking a
//              constant against itself
//

#pragma once

#include <cassert>
#include <string>
#include <iostream>
#include <typeinfo>  // for typeid
#include <vector>



//
//  TestHelper
//
//  A namespace to store the testing commands.
//
namespace TestHelper
{

//
//  startup
//
//  Purpose: To print the starting message for the test program.
//  Parameter(s):
//    <1> name: The name for the test program
//    <2> compile_marks: How many marks the student gets for the
//                       program compiling and starting correctly
//    <3> total_marks: The maximum marks for this test program
//  Precondition(s):
//    <1> name != ""
//    <2> compile_marks <= total_marks
//  Returns: N/A
//  Side Effect: The starting message is printed.  This includes
//               the marks for compiling and starting without
//               crashing.
//
void startup (const std::string& name,
              int compile_marks,
              int total_marks);

//
//  isConst
//
//  Purpose: To determine whether the specified variable is a
//           constant.  
//  Parameter(s):
//    <1> v: The variable to test
//  Precondition(s): N/A
//  Returns: Whether v is a constant in the scope that isConst
//           is called from.
//  Side Effect: N/A
//  Note: This "function" is really a pair of functions that
//        work together.  The value returned depends on which
//        function is invoked, which depends on the compile-time
//        type information.
//  Note: This function pair uses templates, which are covered
//        in Section 13 of the course.
//
template <typename T>
bool isConst (const T& v) { return true; }
template <typename T>
bool isConst (T& v) { return false; }

//
//  testConstant
//
//  Purpose: To perform the tests needed to ensure that a
//           constant is defined correctly.  The constant should
//           be defined with the correct type, as const, and
//           with the correct value.
//  Parameter(s):
//    <1> to_test: The constant to test
//    <2> correct: A constant with the correct type and value
//    <3> name: The constant's name, expressed as a string
//  Precondition(s):
//    <1> constant_name != ""
//  Returns: Whether the constant was correct.
//  Side Effect: A message indicating whether the constant was
//               correct is printed.  If the constant is
//               incorrect, the message includes why.
//
template <typename TestType, typename CorrectType>
bool testConstant (TestType& to_test,
                   const CorrectType& correct,
                   const std::string& name)
{
	assert(name != "");
	assert(&to_test != &correct);  // do not test a constant against itself

	if(typeid(CorrectType) != typeid(to_test))
	{
		std::cout << "* Incorrect: " << name << " has incorrect type" << std::endl;
		return false;
	}
	else if(!isConst(to_test))
	{
		std::cout << "* Incorrect: " << name << " is not const" << std::endl;
		return false;
	}
	else if(!(to_test == correct))
	{
		std::cout << "* Incorrect: " << name << " has incorrect value, should be " << correct << std::endl;
		return false;
	}
	else
	{
		std::cout << "* Correct" << std::endl;
		return true;
	}
}

//
//  isCapturing
//
//  Purpose: To determine if the TestHelper module is capturing
//           data from standard output (cout).
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether output is being captured.
//  Side Effect: N/A
//
bool isCapturing ();

//
//  startCapture
//
//  Purpose: To begin capturing standard output (cout).  Until
//           EndCapture is called, all standard output will be
//           stored instead of printed.
//  Parameter(s): N/A
//  Precondition(s):
//    <1> !isCapturing()
//  Returns: N/A
//  Side Effect: Any existing captured output is discarded.  The
//               output buffer is flushed.  Then TestHelper
//               module begins to capture all data printed to
//               standard output.
//
void startCapture ();

//
//  endCapture
//
//  Purpose: To stop capturing standard output (cout).
//  Parameter(s):
//    <1> is_print_captured: Whether to print the captured
//                           output
//  Precondition(s):
//    <1> isCapturing()
//  Returns: The captured output, as a string.
//  Side Effect: The TestHelper module stops capturing data from
//               standard output.  If is_print == true, all data
//               captured since startCapture was called is
//               printed.  Otherwise, nothing is printed.
//
std::string endCapture (bool is_print_captured);

//
//  printCaptureBlockLine
//
//  Purpose: To print a line across the output used to denote
//           the start and end of a block of captured text.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: A line of 48 '=' characters is printed.
//  Side Effect: N/A
//
void printCaptureBlockLine ();

//
//  toLowercase
//
//  Purpose: To convert a string to lowercase.
//  Parameter(s): N/A
//    <1> str: The string to convert
//  Precondition(s): N/A
//  Returns: A string with every uppercase character converted
//           to the corresponding lowercase character.
//  Side Effect: N/A
//
std::string toLowercase (const std::string& str);

//
//  trim
//
//  Purpose: To remove leading and trailing whitespace from a
//           string.
//  Parameter(s): N/A
//    <1> str: The string to remove whitespace from
//  Precondition(s): N/A
//  Returns: A string containing str with the leading and
//           trailing whitespace removed.  Whitespace inside the
//           string is not removed.  If str is the empty string
//           or only contains whitespace, the empty string is
//           returned.
//  Side Effect: N/A
//  Example: Trimming the string "  Hello World!  " would return
//           "Hello World!".
//
std::string trim (const std::string& str);

//
//  splitWithoutEmpty
//
//  Purpose: To split a string int substrings at each instance
//           of a certain character.
//  Parameter(s): N/A
//    <1> str: The string to split
//    <2> split_on: The character to split the string on
//  Precondition(s): N/A
//  Returns: A vector containing the substrings.  Character
//           split_on is not included in the reasults.  Empty
//           substrings are not included in the results.
//  Side Effect: N/A
//  Example: Splitting the string "ABC|DEFG||HIJ|" on '|' would
//           return { "ABC", "DEFG", "HIJ" }.
//
std::vector<std::string> splitWithoutEmpty (
                                         const std::string& str,
                                         char split_on);

//
//  printSummaryHeaderFinal
//
//  Purpose: To print the header text for the final mark
//           summary.
//  Parameter(s):
//    <1> indent: How far to indent the results in the summary
//    <2> max_digits: The maximum number of digits in a number
//    <3> didnt_crash_marks: How many marks the student gets for
//                           the program not crashing during the
//                           tests
//  Precondition(s):
//    <1> max_digits >= 1
//  Returns: N/A
//  Side Effect: The header for the summary test is printed.
//               Summary lines printed after this will be
//               indented so that the results begin indent
//               characters to the right.  Numbers will be
//               right-aligned as if they contain a maximum of
//               max_digits digits.
//
void printSummaryHeaderFinal (unsigned int indent,
                              unsigned int max_digits,
                              unsigned int didnt_crash_marks);

//
//  printSummaryHeaderPartial
//
//  Purpose: To print the header text for a partial mark summary
//           part way through the output.
//  Parameter(s):
//    <1> indent: How far to indent the results in the summary
//    <2> max_digits: The maximum number of digits in a number
//    <3> compile_marks: How many marks the student gets for the
//                       program compiling and starting
//  Precondition(s):
//    <1> max_digits >= 1
//  Returns: N/A
//  Side Effect: The header for the summary test is printed.
//               Summary lines printed after this will be
//               indented so that the results begin indent
//               characters to the right.  Numbers will be
//               right-aligned as if they contain a maximum of
//               max_digits digits.
//
void printSummaryHeaderPartial (unsigned int indent,
                                unsigned int max_digits,
                                unsigned int compile_marks);

//
//  printSummaryHeaderEndOfFile
//
//  Purpose: To print the header text for the partial mark
//           summary at the end of a file.  The remainder of the
//           marks require a differennt (harder) test file.
//  Parameter(s):
//    <1> indent: How far to indent the results in the summary
//    <2> max_digits: The maximum number of digits in a number
//    <3> didnt_crash_marks: How many marks the student gets for
//                           the program not crashing during the
//                           tests
//  Precondition(s):
//    <1> max_digits >= 1
//  Returns: N/A
//  Side Effect: The header for the summary test is printed.
//               Summary lines printed after this will be
//               indented so that the results begin indent
//               characters to the right.  Numbers will be
//               right-aligned as if they contain a maximum of
//               max_digits digits.
//
void printSummaryHeaderEndOfFile (
                               unsigned int indent,
                                unsigned int max_digits,
                                unsigned int didnt_crash_marks);

//
//  printSummaryLine
//
//  Purpose: To print a true/false line for the summary.
//  Parameter(s):
//    <1> text: The description for the line
//    <2> is_correct: Whether the student program produced the
//                    correct output
//  Precondition(s):
//    <1> text != ""
//  Returns: N/A
//  Side Effect: A summary line is printed with description
//               text, followed by "true" or "false", depending
//               on is_correct.
//
void printSummaryLine (const std::string& text,
                       bool is_correct);

//
//  printSummaryLine
//
//  Purpose: To print a fraction line for the summary.
//  Parameter(s):
//    <1> text: The description for the line
//    <2> achieved: The number of cases correct
//    <3> maximum: The total number of cases
//  Precondition(s):
//    <1> text != ""
//    <2> maximum > 0
//    <3> achieved <= maximum
//  Returns: N/A
//  Side Effect: A summary line is printed with description
//               text, followed by achieved and maximum
//               formatted as a fraction.
//
void printSummaryLine (const std::string& text,
                       unsigned int achieved,
                       unsigned int maximum);

//
//  printSummaryLine
//
//  Purpose: To print a percentage line for the summary.
//  Parameter(s):
//    <1> text: The description for the line
//    <2> percentage: The percentage of cases correct
//  Precondition(s):
//    <1> text != ""
//    <2> percentage >= 0.0
//    <3> percentage <= 1.0
//  Returns: N/A
//  Side Effect: A summary line is printed with description
//               text, followed by percentage formatted as a
//               percentage.
//
void printSummaryLine (const std::string& text,
                       double percentage);

//
//  markAnyAll
//
//  Purpose: To calculate the student mark for a question where
//           there is 1 mark for ever getting the right answer
//           and a second for always getting it.
//  Parameter(s):
//    <1> correct: The number of correct answers
//    <2> maximum: The total number of answers
//  Precondition(s):
//    <1> maximum >= 2
//    <2> correct <= maximum
//  Returns: The student mark.  This will be 0, 1, or 2.
//  Side Effect: N/A
//
unsigned int markAnyAll (unsigned int correct,
                         unsigned int maximum);

//
//  printMark
//
//  Purpose: To print the student mark for this test program.
//  Parameter(s):
//    <1> achieved: The student mark
//    <2> maximum: The maximum mark
//  Precondition(s):
//    <1> maximum > 0
//    <2> achieved <= maximum
//  Returns: N/A
//  Side Effect: The student mark is printed.  If the student
//               achieved a perfect score, an additional message
//               is printed to show this.
//
void printMark (unsigned int achieved,
                unsigned int maximum);

//
//  printMarkPartial
//
//  Purpose: To print the student mark for a partial mark
//           summary.  This may be part way through the output
//           or at the end end of a test file whose contents are
//           included in another file.
//  Parameter(s):
//    <1> achieved: The student mark
//    <2> highest_possible: The largest achievable mark
//    <3> maximum: The theoretical maximum mark
//  Precondition(s):
//    <1> maximum > 0
//    <2> achieved <= highest_possible
//    <3> highest_possible <= maximum
//  Returns: N/A
//  Side Effect: The student mark is printed.  If the student
//               achieved a score of highest_possible, an
//               additional message is printed to show this.
//
void printMarkPartial (unsigned int achieved,
                       unsigned int highest_possible,
                       unsigned int maximum);

//
//  waitForEnter
//
//  Purpose: To wait for the user to press [ENTER].
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A message instructing the user to press [ENTER]
//               is printed.  Then the program waits until the
//               user does so.
//
void waitForEnter ();

} // end of namespace TestHeader
