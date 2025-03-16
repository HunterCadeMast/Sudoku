/*
 * Name: Hunter Mast
 * Date: 11/29/2024
 */

import scala.io.Source

/**
* This is the entire solver inside of an object.
* The main method will run the Sudoku class that attempts to solve the puzzle.
*
* @param box This is an array inside of an array (9x9) that fills out an empty (0) puzzle.
* @return The solved and unsolved puzzles printed.
*/
object SudokuSolver {
  class Sudoku {
    val box: Array[Array[Int]] = Array.fill(9, 9)(0)

    /**
    * This is only used in testing for our puzzles.
    * It will go through and make sure the solved puzzle is the same for each square as unsolved.
    *
    * @param puzzle This is one of the puzzles that we are checking.
    * @return True if the puzzles are equal and false if not.
    */
    def equals(puzzle: Sudoku): Boolean = {
      (0 until 9).forall { i =>
        (0 until 9).forall { j =>
          box(i)(j) == puzzle.box(i)(j)
        }
      }
    }

    /**
    * This should load out Sudoku puzzle with the unsolved squares.
    * If the file is empty or lines are incorrect, then we throw an error.
    * We make sure to move through the whitespace to get each number and if blank, enter a zero.
    *
    * @param fileName This is the file with the puzzle we are trying to solve.
    * @return A Sudoku puzzle that is populated with some known squares.
    */
    def loadFromFile(fileName: String): Unit = {
      val source = Source.fromFile(fileName)
      val lines = source.getLines().toList
      if (lines.isEmpty) {
        println(s"ERROR: The file, '$fileName', is empty or lines are mismatched!")
        source.close()
        sys.exit(1)
      }
      for (i <- 0 until 9) {
        val line = lines(i).trim
        val numbers = line.split("\\s+").map(_.toInt)
        for (j <- 0 until 9) {
          box(i)(j) = numbers(j)
        }
      }
      source.close()
    }

    /**
    * This function should solve the puzzle all on it's own.
    * It starts by going through and checking each empty box.
    * If not empty, we check if the number we are looking at is inside the current box.
    * If not, we make sure each row and column do not have said value either.
    * If all of these are true, we place the number and continue.
    * We will backtrack recursively if our value ends up causing any issues.
    *
    * @return A solved Sudoku puzzle.
    */
    def solve(): Boolean = {
      for (row <- 0 until 9) {
        for (column <- 0 until 9) {
          if (box(row)(column) == 0) {
            for (value <- 1 to 9) {
              var boxFlag = false
              val startRow = (row / 3) * 3
              val startCol = (column / 3) * 3
              for (i <- startRow until startRow + 3) {
                for (j <- startCol until startCol + 3) {
                  if (box(i)(j) == value) {
                    boxFlag = true
                  }
                }
              }
              if (!box(row).contains(value) && !box.map(_(column)).contains(value) && !boxFlag) {
                box(row)(column) = value
                if (solve()) return true
                box(row)(column) = 0
              }
            }
            return false
          }
        }
      }
      true
    }

    /**
    * This is what prints and writes the solve and unsolved puzzles.
    * This overrides the toString method so we can write the puzzle using toString.
    * It goes through every box and uses StringBuilder to create one long string.
    * It gets the value and checks when it should print something or not.
    *
    * @param puzzleString This is the long string we are building that will be printed.
    * @return A string with all of the values of the puzzle set.
    */
    override def toString: String = {
      val puzzleString = new StringBuilder()
      puzzleString.append("+-------+-------+-------+\n")
      for (i <- 0 until 9) {
        for (j <- 0 until 9) {
          if (j % 3 == 0) puzzleString.append("| ")
          val cellValue = box(i)(j)
          puzzleString.append(if (cellValue == 0) " " else cellValue.toString)
          puzzleString.append(" ")
        }
        puzzleString.append("|\n")
        if (i % 3 == 2) puzzleString.append("+-------+-------+-------+\n")
      }
      puzzleString.toString()
    }
  }

  /**
    * This should load the Sudoku puzzle from a file and populate the new Sudoku puzzle.
    * If the file cannot open, we throw an error.
    * If the file has the incorrect number of lines or is not a correct puzzle, error is thrown.
    * Otherwise, we continue to to ask for the puzzle.
    * We then print the solved and unsolved puzzle.
    *
    * @param args This is the file we are trying to solve and an array with leach lone a string.
    * @return The solved and unsolved Sudoku puzzle.
    */
  def main(args: Array[String]): Unit = {
    val sudoku = new Sudoku()
    var puzzleLoaded = false
    while (!puzzleLoaded) {
      println("Enter file name:")
      val fileName = scala.io.StdIn.readLine()
      try {
        sudoku.loadFromFile(s"txt/$fileName")
        if (sudoku.box.forall(_.length == 9) && sudoku.box.length == 9) {
          puzzleLoaded = true
        }
        else {
          println("ERROR: The file does not contain a valid 9x9 Sudoku puzzle!")
        }
      }
      catch {
        case _: java.io.FileNotFoundException =>
          println(s"ERROR: The file, '$fileName', was not found! Please try again!")
      }
    }
    println("Unsolved Puzzle:")
    println(sudoku)
    if (sudoku.solve()) {
      println("Solved Puzzle:")
      println(sudoku)
    }
    else {
      println("This puzzle has no solution!")
    }
  }
}