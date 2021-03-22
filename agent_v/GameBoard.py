class GameBoard:

    def __init__(self, board):

        self.board = board
        self.ROW = 8
        self.COLUMN = 8


    def isKing(self, piece):
        
        x, y = piece
        if self.board[x][y] == 'W' or self.board[x][y] == 'B':
            return True
        
        return False


    def getPieceColor(self, piece):
        x, y = piece

        if self.board[x][y] == 'B' or self.board[x][y] == 'b':
            return 'b'
        else:
            return 'w'

    
    def allValidMoves(self, piece):

        x, y = piece
        moves = {}
        left = y - 1
        right = y + 1
        is_king = self.isKing(piece)
        piece_color = self.getPieceColor(piece)
        lastplace = (x, y)
        orig_piece = self.board[x][y]
        self.board[x][y] = '.'
        if piece_color == 'w' or is_king:
            moves.update(self.traverseLeft(x-1, max(x-3, -1), -1, piece_color, left, is_king, lastplace))
            moves.update(self.traverseRight(x-1, max(x-3, -1), -1, piece_color, right, is_king, lastplace))

        if piece_color == 'b' or is_king:
            moves.update(self.traverseLeft(x+1, min(x+3, self.ROW), 1, piece_color, left, is_king, lastplace))
            moves.update(self.traverseRight(x+1, min(x+3, self.ROW), 1, piece_color, right, is_king, lastplace)) 
        
        self.board[x][y] = orig_piece
        jump_moves = []
        positional_moves = []
        
        for k in moves.keys():
            if moves[k] != [(-1, -1)]:
                if moves[k] == []:
                    positional_moves.append((k, []))
                else:
                    jump_moves.append((k, moves[k]))

        return jump_moves, positional_moves

    
    def traverseLeft(self, start, stop, step, pieceColor, left, is_king, lastplace = (0,0), jumped = [], temp = []):
        
        moves = {}
        last = []

        for r in range(start, stop, step):

            if left < 0:
                break

            currx, curry = r, left
            curr = self.board[r][left]

            if (currx, curry) in jumped:
                break 

            if (currx, curry) == lastplace:
                break

            if (currx, curry) in temp:
                break

            if curr == '.':
                if jumped and not last:
                    break

                elif jumped:
                    temp = jumped + temp
                    moves[(r, left)] = last + temp
                    moves[lastplace] = [(-1, -1)]
                
                else:
                    moves[(r, left)] = last

                if last:
                    if step == -1:
                        x = max(r-3, -1)
                    else:
                        x = min(r+3, self.ROW)

                    lastplace = (currx, curry)
                    moves.update(self.traverseLeft(r + step, x, step, pieceColor, left - 1, is_king, lastplace, jumped=last, temp = temp.copy()))
                    moves.update(self.traverseRight(r + step, x, step, pieceColor, left + 1, is_king, lastplace, jumped=last, temp = temp.copy()))

                    # reversing so that king can also have opposite side moves
                    
                    if is_king:
                        if step == -1:
                            x = min(r+3, self.ROW)
                        else:
                            x = max(r-3, -1)
                        
                        moves.update(self.traverseLeft(r - step, x, -step, pieceColor, left - 1, is_king, lastplace, jumped=last, temp= temp.copy()))
                        moves.update(self.traverseRight(r - step, x, -step, pieceColor, left + 1, is_king, lastplace, jumped=last, temp= temp.copy()))
                
                break

            elif self.getPieceColor((currx, curry)) == pieceColor:
                break

            else:
                last = [(currx, curry)]
            
            left = left - 1

        return moves
    
    
    def traverseRight(self, start, stop, step, pieceColor, right, is_king, lastplace = (0, 0), jumped = [], temp = []):
        
        moves = {}
        last = []

        for r in range(start, stop, step):

            if right >= self.COLUMN:
                break
            
            currx, curry = r, right 
            curr = self.board[r][right]

            if (currx, curry) in jumped:
                break 

            if (currx, curry) == lastplace:
                break

            if (currx, curry) in temp:
                break

            if curr == '.':
                
                if jumped and not last:
                    break
                
                elif jumped:
                    temp = jumped + temp
                    moves[(r, right)] = last + temp
                    moves[lastplace] = [(-1, -1)] 

                else:
                    moves[(r, right)] = last

                if last:

                    if step == -1:
                        x = max(r-3, -1)
                    else:
                        x = min(r+3, self.ROW)

                    lastplace = (currx, curry)
                    moves.update(self.traverseLeft(r + step, x, step, pieceColor, right - 1, is_king, lastplace, jumped=last, temp = temp.copy()))
                    moves.update(self.traverseRight(r + step, x, step, pieceColor, right + 1, is_king, lastplace, jumped=last, temp = temp.copy()))

                    if is_king:
                        if step == -1:
                            x = min(r+3, self.ROW)
                        else:
                            x = max(r-3, -1)
                        
                        moves.update(self.traverseLeft(r - step, x, -step, pieceColor, right - 1, is_king, lastplace, jumped=last, temp= temp.copy()))
                        moves.update(self.traverseRight(r - step, x, -step, pieceColor, right + 1, is_king, lastplace, jumped=last, temp= temp.copy()))
                
                break

            elif self.getPieceColor((currx, curry)) == pieceColor:
                break
            
            else:
                last = [(currx, curry)]
            right = right + 1
        
        return moves

    
    def returnBoard(self):

         return self.board