// hash算法。无论是开放寻址还是链式地址，都只能保证hash表在发生冲突的时候正常工作，而无法减少hash冲突的发生。
// 添加一个键值对，这个键值对要进入的桶的实际坐标，是由key决定的。由此，键值对的分布状况实际上是由key决定的。
// 而我们可以对输入的key进行处理。这就是hash算法的用处，即转换输入的key值，让其分布符合我们的预期。
// 在先前所举的例子里，我们的hash函数形似有，但其实没有。
// index = hash(key) % capacity;这才是标准步骤。
// 而这个hash(key),即经过了hash函数处理之后的key值，我们一般叫做hash值。

#include "../utils/common.h"

/*加法hash*/
// 对输入的每个字符的asii码进行相加，将得到的总和作为hash值
int addhash(char *key) {
  long long hash = 0; // 长长整型,8bytes
  const int MODLUS = 1000000007;
  for (int i = 0; i < strlen(key); i++) {
    hash = (hash + (unsigned char)key[i]) % MODLUS;
    // 首先注意，key[0]不等于“算”这个中文字符，而仅仅表示字符数组中的第一个字节的值。
    // 其次，这里用到了强制类型转换，将有符号的char强制转换为无符号的char，确保字符的值在[0,255]之间，而不是[-128,127]。
    // 例如，对于字符串 char * key =
    // "我是Hello";来说，key[0]是一个负数，因为它与汉字联系在一起的。而key[6]是一个正数，直接对应英文字符H的ASCII码(72)
    // 而(unsigned
    // char)通过类型转换让key[i]始终是正数。无符号char。至于为什么key[i]返回的是数字，而不是字符本身，现在先不管了。
    // 且保证计算中不要出现负值，确保计算的正确性和一致性
  }
  return (int)hash;
}

/*乘法hash*/
// 利用乘法的不相关性，每轮乘以一个常数，将个字符的ASCII码累计到hash表中
int mulhash(char *key) {
  long long hash = 0;
  const int MODLUS = 1000000007;
  for (int i = 0; i < strlen(key); i++) {
    hash = (31 * hash + (unsigned char)key[i]) % MODLUS;
  }
  return (int)hash;
}

/*异或hash,exclusive or:
A | B | A XOR B
---------------
0 | 0 |   0
0 | 1 |   1
1 | 0 |   1
1 | 1 |   0
*/
// 相应位上的两个值不同时，结果为1；相同时，结果为0
// a ^ b
// 的结果，就是将a和b这两个十进制（也可以是别的进制）数，转换成二进制，然后做比较，最终得出十进制的结果。
// 将输入数据的每一个元素通过异或操作累计到一个hash值中。
int xorhash(char *key) {
  int hash = 0;
  const int MODLUS = 1000000007;
  for (int i = 0; i < strlen(key); i++) {
    hash ^= (unsigned char)key[i];
    // 可以写成：
    // hash = hash^(unsigned char)key[i];
  }
  return hash & MODLUS;
}

/*旋转hash*/
// 将每个字符的ascii码累计到一个hash值中，每次累计之前都会都hash值进行旋转操作。
int rothash(char *key) {
  long long hash = 0;
  const int MODLUS = 1000000007;
  for (int i = 0; i < strlen(key); i++) {
    hash = ((hash << 4) ^ (hash >> 28) ^ (unsigned char)key[i]) % MODLUS;
  }
  //<<左移运算，>>右移运算。这里的移动，也是将原来的十进制转为二进制，然后进行添加（左移）位数或者删除（右移）位数的操作。添加相当于计算一个数的2^n倍，删除相当于计算一个数的2^(-n)。
  //
  return (int)hash;
}

/*driver code*/
int main() {
  char *key = "算法Hello dsad3241241dsa算123法"; // 键为字符串，不稀奇。

  int hash = addhash(key);
  printf("加法哈希值为 %d\n", hash);

  hash = mulhash(key);
  printf("乘法哈希值为 %d\n", hash);

  hash = xorhash(key);
  printf("异或哈希值为 %d\n", hash);

  hash = rothash(key);
  printf("旋转哈希值为 %d\n", hash);

  // 测试字符串长度
  char *key1 = "你好世界！";
  char *key2 = "HelloWorld!";
  printf("key1的字符串长度为：%d\n",
         strlen(key1)); // 在utf8编码中，一个汉字占据3个字节。
  printf("key2的字符串长度为：%d\n",
         strlen(key2)); // 而一个英文，不论大小写，都只占1个字节。
  printf(
      "key1的字符串长度为：%d\n",
      sizeof(
          key1)); // sizeof是用于判断不同的数据类型的大小，而不是实例的大小。指针型变量在64位操作系统里，都是8个字节。
  printf("key2的字符串长度为：%d\n", sizeof(key2));

  // 测试开头为汉字的字符串的首要索引
  char *key3 = "我是Hello";
  printf("key3[0]为: %d\n", key3[0]); // 没有类型转换，就会是复数。
  printf("(unsigned char)key3[0]为: %d\n", (unsigned char)key3[0]);
  printf("key3[6]为: %d\n", key3[6]);

  return 0;
}
