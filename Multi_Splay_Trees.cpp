//Vrinda Dua(2021mcb1223), Ayush Raj(2021mcb1346), Edgar Aditya Thorpe(2021csb1169)
//Multi-Splay Tree implimentation

#include<bits/stdc++.h>
#include<chrono>
#include <iostream>
#include<map>
#include <string>
#include <cmath>
#include <limits>

using namespace std;
using namespace std::chrono;

//data structure for calculating running time
class timer{
    public:
        void start(void){ 
            //function to store the start time
            flag_one = high_resolution_clock::now(); return;
        }
        void stop(void){ //functioin to store the ending time
            flag_two = high_resolution_clock::now(); return;
        }
        void timeElapsed(void){ //function to display the total time taken
            auto duration = duration_cast<nanoseconds>(flag_two - flag_one);
            cout<<setprecision(2)<<fixed<<duration.count()/1000;
        }

    private:
        //variables to store the data
        std::chrono::_V2::system_clock::time_point flag_one,flag_two;
};


//data structure for the multisplay tree
class multiSplayTree{
    public:
        //constructor which would avoid any implicit type casting
        multiSplayTree(const int size);
        //destructor
        ~multiSplayTree(void);

        bool access(size_t key); //function to search an element in the tree
        void display(void){ //function to display the whole tree
            DispTree(root);
            return;
        }
    
    private:
        struct Node{ //stucture for the tree node
            //variables to store the required information
            size_t key, depth, minDepth;
            bool isRoot;
            Node *left, *right, *parent;
            //initialising the node values
            Node(size_t key, size_t depth = 0, size_t minDepth = 0, bool isRoot = 0){
                this->key = key;
                left = right = parent = 0;
                this->isRoot = isRoot;
                this->depth = depth;
                this->minDepth = minDepth;
            }
        };

        Node *root; //root of the whole multisplay tree
        Node *Create(size_t low, size_t high, size_t depth, bool isRoot); //function to create the whole tree
        void rotation(Node *cur); //function to perform the rotation operation 
        void splayOp(Node *cur, Node *top); //function to perform the standard splaying operation
        void DepthNode(Node *cur); //function to adjust the depths and mindepths values of the nodes
        void Transfer(Node *cur); //function to bring the current node to the root of the whole tree
        void DispTree(Node *cur); //auxiliary function to help display the whole tree
        Node *ReferencePar(Node *cur, int num); //function to return the first child whose mindepth value is greater than the depth value 
};

multiSplayTree::Node *multiSplayTree::Create(size_t low, size_t high, size_t depth, bool isRoot){
    //terminating condition
    if(low == high)  return nullptr;
    size_t mid = low + (high - low)/2;
    //allocating memory for the new node and initialising its values
    Node *tmp = new Node {mid, depth, depth, isRoot};
    //calling for the left subtree
    tmp->left = Create(low, mid, depth + 1, false);
    //calling for the right subtree
    tmp->right = Create(mid + 1, high, depth + 1, true);  
    if(tmp->left) tmp->left->parent = tmp;
    if(tmp->right) tmp->right->parent = tmp;
    return tmp;
}

multiSplayTree::multiSplayTree(const int size){
    root = Create(0, size, 0,  true);
    return;
}

multiSplayTree::~multiSplayTree(void){
    //iterating while root is not equal to nullptr
    while(root != nullptr){
        //if root does not have left child
        if(root->left == nullptr){
            Node *next = root->right;
            delete root; //why to delete root?
            root = next;
        }
        //if root have a left child
        else{
            Node *child = root->left;
            root->left = child->right;
            child->right = root;
            root = child;
        }
    }
    return;
}

void multiSplayTree::rotation(Node *cur){
    //pointer to store the parent of the current node
    Node *tmp = cur->parent;
    //if the parent is a root then the current node will become the root after the rotation
    if(tmp->isRoot){
        tmp->isRoot = false;
        cur->isRoot = true;
    }
    if(root == tmp) root = cur;
    //if there is a grandparent then perform the rotation
    if(tmp->parent){
        int cnt =  (tmp->parent->right == tmp);
        //if tmp is the left child of its parent
        if(cnt == 0){
            tmp->parent->left = cur;
        }
        //if tmp is the right child of its parent
        else{
            tmp->parent->right = cur;
        }
    }
    //update the current node's parent 
    cur->parent = tmp->parent;
    int cnt = (tmp->right == cur);
    //if current node is left child of tmp
    if(cnt == 0){
        //performing the rotation
        tmp->left = cur->right;
        if(cur->right){
            cur->right->parent = tmp;
        }
        cur->right = tmp;
        tmp->parent = cur;
    }
    //if current node is right child of tmp
    else{
        //performing the rotation
        tmp->right = cur->left;
        if(cur->left){
            cur->left->parent = tmp;
        }
        cur->left = tmp;
        tmp->parent = cur;
    }
    //updating the mindepths of the current node and tmp
    cur->minDepth = tmp->minDepth;
    tmp->minDepth = tmp->depth;
    if(tmp->left){ 
        tmp->minDepth = min(tmp->minDepth, tmp->left->minDepth);
    }
    if(tmp->right){
        tmp->minDepth = min(tmp->minDepth, tmp->right->minDepth);
    }
    return;
}

void multiSplayTree::splayOp(Node *cur, Node *top = 0){
    //rotate until current node is either a root or current node's parent is equal to top
    while(!(cur->isRoot || cur->parent == top)){
        Node *tmp = cur->parent;
        //if tmp is neither a root not its parent is equal to top
        if(!(tmp->isRoot || tmp->parent == top)){
            Node *tmp2 = tmp->parent;
            if((tmp2->left == tmp && tmp->left == cur) || (tmp2->right == tmp && tmp->right == cur)){
                rotation(tmp);
            }
            else{
                rotation(cur);
            }
        }
        //if tmp is either a root or its parent is equal to top
        rotation(cur);
    }
    return;
}

multiSplayTree::Node *multiSplayTree::ReferencePar(multiSplayTree::Node *cur, int cnt){
    //pointer to store the child
    Node *tmp = NULL;
    if(cnt == 0){
        tmp = cur->left;
    }
    else{
        tmp = cur->right;
    }
    //tmp is equal to left child of current node if cnt is equal to 0 and vice versa
    while(1){
        if(cnt == 0){
            //if mindepth of right child is less than the depth of current node
            if(tmp->right && tmp->right->minDepth < cur->depth){
                tmp = tmp->right;
            }
            //if midepth of left child is less than the depth of the current node
            else if(tmp->left && tmp->left->minDepth < cur->depth){
                tmp = tmp->left;
            }
            //breaking condition
            else{
                break;
            }
        }
        else{
            //if midepth of left child is less than the depth of the current node
            if(tmp->left && tmp->left->minDepth < cur->depth){
                tmp = tmp->left;
            }
            //if mindepth of right child is less than the depth of current node
            else if(tmp->right && tmp->right->minDepth < cur->depth){
                tmp = tmp->right;
            }
            //breaking condition
            else{
                break;
            }
        }
    }
    return tmp;
}

void multiSplayTree::DepthNode(Node *cur){
    //if current node has a left child
    if(cur->left){
        //if the mindepth of the left child is greater than the depth of the current node
        if(cur->left->minDepth > cur->depth){
            cur->left->isRoot = !(cur->left->isRoot);
        }
        else{
            Node *tmp = ReferencePar(cur, 0);
            splayOp(tmp, cur);
            if(tmp->right){
                tmp->right->isRoot = !(tmp->right->isRoot);
            }
        }
    }
    //if the current node has a right child
    if(cur->right){
        //if the mindepth of the right child is greater than the depth of the current node
        if(cur->right->minDepth > cur->depth){
            cur->right->isRoot = !(cur->right->isRoot);
        }
        else{
            Node *tmp2 = ReferencePar(cur, 1);
            splayOp(tmp2, cur);
            if(tmp2->left){
                tmp2->left->isRoot = !(tmp2->left->isRoot);
            }
        }
    }
}

void multiSplayTree::Transfer(Node *cur){
    //pointer to store the current node
    Node *tmp = cur;
    //iterating while tmp has a parent
    while(tmp->parent){
        Node *tmp2 = tmp->parent;
        //if tmp is a root
        if(tmp->isRoot){
            splayOp(tmp2);
            DepthNode(tmp2);
        }
        tmp = tmp2;
    }
    splayOp(cur);
    return;
}

bool multiSplayTree::access(size_t key){ 
    //pointers to store the current and previous nodes
    Node *curr = root;
    Node *prev = root;
    //iterate until we either find the key or find nullptr
    while(curr && curr->key != key){
        prev = curr;
        if(key < curr->key){
            curr = curr->left;
        }
        else{
            curr = curr->right;
        }
    }
    //if we found nullptr
    if(!curr){
        Transfer(prev);
        return false;
    }
    //if we found the key
    Transfer(curr);
    return true;
}
//function to run different test cases for the tree
void tester(string &type, int size){
    timer clk; //making a variable of the class timer
    clk.start(); 
    multiSplayTree tre(size); //creating a tree of the specified type
    clk.stop();
    cout<<"Access type: "<<type<<"\t\tSize: "<<size<<endl<<"\t\tBuild Time: ";
    clk.timeElapsed();
    cout<<"ns"<<endl<<"\t\t";
    //if the access sequence is sequential
    if(type == "Sequential"){
        clk.start();
        for(int i=0;i<size;i++){
            tre.access(i);
        }
        clk.stop();
        cout<<"Query Time: ";
        clk.timeElapsed();
        cout<<"ns"<<endl;
    }
    //if the access sequence is reversed
    else if(type == "Reverse"){
        clk.start();
        for(int i=size-1;i>=0;i--){
            tre.access(i);
        }
        clk.stop();
        cout<<"Query Time: ";
        clk.timeElapsed();
        cout<<"ns"<<endl;
    }
    //if the access sequence is random
    else{
        vector<int> sequence;
        for(int i=0;i<size;i++){
            sequence.push_back(i);
        }
        random_shuffle(sequence.begin(),sequence.end()); //to generate a random shuffle of the given array
        clk.start();
        for(int i=0;i<size;i++){ 
            tre.access(sequence[i]);
        }
        clk.stop();
        cout<<"Query Time: ";
        clk.timeElapsed();
        cout<<"ns"<<endl;
    }
    return;
}

void multiSplayTree::DispTree(multiSplayTree::Node *cur){
    //if current node is a null pointer
    if(!cur) return;
    //printing the parent of the current node if any
    cout<<"Parent of "<<cur->key<<" = ";
    //if the current node has a parent 
    if(cur->parent){
        cout<<cur->parent->key;
    }
    //if the current node does not have a parent
    else{
        cout<<"NULL";
    }
    //checking if the node is a root
    if(cur->isRoot) cout<<", "<<cur->key<<" is a Root";
    cout<<endl;
    //calling for the left subtree
    DispTree(cur->left);
    //calling for the right subtree
    DispTree(cur->right);
    return;
}

int main(){
    cout<<"Multi-Splay Trees:"<<endl;
    //creating the string to define the access type
    //calling the tester function for different test cases
    string access = "Sequential"; //setting the access type to 'sequential'
    tester(access,30); 
    tester(access,3000);
    tester(access,300000);
    access = "Reverse"; //setting the access sequence to 'reverse' 
    tester(access,30);
    tester(access,3000);
    tester(access,300000);
    access = "Random "; //setting the access sequence to 'random'
    tester(access,30);
    tester(access,3000);
    tester(access,300000);

    return 0;
}