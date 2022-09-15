#include<iostream>

using namespace std;
#define OK 0
#define ERROR -1

class ListNode       //结点类定义
{
public:
    int data;
    ListNode *next;

    ListNode() {
        data = -9999, next = NULL;
    }

    ListNode(int item, ListNode *pt) {
        data = item, next = pt;
    }
};

class LinkList {             //带头结点的单链表，位置从0到n，0是头结点，1是首结点，n是尾结点
private:
    ListNode *head;           //头结点
    int size;                  //表长
    ListNode *idex(int i);      //定位函数，返回第i个结点
public:
    LinkList();          //构造函数，创造头结点
    ~LinkList();         //析构函数，逐个结点回收
    int LL_insert(int item, int i);    //第i个位置插入元素，操作成功或失败返回OK或ERROR
    int LL_del(int i);                //删除
    int LL_get(int i);                 //获取位置i的元素
    void LL_print();                    //打印单链表所有数据
};

LinkList::LinkList() : size(0) {
    head = new ListNode();
}

LinkList::~LinkList()       //要逐个结点回收
{
    ListNode *p, *q;
    p = head->next;
    while (p != NULL) {
        q = p;
        p = p->next;
        delete q;
    }
    size = 0;
    head->next = NULL;
}


int LinkList::LL_insert(int item, int i)            //插入新数据
{
    if (i - 1 > size) {
        return ERROR;
    }
    int j = 0;
    ListNode *p = head, *q = head;  //

    while (j <= i - 1) {
        if (j != 0) q = q->next;
        p = p->next;
        j++;
    }
    if (p != nullptr) {
        ListNode *t = new ListNode;
        t->data = p->data;
        t->next = p->next;
        p->data = item;
        p->next = t;
    } else {
        p = new ListNode;
        p->data = item;
        q->next = p;
    }
    size++;
    return OK;
}

int LinkList::LL_del(int i)            //删除数据
{
    ListNode *p = head, *q = head;
    if (i - 1 >= size || i - 1 < 0) {
        return ERROR;
    }
    int j = 0;
    while (j <= i - 1) {
        if (j != 0) q = q->next;
        p = p->next;
        j++;
    }

    q->next = p->next;
    size--;
    delete p;
    return OK;

};

int LinkList::LL_get(int i)            //查找数据
{
    if (i - 1 < 0 || i - 1 >= size) {
        return ERROR;
    }
    ListNode *p = head->next;
    int j = 0;
    while (j != i - 1) {
        p = p->next;
        j++;
    }
    return p->data;
};

void LinkList::LL_print()            //打印数据
{
    ListNode *p;
    for (p = head->next; p != NULL; p = p->next) {
        if (p != head->next)
            cout << " ";
        cout << p->data;
    }
    cout << "\n";
};

//主函数定义
int main()
{	int i, t, temp, pos;
    LinkList sl;	//创建单链表
//初始化链表
    cin>>t;	//输入初始长度
    for (i=1; i<=t; i++)
    {	cin>>temp;	sl.LL_insert(temp, i);	}
    sl.LL_print();
//两次插入
    for (i=0; i<2; i++)
    {	cin>>pos>>temp;
        t=sl.LL_insert(temp, pos);
        if (t==ERROR) cout<<"error"<<endl;
        else sl.LL_print();
    }
//两次删除
    for (i=0; i<2; i++)
    {	cin>>pos;
        t=sl.LL_del(pos);
        if (t==ERROR) cout<<"error"<<endl;
        else sl.LL_print();
    }
//两次查找
    for (i=0; i<2; i++)
    {	cin>>pos;
        t=sl.LL_get(pos);
        if (t==ERROR) cout<<"error"<<endl;
        else cout<<t<<endl;
    }
    return 0;
}
