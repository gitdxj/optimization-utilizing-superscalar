/********************
代码涉及Linux的系统调用
请在Linux下编译运行
*********************/


#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include <cmath>

using namespace std;

double sum_recurse(double *num, int len);  // 递归算法，两两相加
void get_vect(double *vect, int len);  // 为长度为len的double型数组vect赋初值，某一位置元素的初值为该元素的下标，如：vect[2] = 2
double sum_serial(double *num, int len);  // 串行逐个累加
double sum_2(double *num, int len);  // 串行累加，每次有2个操作数 如：s = a + b
double sum_r(double *num, int len);  // 循环使用reduction的方式计算
double sum_reduction(double *num, int len);  // 另外一种更好的循环归约求和算法（这个算法是借鉴了室友的算法）
/*下面三个函数用来研究内存读取次数对上面几个算法的运行时间的影响*/
void read_1(double *num, int len);
void read_2(double *num, int len);
void read_4(double *num, int len);
int main()
{
    struct timeval start;
    struct timeval end;
    unsigned long diff1;


    // 这里测试的数据量选了两个，一个是2^10, 一个是2^20
    int len = 1024*1024; // 16777216
    double *num = new double[len];
    get_vect(num, len);
    double sum = 0;


    // 下面是递归算法，因为在递归函数中把精确计时的代码放进去比较困难所以直接在main函数里计时
    gettimeofday(&start, NULL);
    sum = sum_recurse(num, len);
    gettimeofday(&end, NULL);

    diff1 = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    cout << "time_interval(sum_recurse) is " << diff1 << " us"<<endl
        << "sum is " << sum <<endl;

    sum = sum_serial(num, len);  // 串行累加
    sum = sum_2(num, len);       // 两个相加的串行累加
    sum = sum_r(num, len);       // 循环规约
//    read_1(num, len);
//    read_2(num, len);
//    read_4(num, len);


    return 0;
}

//
double sum_recurse(double *num, int len)
{

//    if(len == 4)
//        return *num + *(num+1) + *(num+2) + *(num+3);
    if(len == 1)
        return *num;
    else return (sum_recurse(num, (int) len/2)) + sum_recurse((num+len/2), (int) len/2);


}

void get_vect(double *vect, int len)
{
    for(int i = 0; i<len; i++)
        vect[i] = len;
}

double sum_serial(double *num, int len)
{
    struct timeval start;
    struct timeval end;

    gettimeofday(&start, NULL);
/***********计算部分************/
/************start*************/
    double sum = 0;
    for(int i = 0; i<len; i++)
        sum += num[i];
/*************end**************/

    gettimeofday(&end, NULL);

    unsigned long time_interval = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    cout << "time interval(sum_serial): " << time_interval << " us" << endl
        << "sum is " << sum <<endl;

    return sum;
}




double sum_2(double *num, int len)
{
    struct timeval start;
    struct timeval end;
    int l = len/2;
    gettimeofday(&start, NULL);
/***********计算部分************/
/************start*************/
    double sum = 0;
    for(int i = 0; i<=l; i = i + 2)
        sum += num[i] + num[i+1];
/*************end**************/
    gettimeofday(&end, NULL);

    unsigned long time_interval = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    cout << "time interval(sum_2_ex): " << time_interval << " us" <<endl
        << "sum is " << sum <<endl;

    return sum;
}

/*
0
0       0
0   0   0   0
0 0 0 0 0 0 0 0
*/
double sum_r(double *num, int len)
{
    int n = (int)log2(len);
//    cout << "n is " << n<<endl;
    int n1 = len/2;
    int n2 = 2;
    int n3 = 1;

    struct timeval start;
    struct timeval end;


    gettimeofday(&start, NULL);
    /***********计算部分************/
    /************start*************/
    for(int k = 0; k < n; k++)
    {
//        cout << "k = " << k << ":" << endl
//            << "n1 = " << n1 << " n2 = " << n2 << " n3 = "<<n3<<endl;
        for(int i = 0; i<n1; i++)
            num[n2*i] = num[n2*i]+num[n2*i+n3];
        n1 = n1/2;
        n2 = n2*2;
        n3 = n3*2;
    }
    /*************end**************/
    gettimeofday(&end, NULL);

    unsigned long time_interval = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    cout << "time interval(sum_r): " << time_interval << " us" << endl
        << "sum is " << *num <<endl;
    return *num;
}

double sum_reduction(double *num, int len)
{
    int l = len - 1;
    struct timeval start;
    struct timeval end;


    gettimeofday(&start, NULL);
/***********计算部分************/
/************start*************/
    while(len > 1)
    {
        len = len/2;
        for(int i = 0; i<len; i++)
            num[i] = num[i] + num[l - i];

    }
/*************end**************/
    gettimeofday(&end, NULL);
    double sum = num[0];
    unsigned long time_interval = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    cout << "time interval(sum_reduction): " << time_interval << " us" <<endl
        << "sum is " << sum <<endl;
}

void read_1(double *num, int len)
{
    struct timeval start;
    struct timeval end;
    double a = 0;
    gettimeofday(&start, NULL);
/***********内存读取************/
/************start*************/

    for(int i = 0; i<len; i++)
        a = num[i];
/*************end**************/
    gettimeofday(&end, NULL);

    unsigned long time_interval = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    cout << "time interval(read_1): " << time_interval << "us" <<endl;
}

void read_2(double *num, int len)
{
    struct timeval start;
    struct timeval end;

    double a = 0;
    double b = 0;

    gettimeofday(&start, NULL);
/***********内存读取************/
/************start*************/

    for(int i = 0; i<len/2; i++)
        {a = num[2*i];
        b = num[2*i+1];}
/*************end**************/
    gettimeofday(&end, NULL);

    unsigned long time_interval = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    cout << "time interval(read_2): " << time_interval << "us" <<endl;


}

void read_4(double *num, int len)
{
    struct timeval start;
    struct timeval end;
    double a,b,c,d;

    gettimeofday(&start, NULL);
/***********内存读取************/
/************start*************/
    double sum = 0;
    for(int i = 0; i<len/4; i++)
        {
            a = num[4*i];
            b = num[4*i + 1];
            c = num[4*i + 2];
            d = num[4*i + 3];
        }
/*************end**************/
    gettimeofday(&end, NULL);

    unsigned long time_interval = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    cout << "time interval(read_4): " << time_interval <<"us" << endl;
}
