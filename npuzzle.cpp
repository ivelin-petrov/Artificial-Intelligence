#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>

std::vector<char> directions;

size_t _SIZE;
size_t _ZEROINDEX;

void print(int **board){
    for(size_t i = 0; i < _SIZE; ++i){
        for(size_t j = 0; j < _SIZE; ++j){
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

std::pair<int, int> findZero(int **board){
    for(size_t i = 0; i < _SIZE; ++i){
        for(size_t j = 0; j < _SIZE; ++j){
            if(board[i][j] == 0){
                return std::make_pair(i, j);
            }
        }
    }

    return std::make_pair(-1, -1);
}

size_t manhattan(int **board){
    size_t distance = 0;

    for(size_t i = 0; i < _SIZE; ++i){
        for(size_t j = 0; j < _SIZE; ++j){
            if(board[i][j] != 0){
                int boardVal = board[i][j];

                if(i*_SIZE + j <= _ZEROINDEX){
                    boardVal -= 1;
                }

                int rowVal = (boardVal / _SIZE) - i;
                int colVal = (boardVal % _SIZE) - j;
                distance += std::abs(rowVal) + std::abs(colVal);
            }
        }
    }

    return distance;
}

int ida_star_search(int **board, std::pair<int, int> prevZeroPos, size_t distance, const size_t& threshold, bool& flag){
    size_t heuristics = manhattan(board);
    size_t estimate = distance + heuristics;

    if(estimate > threshold){
        return estimate;
    }

    if(heuristics == 0){
        flag = true;
        std::cout << distance <<  std::endl;

        for(size_t i = 0; i < directions.size(); ++i){
            if(directions[i] == 'u'){
                std::cout << "up" << " ";
            }else if(directions[i] == 'd'){
                std::cout << "down" << " ";
            }else if(directions[i] == 'l'){
                std::cout << "left" << " ";
            }else if(directions[i] == 'r'){
                std::cout << "right" << " ";
            }
        }
        std::cout << std::endl;

        return estimate;
    }

    int min = INT_MAX;

    int saveI = findZero(board).first;
    int saveJ = findZero(board).second;

    int t;

    // up
    if(saveI - 1 >= 0 && saveI - 1 != prevZeroPos.first){
        board[saveI][saveJ] = board[saveI - 1][saveJ];
        board[saveI - 1][saveJ] = 0;

        directions.push_back('d');
        t = ida_star_search(board, std::make_pair(saveI, saveJ), distance + 1, threshold, flag);
        directions.pop_back();

        board[saveI - 1][saveJ] = board[saveI][saveJ];
        board[saveI][saveJ] = 0;
        
        if(t < min){ 
            min = t;
        }    
        if(flag){ 
            return min;
        }
    }

    // down
    if(saveI + 1 < _SIZE && saveI + 1 != prevZeroPos.first){
        board[saveI][saveJ] = board[saveI + 1][saveJ];
        board[saveI + 1][saveJ] = 0;

        directions.push_back('u');
        t = ida_star_search(board, std::make_pair(saveI, saveJ), distance + 1, threshold, flag);
        directions.pop_back();

        board[saveI + 1][saveJ] = board[saveI][saveJ];
        board[saveI][saveJ] = 0;

        if(t < min){
            min = t;
        }    
        if(flag){
            return min;
        }
    }

    // left
    if(saveJ - 1 >= 0 && saveJ - 1 != prevZeroPos.second){
        board[saveI][saveJ] = board[saveI][saveJ - 1];
        board[saveI][saveJ - 1] = 0;

        directions.push_back('r');
        t = ida_star_search(board, std::make_pair(saveI, saveJ), distance + 1, threshold, flag);
        directions.pop_back();

        board[saveI][saveJ - 1] = board[saveI][saveJ];
        board[saveI][saveJ] = 0;

        if(t < min){
            min = t;
        }    
        if(flag){
            return min;
        }
    }

    // right
    if(saveJ + 1 < _SIZE && saveJ + 1 != prevZeroPos.second){
        board[saveI][saveJ] = board[saveI][saveJ + 1];
        board[saveI][saveJ + 1] = 0;

        directions.push_back('l');
        t = ida_star_search(board, std::make_pair(saveI, saveJ), distance + 1, threshold, flag);
        directions.pop_back();

        board[saveI][saveJ + 1] = board[saveI][saveJ]; 
        board[saveI][saveJ] = 0;

        if(t < min){
            min = t;
        }    
        if(flag){
            return min;
        }
    }

    return min;
}

void ida_star(int **board){
    // ida*
    size_t threshold = manhattan(board);
    size_t tmp = 0;
    bool flag = false;

    while(true){
        tmp = ida_star_search(board, std::make_pair(-1, -1), 0, threshold, flag);
        if(flag){
            return;
        }
        threshold = tmp;
    }
}

void clear(int **board){
    for (size_t i = 0; i < _SIZE; ++i){
        delete[] board[i];
    }
    delete[] board;

    board = nullptr;
}

int** init(){
    size_t N;
    std::cin >> N;
    if(!std::cin){ return nullptr; }

    _SIZE = std::sqrt(N+1);
    
    std::cin >> _ZEROINDEX; // from 0 to n
    if(!std::cin){ return nullptr; }

    if(_ZEROINDEX == -1){
        _ZEROINDEX = N;
    }

    int **board = new int*[_SIZE];
    for(size_t i = 0; i < _SIZE; ++i){
        board[i] = new int[_SIZE];
    }

    int zeroI;
    int zeroJ;
    int value;

    for(size_t i = 0; i < _SIZE; ++i){
        for(size_t j = 0; j < _SIZE; ++j){
            std::cin >> value;

            if(!std::cin){ 
                clear(board);
                return nullptr;
            }

            if(value == 0){
                zeroI = i;
                zeroJ = j;
            }

            board[i][j] = value;
        }
    }

    return board;
}

int main(){
    // g++ npuzzleFaster.cpp -o npuzzleFaster.exe
    // ./npuzzleFaster.exe

    // 8 -1 0 5 6 2 8 4 7 3 1 -> 26
    // 8 -1 4 7 8 0 2 1 3 5 6 -> 27
    // 8 -1 6 8 7 5 3 2 4 0 1 -> 27
    // 15 -1 5 6 3 4 8 0 1 15 10 7 2 11 12 9 14 13 -> 40
    // 15 -1 10 5 8 11 2 12 9 6 13 1 0 15 4 7 3 14 -> 50

    int **board = init();
    if(board == nullptr){ return -1; }

    auto start = std::chrono::high_resolution_clock::now();

    ida_star(board);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration<double>(stop - start).count() << " seconds" << std::endl;
    
    clear(board);



    return 0;
}
