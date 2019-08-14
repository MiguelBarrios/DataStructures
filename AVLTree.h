#ifndef AVL_TREE
#define AVL_TREE

#include "Comparator.h"

template <typename T>
class AVLTree {
	template <typename U>
	friend class AVLTreeEnumerator;
private:
	Comparator<T>* comparator = NULL;				// used to determine tree order and item equality
	unsigned long size = 0;							// actual number of items currently in tree
	T* data = NULL;									// pointer to data at this node
	int diff = 0;									// height of right minus height of left
	AVLTree<T>* left = NULL;						// pointer to left subtree
	AVLTree<T>* right = NULL;						// pointer to right subtree
	void zig();										// right rotation
	void zag();										// left rotation
	void zigzag();									// left rotation on left subtree, followed by right rotation
	void zagzig();									// right rotation on right subtree, followed by left rotation
	void rebalance();								// check for and rebalance this node, if needed
    int getHeight(AVLTree<T>* tree);
    int max(int one, int two);
    int removalCase(AVLTree<T>* currentTree);
    void removeLeftNode(AVLTree<T>* removeNode);
    void removeRightNode(AVLTree<T>* currentTree);
public:
	AVLTree(Comparator<T>* comparator);				// creates empty linked tree with comparator
	virtual ~AVLTree();								// deletes all links and their data items

	// if an equivalent item is not already present, insert item in order and return true
	// if an equivalent item is already present, leave tree unchanged and return false
	bool insert(const T* item);

	// if an equivalent item is already present, replace item and return true
	// if an equivalent item is not already present, leave tree unchanged and return false
	bool replace(const T* item);

	// if an equivalent item is already present, remove item and return true
	// if an equivalent item is not already present, leave tree unchanged and return false
	bool remove(const T* item);

	// if an equivalent item is present, return a copy of the item
	// if an equivalent item is not present, throw a new ExceptionLinkedtreeAccess
	T find(const T* item) const;
    
    TemplatedArray<T>* transferToArray();

	unsigned long getSize() const;				// returns the current number of items in the tree
};

template <typename T>
AVLTree<T>::AVLTree(Comparator<T>* comparator)
{
    this -> comparator = comparator;
}

template <typename T>
AVLTree<T>::~AVLTree()
{
    comparator = NULL;
    data = NULL;
    if(left != NULL)
    {
        delete left;
        left = NULL;
    }
    if(right != NULL)
    {
        delete right;
        right = NULL;
    }
    
}

template <typename T>
bool AVLTree<T>::insert(const T* item)
{
    if(data == NULL)
    {
        data = new T(*item);
        ++size;
        return true;
    }
    else
    {
        int place = comparator -> compare(*item, *data);
        
        if(place == 0)
        {
            return false;
        }
        else if(place == -1)                //Record belongs on left side
        {
            if(this -> left == NULL)
            {
                this -> left = new AVLTree(comparator);
            }

            if(left -> insert(item))
            {
                diff = getHeight(left) - getHeight(right);
                ++size;
                rebalance();
                return true;
            }
            else
            {
                return false;
            }
        }
        else                            //Record belongs on right side
        {
            if(this -> right == NULL)
            {
                this -> right = new AVLTree(comparator);
            }
            
            if(right -> insert(item))
            {
                diff = getHeight(left) - getHeight(right);
                ++size;
                rebalance();
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

template <typename T>
int AVLTree<T>::getHeight(AVLTree<T>* tree)
{
    if(tree == NULL)
    {
        return 0;
    }
    else
    {
        int leftHeight = getHeight(tree -> left);
        int rightHeight = getHeight(tree -> right);
        return max(leftHeight, rightHeight) + 1;
    }
}

template<typename T>
int AVLTree<T>::max(int one, int two)
{
    if(one > two)
        return one;
    else if(two > one)
        return two;
    else
        return one;
}

template <typename T>
bool AVLTree<T>::replace(const T* item)
{
    if(data != NULL)
    {
        int place = comparator -> compare(*item, *data);
        
        if(place == 0)              //Item Found
        {
            this -> data = new T(*item);
            return true;
        }
        else if(place == -1)
        {
            if(this -> left == NULL)
                return false;
            
            return left -> replace(item);
        }
        else
        {
            if(this -> right == NULL)
                return false;
            
            return right -> insert(item);
        }
    }
    return false;
}

template<typename T>
int AVLTree<T>::removalCase(AVLTree<T>* currentTree)
{
    if(currentTree -> left == NULL && currentTree -> right == NULL)
    {
        //node with no children
        return 0;
    }
    else if(currentTree -> left == NULL)
    {
        //node with a right child, no left
        return 1;
    }
    else if(currentTree -> right == NULL)
    {
        //node with a left child no right
        return -1;
    }
    else
    {
        //node has two children
        return 2;
    }
}

template<typename T>
void AVLTree<T>::removeLeftNode(AVLTree<T>* currentTree)
{
    int caseType = removalCase(currentTree -> left);
    
    AVLTree<T>* removeNode = currentTree -> left;

    if(caseType == 0)
    {
        currentTree -> left = NULL;
    }
    else if(caseType == 1)
    {
        currentTree -> left = removeNode -> right;
        removeNode -> right = NULL;
        delete removeNode;
    }
    else if(caseType == -1)
    {
        currentTree -> left = currentTree -> left -> left;
        removeNode -> left = NULL;
        delete removeNode;
    }
    else if (caseType == 2)
    {
        AVLTree<T>* succNode = removeNode -> left;
        if(succNode -> right == NULL)
        {
            succNode -> right = removeNode -> right;
            currentTree -> right = succNode;
            removeNode -> left = NULL;
            removeNode -> right = NULL;
            delete removeNode;
        }
        else
        {
            AVLTree<T>* succNode = removeNode -> left;
            //CASE ONE: node has no right chiled
            if(succNode -> right == NULL)
            {
                succNode -> right = removeNode -> right;
                currentTree -> right = succNode;
                removeNode -> left = NULL;
                removeNode -> right = NULL;
                delete removeNode;
            }
            else
            {
                //Findes rightmost chiled
                AVLTree<T>* previous = succNode;
                while(succNode -> right != NULL)
                {
                    previous = succNode;
                    succNode = succNode -> right;
                }
                //Case Two: right childe node's right chiled is a leaf node
                if (succNode -> left == NULL)
                {
                    T* copyData = succNode -> data;
                    succNode -> data = NULL;
                    delete succNode;
                    removeNode -> data = copyData;
                }
                //Case Three: Right childs right, right child has left node only
                else
                {
                    T* copyData = succNode -> data;
                    removeNode -> data = copyData;
                    previous -> right = succNode -> left;
                    succNode -> left = NULL;
                    delete succNode;
                }
            }
        }
    }
}

template<typename T>
void AVLTree<T>::removeRightNode(AVLTree<T>* currentTree)
{
    int caseType = removalCase(currentTree -> right);
    AVLTree<T>* removeNode = currentTree -> right;

    if(caseType == 0)
    {
        currentTree -> right = NULL;
    }
    if(caseType == 1)
    {
        currentTree -> right = removeNode -> right;
        removeNode -> right = NULL;
        delete removeNode;
    }
    else if (caseType == -1)
    {
        currentTree -> right = currentTree -> right -> left;
        removeNode -> left = NULL;
        delete removeNode;
    }
    else if (caseType == 2)
    {
        AVLTree<T>* succNode = currentTree -> right -> left;
        if(succNode -> right == NULL)
        {
            succNode -> right = removeNode -> right;
            currentTree -> right = succNode;
            removeNode -> left = NULL;
            removeNode -> right = NULL;
            delete removeNode;
        }
        else
        {
            AVLTree<T>* succNode = currentTree -> right -> left;
            //CASE ONE: node has no right chiled
            if(succNode -> right == NULL)
            {
                succNode -> right = removeNode -> right;
                currentTree -> right = succNode;
                removeNode -> left = NULL;
                removeNode -> right = NULL;
                delete removeNode;
            }
            else
            {
                //Findes rightmost chiled
                AVLTree<T>* previous = succNode;
                while(succNode -> right != NULL)
                {
                    previous = succNode;
                    succNode = succNode -> right;
                }
                //Case Two: right childe node's right chiled is a leaf node
                if (succNode -> left == NULL)
                {
                    T* copyData = succNode -> data;
                    succNode -> data = NULL;
                    delete succNode;
                    currentTree -> right -> data = copyData;
                }
                //Case Three: Right childs right, right child has left node only
                else
                {
                    T* copyData = succNode -> data;
                    currentTree -> right -> data = copyData;
                    previous -> right = succNode -> left;
                    succNode -> left = NULL;
                    delete succNode;
                }
            }
            
        }
    }
}

template <typename T>
bool AVLTree<T>::remove(const T* item)
{
    if(size == 0)
    {
        throw new ExceptionAVLTreeAccess;
    }
    
    AVLTree<T>* currentTree = this;
    
    if(comparator -> compare(*item, *currentTree -> data) == 0)
    {
        int caseType = removalCase(currentTree);
        if(caseType == 0)
        {
            --size;
           data = NULL;
        }
        else if(caseType == 1)
        {
            T* replacementData = new T(*this -> right -> data);
            this -> data = replacementData;
            delete this -> right;
            this -> right = NULL;
            --size;
        }
        else if(caseType == -1)
        {
            T* replacementData = new T(*this -> left -> data);
            this -> data = replacementData;
            delete this -> left;
            this -> left = NULL;
            --size;
        }
        else if(caseType == 2)
        {
            AVLTree<T>* removeNode = currentTree -> left;
            AVLTree<T>* succNode = currentTree -> right -> left;
            //CASE ONE: node has no right chiled
            if(succNode -> right == NULL)
            {
                succNode -> right = removeNode -> right;
                currentTree -> right = succNode;
                removeNode -> left = NULL;
                removeNode -> right = NULL;
                delete removeNode;
            }
            else
            {
                //Findes rightmost chiled
                AVLTree<T>* previous = succNode;
                while(succNode -> right != NULL)
                {
                    previous = succNode;
                    succNode = succNode -> right;
                }
                //Case Two: right childe node's right chiled is a leaf node
                if (succNode -> left == NULL)
                {
                    T* copyData = succNode -> data;
                    succNode -> data = NULL;
                    delete succNode;
                    currentTree -> right -> data = copyData;
                }
                //Case Three: Right childs right, right child has left node only
                else
                {
                    T* copyData = succNode -> data;
                    currentTree -> right -> data = copyData;
                    previous -> right = succNode -> left;
                    succNode -> left = NULL;
                    delete succNode;
                }
            }
        }
        return true;
    }
    else if(comparator -> compare(*item, *currentTree -> data) == -1)
    {
        if(this -> left != NULL)
        {
            if(currentTree -> left -> remove(item))
            {
                removeLeftNode(currentTree);
                rebalance();
                return true;
            }
        }
        
        return false;
    }
    else
    {
        if(right != NULL)
        {
            if(currentTree -> right -> remove(item))
            {
                removeRightNode(currentTree);
                --size;
                rebalance();
                return true;
            }
        }
        
        return false;
    }
}

template <typename T>
T AVLTree<T>::find(const T* item) const              
{
    AVLTree<T>* currentTree = NULL;
    if(size == 0)
    {
        throw new ExceptionAVLTreeAccess;
    }
    else
    {
        if(comparator -> compare(*item, *data)  == -1)
            currentTree = left;
        else if(comparator -> compare(*item, *data)  == 1)
            currentTree = right;
        else
            return *data;
    }
    
    while(currentTree != NULL && comparator -> compare(*item, *currentTree -> data) != 0)
    {
        
        if(comparator -> compare(*item, *currentTree -> data)  == -1)
        {
            if(left != NULL)
            {
                currentTree = currentTree -> left;
            }
            else
            {
                throw new ExceptionAVLTreeAccess;
            }
            
        }
        else if(comparator -> compare(*item, *currentTree -> data)  == 1)
        {
            if(right != NULL)
            {
                currentTree = currentTree -> right;
            }
            else
            {
                throw new ExceptionAVLTreeAccess;
            }
        }
    }
    
    if(currentTree != NULL && comparator -> compare(*item, *currentTree -> data) == 0)
    {
        return *currentTree -> data;
    }
    throw new ExceptionAVLTreeAccess;
}


template <typename T>
unsigned long AVLTree<T>::getSize() const
{
    return size;
}

template<typename T>
void AVLTree<T>::zig()  //Right Rotation
{
    if (left == NULL) {  // no left, no way to zig
        return;
    }
    // keep track of diff of node and left for changes
    int gdiff = diff;
    int pdiff = left->diff;
    
    // modify the tree
    AVLTree<T>* olnr = left;  // olnr is "old left, new right"
    left = olnr->left;
    olnr->left = olnr->right;
    olnr->right = right;
    right = olnr;
    
    // note that the modifications kept the node itself in place, so we need to swap its data with its olnr's
    T* tempData = data;
    data = olnr->data;
    olnr->data = tempData;
    
    // update the diff fields for node and new right
    if (pdiff < 0) {  // if imbalance was outside left heavy (left-left violation)
        diff = gdiff + 2;
        right->diff = gdiff - pdiff + 1;
    }
    else {  // otherwise imbalance was inside left heavy (left-right violation)
        diff = pdiff + 1;
        right->diff = gdiff + 1;
    }
}

template<typename T>
void AVLTree<T>::zag()  //Left Rotation
{
    if (right == NULL)
    {  // no right, no way to zig
        return;
    }
    // keep track of diff of node and right for changes
    int gdiff = diff;
    int pdiff = right->diff;
    
    // modify the tree
    AVLTree<T>* ornl = right;  // ornl is "old right, new left"
    right = ornl->right;
    ornl->right = ornl->left;
    ornl->left = left;
    left = ornl;
    
    // note that the modifications kept the node itself in place, so we need to swap its data with its ornl's
    T* tempData = data;
    data = ornl->data;
    ornl->data = tempData;
    
    // update the diff fields for node and new left
    if (pdiff > 0)
    {  // if imbalance was outside right heavy (right-right violation)
        diff = gdiff - 2;
        left->diff = gdiff - pdiff - 1;
    }
    else
    {  // otherwise imbalance was inside right heavy (right-left violation)
        diff = pdiff - 1;
        left->diff = gdiff - 1;
    }
}

template <typename T>
void AVLTree<T>::zigzag()  //right rotations, leftRotation
{
    this -> right -> zig();
    this -> zag();
}

template <typename T>
void AVLTree<T>::zagzig() //left rotation, right rotation
{
    this -> left -> zag();
    this -> zig();
}

template <typename T> 
void AVLTree<T>::rebalance()
{
    if(diff >= 2)
    {
        if( this -> left -> diff == 1)
        {
            this -> zig();
        }
        else if(this -> left -> diff == -1)
        {
            this -> zagzig();
        }
    }
    else if(diff <= -2)
    {

        if(this -> right -> diff == 1)
        {
            this -> zigzag();
        }
        else if(this -> right -> diff == -1)
        {
            this -> zag();
        }
    }
}

// add public functions here


// add additional private functions here

#endif // !AVL_TREE



