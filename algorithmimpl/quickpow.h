#include <stdlib.h>

double myPow(double x, int n) {
    double res=1.0;
    n=abs(n);
    int flag=n;
    while(n!=0){
        if(n%2==1)  res*=x;
        x*=x;
        n/=2;
    }
    return flag>0?res:1/res;
}