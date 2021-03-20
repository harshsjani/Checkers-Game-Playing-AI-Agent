#include <string>
#include <iostream>
#include <fstream>


using namespace std;

class Constants {
public:
    static char col_labels[8];
    static int row_labels[8];
    const static int num_rows = 8;
    const static int num_cols = 8;
    const static char unoccupied_square = '.';

    static string input_path;
    static string output_path;
};

enum GameType {
    SINGLE,
    GAME
};

enum Color {
    BLACK,
    WHITE
};

char Constants::col_labels[8] =  {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
int Constants::row_labels[8] =  {8, 7, 6, 5, 4, 3, 2, 1};
string Constants::input_path = "input.txt";
string Constants::output_path = "output.txt";

class Board {
public:
    char squares[8][8];
};

class Checkers {
    GameType gameType;
    Color playerColor;
    float playTime;
    Board board;

public:
    Checkers() {
        gameType = GameType::SINGLE;
        playerColor = Color::BLACK;
        playTime = 0.0;
    }

    void read_board() {
        ifstream input_file;

        input_file.open(Constants::input_path);

        string line;

        getline(input_file, line);
        gameType = (line == "SINGLE") ? GameType::SINGLE : GameType::GAME;

        getline(input_file, line);
        playerColor = (line == "BLACK") ? Color::BLACK : Color::WHITE;

        getline(input_file, line);
        playTime = stof(line);

        board = Board();

        for (int i = 0; i < Constants::num_rows; ++i) {
            getline(input_file, line);

            for (int j = 0; j < Constants::num_cols; ++j) {
                board.squares[i][j] = line[j];
            }
        }
    }
};

int main() {
    Checkers game = Checkers();
    game.read_board();

    
}