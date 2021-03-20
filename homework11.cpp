#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>


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

    static string white_man;
    static string black_man;
    static string white_king;
    static string black_king;
};

enum GameType {
    SINGLE,
    GAME
};

enum Color {
    NONE,
    BLACK,
    WHITE
};

enum PieceType {
    BLANK,
    MAN,
    KING
};

char Constants::col_labels[8] =  {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
int Constants::row_labels[8] =  {8, 7, 6, 5, 4, 3, 2, 1};
string Constants::input_path = "input.txt";
string Constants::output_path = "output.txt";

string Constants::white_man = "w";
string Constants::black_man = "b";
string Constants::white_king = "w";
string Constants::black_king = "b";

class Piece {
public:
    Piece () {
        type = BLANK;
        color = NONE;
    }

    Piece(char rep) {
        if (rep == '.') {
            type = BLANK;
            color = NONE;
        } else if (rep == 'w') {
            type = MAN;
            color = WHITE;
        } else if (rep == 'W') {
            type = KING;
            color = WHITE;
        } else if (rep == 'b') {
            type = MAN;
            color = BLACK;
        } else if (rep == 'B') {
            type = KING;
            color = BLACK;
        }
    }

    string get_string() {
        if (type == BLANK)
            return ".";
        if (type == MAN) {
            return color == WHITE ? Constants::white_man : Constants::black_man;
        } else {
            return color == WHITE ? Constants::white_king : Constants::black_king;
        }
    }

    PieceType type;
    Color color;
};

class Board {
public:
    Piece squares[8][8];

    Board () {

    }
};

class Move {
    int start_x, start_y;
    int final_x, final_y;
};

class CaptureSequence : Move {
public:
    struct Capture {
        int start_x, start_y;
        int final_x, final_y;
        int captured_x, captured_y;
        Piece capturing_piece;
        Piece captured_piece;
    };

    CaptureSequence() {

    }

    vector<Capture> captureSequence;
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
                board.squares[i][j] = Piece(line[j]);
            }
        }
    }

    void write_board() {
        for (int i = 0; i < Constants::num_rows; ++i) {
            for (int j = 0; j < Constants::num_cols; ++j) {
                cout << board.squares[i][j].get_string() << " ";
            }
            cout << endl;
        }
    }

    pair<vector<CaptureSequence>, vector<Move>> get_piece_moves(Piece piece, int cx, int cy) {
        
    }

    vector<Move> get_all_moves (Color playing_color) {
        vector<CaptureSequence> capture_sequences;
        vector<Move> moves;

        for (int i = 0; i < Constants::num_rows; ++i) {
            for (int j = 0; j < Constants::num_cols; ++j) {
                Piece piece = board.squares[i][j];

                if (piece.type != BLANK && piece.color == playing_color) {

                }
            }
        }
    }

    int eval() {
        map<string, int> counts;
        for (int i = 0; i < Constants::num_rows; ++i) {
            for (int j = 0; j < Constants::num_cols; ++j) {
                counts[board.squares[i][j].get_string()];
            }
        }

        int val = counts[Constants::white_man] + counts[Constants::white_king]
                    - counts[Constants::black_man] - counts[Constants::black_king];
        
        return val;
    }
};

int main() {
    Checkers game = Checkers();
    game.read_board();
    game.write_board();
}