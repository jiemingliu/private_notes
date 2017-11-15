stack
栈：先进后出
队列：先进先出

树tree

二叉树 binary tree
struct Node{
	T data;
	Node* left;
	Node* right;
};

二叉查找树
前序 跟左右  中序 坐跟右   后序 右跟左         一般采用中序，因为二叉树排序是左子树比跟节点数据小，右子树比跟节点数据大
二叉树是个典型的递归问题 