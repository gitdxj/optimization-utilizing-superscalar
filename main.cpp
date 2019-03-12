/********************
代码涉及Linux的系统调用
请在Linux下编译运行
*********************/


#include <iostream>
#include <stdio.h>
#include <sys/time.h>
using namespace std;

double sum_recurse(double *num, int dim);  // 递归算法，两两相加
void get_vect(double *vect, int dimention);  // 为长度为dimension的double型数组vect赋初值，某一位置元素的初值为该元素的下标，如：vect[2] = 2
double sum_no_recurse(double *num, int dim);  // 串行逐个累加
int main()
{
    struct timeval start;
    struct timeval end;
    unsigned long diff1;
    unsigned long diff2;

    // 这里测试的数据量选了两个，一个是2^10, 一个是2^20
    int dimension = 4096; // 16777216
    double *num = new double[dimension];
    get_vect(num, dimension);
    double sum = 0;

    gettimeofday(&start, NULL);
    sum = sum_recurse(num, dimension);
//    sum = sum_no_recurse(num, dimension);
    gettimeofday(&end, NULL);

    diff1 = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    gettimeofday(&start, NULL);
//    sum = sum_recurse(num, dimension);
    sum = sum_no_recurse(num, dimension);
    gettimeofday(&end, NULL);

    diff2 = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;




    cout << "diff1 is " << diff1 << "us"<<endl
         << "diff2 is" << diff2<<"us"<<endl;

    return 0;
}

double sum_recurse(double *num, int dim)
{
    if(dim == 4)
    {
//        double s1 = *num + *(num+1);
//        double s2 = *(num+2) + *(num+3);
//        return s1+s2;
        return *num + *(num+1) + *(num+2) + *(num+3);
    }
    else return (sum_recurse(num, (int) dim/2)) + sum_recurse((num+dim/2), (int) dim/2);
}

void get_vect(double *vect, int dimention)
{
    for(int i = 0; i<dimention; i++)
        vect[i] = dimention;
}

double sum_no_recurse(double *num, int dim)
{
    double sum = 0;
    for(int i = 0; i<dim; i++)
        sum += num[i];
    return sum;
}
