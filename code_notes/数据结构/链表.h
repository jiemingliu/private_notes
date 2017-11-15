#include <iostream>
using namespace std;
typedef int T；
class List{
	struct Node{
		T data;
		Node* next;
		Node(const T& d=T()):data(d),next(0){}      //d=T()表示零初始化，如果T是基本类型表示数值0，如果是自定义类型表示建一个匿名对象不传参数
	};
	Node* head;  //头指针，用来保存头节点的地址
public:
    List():head(NULL),len(0){}
	void push_front(const T& d){         //前插，可以直接写成insert(d,0);
		Node* p = new Node(d);
		p->next = head;
		head = p;
	}
	void push_front(const T& d){         //前插
		insert(d,0);
	}
	void push_back(const T& d){         //尾插,返回List &   然后return *this可以连续插入
		insert(d,size());
	}
	Node*& getptr(int pos){		 //返回链表中指向某个位置的原始指针，所以用引用
		if(pos<0||pos>size()) pos = 0;    //超过链表范围
		if(pos==0) return head;      //pos=1:  return (*p).next
		Node * p = head;             //pos=2:  p=p->next; return (*p).next
		for(int i = 1;i<pos;i++)     //pos=3:  p=p->next;p=p->next;   return (*p).next
			p = p->next;
		return (*p).next;
	}
	int size()const{       //应该直接return  len;
		int count;
		Node* p = head;
		 while(p!=NULL){
			++count;
			p = p->next;
		 }
		 return count;
	}
	void insert(const T& d, int pos){           //在任意位置插入(1，在链表里面找到指向那个要插入的位置的指针；2，让新节点的next跟找到的那个指针指向同一个位置；3，让被找到的指针指向新的节点)
		Node*& p = getptr(pos);       //此处用引用保证使用的原始指针
		Node* p = new Node(d);
		p->next = pn;
		pn = p;
		++len;
		
	}
	void travel()const{      //遍历
		 Node* p = head;
		 while(p!=NULL)
			cout << p->data << ' ';
			p = p->next;
		}
		cout << endl;
	void clear(){            //清空这个链表
		while(head!=NULL){
			Node* p =head->next;
			delete head;
			head = p;
		}
	}
	~List(){
		clear();
	}
	}
public:
	List():head(Null){}
};
void main
{
	List l;
	l.push_front(11);
	l.push_front(12);
	l.push_front(13);
	l.travel();
	return 0;
}

链表里的节点都是动态创建的,所以创建了要释放