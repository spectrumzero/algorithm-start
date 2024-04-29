#include "../head/array.h"

/*随机访问元素*/
int randomaccess(int *nums, int size)
{
    //  参数：数组、数组长度
    //  在区间[0,size)中随机抽取一个数字作为下标
    //  rand()是c标准库中的函数。要看懂这里的取模运算，不管取到的随机整数是多少，取模后的结果都是在0到size-1之间，这是一种常规的处理办法。
    srand(time(NULL)); //设定一个随机种子，确保是真正随机的。在使用rand函数之前，调用c标准库中的另一个函数，利用时间去随机化伪随机数生成器的种子。如果不调用，生成器会使用固定的种子值，达不到随机的效果。
    int randomindex = rand() % size;
    //  获取并返回随机元素
    int randomnum = nums[randomindex];
    return randomnum;
}

/*初始化一个扩展长度后的数组*/
int *extend(int *nums, int size, int enlarge)
{
    // 参数：数组，数组长度，增加的长度。

    // malloc()同样是c标准库中的函数，利用此函数来动态分配内存。默认情况下，返回的是指向分配的内存块的起始地址的void指针，而这里通过(int*)类型转换，将这里返回的void指针转换为指向整型数组的第一个元素的指针类型。
    // malloc函数将内存动态地开辟在堆区，后面的参数确保有足够的内存空间来存储数组。
    // 返回的地址应该被保存（res）起来仪表后续对该内存块的操作，同时在不需要使用这块内存时，应该使用free函数来释放这块内存，以避免内存泄露。
    // 而后面的参数列表，表示要分配的内存块的大小，一般以字节为单位。

    int *res = (int *)malloc(sizeof(int) * (size + enlarge));
    // 第一步，将原数组中的所有元素都复制到新数组
    for (int i = 0; i < size; i++)
    {
        // res指针，根本上指向的是一块连续的内存空间，所以这样写一点问题没有。
        res[i] = nums[i];
    }
    // 第二步，初始化扩展后的空间
    for (int i = size; i < size + enlarge; i++)
    {
        res[i] = 0;
    }
    // 返回扩展后的新数组——res本身就是指向一个连续的内存块。
    return res;
}

/*在数组的索引处插入元素num*/
void insert(int *nums, int size, int num, int index)
{
    /*说明一下这里的参数列表：
     nums,既然是插入数组，那么，肯定就需要传入数组，*nums，和nums[],都是一样的，都是传入数组第一个元素的地址。第一种写法更直观一些。
     size，确定数组的大小，知道了大小，才能进行遍历的操作
     num，要插入的元素
     index，确定插入的位置。
    */

    // 在数组中插入，需要把索引index以及之后的所有元素都向后移动一位。所以也会涉及到遍历。
    // 第一步，从最后向前开始遍历，到了index的时候停止干涉。
    for (int i = size - 1; i > index; i--)
    {
        nums[i] = nums[i - 1]; // 挪出位置，把前面的赋值给后面的
    }
    // 第二步将num赋值给index处的元素
    nums[index] = num;

    // 值得注意的是，由于我们这里数组的长度是固定的，因此插入一个元素必定会导致数组尾部元素丢失。
}

/*删除索引index处的元素*/
// 注意，stdio.h占用了remove关键词
void removeitem(int *nums, int size, int index)
{
    // 把索引index之后的所有元素都向前移动一位
    // 这里的遍历，是从前向后开始的，从index开始，一直到倒数第二个元素，最后一个元素已经赋值给了倒数第二个元素，尾部元素已经没有意义，无须特意修改。
    for (int i = index; i < size - 1; i++)
    {
        nums[i] = nums[i + 1]; // 挤压,把后面的值赋值给前面的
    }
}

/*遍历数组*/
void traverse(int *nums, int size)
{
    int count = 0;
    // 通过索引遍历数组
    for (int i = 0; i < size; i++)
    {
        count += nums[i];
    }
}

/*在数组中查找指定元素，target表示要查找的目标值*/
int find(int *nums, int size, int target)
{
    for (int i = 0; i < size; i++)
    {
        if (nums[i] == target)
            return i;
    }
    return -1;
}

/*drivercode*/
int main()
{
    /*初始化数组数值*/
    int nums[] = {1, 3, 4, 2, 9};
    int size = 5;
    printf("数组 nums = ");
    printArray(nums, size); //数组名表示数组的起始地址

    /*随机访问*/
    int randomnum = randomaccess(nums, size); // 注意，在主函数中的randomnum和randomaccess函数最后返回的randomnum，是两个变量。
    printf("随机访问得到的元素为：%d\n", randomnum);

    /*长度扩展*/
    int enlarge = 4;
    int *res = extend(nums, size, enlarge); // 新建了一个指针型变量，接收extend函数的返回值，也就是位于堆区的、扩展了长度之后的数组。
    size += enlarge;
    printf("将数组长度扩展至9，得到nums = ");
    printArray(res, size);

    /*插入元素*/
    insert(res, size, 6, 4); //提醒，res是扩展了长度后的数组的起始地址。
    printf("在索引4处插入数字6，得到nums = ");
    printArray(res, size);

    /*删除元素*/
    removeitem(res, size, 2);
    printf("删除索引2处的元素，得到nums = ");
    printArray(res, size);

    /*遍历数组*/
    traverse(res, size);

    /*查找元素*/
    int index = find(res, size, 9);
    printf("在res中查找元素9，得到索引 = %d\n", index);

    /*释放内存*/
    free(res); // 利用free函数主动释放堆区内存。
    return 0;

    //另外，malloc(),rand()以及srand()函数，都位于标准库<stdlib.h>中,time()函数位于<time.h>标准库中
}