/**
 * File: built_in_hash.cpp
 * Created Time: 2023-06-21
 * Author: krahets (krahets@163.com)
 */

#include "../utils/common.hpp"

/* Driver Code */
int main() {
    int num = 3;
    // hash<int> is a function object (functor) provided by the C++ Standard Library. hash<int>()
    // creates an instance of the std::hash<int> class. It is used to compute a hash value for an
    // integer (int in this case). size_t is an unsigned integral type used for representing sizes
    // and indices, with a range from 0 to 2^64 - 1.
    size_t hashNum = hash<int>()(num);
    cout << "整数 " << num << " 的哈希值为 " << hashNum << "\n";

    bool bol = true;
    size_t hashBol = hash<bool>()(bol);
    cout << "布尔量 " << bol << " 的哈希值为 " << hashBol << "\n";

    double dec = 3.14159;
    size_t hashDec = hash<double>()(dec);
    cout << "小数 " << dec << " 的哈希值为 " << hashDec << "\n";

    string str = "Hello 算法";
    size_t hashStr = hash<string>()(str);
    cout << "字符串 " << str << " 的哈希值为 " << hashStr << "\n";

    // 在 C++ 中，内置 std:hash() 仅提供基本数据类型的哈希值计算
    // 数组、对象的哈希值计算需要自行实现
}
