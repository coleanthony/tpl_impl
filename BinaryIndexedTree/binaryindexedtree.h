#pragma once

//树状数组能解决的问题是线段树能解决的问题的子集：树状数组能做的，
//线段树一定能做；线段树能做的，树状数组不一定可以。然而，
//树状数组的代码要远比线段树短，时间效率常数也更小
//我们总能将一段前缀 [1, n] 拆成 不多于 \boldsymbol{\log n} 段区间，使得这 \log n 段区间的信息是 已知的。

//管辖区间
//树状数组中，规定 c[x] 管辖的区间长度为 2^{k}，其中：
//设二进制最低位为第 0 位，则 k 恰好为 x 二进制表示中，最低位的 1 所在的二进制位数；
//2^k（c[x] 的管辖区间长度）恰好为 x 二进制表示中，最低位的 1 以及后面所有 0 组成的数。
//举个例子，c_{88} 管辖的是哪个区间？
//因为 88_{(10)}=01011000_{(2)}，其二进制最低位的 1 以及后面的 0 组成的二进制是 1000，即 8，所以 c_{88} 管辖 8 个 a 数组中的元素。
//因此，c_{88} 代表 a[81 \ldots 88] 的区间信息。
//我们记 x 二进制最低位 1 以及后面的 0 组成的数为 \operatorname{lowbit}(x)，那么 c[x] 管辖的区间就是 [x-\operatorname{lowbit}(x)+1, x]。
int lowbit(int x) {
  // x 的二进制中，最低位的 1 以及后面所有 0 组成的数。
  // lowbit(0b01011000) == 0b00001000
  //          ~~~~^~~~
  // lowbit(0b01110010) == 0b00000010
  //          ~~~~~~^~
  return x & -x;
}

//区间查询
//查询 a[l \ldots r] 的和，就是 a[1 \ldots r] 的和减去 a[1 \ldots l - 1] 的和，
//从而把区间问题转化为前缀问题，更方便处理
//可以写出查询 a[1 \ldots x] 的过程：
//1.从 c[x] 开始往前跳，有 c[x] 管辖 a[x-\operatorname{lowbit}(x)+1 \ldots x]；
//2.令 x <- x -{lowbit}(x)，如果 x = 0 说明已经跳到尽头了，终止循环；否则回到第一步。
//3.将跳到的 c 合并。
int c[100];
int getsum(int x) {  // a[1]..a[x]的和
  int ans = 0;
  while (x > 0) {
    ans = ans + c[x];
    x = x - lowbit(x);
  }
  return ans;
}

//树状数组与其树形态的性质
//我们约定：
//l(x) = x - \operatorname{lowbit}(x) + 1。即，l(x) 是 c[x] 管辖范围的左端点。
//对于任意正整数 x，总能将 x 表示成 s \times 2^{k + 1} + 2^k 的形式，其中 \operatorname{lowbit}(x) = 2^k。
//下面「c[x] 和 c[y] 不交」指 c[x] 的管辖范围和 c[y] 的管辖范围不相交，即 [l(x), x] 和 [l(y), y] 不相交。「c[x] 包含于 c[y]」等表述同理。
//性质 {1}：对于 {x <=> y}，要么有 {c[x]} 和 {c[y]} 不交，要么有 {c[x]} 包含于 {c[y]}。
//性质 \boldsymbol{2}：在 \boldsymbol{c[x]} 真包含于 \boldsymbol{c[x + \operatorname{lowbit}(x)]}。
//性质 3：对于任意 \boldsymbol{x < y < x + \operatorname{lowbit}(x)}，有 \boldsymbol{c[x]} 和 \boldsymbol{c[y]} 不交。

//单点修改
//初始令 x' = x。
//修改 c[x']。
//令 x' \gets x' + \operatorname{lowbit}(x')，如果 x' > n 说明已经跳到尽头了，终止循环；否则回到第二步。
int n=100;
void add(int x, int k) {
  while (x <= n) {  // 不能越界
    c[x] = c[x] + k;
    x = x + lowbit(x);
  }
}














