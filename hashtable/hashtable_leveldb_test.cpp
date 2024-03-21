#include "hashtable_leveldb.h"
#include "../util/hash.h"
#include <iostream>

int main(){
    HandleTable<testtype> h;
    uint32_t length=0,elems=0;
    testtype *t1=new testtype();
    testtype *t2=new testtype();
    testtype *t3=new testtype();

    t1->keyx="12345";
    t2->keyx="67890";
    t3->keyx="abc";
    
    t1->hash=Hash(t1->keyx.c_str(),t1->keyx.size(),1);
    t2->hash=Hash(t2->keyx.c_str(),t2->keyx.size(),1);
    t3->hash=Hash(t3->keyx.c_str(),t3->keyx.size(),1);

    std::cout<<t1->keyx<<" "<<t1->hash<<std::endl;

    h.Insert(t1);
    length=h.GetLength(),elems=h.GetElems();
    std::cout<<"after insert t1, length:"<<length<<"  elems:"<<elems<< std::endl;

    auto tmpres=h.Lookup(t1->keyx,t1->hash);
    std::cout<<"find:"<<tmpres->keyx<<" "<<tmpres->hash<<std::endl;

    h.Insert(t2);
    h.Insert(t3);
    length=h.GetLength(),elems=h.GetElems();
    std::cout<<"after insert t2 t3, length:"<<length<<"  elems:"<<elems<< std::endl;

    h.Remove(t1->keyx,t1->hash);
    length=h.GetLength(),elems=h.GetElems();
    std::cout<<"after remove t1, length:"<<length<<"  elems:"<<elems<< std::endl;
    
    return 0;
}