from GameBoard import GameBoard
from Evaluate import Evaluate
from copy import copy, deepcopy
import timeit

class Checkers:

    def __init__(self, path):

        self.gameMode, self.color, self.timeRem, self.board = self.takeInput(path)
        self.eColor = ''
        self.pColor = ''
        self.output_move = []
        self.definecolor()


    def takeInput(self, path):

        with open(path) as file:

            inputLines = [line.strip() for line in file]
            board = inputLines[3:]
            board = [[ele for ele in ip] for ip in board]
        
        file.close()
        return inputLines[0], inputLines[1], float(inputLines[2]), board

    
    def definecolor(self):
        if self.color == 'WHITE':
            self.pColor = 'w'
            self.eColor = 'b'
        else:
            self.pColor = 'b'
            self.eColor = 'w'

    def output(self, best_move):
        print(best_move)
        mapping = {0:'a', 1:'b', 2:'c', 3:'d' ,4:'e', 5:'f' , 6:'g', 7:'h'}
        start_pos, (end_pos, skipped_pieces) = best_move
        if len(skipped_pieces) == 0:
            move_type = 'E'
        else:
            move_type = 'J'
        
        from_pos = mapping[start_pos[1]] + str(8-start_pos[0])

        if move_type == 'E':
            f = open("./output.txt", 'w')
            f.write(move_type + " " + from_pos + " " +  mapping[end_pos[1]] + str(8-end_pos[0]))
            f.close() 
        
        else:
            moves = []
            for piece in skipped_pieces[::-1]:
                dx = piece[0] - start_pos[0]
                dy = piece[1] - start_pos[1]
                end_pos = (piece[0] + dx, piece[1] + dy)
                print(end_pos)
                moves.append(move_type + " " + mapping[start_pos[1]] + str(8-start_pos[0]) + 
                    " " + mapping[end_pos[1]] + str(8-end_pos[0]) )
                start_pos = end_pos
        
            f = open("./output.txt", 'w')
            f.write("\n".join(moves))
            f.close()

if __name__ == "__main__":

    game = Checkers("./input.txt")
    start = timeit.default_timer()
    evaluate = Evaluate(game.board, game.pColor, game.eColor)
    eval_value, best_move = evaluate.minimax(game.board, 3, True, float('-inf'), float('inf'))
    stop = timeit.default_timer()
    # print(eval_value, best_move)
    game.output(best_move)
    print("Time: ", stop - start)
    

