#include <vector>

template<size_t N>
class bitset
{
public:
    // 构造函数
    bitset()
    {
        // 计算需要分配的char数组的大小，并使用vector容器创建一个大小为该值的char数组
        // 将所有元素初始化为0
        _bits.resize(N/8 + 1, 0);
    }

    // 设置位图中的元素
    void set(size_t x)
    {
        // 计算元素在位图中对应的索引和偏移量
        size_t i = x / 8;
        size_t j = x % 8;

        // 使用按位或操作符|将对应的二进制位设置为1
        _bits[i] |= (1 << j);
    }

    // 重置位图中的元素
    void reset(size_t x)
    {
        // 计算元素在位图中对应的索引和偏移量
        size_t i = x / 8;
        size_t j = x % 8;

        // 使用按位与非操作符~和按位与操作符&将对应的二进制位设置为0
        _bits[i] &= ~(1 << j);
    }

    // 测试位图中的元素是否存在
    bool test(size_t x)
    {
        // 计算元素在位图中对应的索引和偏移量
        size_t i = x / 8;
        size_t j = x % 8;

        // 使用按位与操作符&将对应的二进制位提取出来，并返回其值
        return _bits[i] & (1 << j);
    }

private:
    // 存储位图数据的私有成员变量
    std::vector<char> _bits;
};

