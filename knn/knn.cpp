#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cmath>
#include <ctime>
#include <algorithm>

const int _SIZE = 5; // dimensions -> sepal values: x1, y1; petal values: x2, y2; classLabel
const int _FILESIZE = 150; // training elements: 9 sets * 15 elements = 135 | testing elements: 1 set * 15 elements = 15 

std::vector<std::vector<double>> readFile(const std::string &filename){
    std::ifstream in(filename);

    if(!in.is_open()){ return {{-1}}; }
    
    std::vector<std::vector<double>> result;
    char c;

    for(size_t i = 0; i < _FILESIZE; ++i){
        std::vector<double> v(5);
        for(size_t j = 0; j < _SIZE - 1; ++j){
            in >> v[j];
            in.get();
        }

        c = in.get();
        while(c == 'I' || (c >= 'a' && c <= 'z') || c == '-'){
            c = in.get();
        }

        // sequence preset in file
        if(i >= 0 && i < 50){
            v[4] = 1;
        }else if(i >= 50 && i < 100){
            v[4] = 2;
        }else if(i >= 100 && i < 150){
            v[4] = 3;
        }

        result.push_back(v);
    }
    
    return result;
}

struct Node{
    std::vector<double> point;
    Node *left, *right;
};

Node* newNode(std::vector<double> point){
    Node *tmp = new Node;

    tmp->point = point;
    tmp->left = tmp->right = nullptr;

    return tmp;
}

Node* insertRec(Node *root, std::vector<double> point, size_t depth){
    if(root == nullptr){
        return newNode(point);
    }

    size_t cd = depth % (_SIZE - 1); // skip last dimension with class value

    if(point[cd] < root->point[cd]){ 
        root->left = insertRec(root->left, point, depth + 1);
    }else{
        root->right = insertRec(root->right, point, depth + 1);
    }

    return root;
}

Node* insert(Node *root, std::vector<double> point){
    return insertRec(root, point, 0);
}

void clear(Node *&root){
    if(root == nullptr){
        return;
    }

    clear(root->left);
    clear(root->right);

    delete root;
    root = nullptr;
}

struct Compare{
    bool operator()(std::pair<int, double> a, std::pair<int, double> b){
        return a.second < b.second;
    }
};

double distance(double x1, double y1, double x2, double y2){
    return std::sqrt(((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)));
}

void kNN(Node *root, std::vector<double> point, size_t dimension,
        std::priority_queue<std::pair<size_t, double>, std::vector<std::pair<size_t, double>>, Compare> &pq){
    if(root == nullptr){
        return;
    }
    
    double dist = distance(root->point[dimension], root->point[dimension + 1], point[dimension], point[dimension + 1]);
    pq.push(std::make_pair(root->point[4], dist));

    kNN(root->left, point, dimension, pq);
    kNN(root->right, point, dimension, pq);
}

size_t kNNClassify(Node *root, const std::vector<std::vector<double>> &data, size_t testIndex, size_t k, size_t dimension){
    std::priority_queue<std::pair<size_t, double>, std::vector<std::pair<size_t, double>>, Compare> pq;    
    
    std::vector<double> test = data[testIndex];
    kNN(root, test, dimension, pq);

    for(size_t i = 0; i < (_FILESIZE - 15) - k; ++i){
        std::pair<size_t, double> tmp = pq.top();
        pq.pop();
    }

    // 3 classes: 1,2,3
    size_t count1 = 0;
    size_t count2 = 0;
    size_t count3 = 0;

    // index of first encounter of each class
    size_t index1 = _FILESIZE;
    size_t index2 = _FILESIZE;
    size_t index3 = _FILESIZE;

    for(size_t i = 0; i < k; ++i){
        std::pair<size_t, double> tmp = pq.top(); // (classLabel, distance)
        pq.pop();

        if(tmp.first == 1){
            ++count1;
            if(index1 > i){ index1 = i; }
        }else if(tmp.first  == 2){
            ++count2;
            if(index2 > i){ index2 = i; }
        }else if(tmp.first  == 3){
            ++count3;
            if(index3 > i){ index3 = i; }
        }
    }

    if(count1 > count2){
        if(count1 > count3){
            return 1;
        }else if(count1 == count3){
            return (index1 <= index3) ? 1 : 3;
        }else{
            return 3;
        }
    }else if(count1 == count2){
        if(count1 > count3){
            return (index1 <= index2) ? 1 : 2;
        }else if(count1 == count3){
            return (index1 <= index2) ? ((index1 <= index3) ? 1 : 3) : ((index2 <= index3) ? 2 : 3);
        }else{
            return 3;
        }
    }else if(count1 < count2){
        if(count2 > count3){
            return 2;
        }else if(count2 == count3){
            return (index2 <= index3) ? 2 : 3;
        }else{
            return 3;
        }
    }

    return 0;
}

int main(){
    // g++ knn.cpp -o knn.exe
    // ./knn.exe

    // sepal(x1,y1), petal(x2,y2), classLabel -> 5 values on a line
    std::vector<std::vector<double>> data = readFile("iris.txt");
    if(data[0][0] == -1){ return -1; }

    std::srand(std::time(0));
    std::random_shuffle(data.begin(), data.end());

    bool flag = false;
    std::cout << "set value for k?" << std::endl;
    std::cin >> flag;
    if(!std::cin){ return -1; }

    int k;
    if(flag){
        std::cout << "k: ";
        std::cin >> k;
        if(!std::cin || k <= 0 || k > _FILESIZE - 15){ return -1; }
    }

    std::vector<size_t> indices(15);
    double successPercentage = 0;
    double maxPercetage = 0;
    size_t maxK = 0;

    for(size_t p = 1; p <= _FILESIZE/2; ++p){
        if(!flag){ k = p; }

        // 10 cross-validations -> expected mean value ~ 96%
        for(size_t i = 0; i < 10; ++i){
            double successCount = 0;

            for(size_t j = 0; j < indices.size(); ++j){
                indices[j] = 15*i + j;
            }

            // build tree
            Node *root = nullptr;
            for(size_t index = 0; index < _FILESIZE; ++index){                
                if(index < 15*i || index >= 15*i + indices.size()){
                    root = insert(root, data[index]);
                }
            }

            for(size_t j = 0; j < indices.size(); ++j){
                size_t sepal = kNNClassify(root, data, indices[j], k, 0);
                size_t petal = kNNClassify(root, data, indices[j], k, 2);
                if(sepal == data[indices[j]][4] && petal == data[indices[j]][4]){
                    ++successCount;
                }
            }
            // destroy tree
            clear(root);
            
            successCount = (successCount * 100) / 15;
            successPercentage += successCount;

            if(flag){ std::cout << successCount << "% | "; }
        }

        successPercentage /= 10;
        
        if(flag){
            std::cout << std::endl;
            std::cout << "arithmetic mean = " << successPercentage << "%" << std::endl;
            break;
        }

        if(successPercentage > maxPercetage){ 
            maxPercetage = successPercentage; 
            maxK = p; 
            std::cout << maxK << " ";
        }
    }
    
    if(!flag){ std::cout << "\nbest k value = " << maxK << " | arithmetic mean = " << maxPercetage << "%" << std::endl; }



    return 0;
}