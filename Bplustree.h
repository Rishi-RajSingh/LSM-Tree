#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<climits>
using namespace std;

int MAX; //size of each node
//class BPTree; //self explanatory classes
class Node
{
	bool checkleaf;
	int *key, size;
	Node** ptr;
	friend class BPTree;
public:
	Node();
};
class BPTree
{
	void insertInternal(int,Node*,Node*);
	void removeInternal(int,Node*,Node*);
	Node* findParent(Node*,Node*);
public:
	BPTree();
	Node *root;
	int sizeofBPTree;
	int MaxsizeofBPTree;
	int A[100];
	int check;
	void search(int);
	void insert(int);
	void remove(int);
	void display(Node*);
	Node* getRoot();
	void cleanUp(Node*);
	void checked(Node*);
	~BPTree();
};

Node::Node()
{
	//dynamic memory allocation
	key = new int[MAX];
	ptr = new Node*[MAX+1];
}
BPTree::BPTree()
{
	root = NULL;
	sizeofBPTree = 0;
	check=0;
}
void BPTree::search(int x)
{
	//search logic
	if(root==NULL)
	{
		//empty
		cout<<"Tree empty\n";
	}
	else
	{
		Node* point = root;
		//in the following while loop, point will will travel to the leaf node possibly consisting the key
		while(point->checkleaf == false)
		{
			for(int i = 0; i < point->size; i++)
			{
				if(x < point->key[i])
				{
					point = point->ptr[i];
					break;
				}
				if(i == point->size - 1)
				{
					point = point->ptr[i+1];
					break;
				}
			}
		}
		//in the following for loop, we search for the key if it exists
		for(int i = 0; i < point->size; i++)
		{
			if(point->key[i] == x)
			{
				cout<<"Found\n";
				return;
			}
		}
		cout<<"Not found\n";
	}
}
void BPTree::insert(int x)
{
	//insert logic
	sizeofBPTree+=1;
	if(root==NULL)
	{
		root = new Node;
		root->key[0] = x;
		root->checkleaf = true;
		root->size = 1;
		cout<<"Created root\nInserted "<<x<<" successfully\n";
	}
	else
	{
		Node* point = root;
		Node* parent;
		//in the following while loop, point will will travel to the leaf node possibly consisting the key
		while(point->checkleaf == false)
		{
			parent = point;
			for(int i = 0; i < point->size; i++)
			{
				if(x < point->key[i])
				{
					point = point->ptr[i];
					break;
				}
				if(i == point->size - 1)
				{
					point = point->ptr[i+1];
					break;
				}
			}
		}
		//now point is the leaf node in which we'll insert the new key
		if(point->size < MAX)
		{
			//if point is not full
			//find the correct position for new key
			int i = 0;
			while(x > point->key[i] && i < point->size) i++;
			//make space for new key
			for(int j = point->size;j > i; j--)
			{
				point->key[j] = point->key[j-1];
			}
			point->key[i] = x;
			point->size++;
			point->ptr[point->size] = point->ptr[point->size-1];
			point->ptr[point->size-1] = NULL;
			cout<<"Inserted "<<x<<" successfully\n";
		}
		else
		{
			cout<<"Inserted "<<x<<" successfully\n";
			cout<<"Overflow in leaf node!\nSplitting leaf node\n";
			//overflow condition
			//create new leaf node
			Node* newLeaf = new Node;
			//create a virtual node and insert x into it
			int virtualNode[MAX+1];
			for(int i = 0; i < MAX; i++)
			{
				virtualNode[i] = point->key[i];
			}
			int i = 0, j;
			while(x > virtualNode[i] && i < MAX) i++;
			//make space for new key
			for(int j = MAX+1;j > i; j--)
			{
				virtualNode[j] = virtualNode[j-1];
			}
			virtualNode[i] = x;
			newLeaf->checkleaf = true;
			//split the point into two leaf nodes
			point->size = (MAX+1)/2;
			newLeaf->size = MAX+1-(MAX+1)/2;
			//make point point to new leaf node
			point->ptr[point->size] = newLeaf;
			//make new leaf node point to the next leaf node
			newLeaf->ptr[newLeaf->size] = point->ptr[MAX];
			point->ptr[MAX] = NULL;
			//now give elements to new leaf nodes
			for(i = 0; i < point->size; i++)
			{
				point->key[i] = virtualNode[i];
			}
			for(i = 0, j = point->size; i < newLeaf->size; i++, j++)
			{
				newLeaf->key[i] = virtualNode[j];
			}
			//modify the parent
			if(point == root)
			{
				//if point is a root node, we create a new root
				Node* newRoot = new Node;
				newRoot->key[0] = newLeaf->key[0];
				newRoot->ptr[0] = point;
				newRoot->ptr[1] = newLeaf;
				newRoot->checkleaf = false;
				newRoot->size = 1;
				root = newRoot;
				cout<<"Created new root\n";
			}
			else
			{
				//insert new key in parent node
				insertInternal(newLeaf->key[0],parent,newLeaf);
			}
		}
	}
}
void BPTree::insertInternal(int x, Node* point, Node* child)
{
	if(point->size < MAX)
	{
		//if point is not full
		//find the correct position for new key
		int i = 0;
		while(x > point->key[i] && i < point->size) i++;
		//make space for new key
		for(int j = point->size;j > i; j--)
		{
			point->key[j] = point->key[j-1];
		}//make space for new pointer
		for(int j = point->size+1; j > i+1; j--)
		{
			point->ptr[j] = point->ptr[j-1];
		}
		point->key[i] = x;
		point->size++;
		point->ptr[i+1] = child;
		cout<<"Inserted key in an Internal node successfully\n";
	}
	else
	{
		cout<<"Inserted key in an Internal node successfully\n";
		cout<<"Overflow in internal node!\nSplitting internal node\n";
		//if overflow in internal node
		//create new internal node
		Node* newInternal = new Node;
		//create virtual Internal Node;
		int virtualKey[MAX+1];
		Node* virtualPtr[MAX+2];
		for(int i = 0; i < MAX; i++)
		{
			virtualKey[i] = point->key[i];
		}
		for(int i = 0; i < MAX+1; i++)
		{
			virtualPtr[i] = point->ptr[i];
		}
		int i = 0, j;
		while(x > virtualKey[i] && i < MAX) i++;
		//make space for new key
		for(int j = MAX+1;j > i; j--)
		{
			virtualKey[j] = virtualKey[j-1];
		}
		virtualKey[i] = x;
		//make space for new ptr
		for(int j = MAX+2;j > i+1; j--)
		{
			virtualPtr[j] = virtualPtr[j-1];
		}
		virtualPtr[i+1] = child;
		newInternal->checkleaf = false;
		//split point into two nodes
		point->size = (MAX+1)/2;
		newInternal->size = MAX-(MAX+1)/2;
		//give elements and pointers to the new node
		for(i = 0, j = point->size+1; i < newInternal->size; i++, j++)
		{
			newInternal->key[i] = virtualKey[j];
		}
		for(i = 0, j = point->size+1; i < newInternal->size+1; i++, j++)
		{
			newInternal->ptr[i] = virtualPtr[j];
		}
		// m = point->key[point->size]
		if(point == root)
		{
			//if point is a root node, we create a new root
			Node* newRoot = new Node;
			newRoot->key[0] = point->key[point->size];
			newRoot->ptr[0] = point;
			newRoot->ptr[1] = newInternal;
			newRoot->checkleaf = false;
			newRoot->size = 1;
			root = newRoot;
			cout<<"Created new root\n";
		}
		else
		{
			//recursion
			//find depth first search to find parent of point
			insertInternal(point->key[point->size] ,findParent(root,point) ,point);
		}
	}
}
Node* BPTree::findParent(Node* point, Node* child)
{
	//finds parent using depth first traversal and ignores leaf nodes as they cannot be parents
	//also ignores second last level because we will never find parent of a leaf node during insertion using this function
	Node* parent;
	if(point->checkleaf || (point->ptr[0])->checkleaf)
	{
		return NULL;
	}
	for(int i = 0; i < point->size+1; i++)
	{
		if(point->ptr[i] == child)
		{
			parent = point;
			return parent;
		}
		else
		{
			parent = findParent(point->ptr[i],child);
		}
	}
	return parent;
}
void BPTree::remove(int x)
{
	//delete logic
	if(root==NULL)
	{
		cout<<"Tree empty\n";
	}
	else
	{
		Node* point = root;
		Node* parent;
		int leftSibling, rightSibling;
		//in the following while loop, point will will travel to the leaf node possibly consisting the key
		while(point->checkleaf == false)
		{
			for(int i = 0; i < point->size; i++)
			{
				parent = point;
				leftSibling = i-1; //leftSibling is the index of left sibling in the parent node
				rightSibling =  i+1; //rightSibling is the index of right sibling in the parent node
				if(x < point->key[i])
				{
					point = point->ptr[i];
					break;
				}
				if(i == point->size - 1)
				{
					leftSibling = i;
					rightSibling = i+2;
					point = point->ptr[i+1];
					break;
				}
			}
		}
		//in the following for loop, we search for the key if it exists
		bool found = false;
		int pos;
		for(pos = 0; pos < point->size; pos++)
		{
			if(point->key[pos] == x)
			{
				found = true;
				break;
			}
		}
		if(!found)//if key does not exist in that leaf node
		{
			cout<<"Not found\n";
			return;
		}
		//deleting the key
		for(int i = pos; i < point->size; i++)
		{
			point->key[i] = point->key[i+1];
		}
		point->size--;
		if(point == root)//if it is root node, then make all pointers NULL
		{
			cout<<"Deleted "<<x<<" from leaf node successfully\n";
			for(int i = 0; i < MAX+1; i++)
			{
				point->ptr[i] = NULL;
			}
			if(point->size == 0)//if all keys are deleted
			{
				cout<<"Tree died\n";
				delete[] point->key;
				delete[] point->ptr;
				delete point;
				root = NULL;
			}
			return;
		}
		point->ptr[point->size] = point->ptr[point->size+1];
		point->ptr[point->size+1] = NULL;
		cout<<"Deleted "<<x<<" from leaf node successfully\n";
		if(point->size >= (MAX+1)/2)//no underflow
		{
			return;
		}
		cout<<"Underflow in leaf node!\n";
		//underflow condition
		//first we try to transfer a key from sibling node
		//check if left sibling exists
		if(leftSibling >= 0)
		{
			Node *leftNode = parent->ptr[leftSibling];
			//check if it is possible to transfer
			if(leftNode->size >= (MAX+1)/2+1)
			{
				//make space for transfer
				for(int i = point->size; i > 0; i--)
				{
					point->key[i] = point->key[i-1];
				}
				//shift pointer to next leaf
				point->size++;
				point->ptr[point->size] = point->ptr[point->size-1];
				point->ptr[point->size-1] = NULL;
				//transfer
				point->key[0] = leftNode->key[leftNode->size-1];
				//shift pointer of leftsibling
				leftNode->size--;
				leftNode->ptr[leftNode->size] = point;
				leftNode->ptr[leftNode->size+1] = NULL;
				//update parent
				parent->key[leftSibling] = point->key[0];
				cout<<"Transferred "<<point->key[0]<<" from left sibling of leaf node\n";
				return;
			}
		}
		if(rightSibling <= parent->size)//chech if right sibling exist
		{
			Node *rightNode = parent->ptr[rightSibling];
			//check if it is possible to transfer
			if(rightNode->size >= (MAX+1)/2+1)
			{
				//shift pointer to next leaf
				point->size++;
				point->ptr[point->size] = point->ptr[point->size-1];
				point->ptr[point->size-1] = NULL;
				//transfer
				point->key[point->size-1] = rightNode->key[0];
				//shift pointer of rightsibling
				rightNode->size--;
				rightNode->ptr[rightNode->size] = rightNode->ptr[rightNode->size+1];
				rightNode->ptr[rightNode->size+1] = NULL;
				//shift conent of right sibling
				for(int i = 0; i < rightNode->size; i++)
				{
					rightNode->key[i] = rightNode->key[i+1];
				}
				//update parent
				parent->key[rightSibling-1] = rightNode->key[0];
				cout<<"Transferred "<<point->key[point->size-1]<<" from right sibling of leaf node\n";
				return;
			}
		}
		//must merge and delete a node
		if(leftSibling >= 0)//if left sibling exist
		{
			Node* leftNode = parent->ptr[leftSibling];
			// transfer all keys to leftsibling and then transfer pointer to next leaf node
			for(int i = leftNode->size, j = 0; j < point->size; i++, j++)
			{
				leftNode->key[i] = point->key[j];
			}
			leftNode->ptr[leftNode->size] = NULL;
			leftNode->size += point->size;
			leftNode->ptr[leftNode->size] = point->ptr[point->size];
			cout<<"Merging two leaf nodes\n";
			removeInternal(parent->key[leftSibling],parent,point);// delete parent node key
			delete[] point->key;
			delete[] point->ptr;
			delete point;
		}
		else if(rightSibling <= parent->size)//if right sibling exist
		{
			Node* rightNode = parent->ptr[rightSibling];
			// transfer all keys to point and then transfer pointer to next leaf node
			for(int i = point->size, j = 0; j < rightNode->size; i++, j++)
			{
				point->key[i] = rightNode->key[j];
			}
			point->ptr[point->size] = NULL;
			point->size += rightNode->size;
			point->ptr[point->size] = rightNode->ptr[rightNode->size];
			cout<<"Merging two leaf nodes\n";
			removeInternal(parent->key[leftSibling+1],parent,rightNode);// delete parent node key
			delete[] rightNode->key;
			delete[] rightNode->ptr;
			delete rightNode;
		}
	}
}
void BPTree::removeInternal(int x, Node* point, Node* child)
{
	//deleting the key x first
	//checking if key from root is to be deleted
	if(point == root)
	{
		if(point->size == 1)//if only one key is left, change root
		{
			if(point->ptr[1] == child)
			{
				delete[] child->key;
				delete[] child->ptr;
				delete child;
				root = point->ptr[0];
				delete[] point->key;
				delete[] point->ptr;
				delete point;
				cout<<"Changed root node\n";
				return;
			}
			else if(point->ptr[0] == child)
			{
				delete[] child->key;
				delete[] child->ptr;
				delete child;
				root = point->ptr[1];
				delete[] point->key;
				delete[] point->ptr;
				delete point;
				cout<<"Changed root node\n";
				return;
			}
		}
	}
	int pos;
	for(pos = 0; pos < point->size; pos++)
	{
		if(point->key[pos] == x)
		{
			break;
		}
	}
	for(int i = pos; i < point->size; i++)
	{
		point->key[i] = point->key[i+1];
	}
	//now deleting the pointer child
	for(pos = 0; pos < point->size+1; pos++)
	{
		if(point->ptr[pos] == child)
		{
			break;
		}
	}
	for(int i = pos; i < point->size+1; i++)
	{
		point->ptr[i] = point->ptr[i+1];
	}
	point->size--;
	if(point->size >= (MAX+1)/2-1)//no underflow
	{
		cout<<"Deleted "<<x<<" from internal node successfully\n";
		return;
	}
	cout<<"Underflow in internal node!\n";
	//underflow, try to transfer first
	Node* parent = findParent(root, point);
	int leftSibling, rightSibling;
	//finding left n right sibling of point
	for(pos = 0; pos < parent->size+1; pos++)
	{
		if(parent->ptr[pos] == point)
		{
			leftSibling = pos - 1;
			rightSibling = pos + 1;
			break;
		}
	}
	//try to transfer
	if(leftSibling >= 0)//if left sibling exists
	{
		Node *leftNode = parent->ptr[leftSibling];
		//check if it is possible to transfer
		if(leftNode->size >= (MAX+1)/2)
		{
			//make space for transfer of key
			for(int i = point->size; i > 0; i--)
			{
				point->key[i] = point->key[i-1];
			}
			//transfer key from left sibling through parent
			point->key[0] = parent->key[leftSibling];
			parent->key[leftSibling] = leftNode->key[leftNode->size-1];
			//transfer last pointer from leftnode to point
			//make space for transfer of ptr
			for (int i = point->size+1; i > 0; i--)
			{
				point->ptr[i] = point->ptr[i-1];
			}
			//transfer ptr
			point->ptr[0] = leftNode->ptr[leftNode->size];
			point->size++;
			leftNode->size--;
			cout<<"Transferred "<<point->key[0]<<" from left sibling of internal node\n";
			return;
		}
	}
	if(rightSibling <= parent->size)//check if right sibling exist
	{
		Node *rightNode = parent->ptr[rightSibling];
		//check if it is possible to transfer
		if(rightNode->size >= (MAX+1)/2)
		{
			//transfer key from right sibling through parent
			point->key[point->size] = parent->key[pos];
			parent->key[pos] = rightNode->key[0];
			for (int i = 0; i < rightNode->size -1; i++)
			{
				rightNode->key[i] = rightNode->key[i+1];
			}
			//transfer first pointer from rightnode to point
			//transfer ptr
			point->ptr[point->size+1] = rightNode->ptr[0];
			for (int i = 0; i < rightNode->size; ++i)
			{
				rightNode->ptr[i] = rightNode->ptr[i+1];
			}
			point->size++;
			rightNode->size--;
			cout<<"Transferred "<<point->key[0]<<" from right sibling of internal node\n";
			return;
		}
	}
	//transfer wasnt posssible hence do merging
	if(leftSibling >= 0)
	{
		//leftnode + parent key + point
		Node *leftNode = parent->ptr[leftSibling];
		leftNode->key[leftNode->size] = parent->key[leftSibling];
		for(int i = leftNode->size+1, j = 0; j < point->size; j++)
		{
			leftNode->key[i] = point->key[j];
		}
		for(int i = leftNode->size+1, j = 0; j < point->size; j++)
		{
			leftNode->ptr[i] = point->ptr[j];
			point->ptr[j] = NULL;
		}
		leftNode->size += point->size+1;
		point->size = 0;
		//delete point
		removeInternal(parent->key[leftSibling], parent, point);
		cout<<"Merged with left sibling\n";

	}
	else if(rightSibling <= parent->size)
	{
		//point + parent key + rightnode
		Node *rightNode = parent->ptr[rightSibling];
		point->key[point->size] = parent->key[leftSibling];
		for(int i = point->size+1, j = 0; j < rightNode->size; j++)
		{
			point->key[i] = rightNode->key[j];
		}
		for(int i = point->size+1, j = 0; j < rightNode->size; j++)
		{
			point->ptr[i] = rightNode->ptr[j];
			rightNode->ptr[j] = NULL;
		}
		point->size += rightNode->size+1;
		rightNode->size = 0;
		//delete point
		removeInternal(parent->key[leftSibling], parent, rightNode);
		cout<<"Merged with right sibling\n";
	}
}
void BPTree::display(Node* point)
{
	//depth first display
	if(point!=NULL)
	{
		for(int i = 0; i < point->size; i++)
		{
			cout<<point->key[i]<<" ";
		}
		cout<<"\n";
		if(point->checkleaf != true)
		{
			for(int i = 0; i < point->size+1; i++)
			{
				display(point->ptr[i]);
			}
		}
	}
}
void BPTree::checked(Node* point)
{
	if(point!=NULL)
	{
		for(int i = 0; i < point->size; i++)
		{
			if(point->checkleaf == true)
			{A[check]=point->key[i];
			++check;}
		}
		cout<<"\n";
		if(point->checkleaf != true)
		{
			for(int i = 0; i < point->size+1; i++)
			{
				checked(point->ptr[i]);
			}
		}
	}
}
Node* BPTree::getRoot()
{
	return root;
}
void BPTree::cleanUp(Node* point)
{
	//clean up logic
	if(point!=NULL)
	{
		if(point->checkleaf != true)
		{
			for(int i = 0; i < point->size+1; i++)
			{
				cleanUp(point->ptr[i]);
			}
		}
		delete[] point->key;
		delete[] point->ptr;
		delete point;
	}
}
BPTree::~BPTree()
{
	//calling cleanUp routine
	cleanUp(root);
}
