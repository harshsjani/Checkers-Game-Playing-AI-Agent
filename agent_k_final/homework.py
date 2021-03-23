from board import Board
import constant
from playGame import PlayGame
import os
import time
class CheckersOrchestrator:
    def __init__(self):
        self.b = Board()
        self.read_input_file()

    def read_input_file(self) -> None:
        self.b.read_board()
    
    def write_playdata(self, ev):
        ofp = open(os.path.join(os.getcwd(), 'playdata.txt'), "a")
        ofp.write(str(ev) + "\n")
        ofp.close()

    def write_move(self, seq: str) -> None:
        ofp = open(os.path.join(os.getcwd(), 'output.txt'), "w")
        n = len(seq)
        for (i, s) in enumerate(seq):
            end_line = "\n" if i != n - 1 else ""
            ofp.write(s + end_line)
        ofp.close()

    def get_depth(self, rem_time: float) -> int:
        if self.b.get_game_mode() == constant.SINGLE:
            return 2
        if rem_time > 100:
            return 9
        elif rem_time > 50:
            return 7
        elif rem_time > 10:
            return 5
        elif rem_time > 2:
            return 1
        else:
            return 0

    def check_move_number(self):
        init_pos = set(((0, 1), (0, 3), (0, 5), (0, 7), (1, 0), (1, 2), (1, 4), (1, 6), (2, 1), (2,3), (2,5), (2, 7), (5, 0), (5, 2), (5, 4), (5, 6), (6, 1), (6, 3), (6, 5), (6, 7), (7, 0), (7, 2), (7, 4), (7, 6)))
        board = self.b.get_board()
        displacement = 0
        n = len(init_pos)
        for init_pos_tuple in init_pos:
            (i, j) = init_pos_tuple
            if board[i][j] == constant.EMPTY_SQUARE:
                displacement += 1
            if displacement > 4:
                break
        if displacement < 4:
            return 4
        return None

    def run(self):
        p = PlayGame(self.b.get_board())
        p.set_max_player(self.b.get_playing_color())
        depth = self.get_depth(self.b.get_rem_time())
        p.set_depth(depth)
        p.set_piece_counts()
        new_depth = self.check_move_number()
        if new_depth != None and self.b.get_game_mode() != constant.SINGLE:
            p.set_depth(new_depth)
        v, jump_seq = p.alphabeta(None, None, False, 0, True, -constant.INFINITY, constant.INFINITY)
        self.write_move(jump_seq)

def main():
    s = CheckersOrchestrator()
    s.run()

if __name__ == "__main__":
    main()