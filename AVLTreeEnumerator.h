#ifndef AVL_TREE_ENUMERATOR
#define AVL_TREE_ENUMERATOR

#include <stack>
#include "Enumerator.h"
#include "AVLTreeOrder.h"
#include "AVLTree.h"

template <typename T>
class AVLTreeEnumerator : public Enumerator<T>
{
private:
	AVLTreeOrder order;
	std::stack<const AVLTree<T>*> traversalStack;
	void buildTraversalStack(const AVLTree<T>* current);
    //MY Methods
    void inOrder(const AVLTree<T> *current);
    void preOrder(const AVLTree<T>* current);
    void postOrder(const AVLTree<T>* current);
public:
	AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order);
	virtual ~AVLTreeEnumerator();
    
	bool hasNext() const;
	T next();						// throws ExceptionEnumerationBeyondEnd if no next item is available
	T peek() const;					// throws ExceptionEnumerationBeyondEnd if no next item is available
	AVLTreeOrder getOrder();		// returns the order of this enumerator (preorder, inorder, or postorder)
    
};

template <typename T>
AVLTreeEnumerator<T>::AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order)
{
    if(root -> size == 0)
        throw new ExceptionAVLTreeAccess;
    
    this -> order = order;
    
    buildTraversalStack(root);
}

template <typename T>
void AVLTreeEnumerator<T>::buildTraversalStack(const AVLTree<T> *current)
{
    //Tree Traversal medothds borrowed from https://www.geeksforgeeks.org/tree-traversals-inorder-preorder-and-postorder/
    if(order == AVLTreeOrder::inorder)                //Inorder (Left, Root, Right)
    {
        inOrder(current);
    }
    else if (order  == AVLTreeOrder::preorder)        //Preorder (Root, Left, Right)
    {
        if(current -> right != NULL)
            buildTraversalStack(current -> right);
        
        if(current -> left != NULL)
            buildTraversalStack(current -> left);
        
        if(current -> data != NULL)
            traversalStack.push(current);
        
        //preOrder(current);
    }
    else if( order == AVLTreeOrder::postorder)                                             //Postorder (Left, Right, Root)
    {
        postOrder(current);
    }
    else
    {
        throw new ExceptionAVLTreeAccess;
    }
}

template <typename T>
void AVLTreeEnumerator<T>::inOrder(const AVLTree<T>* current)  //Inorder (Left, Root, Right)            Works !!!
{
    if (current == NULL)
        return;
    
    /* recur on right child */
    inOrder(current -> right);
    
    traversalStack.push(current);
    
    /* recur on left child */
    inOrder(current ->left);
}

template <typename T>
void AVLTreeEnumerator<T>::postOrder(const AVLTree<T>* current)         //Postorder (Left, Right, Root)             Works!!!!
{
    if (current == NULL)
        return;

    traversalStack.push(current);

     // now recur on right subtree
     postOrder(current -> right);
     
     // then recur on left sutree
     postOrder(current -> left);
}

template <typename T>
void AVLTreeEnumerator<T>::preOrder(const AVLTree<T>* current)      ////Preorder (Root, Left, Right)            Works!!!
{
    if (current == NULL)
        return;
    
    // now recur on right subtree
    preOrder(current->right);
    
    // then recur on left sutree
    preOrder(current->left);
    
    traversalStack.push(current);
}

template <typename T>
AVLTreeEnumerator<T>::~AVLTreeEnumerator()
{
    
}

template <typename T>
bool AVLTreeEnumerator<T>::hasNext() const
{
    return !traversalStack.empty();
}

template <typename T>
T AVLTreeEnumerator<T>::next()
{
    const AVLTree<T>* tree = traversalStack.top();
    T item = *tree -> data;
    traversalStack.pop();
    return item;
}

template <typename T>
T AVLTreeEnumerator<T>::peek() const
{
    const AVLTree<T>* tree = traversalStack.top();
    T item = *tree -> data;
    return item;
}

template <typename T>
AVLTreeOrder AVLTreeEnumerator<T>::getOrder()
{
    return order;
}

// put implementation here

#endif // !AVL_TREE_ENUMERATOR
