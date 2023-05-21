// run forest run.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <fstream>

using namespace std;

struct Node {
    int data;
    Node* left, *right;
};
Node* newNode(int data)
{
    Node* node = new Node;
    node->data = data;
    node->left = node->right = NULL;
    return (node);
}

void preOrder(Node* node, vector<int>& vec)
{
    if (node == NULL)
        return;
    preOrder(node->left, vec);
    printf("%d ", node->data);
    vec.push_back(node->data);
    preOrder(node->right, vec);
}
int findIndex(string str, int si, int ei)
{
    if (si > ei)
        return -1;

    stack <char> s;
    for (int i = si; i <= ei; i++) {

        if (str[i] == '(')
            s.push(str[i]);

        else if (str[i] == ')') {
            if (s.top() == '(') {
                s.pop();

                if (s.empty())
                    return i;
            }
        }
    }
    return -1;
}

Node* treeFromString(string str, int si, int ei)
{
    // Base case    
    if (str[si] == '(' || str[si] == ')' || str[si] >= '0' && str[si] <= '9') {
        if (si > ei)
            return NULL;


        int num = 0;
        while (/*si <= ei &&*/ str[si] >= '0' && str[si] <= '9')
        {
            num *= 10;
            num += (str[si] - '0');
            si++;
            if (str[si] == '(' || str[si] == ')' || str[si] >= '0' && str[si] <= '9') {
                continue;
            }
            else {
                cout << "ошибка";
                exit(0);
            }
        }
        Node* root = newNode(num);
        int index = -1;
            if (si <= ei && str[si] == '(')
                index = findIndex(str, si, ei);

        if (index != -1) {

            root->left = treeFromString(str, si + 1, index - 1);

            root->right = treeFromString(str, index + 2, ei - 1);
        }
        return root;
    }
    else
    {
        cout << "ошибка";
        exit(0);
    }
}

class AVLNode
{
public:
    int key;
    AVLNode* left;
    AVLNode* right;
    int depth;
};

int depth(AVLNode* n)
{
    if (n == NULL)
        return 0;
    return n->depth;
}
AVLNode* new_Node(int key)
{
    AVLNode* node = new AVLNode();
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->depth = 1;
    return(node);
}
AVLNode* rightRotate(AVLNode* y)
{
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->depth = max(depth(y->left),
        depth(y->right)) + 1;
    x->depth = max(depth(x->left),
        depth(x->right)) + 1;

    return x;
}

AVLNode* leftRotate(AVLNode* x)
{
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;
    x->depth = max(depth(x->left),
        depth(x->right)) + 1;
    y->depth = max(depth(y->left),
        depth(y->right)) + 1;

    return y;
}

int getBalance(AVLNode* N)
{
    if (N == NULL)
        return 0;
    return depth(N->left) -
        depth(N->right);
}
AVLNode* insert(AVLNode* node, int key = 0) {
    if (node == NULL)
        return (new_Node(key));

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else                                                          
        return node;

    node->depth = 1 + max(depth(node->left), depth(node->right));

    int balance = getBalance(node);        

    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(node);
    if (balance > 1 && key > node->left->key)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

AVLNode* minValueNode(AVLNode* node)
{
    AVLNode* current = node;

    while (current->left != NULL)
        current = current->left;

    return current;
}
AVLNode* deleteNode(AVLNode* root, int key)
{
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);

    else if (key > root->key)
        root->right = deleteNode(root->right, key);

    else
    {
        if ((root->left == NULL) ||
            (root->right == NULL))
        {
            AVLNode* temp = root->left ?
                root->left :
                root->right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else 
                *root = *temp;
            free(temp);
        }
        else
        {
            AVLNode* temp = minValueNode(root->right);

            root->key = temp->key;

            root->right = deleteNode(root->right,
                temp->key);
        }
    }

    if (root == NULL)
        return root;

    root->depth = 1 + max(depth(root->left),
        depth(root->right));

    int balance = getBalance(root);


    if (balance > 1 &&
        getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}
void allTraversal(AVLNode* root)
{
    vector<int> pre;
 
    vector<int> post;
 
    vector<int> in;
 
    stack<pair<AVLNode*, int> > s;
 
    s.push(make_pair( root, 1));
 
    while (!s.empty()) {
 
        pair<AVLNode*, int> p = s.top();
 
        if (p.second == 1) {
 
            s.top().second++;
 
            pre.push_back(p.first->key);
 
            if (p.first->left) {
 
                s.push(make_pair(p.first->left, 1));
            }
        }
 
        else if (p.second == 2) {
 
            s.top().second++;
 
            in.push_back(p.first->key);
 
            if (p.first->right) {
 
                s.push(make_pair(
                    p.first->right, 1));
            }
        }
 
        else {
 
            post.push_back(p.first->key);
 
            s.pop();
        }
    }
 
    cout << "Preorder Traversal: ";
    for (int i = 0; i < pre.size(); i++) {
        cout << pre[i] << " ";
    }
    cout << "\n";
 
    cout << "Inorder Traversal: ";
 
    for (int i = 0; i < in.size(); i++) {
        cout << in[i] << " ";
    }
    cout << "\n";
 
    cout << "Postorder Traversal: ";
 
    for (int i = 0; i < post.size(); i++) {
        cout << post[i] << " ";
    }
    cout << "\n";
}


int height(AVLNode* node)
{
    if (node == NULL)
        return 0;
    else {
        int lheight = height(node->left);
        int rheight = height(node->right);

        if (lheight > rheight) {
            return (lheight + 1);
        }
        else {
            return (rheight + 1);
        }
    }
}
void printCurrentLevel(AVLNode* root, int level)
{
    if (root == NULL)
        return;
    if (level == 1)
        cout << root->key << " ";
    else if (level > 1) {
        printCurrentLevel(root->left, level - 1);
        printCurrentLevel(root->right, level - 1);
    }
}
void printLevelOrder(AVLNode* root)
{
    int h = height(root);
    for (int i=1; i <= h; i++)
        printCurrentLevel(root, i);
}
void checkCorrect(string str)
{
    if (str[0] < '0' || str[0] > '9') {
        cout << "уберите лишние скобки или иные символы из начала строки";
        exit(0);
    }
    int c1 = 0;
    int c2 = 0;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == '(')
        {
            c1++;
            if (str[i + 1] == '(')
            {
                cout << "какие то проблемы со скобками, перепроверьте выражение";
                exit(0);
            }
        }
        else if (str[i] == ')')
        {
            c2++;
        }
    }
    if (c1 != c2)
    {
        cout << "какие то проблемы со скобками, перепроверьте выражение";
        exit(0);
    }
    if (str.find("()()") != -1)
    {
        cout << "какие то проблемы со скобками, перепроверьте выражение";
        exit(0);
    }
}

// Driver Code
int main()
{
    setlocale(LC_ALL, "rus");
    string str = "8(9(3)(5))(1)";
    /*string str;
    ifstream file("qqq.txt");
    file >> str;
    if (!file.eof())
    {
        cout << "уберите лишние пробелы";
        exit(0);
    }*/
    checkCorrect(str);
    Node* Root = treeFromString(str, 0, str.length() - 1);
    AVLNode* root = NULL;
    vector<int> vec;

    preOrder(Root,vec);

   /* constructing an AVL tree*/
    for (int i = 0; i < vec.size(); i++)
    {
        root = insert(root, vec[i]);
    }
    cout << '\n';

    allTraversal(root);

    root = deleteNode(root, 9);
    root = insert(root, 12);
    root = insert(root, 13);
    cout << "\nInorder traversal after deletion of node 9 and adding 12&13: \n";
    allTraversal(root);
    cout << "\nBFS: \n";
    printLevelOrder(root);
    return 0;
}