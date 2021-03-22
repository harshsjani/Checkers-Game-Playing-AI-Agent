from copy import deepcopy
from GameBoard import GameBoard

class Evaluate:

    def __init__(self, board, pColor, eColor):
        
        self.board = board
        self.pColor = pColor 
        self.eColor = eColor
    

    def getAllPieces(self, board, color = 'both'):

        allBlackPieces = []
        allWhitePieces = []

        for i in range(8):
            for j in range(8):

                if board[i][j] == 'b' or board[i][j] == 'B':
                    allBlackPieces.append((i,j))

                if board[i][j] == 'w' or board[i][j] == 'W':
                    allWhitePieces.append((i,j))
        
        if color == 'b':
            return allBlackPieces
        if color == 'w':
            return allWhitePieces

        return allBlackPieces, allWhitePieces

    def get_pieces_count(self, board):

        bpawn = 0
        wpawn = 0
        bking = 0
        wking = 0

        for i in range(8):
            for j in range(8):

                if board[i][j] == 'b':
                    bpawn += 1
                
                if board[i][j] == 'B':
                    bking += 1

                if board[i][j] == 'w':
                    wpawn += 1

                if board[i][j] == 'W':
                    wking += 1

        return bpawn, wpawn, bking, wking

    
    def any_winner(self, board):
        
        all_black_pieces, all_white_pieces = self.getAllPieces(board)
        
        if len(all_black_pieces) == 0 or len(all_white_pieces) == 0:
            return True 
        
        return False 
        
        
    def get_all_moves(self, board, color):
        
        gameboard = GameBoard(board)
        any_jump_moves = False
        all_pieces = self.getAllPieces(board, color)
        
        jump_moves = []
        positional_moves = []

        for piece in all_pieces:
            j, p = gameboard.allValidMoves(piece)

            if j != []:
                for ele in j:
                    jump_moves.append((piece, ele))
            
            elif p != []:
                for ele in p:
                    positional_moves.append((piece, ele))
        
        return jump_moves, positional_moves


    def alphabeta(self, board, depth, max_player, alpha = float('-inf'), beta = float('inf')):

        if depth == 0 or self.any_winner(board): # add winner situation if possible
            return self.evaluate(board), board
        
        if max_player:
            # print("Max ran")
            maxEval = float('-inf')
            best_move = None
            jump_moves, positional_moves = self.get_all_moves(board, self.pColor)
            jumped = False
            
            if len(jump_moves) > 0:
                all_moves = jump_moves
                jumped = True
            else:
                all_moves = positional_moves
                
            for move in all_moves:

                piece, temp = move
                target, skips = temp
                possible_board = self.movepiece(deepcopy(board), piece, target, skips, jumped)
                evaluation = self.alphabeta(possible_board, depth-1, False, alpha, beta)[0]
                maxEval = max(maxEval, evaluation)
                if maxEval == evaluation:
                    best_move = move
                
                alpha = max(alpha, maxEval)

                if beta <= alpha:
                    return maxEval, best_move
            
            return maxEval, best_move
        
        else:
            # print("Min Ran")
            minEval = float('inf')
            best_move = None
            jump_moves, positional_moves = self.get_all_moves(board, self.eColor)
            jumped = False
            
            if len(jump_moves) > 0:
                all_moves = jump_moves
                jumped = True
            else:
                all_moves = positional_moves
                
            for move in all_moves:

                piece, temp = move
                target, skips = temp
                possible_board = self.movepiece(deepcopy(board), piece, target, skips, jumped)
                evaluation = self.alphabeta(possible_board, depth-1, True, alpha, beta)[0]
                minEval = min(minEval, evaluation)
                if minEval == evaluation:
                    best_move = move
                
                beta = min(beta, minEval)

                if beta <= alpha:
                    return minEval, best_move
            
            return minEval, best_move


    def minimax(self, board, depth, max_player, alpha = float('-inf'), beta = float('inf')):

        if depth == 0 or self.any_winner(board): # add winner situation if possible
            return self.evaluate(board), board
        
        if max_player:
            # print("Max ran")
            maxEval = float('-inf')
            best_move = None
            jump_moves, positional_moves = self.get_all_moves(board, self.pColor)
            jumped = False
            
            if len(jump_moves) > 0:
                all_moves = jump_moves
                jumped = True
            else:
                all_moves = positional_moves
                
            for move in all_moves:

                piece, temp = move
                target, skips = temp
                possible_board = self.movepiece(deepcopy(board), piece, target, skips, jumped)
                evaluation = self.minimax(possible_board, depth-1, False, alpha, beta)[0]
                maxEval = max(maxEval, evaluation)
                if maxEval == evaluation:
                    best_move = move
                
                # alpha = max(alpha, maxEval)

                # if beta <= alpha:
                #     return maxEval, best_move
            
            return maxEval, best_move
        
        else:
            # print("Min Ran")
            minEval = float('inf')
            best_move = None
            jump_moves, positional_moves = self.get_all_moves(board, self.eColor)
            jumped = False
            
            if len(jump_moves) > 0:
                all_moves = jump_moves
                jumped = True
            else:
                all_moves = positional_moves
                
            for move in all_moves:

                piece, temp = move
                target, skips = temp
                possible_board = self.movepiece(deepcopy(board), piece, target, skips, jumped)
                evaluation = self.minimax(possible_board, depth-1, True, alpha, beta)[0]
                minEval = min(minEval, evaluation)
                if minEval == evaluation:
                    best_move = move
                
                # beta = min(beta, minEval)

                # if beta <= alpha:
                #     return minEval, best_move
            
            return minEval, best_move

        
    def movepiece(self, board, piece, target, skips, jumped):
        
        cx, cy = piece
        tx, ty = target
        temp = board[cx][cy]
        board[cx][cy] = '.'
        if jumped:
            for ele in skips:
                sx, sy = ele
                board[sx][sy] = '.'

        if temp == 'w' and tx == 0:
            temp = 'W'

        if temp == 'b' and tx == 7:
            temp = 'B'    

        board[tx][ty] = temp
    
        return board
    
    def evaluate(self, board):
        
        bpawn, wpawn, bking, wking = self.get_pieces_count(board)
        
        if self.pColor == 'w':
            return (100 * wpawn + 150 * wking) - (100 * bpawn + 150 * bking)
        else:
            return (100 * bpawn + 150 * bking) - (100 * wpawn + 150 * wking)

