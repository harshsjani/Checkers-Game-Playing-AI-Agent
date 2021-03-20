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

    const static char white_man = 'w';
    const static char black_man = 'b';
    const static char white_king = 'W';
    const static char black_king = 'B';

    static map<char, vector<pair<int, int>>> directions;
};

enum GameType {
    SINGLE,
    GAME
};

enum Color {
    NONE = -1,
    BLACK = 0,
    WHITE = 1
};

enum PieceType {
    BLANK = -1,
    MAN = 0,
    KING = 1
};

char Constants::col_labels[8] =  {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
int Constants::row_labels[8] =  {8, 7, 6, 5, 4, 3, 2, 1};
string Constants::input_path = "input.txt";
string Constants::output_path = "output.txt";

map<char, vector<pair<int, int>>> Constants::directions = {
    {Constants::white_man, {{-1, -1}, {-1, 1}}},
    {Constants::black_man, {{1, -1}, {1, 1}}},
    {Constants::white_king, {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}}},
    {Constants::black_king, {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}}}
};

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

    char get_char() {
        if (type == BLANK)
            return Constants::unoccupied_square;
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
};

class Move {
    int start_x, start_y;
    int final_x, final_y;

public:
    static void promote_if_possible(Board& board, Piece piece, int x, int y) {
        if (piece.type == MAN) {
            if (piece.color == BLACK) {
                if (x == Constants::num_rows - 1)
                    board.squares[x][y] = Piece(Constants::black_king);
            } else if (piece.color == WHITE) {
                if (x == 0)
                    board.squares[x][y] = Piece(Constants::white_king);
            }
        }
    }
};

class Capture {
public:
    Capture(int sx, int sy, int cx, int cy, int fx, int fy, Piece capturing, Piece captured) {
        start_x = sx;
        start_y = sy;
        captured_x = cx;
        captured_y = cy;
        final_x = fx;
        final_y = fy;
        capturing_piece = capturing;
        captured_piece = captured;
    }

    void execute_capture(Board& board) {
        board.squares[start_x][start_y] = Piece(Constants::unoccupied_square);
        board.squares[captured_x][captured_y] = Piece(Constants::unoccupied_square);
        board.squares[final_x][final_y] = capturing_piece;

        Move::promote_if_possible(board, capturing_piece, final_x, final_y);
    }

    void undo_capture(Board& board) {
        board.squares[start_x][start_y] = capturing_piece;
        board.squares[captured_x][captured_y] = captured_piece;
        board.squares[final_x][final_y] = Piece(Constants::unoccupied_square);
    }

    int start_x, start_y;
    int captured_x, captured_y;
    int final_x, final_y;
    Piece capturing_piece;
    Piece captured_piece;
};

class CaptureSequence : Move {
public:
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
                cout << board.squares[i][j].get_char() << " ";
            }
            cout << endl;
        }
    }

    bool in_bounds(int x, int y) {
        return 0 <= x < Constants::num_rows && 0 <= y < Constants::num_cols;
    }

    bool is_valid_capture(int x, int y, int p, int q, Piece piece) {
        int post_jump_x = x < p ? p + 1 : p - 1;
        int post_jump_y = y < q ? q + 1 : q - 1;

        if (in_bounds(post_jump_x, post_jump_y) && board.squares[post_jump_x][post_jump_y].get_char() == Constants::unoccupied_square)
            return true;
        return false;
    }

    pair<vector<CaptureSequence>, vector<Move>> get_piece_moves(Piece piece, int cx, int cy, bool only_jumps=false) {
        vector<CaptureSequence> capture_sequences;
        vector<Move> moves;

        for (auto d_pair : Constants::directions[piece.get_char()]) {
            int p = d_pair.first + cx;
            int q = d_pair.second + cy;

            if (in_bounds(p, q)) {
                if (board.squares[p][q].color ^ piece.color == 1 && is_valid_capture(cx, cy, p, q, piece)) {
                    int post_jump_x = cx < p ? p + 1 : p - 1;
                    int post_jump_y = cy < q ? q + 1 : q - 1;

                    Capture capture = Capture(cx, cy, p, q, post_jump_x, post_jump_y, piece, board.squares[p][q]);

                    capture.execute_capture(board);

                    auto moves = get_piece_moves(piece, post_jump_x, post_jump_y, true);

                    for (auto capSeq : moves.first) {
                        CaptureSequence cs = CaptureSequence();
                        cs.captureSequence.push_back(capture);
                        cs.captureSequence.insert(cs.captureSequence.begin(), capSeq.captureSequence.begin(), capSeq.captureSequence.end());
                    }

                    capture.undo_capture(board);
                }
            }
        }
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
        map<char, int> counts;
        for (int i = 0; i < Constants::num_rows; ++i) {
            for (int j = 0; j < Constants::num_cols; ++j) {
                counts[board.squares[i][j].get_char()];
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