#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

// informed search algorithms:
//  - best-first search
//    + greedy
//      > beam search
//      > hill climbing
//    + A*
//      > memory-bounded A*
//      > iterative deepening A*

template<class T>
struct Node{
    T data;
    std::vector<std::pair<Node<T>*,int>> children;
    int heuristicValue;
    bool visited = false;

    Node(T _data, int _heuristicValue) : data(_data), heuristicValue(_heuristicValue) {}
};

template<class T>
struct CompareNodes{
    bool operator() (Node<T> *n1, Node<T> *n2){
        return n1->heuristicValue > n2->heuristicValue;
    }
};

template<class T>
struct ComparePairs{
    bool operator() (std::pair<Node<T>*,int> p1, std::pair<Node<T>*,int> p2){
        return p1.first->heuristicValue + p1.second > p2.first->heuristicValue + p2.second;
    }
};

template<class T>
bool comparePairs(std::pair<Node<T>*,int> p1, std::pair<Node<T>*,int> p2){
    return p1.first->heuristicValue < p2.first->heuristicValue;
}

template<class T>
void greedy(Node<T> *root, Node<T> *goal){
    std::priority_queue<Node<T>*, std::vector<Node<T>*>, CompareNodes<T>> pq; // (node, cost)

    pq.push(root);
    root->visited = true;

    while(!pq.empty()){
        Node<T> *crr = pq.top();
        std::cout << crr->data << ", " << crr->heuristicValue << std::endl; 
        pq.pop();

        if(crr == goal){
            return;
        }

        for(size_t i = 0; i < crr->children.size(); ++i){
            if(!crr->children[i].first->visited){
                crr->children[i].first->visited = true;
                pq.push(crr->children[i].first);
            }
        }
    }
}

template<class T>
void beam_search(Node<T> *root, Node<T> *goal, int limit){
    if(limit <= 0){
        return;
    }

    std::priority_queue<Node<T>*, std::vector<Node<T>*>, CompareNodes<T>> pq; // (node, cost)

    pq.push(root);
    root->visited = true;

    while(!pq.empty()){
        Node<T> *crr = pq.top();
        //std::cout << pq.size() << std::endl;
        std::cout << crr->data << ", " << crr->heuristicValue << std::endl; 
        pq.pop();

        if(crr == goal){
            return;
        }

        std::sort(crr->children.begin(), crr->children.end(), comparePairs<T>);
        int diff = limit - pq.size();
        
        //for(size_t i = 0; i < crr->children.size(); ++i){ std::cout << crr->children[i].first->heuristicValue << " "; }

        for(size_t i = 0; i < crr->children.size() && diff > 0; ++i){
            if(!crr->children[i].first->visited){
                crr->children[i].first->visited = true;
                pq.push(crr->children[i].first);
                --diff;
            }
        }

        //std::cout << diff << std::endl;
    }
}

template<class T>
void hill_climbing(Node<T> *root, Node<T> *goal){
    std::priority_queue<Node<T>*, std::vector<Node<T>*>, CompareNodes<T>> pq; // (node, cost)

    pq.push(root);
    root->visited = true;

    while(!pq.empty()){
        Node<T> *crr = pq.top();
        std::cout << crr->data << ", " << crr->heuristicValue << std::endl; 
        pq.pop();

        if(crr == goal){
            return;
        }

        Node<T> *max = new Node<T>('*',INT_MAX);

        for(size_t i = 0; i < crr->children.size(); ++i){
            if(!crr->children[i].first->visited &&
                crr->children[i].first->heuristicValue < max->heuristicValue){
                max = crr->children[i].first;
            }
        }

        if(max->heuristicValue == INT_MAX){
            return;
        }else{
            max->visited = true;
            pq.push(max);
        }
    }
}

template<class T>
void A_star(Node<T> *root, Node<T> *goal){
    std::priority_queue<std::pair<Node<T>*,int>, 
                        std::vector<std::pair<Node<T>*,int>>, 
                        ComparePairs<T>> pq; // (node, cost)

    pq.push(std::make_pair(root,0));
    root->visited = true;

    while(!pq.empty()){
        std::pair<Node<T>*,int> crr = pq.top();
        std::cout << crr.first->data << ", " << crr.second << "+" << crr.first->heuristicValue << std::endl;
        pq.pop();

        if(crr.first == goal){
            return;
        }

        for(size_t i = 0; i < crr.first->children.size(); ++i){
            if(!crr.first->children[i].first->visited){
                crr.first->children[i].first->visited = true;
                pq.push(std::make_pair(crr.first->children[i].first, 
                                       crr.first->children[i].second + crr.second));
            }
        }
    }
}

int main(){
    // g++ algorithms3.cpp -o algorithms3.exe
    // ./algorithms3.exe

    Node<char> *z = new Node<char>('Z', 11);
    Node<char> *s = new Node<char>('S', 8);
    Node<char> *a = new Node<char>('A', 7);
    Node<char> *b = new Node<char>('B', 5); 
    Node<char> *c = new Node<char>('C', 4);
    Node<char> *d = new Node<char>('D', 6); 
    Node<char> *e = new Node<char>('E', 6); 
    Node<char> *f = new Node<char>('F', 3);
    Node<char> *g = new Node<char>('G', 0);
    Node<char> *h = new Node<char>('H', 9);
    Node<char> *i = new Node<char>('I', 8);
    Node<char> *j = new Node<char>('J', 7);
    Node<char> *k = new Node<char>('K', 8);

    z->children.push_back(std::make_pair(s,3));

    s->children.push_back(std::make_pair(z,3));
    s->children.push_back(std::make_pair(a,2));
    s->children.push_back(std::make_pair(b,3));

    a->children.push_back(std::make_pair(s,2));
    a->children.push_back(std::make_pair(c,4));

    c->children.push_back(std::make_pair(a,4));
    c->children.push_back(std::make_pair(g,4));

    g->children.push_back(std::make_pair(c,4));
    g->children.push_back(std::make_pair(f,3));

    f->children.push_back(std::make_pair(g,3));
    f->children.push_back(std::make_pair(e,3));

    e->children.push_back(std::make_pair(d,2));
    e->children.push_back(std::make_pair(f,3));
    e->children.push_back(std::make_pair(j,2));

    j->children.push_back(std::make_pair(e,2));
    j->children.push_back(std::make_pair(k,2));

    k->children.push_back(std::make_pair(j,2));

    d->children.push_back(std::make_pair(b,2));
    d->children.push_back(std::make_pair(e,2));
    d->children.push_back(std::make_pair(i,3));
    d->children.push_back(std::make_pair(h,3));

    b->children.push_back(std::make_pair(d,2));
    b->children.push_back(std::make_pair(s,3));

    h->children.push_back(std::make_pair(d,3));

    i->children.push_back(std::make_pair(d,3));

    //greedy(s,g);
    //beam_search(s,g,2);
    //hill_climbing(s,g);
    A_star(s,g);

    return 0;
}