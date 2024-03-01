//对顶堆
//小顶堆的数总是大于大顶堆的数
#include<vector>
#include <queue>

using namespace std;

class Dualheap
{
private:
    priority_queue<int> down;                              //大顶堆
    priority_queue<int,vector<int>,greater<int>> up;       //小顶堆
    long long lowsum,upsum;

    double getmedium(){
        if ((down.size()+up.size())%2)  return down.top();
        double sum=0.0;
        if (!down.empty()) sum+=down.top();
        if(!up.empty()) sum+=up.top();
        return sum/2.0;
    }
public:
    Dualheap():lowsum(0),upsum(0){}
    void insert(int num){
        if(down.empty()||down.top()>=num){
            down.push(num);
            lowsum+=num;
        }else{
            up.push(num);
            upsum+=num;
        }

        if(down.size()>up.size()+1){
            int popnum=down.top();
            down.pop();
            up.push(popnum);
            lowsum-=popnum;
            upsum+=popnum;
        }else if(up.size()>down.size()){
            int popnum=up.top();
            up.pop();
            down.push(popnum);
            lowsum+=popnum;
            upsum-=popnum;
        }
    }
};