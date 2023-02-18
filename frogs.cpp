#include <iostream>
#include <chrono>

int N;

void print(char *board){
    for(size_t i = 0; i < 2*N+1; ++i){
        std::cout << board[i] << " ";
    }
    std::cout << std::endl;
}

void swapPrint(char *board){
    for(size_t i = 0; i < 2*N+1; ++i){
        switch (board[i]){
            case 'L': std::cout << "R "; break;
            case 'R': std::cout << "L "; break;
            default: std::cout << "_ "; break;
        }
    }
    std::cout << std::endl;
}

bool isGoalState(char *board){
    for(size_t i = 0; i < N; ++i){
        if(board[i] != 'R' || board[N+1+i] != 'L'){
            return false;
        }
    }

    return board[N] == '_';
}

bool solve(char *board, int zeroPos){
    if(isGoalState(board)){
        swapPrint(board);
        return true;
    }

    bool flag = false;

    // left moves
    if(zeroPos + 1 < (2*N + 1) && board[zeroPos + 1] == 'R'){
        board[zeroPos] = 'R';
        board[zeroPos + 1] = '_';

        flag = solve(board, zeroPos + 1);
        
        board[zeroPos + 1] = 'R';
        board[zeroPos] = '_';

        if(flag){
            swapPrint(board);
            return true;
        }
    }
    if(zeroPos + 2 < (2*N + 1) && board[zeroPos + 2] == 'R'){
        board[zeroPos] = 'R';
        board[zeroPos + 2] = '_';
        
        flag = solve(board, zeroPos + 2);
    
        board[zeroPos + 2] = 'R';
        board[zeroPos] = '_'; 
            
        if(flag){    
            swapPrint(board);
            return true; 
        }
    }

    // right moves
    if(zeroPos - 1 >= 0 && board[zeroPos - 1] == 'L'){
        board[zeroPos] = 'L';
        board[zeroPos - 1] = '_';
        
        flag = solve(board, zeroPos - 1);
    
        board[zeroPos - 1] = 'L';
        board[zeroPos] = '_';

        if(flag){
            swapPrint(board);
            return true;
        }
    }
    if(zeroPos - 2 >= 0 && board[zeroPos - 2] == 'L'){
        board[zeroPos] = 'L';
        board[zeroPos - 2] = '_';
        
        flag = solve(board, zeroPos - 2);
    
        board[zeroPos - 2] = 'L';
        board[zeroPos] = '_';

        if(flag){
            swapPrint(board);
            return true;
        }
    }

    return false;
}

int main(){
    // g++ frogs.cpp -o frogs.exe
    // ./frogs.exe

    // no printing -> N=25 ~ 2.03 sec
    // printing    -> N=25 ~ 6.05 sec
    
    std::cin >> N;
    if(!std::cin || N <= 0){ return -1; }

    char *board = new char[2*N+1]; // 'L' [0, N-1]; '_' [N]; 'R' [N+1, 2*N];
    
    for(size_t i = 0; i < N; ++i){
        board[i] = 'L';
        board[N+1+i] = 'R';
    }
    board[N] = '_';

    auto start = std::chrono::high_resolution_clock::now();

    solve(board, N);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration<double>(stop - start).count() << " seconds" << std::endl;



    return 0;
}