//skiplist_impl.h
//created by cwh in 2024/2/15
#pragma once

#include <cassert>
#include "../util/random.h"

template <typename Key, class Comparator>
class SkipList {
private:
    struct Node;
    int KMaxHeight=12;
    int max_height_;
    Comparator const compare_;
    Node* const head_;
    Random rnd_;

    int keynum; //for test

    inline int GetMaxHeight() const{
        return max_height_;
    }

    Node* NewNode(const Key& key,int height);
    int RandomHeight();
    bool Equal(const Key& a, const Key& b) const { return (compare_(a, b) == 0); }
    // Return true if key is greater than the data stored in "n"
    bool KeyIsAfterNode(const Key& key, Node* n) const;
    bool KeyIsBeforeNode(const Key& key, Node* n) const;
    // Return the earliest node that comes at or after key.
    // Return nullptr if there is no such node.
    //
    // If prev is non-null, fills prev[level] with pointer to previous
    // node at "level" for every level in [0..max_height_-1].
    Node* FindGreaterOrEqual(const Key& key, Node** prev) const;
    // Return the latest node with a key < key.
    // Return head_ if there is no such node.
    Node* FindLessThan(const Key& key) const;
    // Return the last node in the list.
    // Return head_ if list is empty.
    Node* FindLast() const;

public:
    explicit SkipList(Comparator cmp);
	~SkipList();
    SkipList(const SkipList&) = delete;
    SkipList& operator=(const SkipList&) = delete;
    // Insert key into the list.
    // REQUIRES: nothing that compares equal to key is currently in the list.
    void Insert(const Key& key);
    // Returns true iff an entry that compares equal to key is in the list.
    bool Contains(const Key& key) const;
    // delete
    bool Delete(const Key& key);
    //clear all the data in the skiplist
    void Clear();
    //for test
    int GetKeyNum(){
        return keynum;
    } 
};

//
// Implementation details follow
template <typename Key, class Comparator>
struct SkipList<Key, Comparator>::Node {
    explicit Node(const Key& k) : key(k) {}
    Key const key;
    // Accessors/mutators for links.  Wrapped in methods so we can
    // add the appropriate barriers as necessary.
    Node* Next(int n) {
        assert((n >= 0));
        return next_[n];
    }
    void SetNext(int n, Node* x) {
        assert(n >= 0);
        next_[n]=x;
    }

private:
  // Array of length equal to the node height.  next_[0] is lowest level link.
    Node* next_[0];
};

//注意template和typename的使用，返回一个SkipList<Key, Comparator>::Node*类型变量
template <typename Key, class Comparator>
typename SkipList<Key,Comparator>::Node* SkipList<Key,Comparator>::NewNode(const Key& key,int height){
    char* node_memory=new char[sizeof(Node)+sizeof(Node*)*height];
    return new (node_memory) Node(key);
}

template <typename Key, class Comparator>
int SkipList<Key,Comparator>::RandomHeight(){
    //get random height for a node
    static const int RandomK=4;
    int height=1;
    while (height<KMaxHeight){
        if (!rnd_.OneIn(RandomK)){
            break;
        }
        height++;
    }
    return height;
}

template <typename Key, class Comparator>
bool SkipList<Key,Comparator>::KeyIsAfterNode(const Key& key, Node* n) const{
    return (n!=nullptr)&&(compare_(key,n->key)>0);
}

template <typename Key, class Comparator>
bool SkipList<Key,Comparator>::KeyIsBeforeNode(const Key& key, Node* n) const{
    return (n!=nullptr)&&(compare_(key,n->key)<0);
}

template <typename Key, class Comparator>
typename SkipList<Key,Comparator>::Node* SkipList<Key,Comparator>::FindGreaterOrEqual(const Key& key, Node** prev) const{
    Node* p=head_;
    int height=GetMaxHeight()-1;
    while (true){
        Node* next=p->Next(height);
        if (KeyIsAfterNode(key,next)){
            //key比node的值更大
            p=next;
        }else{
            //key<=node.key
            if (prev!=nullptr){
                prev[height]=p;
            }
            if (height>0){
                //down
                height--;
            }else{
                //height is 0
                return next;
            }
        }
    }
    return nullptr;
}

template <typename Key, class Comparator>
typename SkipList<Key,Comparator>::Node* SkipList<Key,Comparator>::FindLessThan(const Key& key) const{
    Node* p=head_;
    int height=GetMaxHeight()-1;
    while (true){
        Node* next=p->Next(height);
        if (next!=nullptr&&compare_(key,next->key)>=0){
            //next.val>=key
            if (height>0){
                height--;
            }else{
                return p;
            }
        }else{
            p=next;
        }
    }
    return nullptr;
}

template <typename Key, class Comparator>
typename SkipList<Key,Comparator>::Node* SkipList<Key,Comparator>::FindLast() const{
    Node *p=head_;
    int height=GetMaxHeight()-1;
    while (true){
        Node* next=p->Next(height);
        if (next==nullptr){
            if (height>0){
                height--;
            }else{
                return p;
            }
        }else{
            p=next;
        }
    }
    return nullptr;
}

template <typename Key, class Comparator>
SkipList<Key,Comparator>::SkipList(Comparator cmp):
    compare_(cmp),
    head_(NewNode(0,KMaxHeight)),
    rnd_(0xdeadbeef),
    max_height_(1),
    keynum(0){
    for(int i=0;i<KMaxHeight;i++){
        head_->SetNext(i,nullptr);
    }
}

template <typename Key, class Comparator>
SkipList<Key,Comparator>::~SkipList(){
    Node* p=head_;
    while (p){
        Node* cur=p;
        p=p->Next(0);
        delete cur;
    }
}

template <typename Key, class Comparator>
void SkipList<Key,Comparator>::Clear(){
    Node* p=head_->Next(0);
    while (p){
        Node* cur=p;
        p=p->Next(0);
        delete cur;
    }
    max_height_=1;
        for(int i=0;i<KMaxHeight;i++){
        head_->SetNext(i,nullptr);
    }
    keynum=0;
}

template <typename Key, class Comparator>
void SkipList<Key,Comparator>::Insert(const Key& key){
    Node* prev[KMaxHeight];
    int height=RandomHeight();
    Node* p=FindGreaterOrEqual(key,prev);
    assert(p==nullptr||!Equal(p->key,key));

    if (height>max_height_){
        for (int i = 0; i < height; i++){
            prev[i]=head_;
        }
        max_height_=height;
    }   
    
    Node* newnode=NewNode(key,height);
    for (int i = 0; i < height; i++){
        newnode->SetNext(i,prev[i]->Next(i));
        prev[i]->SetNext(i,newnode);
    }
    keynum++;
}

template <typename Key, class Comparator>
bool SkipList<Key,Comparator>::Contains(const Key& key) const{
    Node *p=FindGreaterOrEqual(key,nullptr);
    if (p!=nullptr&&Equal(key,p->key)){
        return true;
    }
    return false;
}

template <typename Key, class Comparator>
bool SkipList<Key,Comparator>::Delete(const Key& key){
    Node* prev[KMaxHeight];
    Node* p=FindGreaterOrEqual(key,prev);
    assert(p!=nullptr&&Equal(p->key,key));
    for (int i = 0; i < GetMaxHeight(); i++){
        if (p[i]==nullptr){
            break;
        }
        prev[i]->SetNext(i,p[i]->Next(i));
    }
    keynum--;
}