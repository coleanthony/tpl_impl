#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

template<class T>
class PriorityQueue
{
private:
    int Capacity = 100000;  //队列容量
    int size;            //队列大小
    T* data;             //队列变量

public:
    PriorityQueue();
    ~PriorityQueue();

    int Size();
    bool Full();       //判满
    bool Empty();      //判空
    void Push(T key);  //入队
    void Pop();        //出队
    void Clear();      //清空

    T *GetData();
    T Top();           //队首
};

template<class T>
PriorityQueue<T>::PriorityQueue(){
    data = (T*) malloc((Capacity + 1)*sizeof(T));
    if(!data){
        perror("Allocate dynamic memory");
        return;
    }
    size = 0;
}

template<class T>
PriorityQueue<T>::~PriorityQueue(){
    while(!Empty()){
        Pop();
    }
}

template<class T>
//判空
bool PriorityQueue<T>::Empty(){
    if(size > 0){
        return false;
    }
    return true;
}

template<class T>
//清空
void PriorityQueue<T>::Clear(){
    while(!Empty()){
        Pop();
    }
}

template<class T>
T* PriorityQueue<T>::GetData(){
    return data;
}

template<class T>
//判满
bool PriorityQueue<T>::Full(){
    if(size == Capacity){
        return true;
    }
    return false;
}

template<class T>
//大小
int PriorityQueue<T>::Size(){
    return size;
}

template<class T>
//入队
void PriorityQueue<T>::Push(T key){
    //空队则直接入队
    if(Empty()){
        data[++size] = key;
        return;
    }
    int i;
    if(Full()){
        perror("Priority queue is full\n");
        return;
    }
    for(i = ++size; data[i/2] > key; i /= 2){
        data[i] = data[i/2];
    }
    data[i] = key;
}

template<class T>
//队首
T PriorityQueue<T>::Top(){
    if(Empty()){
        //perror("Priority queue is full\n");
        return data[0];
    }
    return data[1];
}

template<class T>
//出队, 取完队首元素后，才执行出队操作，即去除堆顶元素，将末尾元素防止堆顶，并做sink调整
void PriorityQueue<T>::Pop()
{
    int i, child;
    T min, last;

    if(Empty()){
        //perror("Empty queue\n");
        return;
    }
//    min = data[1];
    last = data[size--];
    for(i = 1; i * 2 <= size; i = child){
        child = i * 2;
        if(child != size && data[child + 1] < data[child]){
            child++;
        }
        if(last > data[child]){
            data[i] = data[child];
        } else{
            break;
        }
    }
    data[i] = last;
}

#endif // PRIORITYQUEUE_H

