/**
 * File: array_hash_map.cpp
 * Created Time: 2022-12-14
 * Author: msk397 (machangxinq@gmail.com)
 */

#include "../utils/common.hpp"

/* 键值对 */
struct Pair {
  public:
    /*int is still one of the basic data types in cpp. they are created on the stack and do not
     involve dynamic memory allocation, so they are managed automatically.*/
    int key;
    /*But string is not; its memory is allocated dynamically. And because it is a standard library
     class in cpp, the class itself provide its internal destructor function. It can dynamically
     manage its memory on its own so there is no need to rewrite it. When the destructor of pair
     is called, it will first clean up all its members, including key and val. So the destructor
     of std:string will be called before that of pair.*/
    string val;
    Pair(int key, string val) {
        this->key = key;
        this->val = val;
    }
};

/* 基于数组实现的哈希表 */
class ArrayHashMap {
  private:
    vector<Pair *> buckets;

  public:
    ArrayHashMap() {
        // 初始化数组，包含 100 个桶
        buckets = vector<Pair *>(100);
    }

    ~ArrayHashMap() {
        // 释放内存
        // Range-based for loop in cpp
        for (const auto &bucket : buckets) {
            delete bucket;
        }
        /*If you omit buckets.clear();, the vector buckets would still contain the
         same number of elements, but each would be a nullptr (since the original
         pointers were deleted). you call buckets.clear(); to remove the now-null
         pointers from the vector.This ensures the buckets is empty.*/
        buckets.clear();
    }

    /* 哈希函数 */
    int hashFunc(int key) {
        int index = key % 100;
        return index;
    }

    /* 查询操作 */
    string get(int key) {
        int index = hashFunc(key);
        Pair *pair = buckets[index];
        if (pair == nullptr)
            return "";
        return pair->val;
    }

    /* 添加操作 */
    void put(int key, string val) {
        /* Use new operater to dynamically allocates memory on the heap for a pair object */
        Pair *pair = new Pair(key, val);
        int index = hashFunc(key);
        buckets[index] = pair;
    }

    /* 删除操作 */
    void remove(int key) {
        int index = hashFunc(key);
        // delete the memory which the pointer points to
        delete buckets[index];
        // set the pointer as nullptr
        buckets[index] = nullptr;
    }

    /* 获取所有键值对 */
    vector<Pair *> pairSet() {
        vector<Pair *> pairSet;
        for (Pair *pair : buckets) {
            if (pair != nullptr) {
                // push_back() is a member function of the std::vector class that adds a new element
                // to the end of the vector.
                pairSet.push_back(pair);
            }
        }
        return pairSet;
    }

    /* 获取所有键 */
    vector<int> keySet() {
        vector<int> keySet;
        for (Pair *pair : buckets) {
            if (pair != nullptr) {
                keySet.push_back(pair->key);
            }
        }
        return keySet;
    }

    /* 获取所有值 */
    vector<string> valueSet() {
        vector<string> valueSet;
        for (Pair *pair : buckets) {
            if (pair != nullptr) {
                valueSet.push_back(pair->val);
            }
        }
        return valueSet;
    }

    /* 打印哈希表 */
    void print() {
        for (Pair *kv : pairSet()) {
            cout << kv->key << " -> " << kv->val << endl;
        }
    }
};

// 测试样例请见 array_hash_map_test.cpp
