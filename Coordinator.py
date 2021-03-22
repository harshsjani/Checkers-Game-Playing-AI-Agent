import os, shutil, sys
import subprocess
import time

x_indices = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h']
y_indices = [8, 7, 6, 5, 4, 3, 2, 1]

def initialize_board():
    init_board = [["."] * 8 for _ in range(8)]

    with open(r'./tests/init_board.txt') as f:
        for i in range(8):
            line = f.readline().rstrip("\n")
            for idx, c in enumerate(line):
                init_board[i][idx] = c
    
    return init_board

def print_board(board):
    for row in board:
        print(row)

def call_agent(agent):
    subprocess.run(agent, shell=True)

def check_game_end(color):
    bp = wp = 0
    for x in range(8):
        for y in range(8):
            if board[x][y] == "b" or board[x][y] == "B":
                bp += 1
            elif board[x][y] == "w" or board[x][y] == "W":
                wp += 1
    if bp == 0:
        return "WHITE"
    elif wp == 0:
        return "BLACK"
    return "NO"

def do_jump(board, jump):
    _, sm, fm = jump.split(" ")
    y, x = x_indices.index(sm[0]), y_indices.index(int(sm[1]))
    q, p = x_indices.index(fm[0]), y_indices.index(int(fm[1]))

    capx, capy = None, None

    if x < p:
        capx = p - 1
    else:
        capx = p + 1
    if y < q:
        capy = q - 1
    else:
        capy = q + 1

    board[p][q] = board[x][y]
    board[x][y] = "."
    board[capx][capy] = "."

    if p == 7 and board[p][q] == "b":
        board[p][q] = "B"
    elif p == 0 and board[p][q] == "w":
        board[p][q] = "W"

agent1 = "./homework11"
# agent1 = "python3 ./agent_v/homework.py"
agent2 = "python3 ./agent_k/homework.py"
# agent2 = "./homework11_minimax"

ipf = "input.txt"
opf = "output.txt"

color_nums = [0, 1]
color_names = ["BLACK", "WHITE"]

init_color = 0
movec = 0

agents = [agent2, agent1]
times = [300.0, 300.0]
moves_without_capture = 0
max_moves_without_capture = 50
curagent = 0

board = initialize_board()

while True:
    with open(ipf, "w") as f:
        f.write("GAME\n")
        f.write(color_names[curagent] + "\n")
        f.write(str(times[curagent]) + "\n")
        rows = ["".join(row) for row in board]
        f.writelines("\n".join(rows))
    
    start_time = time.time()
    call_agent(agents[curagent])
    end_time = time.time()
    time_taken = end_time - start_time
    print("Time taken on this move: {0}".format(time_taken))
    times[curagent] -= time_taken

    if times[curagent] <= 0:
        print("TIME LIMIT EXCEEDED for {0}".format(agents[curagent]))

    with open(opf) as f:
        lines = f.readlines()
        if len(lines) == 1 and lines[0][0] != "J":
            print(color_names[curagent] + " did: \n" + lines[0])
            _, sm, fm = lines[0].split(" ")
            y, x = x_indices.index(sm[0]), y_indices.index(int(sm[1]))
            q, p = x_indices.index(fm[0]), y_indices.index(int(fm[1]))

            board[p][q] = board[x][y]
            board[x][y] = "."
            moves_without_capture += 1

            if p == 7 and board[p][q] == "b":
                board[p][q] = "B"
            elif p == 0 and board[p][q] == "w":
                board[p][q] = "W"
        else:
            print(color_names[curagent] + " did: ")
            for line in lines:
                print(line)
            moves_without_capture = 0

            jumps = lines
            for jump in jumps:
                do_jump(board, jump)

    movec += 1
    
    ge = check_game_end(color_nums[curagent])
    if ge == "WHITE":
        print("WHITE WON!!!")
        print_board(board)
        break
    elif ge == "BLACK":
        print("BLACK WON!!!")
        print_board(board)
        break
    else:
        if moves_without_capture == 50:
            print("DRAW AFTER 50 MOVES!!!")
            print_board(board)
            break

    print("Board at move {0}:".format(movec))
    print_board(board)

    curagent = (curagent + 1) % 2
