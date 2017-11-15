数据结构和算法
（数组的天然缺陷:中间或者前面插入数据麻烦。）链表解决问题：链表存放数据零散，每个数据后面附带个东西来保存下一个数据的地址，当插入数据的时候，原本指向下一个数据的地址指向新插入的数据，新插入的地址指向原本那个被指向的数据

链表 linked-list 定义如上
节点 Node 每一个数据和一个地址称为节点，定义一个结构体struct Node
#include <iostream>
using namespace std;
typedef int T；
struct Node{
	T data;
	Node * next;
	Node(const T& d):data(d),next(NULL){}
	operator T(){return Node;}  //类型转换函数,也可以不用这种方式输出
};
void showlist(Node* head)
{
	Node* p = head;
	while(p!=NULL){
		cout << *p << ' ';
		p = (*p).next;  //p = p->next;
	}
	cout << endl;
}
int main
{
	Node a(10),b(20),c(30),d(40),e(50);
	a.next = &b;
	b.next = &c;
	c.next = &d;
	showlist(&a);
	e.next = b.next;   //b.next == &c;   //将e插入到bc之间(插入的过程需要先将要插入的e.next指向c，然后再将b.next指向e)
	b.next = &e;    //Node* & p = b.next  定义一个Node*引用之后可以将b.next换成p
	showlist(&a);    //e 插入到bc之间
	Node* k = new Node(70);     //插入新节点
	k->next = c.next;
	c.next = k;
	showlist(&a);
	delete k;
	
}

动态分配的节点空间没有名字

