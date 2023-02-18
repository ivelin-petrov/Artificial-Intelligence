#include <iostream>
#include <stack>
#include <queue>

// dfs -> stack (LIFO)
// bfs -> queue (FIFO)

template<class T>
struct Node {
    T data;
    Node<T> *left, *right;
    
    Node(T _data, Node<T> *_left, Node<T> *_right) : data(_data), left(_left), right(_right) {}
};

template<class T>
void dfs(Node<T> *root){    // root, left, right
    if(root == nullptr){
        return;
    }

    std::stack<Node<T>*> s;
    s.push(root);

    while(!s.empty()){
        Node<T> *crr = s.top();
        s.pop();

        if(crr->right != nullptr){
            s.push(crr->right);
        }
        if(crr->left != nullptr){
            s.push(crr->left);
        }
        std::cout << crr->data << " ";

    }
    std::cout << std::endl;
}

template<class T>
void dfs_2(Node<T> *root){  // left, root, right
    if(root == nullptr){
        return;
    }

    std::stack<Node<T>*> s;
    Node<T> *crr = root;

    while(crr != nullptr || !s.empty()){
        while(crr != nullptr){
            s.push(crr);
            crr = crr->left;
        }

        crr = s.top();
        s.pop();

        std::cout << crr->data << " ";
        crr = crr->right;
    }
}

template<class T>
void dfs_recursive(Node<T> *root){
    if(root == nullptr){
        return;
    }

    // root, left, right
    dfs_recursive(root->left);
    // left, root, right
    std::cout << root->data << " ";
    dfs_recursive(root->right);
    // left, right, root
}

template<class T>
void bfs(Node<T> *root){
    if(root == nullptr){
        return;
    }

    std::queue<Node<T>*> q;
    q.push(root);

    while(!q.empty()) {
        Node<T> *crr = q.front();
        
        if(crr->left != nullptr){
            q.push(crr->left);
        }
        if(crr->right != nullptr){
            q.push(crr->right);
        }
        
        std::cout << crr->data << " ";
        q.pop();
    }
    std::cout << std::endl;
}

template<class T>
bool depth_limited_search(Node<T> *root, T goal, int limit){
    if(root == nullptr || limit < 0){
        return false;
    }

    if(root->data == goal){
        std::cout << "goal node found" << std::endl;
        return true;
    }

    return depth_limited_search(root->left, goal, limit-1) ||
           depth_limited_search(root->right, goal, limit-1);
}

template<class T>
bool iterative_deepening_search(Node<T> *root, T goal, int max_depth){
    for(size_t i = 0; i <= max_depth; ++i){
        if(depth_limited_search(root, goal, i)){
            return true;
        }
    }

    return false;
}

int main(){
    // g++ algorithms.cpp -o algorithms.exe
    // ./algorithms.exe

    //     1
    //    / \
    //   2   3
    //  / \   \
    // 4   5   6

/*
    Node<int> *root = new Node<int>(1, new Node<int>(2, new Node<int>(4, nullptr, nullptr), 
                                                        new Node<int>(5, nullptr, nullptr)),
                                       new Node<int>(3, nullptr,
                                                        new Node<int>(6, nullptr, nullptr)));
    std::cout << "dfs: "; 
    dfs(root);

    //dfs_2(root);    
    //std::cout << std::endl;
    
    //dfs_recursive(root);
    //std::cout << std::endl;

    std::cout << "bfs: "; 
    bfs(root);

    depth_limited_search(root, 6, 2);
    iterative_deepening_search(root, 5, 3);
*/

    return 0;
}