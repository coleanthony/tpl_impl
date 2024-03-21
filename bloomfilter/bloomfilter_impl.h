//bloomfilter

//布隆过滤器是由布隆（Burton Howard Bloom）在1970年提出的 一种紧凑型的、比较巧妙的概率型数据结构，特点是高效地插入和查询，
//可以用来告诉你 “某样东西一定不存在或者可能存在”，它是用多个哈希函数，将一个数据映射到位图结构中。此种方式不仅可以提升查询效率，也可以节省大量的内存空间 .

//虽然布隆过滤器还是会出现误判，因为这个数据的比特位被其他数据所占，但是判断一个数据不存在是准确，不存在就是0！
//布隆过滤器改进：映射多个位置，降低误判率（位置越多，消耗也越多）


//如果布隆过滤器长度比较小，比特位很快会被占为1，误判率自然会上升，所以布隆过滤器的长度会影响误判率，理论上来说，如果一个值映射的位置越多，
//则误判的概率越小，但是并不是位置越多越好，空间也会消耗：


//m=- nlnp/(ln2)^2
//k=mln2/n
//k是哈希函数的个数，m是布隆过滤器长度，n是插入元素的个数，p是误判率
//我们可以来估算一下，假设用 3 个哈希函数，即K=3，ln2 的值我们取 0.7，那么 m 和 n 的关系大概是 m = n×k/ln2=4.2n ，也就是过滤器长度应该是插入元素个数的 4 -5倍

//应用 提高查找效率：客户端中查找一个用户的ID与服务器中的是否相同，在增加一层布隆过滤器提高查找效率：


#include<string>
#include <stddef.h>
#include<bitset>

//3个哈希函数
struct BKDRHash
{
	size_t operator()(const std::string& s)
	{
		size_t hash = 0;
		for (auto ch : s)
		{
			hash += ch;
			hash *= 31;
		}
		return hash;
	}
};
struct APHash
{
	size_t operator()(const std::string& s)
	{
		size_t hash = 0;
		
		for (long i = 0; i < s.size(); i++)
		{
			size_t ch = s[i];
			if ((i & 1) == 0)
			{
				hash ^= ((hash << 7) ^ ch ^ (hash >> 3));
			}
			else
			{
				hash ^= (~((hash << 11) ^ ch ^ (hash >> 5)));
			}
		}
		return hash;
	}
};
struct DJBHash
{
	size_t operator()(const std::string& s)
	{
		size_t hash = 5381;
		for (auto ch : s)
		{
			hash += (hash << 5) + ch;
		}
		return hash;
	}
};
//假定三个哈希函数,N个比特位,K是我们所要查找的键值
//Hash:将key转换成整型，才能进行映射
//N表示最多会插入的key的个数
//哈希函数的个数，代表一个key值映射几个位，哈希函数越多，误判率越低
//但是哈希函数越多，我们平均消耗的空间也会越多
template<size_t N,
class K = std::string,
class Hash1 = BKDRHash,
class Hash2 = APHash,
class Hash3 = DJBHash>

class BloomFilter
{
public:
	//布隆过滤器的插入
	void set(const K& key)
	{
		size_t len = N * _X;
		//key值分别通过三个哈希函数进行映射
		size_t hash1 = Hash1()(key) % len;
		_bs.set(hash1);

		size_t hash2 = Hash2()(key) % len;
		_bs.set(hash2);

		size_t hash3 = Hash3()(key) % len;
		_bs.set(hash3);
		
		cout << hash1 << " " << hash2 << " " << hash3 << " " << endl << endl;
	}
	//布隆过滤器的查找
	bool test(const K& key)
	{
		size_t len = N * _X;

		//3个位置key值都存在，才能证明key值存在
		//所以只要有一个位置key不存在，就返回假
		//key值分别通过三个哈希函数进行映射
		size_t hash1 = Hash1()(key) % len;
		if (!_bs.test(hash1))
		{
			return false;
		}

		size_t hash2 = Hash2()(key) % len;
		if (!_bs.test(hash2))
		{
			return false;
		}

		size_t hash3 = Hash3()(key) % len;
		if (!_bs.test(hash3))
		{
			return false;
		}
	}
private:
	static const size_t _X = 4;
	bitset<N*_X> _bs;
};









