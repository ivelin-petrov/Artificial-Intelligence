#include <iostream>
#include <ctime>
#include <chrono>

int *queens;		// values: from 0 to N-1
int *queensPerRow;	// queens[col] = row
int *queensPerD1;	// '\'
int *queensPerD2;	// '/'

size_t N;

void printQueens(){
	for(size_t i = 0; i < N; ++i){
		for(size_t j = 0; j < queens[i]; ++j){
			std::cout << "- ";
		}
		std::cout << "* ";
		for(size_t j = 0; j < N - queens[i] - 1; ++j){
			std::cout << "- ";
		}
		std::cout << std::endl;
	}
}

void removeQueen(int col){
	queensPerRow[queens[col]] -= 1;
	queensPerD1[(N - 1) + (col - queens[col])] -= 1;
	queensPerD2[col + queens[col]] -= 1;
}

void placeQueen(int col, int row){
	queens[col] = row;

	queensPerRow[queens[col]] += 1;
	queensPerD1[(N - 1) + (col - queens[col])] += 1;
	queensPerD2[col + queens[col]] += 1;
}

// each queen to every other -> (N * (N-1)) / 2
int countConflictsCol(int col){
	return  (((queensPerRow[queens[col]] - 1) * queensPerRow[queens[col]]) / 2) + 
			(((queensPerD1[N - 1 + (col - queens[col])] - 1) * queensPerD1[N - 1 + (col - queens[col])]) / 2) + 
			(((queensPerD2[col + queens[col]] - 1) * queensPerD2[col + queens[col]]) / 2);
}

// each queen to every other -> (N * (N-1)) / 2
int countConflictsRow(int row, int maxConfCol){
	return (((queensPerRow[row] + 1) * queensPerRow[row]) / 2) +
		   (((queensPerD1[(N - 1) + (maxConfCol - row)] + 1) * queensPerD1[(N - 1) + (maxConfCol - row)]) / 2) + 
		   (((queensPerD2[row + maxConfCol] + 1) * queensPerD2[row + maxConfCol]) / 2);
}

int maxConflictsCol(int maxCC[]){
	int max = 0;
	int tmp = 0;
	int size = 0;

	for(size_t i = 0; i < N; ++i){
		tmp = countConflictsCol(i);
		if(tmp > max){
			max = tmp;

			size = 0;
			maxCC[size] = i;
			++size;
		}else if(tmp == max){
			maxCC[size] = i;
			++size;
		}
	}

	return maxCC[std::rand() % size];
}

int minConflictsRow(int maxConfCol, int minCR[]){
	int min = INT_MAX;
	int tmp = 0;
	int size = 0;

	removeQueen(maxConfCol);

	for(size_t i = 0; i < N; ++i){
		tmp = countConflictsRow(i, maxConfCol);
		if(tmp < min){
			min = tmp;

			size = 0;
			minCR[size] = i;
			++size;
		}else if(tmp == min){
			minCR[size] = i;
			++size;
		}
	}

	return minCR[std::rand() % size];
}

void initialise(int minCR[]){
	int min = INT_MAX;
	int tmp = 0;
	int size = 0;

	// place first queen randomly
	placeQueen(0, std::rand() % N);

	// i -> col
	for(size_t i = 1; i < N; ++i){
		size = 0;
		min = INT_MAX;

		// j -> row
		for(size_t j = 0; j < N; ++j){
			tmp = countConflictsRow(j, i);
			if(tmp < min){
				min = tmp;

				size = 0;
				minCR[size] = j;
				++size;
			}else if(tmp == min){
				minCR[size] = j;
				++size;
			}
		}

		placeQueen(i, minCR[std::rand() % size]);
	}
}

bool hasConflicts(int maxCC[]){
	return countConflictsCol(maxConflictsCol(maxCC)) != 0;
}

void clear(int *&array){
	delete[] array;
	array = nullptr;
}

int main(){
	// g++ nqueensFaster.cpp -o nqueensFaster.exe
	// ./nqueensFaster.exe

	std::cin >> N;
	if(!std::cin || N < 4){
		return -1;
	}

	queens = new int[N];
	if(queens == nullptr){ return -1; }
	queensPerRow = new int[N];
	if(queensPerRow == nullptr){ return -1; }
	queensPerD1 = new int[2*N-1];
	if(queensPerD1 == nullptr){ return -1; }
	queensPerD2 = new int[2*N-1];
	if(queensPerD2 == nullptr){ return -1; }

	for(size_t i = 0; i < 2*N-1; ++i){
		if(i < N){
			queens[i] = 0;
			queensPerRow[i] = 0;
		}
		queensPerD1[i] = 0;
		queensPerD2[i] = 0;
	}

	//maxConflictsCol
	int *maxCC = new int[N];
	if(maxCC == nullptr){ return -1; }
	// minConflictsRow
	int *minCR = new int[N];
	if(minCR == nullptr){ return -1; }

	auto start = std::chrono::high_resolution_clock::now();

	std::srand(std::time(0));
	initialise(minCR);

	auto stop = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration<double>(stop - start).count() << " seconds" << std::endl;

	if(N <= 80){
		std::cout << "initial state:" << std::endl;
		printQueens();
	}

	int count = 0;
	int col = 0;
	int row = 0;

	start = std::chrono::high_resolution_clock::now();

	while(hasConflicts(maxCC)){
		col = maxConflictsCol(maxCC);
		row = minConflictsRow(col, minCR);
		placeQueen(col, row);

		++count;
		if(count >= N){
			int resetCol = std::rand() % N;
			removeQueen(resetCol);

			int resetRow = std::rand() % N;
			placeQueen(resetCol, resetRow);
			
			count = 0;
		}
	}

	stop = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration<double>(stop - start).count() << " seconds" << std::endl;

	if(N <= 80){
		std::cout << "result:" << std::endl;
		printQueens();
	}

	clear(queens);
	clear(queensPerRow);
	clear(queensPerD1);
	clear(queensPerD2);
	clear(maxCC);
	clear(minCR);

	return 0;
}
