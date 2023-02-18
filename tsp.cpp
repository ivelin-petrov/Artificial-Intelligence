#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <string>

size_t N;
size_t populationSize;

template<class T>
struct Individual{
    std::vector<T> chromosome;
    int fitness;

    Individual() { chromosome.resize(N); }
    Individual(const std::vector<T> &_chromosome) : chromosome(_chromosome) {}

    Individual& operator= (const Individual<T>& other){
        if(this != &other){
            this->chromosome = other.chromosome;
            this->fitness = other.fitness;
        }
        return *this;
    }

    size_t randomIndex(){
        return std::rand() % this->chromosome.size();
    }

    bool hasGene(T elem){
        for(size_t i = 0; i < N; ++i){
            if(this->chromosome[i] == elem){
                return true;
            }
        }

        return false;
    }
    
    // swap mutation
    void mutateGenes(){
        size_t i1 = randomIndex();
        size_t i2 = randomIndex();

        if(i1 != i2){
            T tmp = this->chromosome[i1];
            this->chromosome[i1] = this->chromosome[i2];
            this->chromosome[i2] = tmp;
        }
    }

    // one-point crossover
    Individual crossover(size_t i, const Individual &other){
        Individual result;

        for(size_t k = 0; k < i; ++k){
            result.chromosome[k] = this->chromosome[k];
        }
        size_t size = i;

        for(size_t i = 0; i < N && size < N; ++i){
            if(!result.hasGene(other.chromosome[i])){
                result.chromosome[size] = other.chromosome[i];
                ++size;
            }
        }

        return result;
    }
};

template<class T>
bool lessThan(const Individual<T> &i1, const Individual<T> &i2){
    return i1.fitness < i2.fitness; // firstIndex -> shortest distance
}

template<class T>
bool hasElem(const std::vector<T> &v, T elem){
    for(size_t i = 0; i < v.size(); ++i){
        if(v[i] == elem){
            return true;
        }
    }

    return false;
}

template<class T>
std::vector<T> generateChromosome(){
    std::vector<T> result;

    while(result.size() < N){
        T tmp = std::rand() % N;
        tmp += 1;

        if(!hasElem(result, tmp)){
            result.push_back(tmp);
        }
    }

    return result;
}

template<class T>
void calculateFitness(std::map<T,std::vector<int>> &graph, Individual<T> &ind){
    int result = 0;

    for(size_t i = 0; i < ind.chromosome.size() - 1; ++i){
        if(graph[ind.chromosome[i]][ind.chromosome[i + 1] - 1] == USHRT_MAX){
            ind.fitness = USHRT_MAX;
            return;
        }
        result += graph[ind.chromosome[i]][ind.chromosome[i + 1] - 1];
    }

    ind.fitness = result;
}

template<class T>
void printGeneration(const std::vector<Individual<T>> &population, size_t size){
    for(size_t i = 0; i < size; ++i){
        for(size_t j = 0; j < N; ++j){
            std::cout << population[i].chromosome[j] << " ";
        }
        std::cout << "| " << population[i].fitness << std::endl;
    }
}

template<class T>
void solve(std::map<T,std::vector<int>> &graph, const int generations){
    if(generations <= 0 || populationSize < 2 || N <= 1){
        return;
    }

    std::vector<Individual<T>> population;
    
    // first generation
    for(size_t i = 0; i < populationSize; ++i){
        Individual<T> tmp(generateChromosome<T>());
        calculateFitness<T>(graph, tmp);
        population.push_back(tmp);
    }

    std::sort(population.begin(), population.end(), lessThan<T>);

    for(size_t i = 0; i < generations; ++i){
        std::cout << "generation " << i + 1 << std::endl;
        //printGeneration(population, populationSize);
        printGeneration(population, 1);

        size_t index = N/2;
        size_t eliteSize = std::ceil(populationSize * 0.3);

        Individual<T> child1, child2;
        size_t parent1, parent2;

        for(size_t k = populationSize - eliteSize; k > 0; --k){
            do{
                parent1 = std::rand() % eliteSize;
                parent2 = eliteSize + (std::rand() % (populationSize - eliteSize));
            }while(parent1 == parent2);

            child1 = population[parent1].crossover(index + 1, population[parent2]);
            child1.mutateGenes();
            calculateFitness<T>(graph, child1);
            population.push_back(child1);

            child2 = population[parent2].crossover(index - 1, population[parent1]);
            child2.mutateGenes();
            calculateFitness<T>(graph, child2);
            population.push_back(child2);
        }

        std::sort(population.begin(), population.end(), lessThan<T>);

        for(size_t k = population.size() - populationSize; k > 0; --k){
            population.pop_back();
        }

        std::cout << std::endl;
    }
}

template<class T>
void generateGraph(std::map<T,std::vector<int>> &simpleGraph, size_t N){
    std::map<std::pair<int,int>,std::vector<int>> graph;
    int x, y, dist;

    while(graph.size() < N){
        x = std::rand() % N;
        y = std::rand() % N;
        graph[std::make_pair(x,y)];
    }

    // point1.first -> (first,second) -> (x1,y1)
    // point2.first -> (first,second) -> (x2,y2)

    for(const auto &point1: graph){
        for(const auto &point2 : graph){
            dist = (point2.first.first - point1.first.first) * (point2.first.first - point1.first.first) +
                   (point2.first.second - point1.first.second) * (point2.first.second - point1.first.second);
            graph[point1.first].push_back(std::sqrt(dist));
        }
    }

  /*
    for(const auto &point : graph){
        std::cout << "(" << point.first.first << "," << point.first.second << ") -> ";
        for(size_t i = 0; i < point.second.size(); ++i){
            std::cout << point.second[i] << " ";
        }
        std::cout << std::endl;
    }
  */

    size_t size = 1;

    for(const auto &point : graph){
        simpleGraph[size] = point.second;
        ++size;
    }

  /*
    for(size_t i = 1; i < size; ++i){
        std::cout << i << " -> ";
        for(size_t j = 0; j < simpleGraph[i].size(); ++j){
            std::cout << simpleGraph[i][j] << " ";
        }
        std::cout << std::endl;
    }
  */
}

void calculateCities(std::map<int,std::vector<int>> &result){
    std::map<std::string,std::pair<double,double>> cities;

    cities["Aberystwyth"] = {0.190032E-03, -0.285946E-03};  // 1
    cities["Brighton"] = {383.458, -0.608756E-03};          // 2
    cities["Edinburgh"] = {-27.0206, -282.758};             // 3
    cities["Exeter"] = {335.751, -269.577};                 // 4
    cities["Glasgow"] = {69.4331, -246.780};                // 5
    cities["Inverness"] = {168.521, 31.4012};               // 6
    cities["Liverpool"] = {320.350, -160.900};              // 7
    cities["London"] = {179.933, -318.031};                 // 8
    cities["Newcastle"] = {492.671, -131.563};              // 9
    cities["Nottingham"] = {112.198, -110.561};             // 10
    cities["Oxford"] = {306.320, -108.090};                 // 11
    cities["Stratford"] = {217.343, -447.089};              // 12

    double dist;

    // point1.second -> (first,second) -> (x1,y1)
    // point2.second -> (first,second) -> (x2,y2)

    size_t size = 1;

    for(const auto &c1: cities){
        for(const auto &c2 : cities){
            dist = (c2.second.first - c1.second.first) * (c2.second.first - c1.second.first) +
                   (c2.second.second - c1.second.second) * (c2.second.second - c1.second.second);
            result[size].push_back(int(std::sqrt(dist)));
        }
        ++size;
    }

  /*
    for(const auto &city : result){
        std::cout << city.first << " -> ";
        for(size_t i = 0; i < city.second.size(); ++i){
            std::cout << city.second[i] << " ";
        }
        std::cout << std::endl;
    }
  */
}

int main(){
    // g++ tsp.cpp -o tsp.exe
    // ./tsp.exe

    std::srand(std::time(0));

    std::cin >> N;
    if(!std::cin){ return -1; }

    //std::cin >> populationSize; // = 20
    //if(!std::cin){ return -1; }
  
    populationSize = 100;
    std::map<int,std::vector<int>> graph;
    generateGraph<int>(graph, N);
    solve(graph, 50); // generations = 20

  /*
    populationSize = 500;
    std::map<int,std::vector<int>> cities;
    calculateCities(cities);
    solve(cities, 35);

    // Aberystwyth (1) -> Inverness (6) -> Nottingham (10) -> Glasgow (5) -> Edinburgh (3) -> London (8) 
    //  -> Stratford (12) -> Exeter (4) -> Liverpool (7) -> Oxford (11) -> Brighton (2) -> Newcastle (9)
    // (double) 1595.738522033024 ~ (int) 1588
  */

  /*
    std::map<int,std::vector<int>> graph;

    graph[1] = {0, 12, 10, USHRT_MAX, USHRT_MAX, USHRT_MAX, 12};
    graph[2] = {12, 0, 8, 12, USHRT_MAX, USHRT_MAX, USHRT_MAX};
    graph[3] = {10, 8, 0, 11, 3, USHRT_MAX, 9}; 
    graph[4] = {USHRT_MAX, 12, 11, 0, 11, 10, USHRT_MAX};
    graph[5] = {USHRT_MAX, USHRT_MAX, 3, 11, 0, 6, 7};
    graph[6] = {USHRT_MAX, USHRT_MAX, USHRT_MAX, 10, 6, 0, 9};
    graph[7] = {12, USHRT_MAX, 9, USHRT_MAX, 7, 9, 0};

    solve(graph, 20);
  */



    return 0;
}