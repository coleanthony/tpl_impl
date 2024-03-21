//在leveldb中作者实现了自己的一个hash表，相比标准库而言，去除了移植性，而且要比gcc4.4.3中内置的随机取快5%。

// We provide our own simple hash table since it removes a whole bunch
// of porting hacks and is also faster than some of the built-in hash
// table implementations in some of the compiler/runtime combinations
// we have tested.  E.g., readrandom speeds up by ~5% over the g++
// 4.4.3's builtin hashtable.

//它是用来存lru中的节点的，原本的结构是LRUHandle，我这里用template修改

//特点
//1.默认大小是0，如果元素个数少于4时，哈希表按照元素个数来分配呢欧村；大于等于4时，则按照2的倍数对齐
//2.对冲突采用链式地址法
//3.rehash是实际当元素个数大于链表长度时

#include <string>
#include <string.h>
#include <cassert>

struct testtype{
    std::string keyx;
    uint32_t hash;
    testtype* next_hash;
    testtype* next;
    testtype* prev;

    std::string key() const{
        return keyx;
    }
};

template<typename T>
class HandleTable {
 public:
  HandleTable() : length_(0), elems_(0), list_(nullptr) { Resize(); }
  ~HandleTable() { delete[] list_; }

  //查找某个key
  T* Lookup(const std::string& key, uint32_t hash) {
    return *FindPointer(key, hash);
  }

  T* Insert(T* h) {
    T** ptr = FindPointer(h->key(), h->hash);
    T* old = *ptr;
    h->next_hash = (old == nullptr ? nullptr : old->next_hash);
    *ptr = h;
    if (old == nullptr) {
      ++elems_;
      if (elems_ > length_) {
        // Since each cache entry is fairly large, we aim for a small
        // average linked list length (<= 1).
        Resize();
      }
    }
    return old;
  }

  T* Remove(const std::string& key, uint32_t hash) {
    T** ptr = FindPointer(key, hash);
    T* result = *ptr;
    if (result != nullptr) {
      *ptr = result->next_hash;
      --elems_;
    }
    return result;
  }


    //for test
  uint32_t GetLength(){
    return length_;
  }

  uint32_t GetElems(){
    return elems_;
  }

 private:
  // The table consists of an array of buckets where each bucket is
  // a linked list of cache entries that hash into the bucket.
  uint32_t length_;      //hash表总长度
  uint32_t elems_;       //当前hash表实际元素个数
  T** list_;     //存储实际数据

  // Return a pointer to slot that points to a cache entry that
  // matches key/hash.  If there is no such cache entry, return a
  // pointer to the trailing slot in the corresponding linked list.
  //用来定位元素所在节点
  T** FindPointer(const std::string& key, uint32_t hash) {
    T** ptr = &list_[hash & (length_ - 1)];
    while (*ptr != nullptr && ((*ptr)->hash != hash || key != (*ptr)->key())) {
      ptr = &(*ptr)->next_hash;
    }
    return ptr;
  }

  //链表扩容
  void Resize() {
    uint32_t new_length = 4;
    while (new_length < elems_) {
      new_length *= 2;
    }
    T** new_list = new T*[new_length];
    memset(new_list, 0, sizeof(new_list[0]) * new_length);
    uint32_t count = 0;
    for (uint32_t i = 0; i < length_; i++) {
      T* h = list_[i];
      while (h != nullptr) {
        T* next = h->next_hash;
        uint32_t hash = h->hash;
        T** ptr = &new_list[hash & (new_length - 1)];
        h->next_hash = *ptr;
        *ptr = h;
        h = next;
        count++;
      }
    }
    assert(elems_ == count);
    delete[] list_;
    list_ = new_list;
    length_ = new_length;
  }
};
