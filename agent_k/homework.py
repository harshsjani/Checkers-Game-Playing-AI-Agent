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

    def run(self):
        if os.path.isfile('playdata.txt'):
            os.remove(os.path.join(os.getcwd(), 'playdata.txt'))
        start_time = time.time()
        p = PlayGame(self.b.get_board())
        p.set_max_player(self.b.get_playing_color())
        p.set_piece_counts()
        v, jump_seq = p.alphabeta(None, None, False, 0, True, -constant.INFINITY, constant.INFINITY)

        end_time = time.time()
        print(v, jump_seq, end_time - start_time)
        self.write_move(jump_seq)
        self.write_playdata(v)


def main():
    s = CheckersOrchestrator()
    s.run()

if __name__ == "__main__":
    main()