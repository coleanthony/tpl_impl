#include<unordered_map>

struct node{
    int key,val,frq;
    node *next,*prev;
    node(int key,int val,int frq):key(key),val(val),frq(frq),next(NULL),prev(NULL){}
};

class LFUCache {
private:
    int capacity;
    int minfreq;
    std::unordered_map<int,node*> mymap;//记录LRU的情况
    std::unordered_map<int,std::pair<node*,node*>> frequse;//记录LFU的情况，即计数器

public:
    LFUCache(int capacity) {
        this->capacity=capacity;
        minfreq=1;
    }

    void remove(node* temp){
        node* pre=temp->prev;
        pre->next=temp->next;
        temp->next->prev=pre;
    }
    
    void inserttofirst_(node* temp){
        int frq=temp->frq;
        node* head=frequse[frq].first;
        node* tp=head->next;
        head->next=temp;
        temp->prev=head;
        temp->next=tp;
        tp->prev=temp;
    }

    void buildbylist(node*temp){
        if(frequse[temp->frq].first==NULL){
            node* head=new node(-1,-1,temp->frq);
            node* tail=new node(-1,-1,temp->frq);
            head->next=tail;
            tail->prev=head;
            frequse[temp->frq].first=head;
            frequse[temp->frq].second=tail;
        }
    }
    
    int get(int key) {
        if(capacity==0)
            return -1;
        if(mymap.count(key)){
            node* temp=mymap[key];
            remove(temp);
            int prefrq=temp->frq;
            temp->frq++;
            buildbylist(temp);
            inserttofirst_(temp);
            if(frequse[prefrq].first->next==frequse[prefrq].second){
                frequse.erase(prefrq);
                if(minfreq==prefrq)
                    minfreq=prefrq+1;
            }
            int res=temp->val;
            return res;
        }
        else
            return -1;
    }
    
    void put(int key, int value) {
        if(capacity==0) return;
        if(mymap.count(key)){
            mymap[key]->val=value;
            node* temp=mymap[key];
            remove(temp);
            int prefrq=temp->frq;
            temp->frq++;
            buildbylist(temp);
            inserttofirst_(temp);
            if(frequse[prefrq].first->next==frequse[prefrq].second){
                frequse.erase(prefrq);
                if(minfreq==prefrq)
                    minfreq=prefrq+1;
            }
        }
        else{
            node* temp=new node(key,value,1);
            if(mymap.size()==capacity){
                node* leastfrequse=frequse[minfreq].second->prev;
                int leastkey=leastfrequse->key;
                int prefrq=leastfrequse->frq;
                mymap.erase(leastkey);
                remove(leastfrequse);
                if(frequse[prefrq].first->next==frequse[prefrq].second)
                    frequse.erase(prefrq);
            }
            mymap[key]=temp;
            buildbylist(temp);
            inserttofirst_(temp);
            minfreq=1;
        }
    }
};

