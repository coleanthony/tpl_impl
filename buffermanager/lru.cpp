#include <unordered_map>

class LRUCache {
private:
    struct Node{
        Node *next,*pre;
        int val=0;
        int key=-1;
        Node(int key,int val):val(val),key(key),next(NULL),pre(NULL){}
    };
    int capacity;
    Node* head,*tail;
    std::unordered_map<int,Node*> mymap;
public:
    LRUCache(int capacity) {
        this->capacity=capacity;
        head=new Node(-1,-1);
        tail=new Node(-1,-1);
        head->next=tail;
        tail->pre=head;
    }
    
    void puttofirst(Node* temp){
        if(temp->pre&&temp->next){
            Node* pr=temp->pre;
            pr->next=temp->next;
            temp->next->pre=pr;
        }
        //temp去最前面
        Node *ppr=head->next;
        head->next=temp;
        temp->pre=head;
        temp->next=ppr;
        ppr->pre=temp;
    }
    
    int get(int key) {
        if(mymap.count(key)){
            //temp断开
            Node* temp=mymap[key];
            puttofirst(temp);
            return temp->val;
        }
        else
            return -1;
    }
    
    void put(int key, int value) {
        if(mymap.count(key)){
            mymap[key]->val=value;
            puttofirst(mymap[key]);
        }
        else{
            Node* nnode=new Node(key,value);
            if(mymap.size()==capacity){
                //删除链尾元素
                Node* p=tail->pre;
                int keynum=p->key;
                mymap.erase(keynum);
                tail->pre=p->pre;
                p->pre->next=tail;
                //插到最前面
                mymap[key]=nnode;
                puttofirst(mymap[key]);
            }
            else{
                mymap[key]=nnode;
                puttofirst(mymap[key]);
            }
        }
    }
};

