#include <iostream>
#include <stack>
#include <queue>
#include <list>
#include <vector>
#include <map>

template<class T>
struct Node{
    T data;
    bool visited = false;
    Node<T> *parent = nullptr;
    std::list<Node<T>*> children;

    Node(T _data, Node<T> *_parent) : data(_data), parent(_parent) {}
};

template<class T>
void dfs_graph(Node<T> *root, Node<T> *goal){
    if(root == nullptr){
        return;
    }
    
    std::stack<Node<T>*> s;
    s.push(root);
    root->visited = true;
    //std::cout << root->data << " ";

    while(!s.empty()){
        Node<T> *crr = s.top();
        
        if(crr->children.empty()){
            s.pop();
        }else{
            if(!crr->children.front()->visited){
                //std::cout << crr->children.front()->data << " ";
                crr->children.front()->visited = true;
                Node<T> *tmp = crr->children.front();
                s.push(tmp);
            }

            if(crr->children.front() == goal){
                std::stack<Node<T>*> r;
                while(!s.empty()){
                    Node<T> *tmp = s.top();
                    r.push(tmp);
                    s.pop();
                }
                while(!r.empty()){
                    Node<T> *tmp = r.top();
                    std::cout << tmp->data << " ";
                    r.pop();
                }
                return;
            }

            crr->children.pop_front();
        }       
    }
}

template<class T>
void bfs_graph(Node<T> *root, Node<T> *goal){
    if(root == nullptr){
        return;
    }

    std::queue<Node<T>*> q;
    q.push(root);

    while(!q.empty()){
        Node<T> *crr = q.front();
        //std::cout << crr->data << " ";
        q.pop();

        for(Node<T> *child : crr->children){
            if(child == goal){
                std::stack<T> r;
                r.push(child->data);
                
                while(crr != nullptr){
                    r.push(crr->data);
                    crr = crr->parent;
                }
                while(!r.empty()){
                    T tmp = r.top();
                    std::cout << tmp << " ";
                    r.pop();
                }
                return;
            }

            if(!child->visited){
                child->visited = true;
                q.push(child);
            }
        }
    }
}

template<class T>
int uniform_cost_search(std::map<T, std::vector<T>> &graph, std::map<std::pair<T,T>, int> &cost, T start, T goal){
    int result = INT_MAX;

    std::priority_queue<std::pair<T,int>, 
                        std::vector<std::pair<T,int>>, 
                        std::greater<std::pair<T,int>>> q; // (node, cost)
    std::map<T,bool> visited;                              // (node, visited -> bool)

    q.push(std::make_pair(start,0));

    while(!q.empty()){
        std::pair<T,int> p = q.top();
        //std::cout << p.first << " " << p.second << std::endl;
        q.pop();

        if(p.first == goal){
            if(result > p.second){
                result = p.second;
            }

            return result;
        }

        if(!visited[p.first]){
            for(size_t i = 0; i < graph[p.first].size(); ++i){
                q.push(std::make_pair(graph[p.first][i], (p.second + cost[std::make_pair(p.first, graph[p.first][i])])));
            }

            visited[p.first] = true;
        }
    }

    return result;
}

int main(){
    // g++ algorithms2.cpp -o algorithms2.exe
    // ./algorithms2.exe

/*
    Node<char> *a = new Node<char>('A', nullptr);
    Node<char> *b = new Node<char>('B', a); 
    Node<char> *s = new Node<char>('S', a);
    Node<char> *c = new Node<char>('C', s);
    Node<char> *g = new Node<char>('G', s);
    Node<char> *d = new Node<char>('D', c); 
    Node<char> *e = new Node<char>('E', c); 
    Node<char> *f = new Node<char>('F', c);
    Node<char> *h = new Node<char>('H', g);

    a->children.push_back(b);
    a->children.push_back(s);

    s->children.push_back(a);
    s->children.push_back(c);
    s->children.push_back(g);
    
    c->children.push_back(d);
    c->children.push_back(e);
    c->children.push_back(f);
    c->children.push_back(s);

    g->children.push_back(h);
    g->children.push_back(f);
    g->children.push_back(s);
    
    d->children.push_back(c);

    e->children.push_back(c);
    e->children.push_back(h);

    h->children.push_back(e);
    h->children.push_back(g);
    
    f->children.push_back(g);
    f->children.push_back(c);

    //dfs_graph(a, f);
    //bfs_graph(a, f);
*/

/*
    std::map<char, std::vector<char>> graph{{'S', {'A','B','C'}}, 
                                            {'A', {'S','B','D'}},
                                            {'B', {'S','A','D','C'}},
                                            {'C', {'S','B','G'}},
                                            {'D', {'A','B','G'}},
                                            {'G', {'C','D'}}};

    std::map<std::pair<char, char>, int> cost{{{'S','A'}, 7}, {{'S','B'}, 9}, {{'S','C'}, 14},
                                              {{'A','S'}, 7}, {{'A','B'}, 10}, {{'A','D'}, 15},
                                              {{'B','S'}, 9}, {{'B','A'}, 10}, {{'B','D'}, 11}, {{'B','C'}, 2},
                                              {{'C','S'}, 14}, {{'C','B'}, 2}, {{'C','G'}, 9},
                                              {{'D','A'}, 15}, {{'D','B'}, 11}, {{'D','G'}, 6},
                                              {{'G','C'}, 9}, {{'G','D'}, 6}};

    std::cout << uniform_cost_search(graph, cost, 'S', 'G') << std::endl;
*/

    return 0;
}