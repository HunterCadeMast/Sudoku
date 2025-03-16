/*
 * Name: Hunter Mast
 */

#include "Sudoku.h"

/**
 * Here, we create a constructor for our Sudoku puzzle that initializes it as an empty 9 by 9 box.
 * We use this sort of double for-loop quite a bit, but 'i' can represent rows and 'j' as columns.
 *
 * @param None
 * @return None, but will initialize the current puzzle for input.
 */
Sudoku::Sudoku()
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            // box used to be in the public class, but moved it to private.
            // Was not sure as we cannot add or remove any public classes.
            // At least not any that was not given to us.
            // It still works fine in the private, so we will leave it there.
            box[i][j] = 0;
        }
    }
}

/**
 * We load the puzzle we are solving here from the file name and get all currently filled boxes.
 * 'file >> box[i][j]' will put each individual number from the file into our box in sequence.
 * Similar to using a stack to pop, '<<' is a bitwise right shift.
 * If we cannot find the file or open it, we send an error and end the program.
 *
 * @param fileName Name of the file that holds our initial puzzle.
 * @return box with initial puzzles filled out.
 */
void Sudoku::loadFromFile(string fileName)
{
    ifstream file(fileName);
    if (file.is_open()) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                file >> box[i][j];
            }
        }
        file.close();
    } else {
        cout << "ERROR: Cannot open the file, \'" << fileName << "\'!" << endl;
        exit(1);
    }
}

/**
 * For solve(), we are checking if we can solve this puzzle or not and fill in our box array.
 * This first section we check each box and if we come across any that is 0 (Or empty),
 * we say that the puzzle is not solved and continue to verify it.
 * The second section we are going through each row and column inline with the current box.
 * We are checking the current box and continue if there are no duplicate numbers.
 * The third section we do something similar to the second section,
 * but we just check the 3 by 3 section that our current value is in.
 * After, we set the value of that box and do recursive backtracking to check another box.
 * If the backtracking fails, then we reset our last box back to 0.
 * Then, we try to solve for a new number until we solve the whole puzzle.
 *
 * @param None
 * @return Bool Verifies if the puzzle is solvable or not.
 */
bool Sudoku::solve()
{
    int row = 0;
    int column = 0;
    bool solved = true;
    // First, we check and make sure that at least one box is not solved or empty.
    for (row = 0; row < 9; row++) {
        for (column = 0; column < 9; column++) {
            // If the current box is empty, we are going to want to try and solve it.
            if (box[row][column] == 0) {
                solved = false;
                break;
            }
        }
        if (!solved) {
            break;
        }
    }
    // If solved, we end the program.
    if (solved) {
        return true;
    }
    for (int currentBoxValue = 1; currentBoxValue <= 9; currentBoxValue++) {
        bool inlineCheck = true;
        // Here, we verify that within each row and column for the entire puzzle,
        // the current box's currentBoxValue does not already exist in both the row and column.
        for (int i = 0; i < 9; i++) {
            // This checks that the row and column inline with the current box are not the same.
            if (box[row][i] == currentBoxValue || box[i][column] == currentBoxValue) {
                inlineCheck = false;
                break;
            }
        }
        // Here, we check the 3 by 3 section to make sure the number does not exist here also.
        if (inlineCheck) {
            // We should get the 3 by 3 index for the row and column of our box.
            const int SECTION_ROW = row - row % 3;
            const int SECTION_COLUMN = column - column % 3;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    // If any of them have the current box value,
                    // we leave our loop and try another number.
                    if (box[SECTION_ROW + i][SECTION_COLUMN + j] == currentBoxValue) {
                        inlineCheck = false;
                        break;
                    }
                }
                if (!inlineCheck) {
                    break;
                }
            }
        }
        // We place the current value in its cell to where it should go and call solve() again.
        // This will recursively backtrack and try another number if it returns false.
        // Otherwise, true.
        if (inlineCheck) {
            // We set the value we were checking to the box at the row and column indicated.
            box[row][column] = currentBoxValue;
            if (solve()) {
                return true;
            }
            // If solve() comes back false, we reset our last box back to 0 and start over.
            box[row][column] = 0;
        }
    }
    return false;
}

/**
 * This checks both our solved and original puzzle to ensure any value has not changed.
 *
 * @param other We use 'other' as an object of Sudoku.
 * @return Bool Verifies if the puzzles are equal and original boxes are not modified.
 */
bool Sudoku::equals(const Sudoku& OTHER) const
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            // Here, we are checking that the original puzzle is the same as the solved puzzle.
            if (box[i][j] != OTHER.box[i][j]) {
                return false;
            }
        }
    }
    return true;
}

/**
 * This last function is a 'friend' of our public class.
 * Being a 'friend' means that it allows ostream to use the Sudoku object by overloading.
 * This means that we will not mess anything up or change any values.
 * Being a friend means it can also access private variables,
 * but we have none so that part does not matter.
 * Without the friend declaration,
 * I would run into an issue of not understanding how to convert Sudoku into a character at all,
 * and it required binary operators.
 * This function is also overloaded (<<),
 * so we can make our Sudoku object into the output stream and print it all.
 * We first print the top of our puzzle border.
 * After, we go through our box and print each value.
 * It is written as a ternary conditional operator that pints nothing if empty and if not,
 * we print the value of the box as a string with a space afterwords.
 * Every 3 values printed, we add a line to separate the sections.
 * End of rows add a final border and then go onto a newline.
 * Once we reach the end of our box, we print the bottom of our puzzle border and end.
 *
 * @param out This allows us to print to the out stream.
 * @param sudoku This gets us the Sudoku object as a constant and print using the out stream.
 * @return out This is to send our results to the ostream to print them all at once.
 */
ostream& operator<<(ostream& out, const Sudoku& SUDOKU)
{
    // We print the top of the puzzle border.
    out << "+-------+-------+-------+" << endl;
    // Starts to go through rows and columns (Left to right then up to down).
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            // Once we finish a section, we add a bar to separate them.
            if (j % 3 == 0) {
                out << "| ";
            }
            // We print a space if there is an empty box, but the value of the box if not empty.
            out << (SUDOKU.box[i][j] == 0 ? " " : to_string(SUDOKU.box[i][j])) << " ";
        }
        // Move to next line.
        out << "|" << endl;
        // Every 3 lines, we print the section separation.
        if (i % 3 == 2) {
            out << "+-------+-------+-------+" << endl;
        }
    }
    // We return the finished puzzle to be printed.
    return out;
}