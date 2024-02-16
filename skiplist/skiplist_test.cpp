//skiplist_test.cpp
//created by cwh in 2024/2/15

#include "skiplist_impl.h"
#include "../util/random.h"
#include <iostream>
#include <set>

using namespace std;

typedef uint64_t Key;

struct Comparator {
  int operator()(const Key& a, const Key& b) const {
    if (a < b) {
      return -1;
    } else if (a > b) {
      return +1;
    } else {
      return 0;
    }
  }
};

int main(){
    const int N = 2000;
    const int R = 5000;
    Random rnd(1000);
    std::set<Key> keys;
    Comparator cmp;
    SkipList<Key, Comparator> list(cmp);
    std::cout<<"listnum:"<<list.GetKeyNum()<<std::endl;
    for (int i = 0; i < N; i++) {
        Key key = rnd.Next() % R;
        if (keys.insert(key).second) {
            list.Insert(key);
        }
    }
    std::cout<<"after insertion, listnum:"<<list.GetKeyNum()<<std::endl;

    for (int i = 0; i < R; i++) {
        if (list.Contains(i)&&!keys.count(i)) {
            std::cout<<"insert data error"<<std::endl;
        }
    }

    list.Clear();
    std::cout<<"after clead, listnum:"<<list.GetKeyNum()<<std::endl;

    list.Insert(0);
    list.Insert(1);
    list.Insert(3);
    list.Insert(10);
    list.Insert(7);
    list.Insert(9);
    std::cout<<"after insert, listnum:"<<list.GetKeyNum()<<std::endl;

    std::cout<<"test successfully"<<std::endl;
}