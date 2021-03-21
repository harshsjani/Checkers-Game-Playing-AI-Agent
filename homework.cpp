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

    const static char white_man;
    const static char black_man;
    const static char white_king;
    const static char black_king;

    static map<char, vector<pair<int, int>>> directions;

    const static int max_depth = 10;

    const static int INF = 36001;
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

const char Constants::white_man = 'w';
const char Constants::black_man = 'b';
const char Constants::white_king = 'W';
const char Constants::black_king = 'B';

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

    void write_board() {
        for (int i = 0; i < Constants::num_rows; ++i) {
            for (int j = 0; j < Constants::num_cols; ++j) {
                cout << squares[i][j].get_char() << " ";
            }
            cout << endl;
        }
    }
};

class Move {
    int start_x, start_y;
    int final_x, final_y;
    Piece piece;

public:
    Move () {}
    Move (int x, int y, int p, int q, Piece& the_piece) {
        start_x = x;
        start_y = y;
        final_x = p;
        final_y = q;
        piece = the_piece;
    }

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

    void execute(Board& board) {
        board.squares[final_x][final_y] = piece;
        board.squares[start_x][start_y] = Piece(Constants::unoccupied_square);
        promote_if_possible(board, piece, final_x, final_y);
    }

    void undo(Board& board) {
        board.squares[start_x][start_y] = piece;
        board.squares[final_x][final_y] = Piece(Constants::unoccupied_square);
    }

    void write_to_output() {
        ofstream output_file;
        output_file.open(Constants::output_path);

        output_file << "E " << Constants::col_labels[start_y] << Constants::row_labels[start_x] << " " << Constants::col_labels[final_y] << Constants::row_labels[final_x];

        output_file.close();
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
        board.squares[final_x][final_y] = capturing_piece;
        board.squares[start_x][start_y] = Piece(Constants::unoccupied_square);
        board.squares[captured_x][captured_y] = Piece(Constants::unoccupied_square);

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
    void execute(Board& board) {
        for (auto capture: captureSequence) {
            capture.execute_capture(board);
        }
    }

    void undo(Board& board) {
        for (auto it = captureSequence.rbegin(); it != captureSequence.rend(); ++it)
            it->undo_capture(board);
    }

    void write_to_output() {
        ofstream output_file;
        output_file.open(Constants::output_path);

        for (auto capture: captureSequence) {
            output_file << "J " << Constants::col_labels[capture.start_y] << Constants::row_labels[capture.start_x] << " " << Constants::col_labels[capture.final_y] << Constants::row_labels[capture.final_x] << endl;
        }

        output_file.close();
    }

    vector<Capture> captureSequence;
};

class Checkers {
    GameType gameType;
    Color playerColor;
    Color enemyColor;
    float playTime;
    Board board;

public:
    Checkers() {
        gameType = GameType::SINGLE;
        playerColor = Color::BLACK;
        playTime = 0.0;
        evaluated_search_depth = 10;
    }

    void read_board() {
        ifstream input_file;

        input_file.open(Constants::input_path);

        string line;

        getline(input_file, line);
        gameType = (line == "SINGLE") ? GameType::SINGLE : GameType::GAME;

        getline(input_file, line);
        playerColor = (line == "BLACK") ? Color::BLACK : Color::WHITE;
        enemyColor = (line == "BLACK") ? Color::WHITE : Color::BLACK;

        getline(input_file, line);
        playTime = stof(line);

        board = Board();

        for (int i = 0; i < Constants::num_rows; ++i) {
            getline(input_file, line);

            for (int j = 0; j < Constants::num_cols; ++j) {
                board.squares[i][j] = Piece(line[j]);
            }
        }

        input_file.close();
    }

    bool in_bounds(int x, int y) {
        return 0 <= x  && x < Constants::num_rows && 0 <= y && y < Constants::num_cols;
    }

    bool is_valid_capture(int x, int y, int p, int q, Piece piece) {
        int post_jump_x = x < p ? p + 1 : p - 1;
        int post_jump_y = y < q ? q + 1 : q - 1;

        if (in_bounds(post_jump_x, post_jump_y) && board.squares[post_jump_x][post_jump_y].get_char() == Constants::unoccupied_square)
            return true;
        return false;
    }

    pair<vector<CaptureSequence>, vector<Move>>* get_piece_moves(Piece piece, int cx, int cy, bool only_jumps=false) {
        vector<CaptureSequence> capture_sequences;
        vector<Move> moves;

        for (auto d_pair : Constants::directions[piece.get_char()]) {
            int p = d_pair.first + cx;
            int q = d_pair.second + cy;

            if (in_bounds(p, q)) {
                if ((board.squares[p][q].color ^ piece.color) == 1 && is_valid_capture(cx, cy, p, q, piece)) {
                    int post_jump_x = cx < p ? p + 1 : p - 1;
                    int post_jump_y = cy < q ? q + 1 : q - 1;

                    Capture capture = Capture(cx, cy, p, q, post_jump_x, post_jump_y, piece, board.squares[p][q]);
                    CaptureSequence cs = CaptureSequence();
                    cs.captureSequence.push_back(capture);

                    capture.execute_capture(board);

                    auto moves = get_piece_moves(piece, post_jump_x, post_jump_y, true);

                    for (auto capSeq : moves->first) {
                        capSeq.captureSequence.insert(capSeq.captureSequence.begin(), capture);
                        capture_sequences.push_back(capSeq);
                    }

                    capture.undo_capture(board);

                    if (moves->first.size() == 0)
                        capture_sequences.push_back(cs);
                }
                else if (capture_sequences.size() == 0 && !only_jumps && board.squares[p][q].type == BLANK) {
                    moves.push_back(Move(cx, cy, p, q, piece));
                }
            }
        }

        pair<vector<CaptureSequence>, vector<Move>>* pr = new pair<vector<CaptureSequence>, vector<Move>>();
        pr->first = capture_sequences;
        pr->second = moves;
        return pr;
        //return &make_pair(capture_sequences, moves);
    }

    pair<vector<CaptureSequence>, vector<Move>>* get_all_moves (Color playing_color) {
        vector<CaptureSequence> capture_sequences;
        vector<Move> moves;

        for (int i = 0; i < Constants::num_rows; ++i) {
            for (int j = 0; j < Constants::num_cols; ++j) {
                Piece piece = board.squares[i][j];

                if (piece.type != BLANK && piece.color == playing_color) {
                    auto piece_moves = get_piece_moves(piece, i, j);

                    if (piece_moves->first.size() > 0) {
                        capture_sequences.insert(capture_sequences.begin(), piece_moves->first.begin(), piece_moves->first.end());
                    }
                    else if (piece_moves->second.size() > 0) {
                        moves.insert(moves.begin(), piece_moves->second.begin(), piece_moves->second.end());
                    }
                }
            }
        }

        pair<vector<CaptureSequence>, vector<Move>>* pr = new pair<vector<CaptureSequence>, vector<Move>>();
        pr->first = capture_sequences;
        pr->second = moves;

        return pr;
        //return &make_pair(capture_sequences, moves);
    }

    int max_value(Color playing_color, int alpha, int beta, int depth) {
        if (depth >= evaluated_search_depth)
            return eval(playing_color);
        
        int v = -Constants::INF;

        auto items = get_all_moves(playing_color);

        if (items->first.size() > 0) {
            for (auto captureSeq : items->first) {
                captureSeq.execute(board);

                v = max(v, min_value(enemyColor, alpha, beta, depth + 1));

                if (v >= beta) {
                    captureSeq.undo(board);
                    return v;
                }

                alpha = max(alpha, v);

                captureSeq.undo(board);
            }
        } else if (items->second.size() > 0) {
            for (auto move: items->second) {
                move.execute(board);

                v = max(v, min_value(enemyColor, alpha, beta, depth + 1));

                if (v >= beta) {
                    move.undo(board);
                    return v;
                }

                alpha = max(alpha, v);

                move.undo(board);
            }
        } else {
            // No valid moves
            return -Constants::INF;
        }

        return v;
    }

    int min_value(Color playing_color, int alpha, int beta, int depth) {
        if (depth >= evaluated_search_depth) {
            return eval(playing_color);
        }

        int v = Constants::INF;

        auto items = get_all_moves(playing_color);

        if (items->first.size() > 0) {
            for (auto captureSeq : items->first) {
                captureSeq.execute(board);

                v = min(v, max_value(playerColor, alpha, beta, depth + 1));

                if (v <= alpha) {
                    captureSeq.undo(board);
                    return v;
                }

                beta = min(beta, v);

                captureSeq.undo(board);
            }
        } else if (items->second.size() > 0) {
            for (auto move: items->second) {
                move.execute(board);

                v = min(v, max_value(playerColor, alpha, beta, depth + 1));

                if (v <= alpha) {
                    move.undo(board);
                    return v;
                }

                beta = min(beta, v);
                
                move.undo(board);
            }
        } else {
            // No valid moves
            return Constants::INF;
        }

        return v;
    }

    pair<CaptureSequence, Move>* alphabeta_search() {
        auto items = get_all_moves(playerColor);
        int val = -Constants::INF;

        pair<CaptureSequence, Move>* final_move = new pair<CaptureSequence, Move>();

        if (items->first.size() > 0) {
            for (auto captureSeq : items->first) {
                captureSeq.execute(board);

                int abv = min_value(enemyColor, -Constants::INF, Constants::INF, 1);

                if (abv >= val) {
                    val = abv;
                    final_move->first = captureSeq;
                }

                captureSeq.undo(board);
            }
        } else {
            for (auto move: items->second) {
                move.execute(board);

                int abv = min_value(enemyColor, -Constants::INF, Constants::INF, 1);

                if (abv >= val) {
                    val = abv;
                    final_move->second = move;
                }

                move.undo(board);
            }
        }

        return final_move;
    }

    int eval(Color playing_color) {
        map<char, int> counts;
        for (int i = 0; i < Constants::num_rows; ++i) {
            for (int j = 0; j < Constants::num_cols; ++j) {
                counts[board.squares[i][j].get_char()]++;
            }
        }

        int val = counts[Constants::white_man] * 100 + counts[Constants::white_king] * 150
                    - counts[Constants::black_man] * 100 - counts[Constants::black_king] * 150;
        
        return playing_color == BLACK ? -val : val;
    }

    void set_search_depth() {
        if (playTime < 5) {
            evaluated_search_depth = 5;
        } else if (playTime < 10) {
            evaluated_search_depth = 6;
        } else if (playTime < 30) {
            evaluated_search_depth = 8;
        } else if (playTime < 100) {
            evaluated_search_depth = 9;
        } else {
            evaluated_search_depth = 11;
        }
    }

    int evaluated_search_depth;
};

int main() {
    Checkers game = Checkers();
    game.read_board();
    game.set_search_depth();
    
    pair<CaptureSequence, Move>* move = game.alphabeta_search();

    if (move->first.captureSequence.size() > 0) {
        move->first.write_to_output();
    } else {
        move->second.write_to_output();
    }
}