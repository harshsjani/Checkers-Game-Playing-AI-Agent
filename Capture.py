from enum import Enum


class PieceType(Enum):
    MAN = 0
    KING = 1

class PieceColor(Enum):
    BLACK = 0
    WIHITE = 1

class BoardConstants:
    col_labels = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h']
    row_labels = [8, 7, 6, 5, 4, 3, 2, 1]
    num_rows = 8
    num_cols = 8
    unoccupied_square = "."
    piece_color_map = {'b': 0, 'B': 0, 'w': 1, 'W': 1}

class FileConstants:
    input_path = r'input.txt'
    output_path = r'output.txt'

class GameType(Enum):
    SINGLE = 0
    GAME = 1

class PlayerColor:
    BLACK = 0
    WHITE = 1

class PieceMoves:
    king = [(-1, -1), (-1, 1), (1, -1), (1, 1)]
    men_b = [(1, 1), (1, -1)]
    men_w = [(-1, -1), (-1, 1)]

    enemy = {"W": ["b", "B"], "w": ["b", "B"], "B": ["w", "W"], "b": ["w", "W"]}
    jump = "J"
    move = "E"

class GameConstants:
    INF = 10 ** 9

class Capture:
    def get_all_moves(self, playing_color):
        jumps = []
        moves = []

        for ridx, row in enumerate(self.board.squares):
            for cidx, piece in enumerate(row):
                if piece != BoardConstants.unoccupied_square and BoardConstants.piece_color_map[piece] == playing_color:
                    j, e = self.get_piece_moves(piece, ridx, cidx)

                    if j:
                        jumps.extend(j)
                    else:
                        moves.extend(e)
        
        if jumps:
            jumps = self.generate_capture_lists(jumps)
        
        return jumps, moves

    def get_piece_moves(self, piece, x, y, only_jumps=False):
        jumps = []
        moves = []

        d = PieceMoves.king if piece == "W" or piece == "B" else (PieceMoves.men_b if piece == "b" else PieceMoves.men_w)
        for i, j in d:
            p = x + i
            q = y + j

            if 0 <= p < BoardConstants.num_rows and 0 <= q < BoardConstants.num_cols:
                if self.board.squares[p][q] in PieceMoves.enemy[piece]:
                    valid_jump, new_x, new_y = self.is_valid_jump(x, y, p, q, piece)
                    if valid_jump:
                        # Jump format: "J", capturing position (x, y), captured position(p, q), post-capture position (new_x, new_y), captured_piece, current_piece
                        jump = Capture(x, y, p, q, new_x, new_y, self.board.squares[p][q], self.board.squares[x][y])

                        self.jump(jump)
                        captures = self.get_piece_moves(piece, new_x, new_y, only_jumps=True)
                        for capture in captures[0]:
                            jump.children.append(capture)
                        self.unjump(jump)

                        jumps.append(jump)
                elif not jumps and not only_jumps and self.board.squares[p][q] == BoardConstants.unoccupied_square:
                    # Move format: "E", current position(x, y), moved position(p, q), current_piece
                    moves.append((PieceMoves.move, x, y, p, q, self.board.squares[x][y]))
        
        return (jumps, moves)