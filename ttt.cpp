#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

char _player = 'X';
char _ai = 'O';

char board[3][3];

void printBoard(){
    std::cout << std::endl;
    std::cout << ' ' << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << std::endl;
    std::cout << "-----------" << std::endl;
    std::cout << ' ' << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << std::endl;
    std::cout << "-----------" << std::endl;
    std::cout << ' ' << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << std::endl;
    std::cout << std::endl;
}

std::vector<std::pair<size_t, size_t>> getMoves(){
    std::vector<std::pair<size_t, size_t>> moves;
    
    for(size_t i = 0; i < 3; ++i){
        for(size_t j = 0; j < 3; ++j){
            if(board[i][j] != _ai && board[i][j] != _player){
                moves.push_back(std::make_pair(i,j));
            }
        }
    }

    return moves;
}

bool positionOccupied(size_t row, size_t col){
    return board[row][col] == _ai || board[row][col] == _player;
}

bool boardFull(){
    std::vector<std::pair<size_t, size_t>> moves = getMoves();
    return moves.size() == 0;
}

bool gameWon(char marker){
    return (board[0][0] == marker && board[0][1] == marker && board[0][2] == marker ||
            board[1][0] == marker && board[1][1] == marker && board[1][2] == marker ||
            board[2][0] == marker && board[2][1] == marker && board[2][2] == marker) ||
           (board[0][0] == marker && board[1][0] == marker && board[2][0] == marker ||
            board[0][1] == marker && board[1][1] == marker && board[2][1] == marker ||
            board[0][2] == marker && board[1][2] == marker && board[2][2] == marker) ||
           (board[0][0] == marker && board[1][1] == marker && board[2][2] == marker ||
            board[0][2] == marker && board[1][1] == marker && board[2][0] == marker);
}

char getOpponentMarker(char marker){
    return (marker == _player) ? _ai : _player;
}

int getBoardState(char marker, int depth){
    if(gameWon(marker)){
        return 10 - depth;
    }

    if(gameWon(getOpponentMarker(marker))){
        return depth - 10;
    }

    if(boardFull()){
        return 0;
    }

    return 0;
}

bool gameOver(int depth){
    return boardFull() || getBoardState(_ai, depth) != 0;
}

std::pair<int, std::pair<int, int>> minimax(char marker, int depth, int alpha, int beta){
    std::pair<int, int> bestMove = std::make_pair(-1, -1);
    int bestScore = (marker == _ai) ? -100 : 100;
    
    if(gameOver(depth)){
        bestScore = getBoardState(_ai, depth);
        return std::make_pair(bestScore, bestMove);
    }

    std::vector<std::pair<size_t, size_t>> moves = getMoves();

    for(size_t i = 0; i < moves.size(); ++i){
        std::pair<int, int> crrMove = moves[i];

        board[crrMove.first][crrMove.second] = marker;

        if(marker == _ai){ // maximize player's turn
            int score = minimax(_player, depth + 1, alpha, beta).first;

            if(bestScore < score){
                bestScore = score;
                bestMove = crrMove;

                alpha = std::max(alpha, bestScore);
                board[crrMove.first][crrMove.second] = ' ';
                if(alpha >= beta){
                    break;
                }
            }

        }else if(marker == _player){ // minimize opponent's turn
            int score = minimax(_ai, depth + 1, alpha, beta).first;

            if(bestScore > score){
                bestScore = score;
                bestMove = crrMove;

                beta = std::min(beta, bestScore);
                board[crrMove.first][crrMove.second] = ' ';
                if(alpha >= beta){
                    break;
                }
            }
        }

        board[crrMove.first][crrMove.second] = ' ';
    }

    return std::make_pair(bestScore, bestMove);
}

int main(){
    // g++ ttt.cpp -o ttt.exe
    // ./ttt.exe

    bool flag = false;
    std::cout << "play first?" << std::endl;
    std::cin >> flag;
    if(!std::cin) { return -1; }

    for(size_t i = 0; i < 3; ++i){ for(size_t j = 0; j < 3; ++j){ board[i][j] = ' '; } }

    if(!flag){
        _ai = 'X';
        _player = 'O';

        std::srand(std::time(0));
        size_t random = std::rand() % 5;

        switch(random){
            case 0: board[0][0] = _ai; break;
            case 1: board[0][2] = _ai; break;
            case 2: board[2][0] = _ai; break; 
            case 3: board[2][2] = _ai; break;
            case 4: board[1][1] = _ai; break;
        }
    }

    printBoard();

    int row;
    int col;

    while(!gameOver(0)){
        std::cout << "enter row:" << std::endl;
        std::cin >> row;
        std::cout << "enter col:" << std::endl;
        std::cin >> col;

        if(!std::cin){
            std::cout << "incorrect input, exiting..." << std::endl;
            return -1;    
        }else if((row <= 0 || row > 3) || (col <= 0 || col > 3)){
            std::cout << "out of bounds, try again..." << std::endl;
            continue;
        }else if(positionOccupied(row-1, col-1)){
            std::cout << "position occupied, try again..." << std::endl;
            continue;
        }else{
            board[row-1][col-1] = _player;
        }

        std::pair<int, std::pair<int, int>> move = minimax(_ai, 1, -100, 100);
        board[move.second.first][move.second.second] = _ai;
        
        printBoard();
    }

    int result = getBoardState(_player, 0);
    
    if(result == 10){
        std::cout << "WIN" << std::endl;
    }else if(result == -10){
        std::cout << "LOSS" << std::endl;
    }else{
        std::cout << "DRAW" << std::endl;
    }



    return 0;
}