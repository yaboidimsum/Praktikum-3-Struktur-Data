
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct AVLNode_t
{
    int data;
    struct AVLNode_t *left,*right;
    int height;
}AVLNode;

typedef struct AVL_t
{
    AVLNode *_root;
    unsigned int _size;
}AVL;

void avl_init(AVL *avl) {
    avl->_root = NULL;
    avl->_size = 0u;
}

int _max(int a,int b){
    return (a > b)? a : b;
}

int _getHeight(AVLNode* node){
    if(node==NULL)
        return 0;
    return node->height;
}

int _getBalanceFactor(AVLNode* node){
    if(node==NULL)
        return 0;
    return _getHeight(node->left)-_getHeight(node->right);
}

AVLNode* _rightRotate(AVLNode* pivotNode){

    AVLNode* newParrent=pivotNode->left;
    pivotNode->left=newParrent->right;
    newParrent->right=pivotNode;

    pivotNode->height=_max(_getHeight(pivotNode->left),
                           _getHeight(pivotNode->right))+1;
    newParrent->height=_max(_getHeight(newParrent->left),
                            _getHeight(newParrent->right))+1;

    return newParrent;
}

AVLNode* _leftRotate(AVLNode* pivotNode){

    AVLNode* newParrent=pivotNode->right;
    pivotNode->right=newParrent->left;
    newParrent->left=pivotNode;

    pivotNode->height=_max(_getHeight(pivotNode->left),
                           _getHeight(pivotNode->right))+1;
    newParrent->height=_max(_getHeight(newParrent->left),
                            _getHeight(newParrent->right))+1;

    return newParrent;
}

AVLNode* _leftCaseRotate(AVLNode* node){
    return _rightRotate(node);
}

AVLNode* _rightCaseRotate(AVLNode* node){
    return _leftRotate(node);
}

AVLNode* _leftRightCaseRotate(AVLNode* node){
    node->left=_leftRotate(node->left);
    return _rightRotate(node);
}

AVLNode* _rightLeftCaseRotate(AVLNode* node){
    node->right=_rightRotate(node->right);
    return _leftRotate(node);
}

AVLNode* _avl_createNode(int value) {
    AVLNode *newNode = (AVLNode*) malloc(sizeof(AVLNode));
    newNode->data = value;
    newNode->height=1;
    newNode->left = newNode->right = NULL;
    return newNode;
}

AVLNode* _search(AVLNode *root, int value) {
    while (root != NULL) {
        if (value < root->data)
            root = root->left;
        else if (value > root->data)
            root = root->right;
        else
            return root;
    }
    return root;
}

AVLNode* _insert_AVL(AVL *avl,AVLNode* node,int value){

    if(node==NULL)
        return _avl_createNode(value);
    if(value < node->data)
        node->left = _insert_AVL(avl,node->left,value);
    else if(value > node->data)
        node->right = _insert_AVL(avl,node->right,value);

    node->height= 1 + _max(_getHeight(node->left),_getHeight(node->right));

    int balanceFactor=_getBalanceFactor(node);

    if(balanceFactor > 1 && value < node->left->data)
        return _leftCaseRotate(node);
    if(balanceFactor > 1 && value > node->left->data)
        return _leftRightCaseRotate(node);
    if(balanceFactor < -1 && value > node->right->data)
        return _rightCaseRotate(node);
    if(balanceFactor < -1 && value < node->right->data)
        return _rightLeftCaseRotate(node);

    return node;
}

bool avl_find(AVL *avl, int value) {
    AVLNode *temp = _search(avl->_root, value);
    if (temp == NULL)
        return false;

    if (temp->data == value)
        return true;
    else
        return false;
}

void avl_insert(AVL *avl,int value){
    if(!avl_find(avl,value)){
        avl->_root = _insert_AVL(avl,avl->_root,value);
        avl->_size++;
    }
}

void functionFind_path(AVLNode *current, AVLNode *temp, int target, int sum, bool *Find_path, bool flag) {

    if (target == sum)
    {
        *Find_path = true;
    }
    else if (!flag)
    {
        if (current)
        {
            if ((current->left || current->right) || (temp->left && temp->right))
            {
                sum += current->data;

                if (sum == target)
                {
                    *Find_path = true;
                    return;
                }
                if (sum > target) return;

                if (!(*Find_path))
                    functionFind_path(current->left, temp, target, sum, Find_path, false);


                if (!(*Find_path))
                    functionFind_path(current->right, temp, target, sum, Find_path, false);


                if (temp->data > current->data)
                {
                    if (!(*Find_path))
                        functionFind_path(current, temp->right, target, sum, Find_path, true);
                }

                else if (temp->data < current->data)
                {
                    if (!(*Find_path))
                        functionFind_path(current, temp->left, target, sum, Find_path, true);
                }


                if (!(*Find_path))
                    functionFind_path(current->left, current->left, target, 0, Find_path, false);

                if (!(*Find_path))
                    functionFind_path(current->right, current->right, target, 0, Find_path, false);
            }
        }
    }


    else
    {
        if (temp)
        {

            if ((current->left || current->right) || (temp->left || temp->right))
            {

                sum += temp->data;

                if (sum == target)
                {
                    *Find_path = true;
                    return;
                }
                if (sum > target) return;
                if (!(*Find_path))
                    functionFind_path(current, temp->left, target, sum, Find_path, true);
                if (!(*Find_path))
                    functionFind_path(current, temp->right, target, sum, Find_path, true);
            }
        }
    }
}

int main() {

    AVL avltree;
    avl_init(&avltree);

    int query_r;
    scanf("%d", &query_r);

    while(query_r--) {

        int query, value;
        scanf("%d %d", &query, &value);

        if (query == 1)
        {
            avl_insert(&avltree, value);
        }
        else
        {

            bool Find_path = false;

            if (avltree._size <= 1)
            {
            }

            else if (avl_find(&avltree, value))
            {
                Find_path = true;
            }

            else
            {
                functionFind_path(avltree._root, avltree._root, value, 0, &Find_path, false);
            }

            if (!Find_path)
            {
                printf("Apasi, ngajak berantem mulu dah kerjaannya:(\n");
            }
            else
            {
                printf("Aaa udh lama bgt ga diboongin:)\n");
            }
        }
    }
    return 0;
}
