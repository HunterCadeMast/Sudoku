/*
 * Name: Hunter Mast
 * Date: 11/25/2024
 */

/**
* 
* This imports from the standard library the File struct for working with files.
* It also imports I/O functions with BufRead and BufReader, which allow lines to be read.
* fmt is useful to formatting, which is used when attempting to display the output.
*
*/
use std::fs::File;
use std::io::{self, BufRead, BufReader};
use std::fmt;


/**
* This public object defines the Sudoku puzzle initially.
* It states that the sqare is a 9x9 puzzle and each box as an unsigned 8-bit integer (1 to 9).
*
* @param  square An array with an array inside, storing integers.
* @return Empty Sudoku puzzle.
*/
pub struct Sudoku {
    square: [[u8; 9]; 9],
}


/**
* 
* This are methods that will interact and modify our Sudoku struct.
*
*/
impl Sudoku {
    /**
    * This constructor creates a new Sudoku puzzle, each box initialized to 0.
    *
    * @param square An array with an array inside, each element being initialized to 0.
    * @return A new Sudoku struct.
    */
    pub fn new() -> Self {
        Sudoku { 
            square: [[0; 9]; 9] 
        }
    }

     /**
    * This should load the Sudoku puzzle from a file and populate the new Sudoku puzzle.
    * If the file cannot open, we throw an error.
    * If the file has the incorrect number of lines or is not a correct puzzle, error is thrown.
    *
    * @param file_name This is the file we are trying to solve.
    * @return A Sudoku puzzle that is populated with some known squares.
    */
    pub fn load_from_file(&mut self, file_name: &str) {
        let file = File::open(&file_name).unwrap_or_else(|_| {
            panic!("ERROR: Cannot open the file '{}'.", file_name);
        });
        let reader = BufReader::new(file);
        for (i, line) in reader.lines().enumerate() {
            for (j, value) in line.unwrap().split_whitespace().enumerate() {
                self.square[i][j] = value.parse().unwrap_or_else(|_| {
                    panic!("ERROR: Invalid number '{}' in file.", value);
                });
            }
        }
    }

    /**
    * This attempts to solve the Sudoku puzzle using recursive backtracking.
    * We call the find_empty() method to find an empty square.
    * We try to check if we can place the square and if not, return it to 0.
    *
    * @return True if solved successfully and false if not.
    */
    pub fn solve(&mut self) -> bool {
        if let Some((row, col)) = self.find_empty() {
            for value in 1..=9 {
                if self.placement_check(row, col, value) {
                    self.square[row][col] = value;
                    if self.solve() {
                        return true;
                    }
                    self.square[row][col] = 0;
                }
            }
            false
        } else {
            true
        }
    }

    /**
    * This attempts to find the coordinates of the next empty square.
    *
    * @return Tuple of row and column for the next empty square and none if complete puzzle.
    */
    fn find_empty(&self) -> Option<(usize, usize)> {
        for row in 0..9 {
            for col in 0..9 {
                if self.square[row][col] == 0 {
                    return Some((row, col));
                }
            }
        }
        None
    }

    /**
    * This checks if placement is correct for each box, column, and row.
    *
    * @param row Row index of current box.
    * @param col Column index of current box.
    * @param value Value to be placed into the current box.
    * @return True if we can place the value and false if not.
    */
    fn placement_check(&self, row: usize, col: usize, value: u8) -> bool {
        let section_row = row / 3 * 3;
        let section_col = col / 3 * 3;
        !self.square[row].contains(&value)
            && !(0..9)
                .any(|r| self.square[r][col] == value)
            && !(0..3)
                .any(|i| (0..3)
                .any(|j| self.square[section_row + i][section_col + j] == value))
    }
}

/**
* Formats display for the Sudoku puzzzle to be printed.
*/
impl fmt::Display for Sudoku {
    /**
    * This should format the puzzle result (Solved and unsolved).
    * It checks rows and columns to know when boxes should be separated.
    * Also, will check value that should be printed in which space.
    *
    * @param f Reference to the formatter.
    * @return Results of the solved puzzle as a string to print.
    */
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        for (i, row) in self.square.iter().enumerate() {
            if i % 3 == 0 {
                writeln!(f, "+-------+-------+-------+")?;
            }
            for (j, &value) in row.iter().enumerate() {
                if j % 3 == 0 {
                    write!(f, "| ")?;
                }
                write!(f, "{} ", if value == 0 { ' ' } else { (value + b'0') as char })?;
            }
            writeln!(f, "|")?;
        }
        writeln!(f, "+-------+-------+-------+")
    }
}

/**
* This is where the program starts.
* We create a new puzzle.
* We recieve input on the file's name, then load it in.
* We print the unsolved puzzle.
* Afterwords, we attempt to solve the puzzle and if we can not, then we print it has no solution.
* If we can, we print the solved puzzle.
*
* @return Prints solved puzzle (Returns no value).
*/
fn main() {
    let mut sudoku = Sudoku::new();
    let file_name = loop {
        println!("Enter the puzzle file name from the 'txt' folder: ");
        let mut file_name = String::new();
        io::stdin().read_line(&mut file_name).unwrap();
        let file_name = file_name.trim();
        let file_path = ["txt/", file_name].concat();
        if File::open(&file_path).is_ok() {
            break file_path;
        } else {
            println!("ERROR: Cannot open the file '{}'. Please try again.", file_name);
        }
    };
    sudoku.load_from_file(&file_name);
    println!("Unsolved Puzzle: ");
    println!("{}", sudoku);
    if sudoku.solve() {
        println!("Solved Puzzle: ");
        println!("{}", sudoku);
    } else {
        println!("This puzzle has no solution!");
    }
}