#include "Data.h"
#include "DataCreate.h"
#include "InsertSort.h"
#include "MergeSort.h"
#include "QuickSort.h"
#include <iostream>
using namespace std;

#define CORRECT_TEST(_sort) /*FOR(i, 0, n - 1) cout << B[i].Value << " ";*/\
/*cout << endl;*/\
cout << "检测排序结果从1到" << n << "是否有序:" << endl;\
Sorted = 1;\
FOR(i, 0, n - 2)\
if (B[i] > B[i + 1])\
{\
    Sorted = 0;\
    break;\
}\
if (Sorted)\
cout << "检测结果: 排序正确，数列有序" << endl;\
else cout << "[Error!] 排序错误" << endl;\
sort.showBranchTest();

int DataCreate::Time = 0;
#define Reg register
#define FOR(_i,_a,_b) for (Reg int _i = (_a) ; _i <= (_b) ; _i++)
int main()
{
    int n;
    int test_mode, Sorted, choose;
    long long best_move, best_cmp, worst_move, worst_cmp, avg_move, avg_cmp;
    while (1)
    {
        cout << "请选择测试模式" << endl;
        cout << "正确性测试：0" << endl;
        cout << "算法复杂性测试：1" << endl;
        cout << "算法效率测试：2" << endl;
        cout << "请输入测试模式:";
        cin >> test_mode;
        switch (test_mode)
        {
        case 0:
            cout << "请选择要测试的算法" << endl;
            cout << "插入排序：0" << endl;
            cout << "合并排序：1" << endl;
            cout << "快速排序：2" << endl;
            cout << "请输入测试算法:";
            cin >> choose;
            cout << "N=";
            cin >> n;
            switch (choose)
            {
            case 0:
            {
                DataCreate RandData(n);
                Data* A = RandData.GetData();

                cout << "原始序列：";
                FOR(i, 0, n - 1) cout << A[i].Value << " ";
                cout << endl << endl;

                Data* B = new Data[n];
                cout << "----------------------插入排序------------------------" << endl;
                FOR(i, 0, n - 1) B[i].Init(A[i].Value);
                cout << "排序结果：";
                InsertSort sort(n, B);
                sort.Run();
                CORRECT_TEST(sort);
                cout << endl;
                delete[] B;
                break;
            }
            case 1:
            {
                DataCreate RandData(n);
                Data* A = RandData.GetData();

                cout << "原始序列：";
                FOR(i, 0, n - 1) cout << A[i].Value << " ";
                cout << endl << endl;

                Data* B = new Data[n];
                cout << "----------------------合并排序------------------------" << endl;
                FOR(i, 0, n - 1) B[i].Init(A[i].Value);
                cout << "排序结果：";
                MergeSort sort(n, B);
                sort.Run();
                CORRECT_TEST(sort);
                cout << endl;
                delete[] B;
                break;
            }
            case 2:
            {
                DataCreate RandData(n);
                Data* A = RandData.GetData();

                //cout << "原始序列：";
               // FOR(i, 0, n - 1) cout << A[i].Value << " ";
               // cout << endl << endl;

                Data* B = new Data[n];
                cout << "----------------------快速排序------------------------" << endl;
                FOR(i, 0, n - 1) B[i].Init(A[i].Value);
                //cout << "排序结果：";
                QuickSort sort(n, B);
                sort.Run();
                CORRECT_TEST(sort);
                cout << endl;
                delete[] B;
                break;
            }
            default:
                break;
            }
            break;
        case 1:
            cout << "请选择要测试的算法" << endl;
            cout << "插入排序：0" << endl;
            cout << "合并排序：1" << endl;
            cout << "快速排序：2" << endl;
            cout << "请输入测试算法:";
            cin >> choose;
            cout << "N=";
            cin >> n;
            switch (choose)
            {
            case 0:
            {
                {
                    cout << "----------------------最好情况----------------------" << endl;
                    cout << "构造最好情况下的原始序列:";
                    DataCreate SmallToLarge(n, DataCreate::SmallToLarge);
                    Data* A = SmallToLarge.GetData();
                    FOR(i, 0, n - 1) cout << A[i].Value << " ";
                    cout << endl << endl;

                    Data* B = new Data[n];
                    FOR(i, 0, n - 1) B[i].Init(A[i].Value);
                    InsertSort sort(n, B);
                    sort.Run();
                    cout << "排序完毕, 统计结果：";
                    best_move = sort.CountMove();
                    best_cmp = sort.CountCompare();
                    cout << "腾挪次数：" << best_move << "  "
                        << "比较次数：" << best_cmp << endl;
                    cout << endl;
                    delete[] B;
                }

                {
                    cout << "----------------------最坏情况----------------------" << endl;
                    cout << "构造最坏情况下的原始序列:";
                    DataCreate LargeToSmall(n, DataCreate::LargeToSmall);
                    Data* A = LargeToSmall.GetData();
                    FOR(i, 0, n - 1) cout << A[i].Value << " ";
                    cout << endl << endl;

                    Data* B = new Data[n];
                    FOR(i, 0, n - 1) B[i].Init(A[i].Value);
                    InsertSort sort(n, B);
                    sort.Run();
                    cout << "排序完毕, 统计结果：";
                    worst_move = sort.CountMove();
                    worst_cmp = sort.CountCompare();
                    cout << "腾挪次数：" << worst_move << "  "
                        << "比较次数：" << worst_cmp << endl;
                    cout << endl;
                    delete[] B;
                }

                {
                    cout << "----------------------平均情况----------------------" << endl;
                    cout << "构造10次随机原始序列:" << endl;
                    long long tot_move = 0, tot_cmp = 0;
                    FOR(i, 1, 10)
                    {
                        cout << "第" << i << "次测试 : ";
                        DataCreate Random(n);
                        Data* A = Random.GetData();

                        Data* B = new Data[n];
                        FOR(i, 0, n - 1) B[i].Init(A[i].Value);
                        InsertSort sort(n, B);
                        sort.Run();
                        cout << "腾挪次数：" << sort.CountMove() << "  "
                            << "比较次数：" << sort.CountCompare() << endl;
                        tot_move += sort.CountMove();
                        tot_cmp += sort.CountCompare();
                        delete[] B;
                    }
                    avg_move = tot_move / 10;
                    avg_cmp = tot_cmp / 10;
                    cout << "平均腾挪次数：" << avg_move << "  "
                        << "平均比较次数：" << avg_cmp<< endl;
                    cout << endl;
                }

                cout << "----------------------总情况----------------------" << endl;
                cout << "对于插入排序, N = " << n << endl;
                cout << "最好情况:" << "腾挪次数：" << best_move << "  "
                    << "比较次数：" << best_cmp << endl;
                cout << "最坏情况:" << "腾挪次数：" << worst_move << "  "
                    << "比较次数：" << worst_cmp << endl;
                cout << "平均情况:" << "腾挪次数：" << avg_move << "  "
                    << "比较次数：" << avg_cmp << endl;
                break;
            }
            case 1:
            {
                {
                    cout << "----------------------最好情况----------------------" << endl;
                    cout << "构造最好情况下的原始序列:";
                    DataCreate LargeToSmall(n, DataCreate::LargeToSmall);
                    Data* A = LargeToSmall.GetData();
                    //FOR(i, 0, n - 1) cout << A[i].Value << " ";
                    cout << endl << endl;
                    
                    Data* B = new Data[n];
                    FOR(i, 0, n - 1) B[i].Init(A[i].Value);
                    MergeSort sort(n, B);
                    sort.Run();
                    cout << "排序完毕, 统计结果：";
                    best_move = sort.CountMove();
                    best_cmp = sort.CountCompare();
                    cout << "腾挪次数：" << best_move << "  "
                        << "比较次数：" << best_cmp << endl;
                    cout << endl;
                    delete[] B;
                }

                {
                    cout << "----------------------最坏情况----------------------" << endl;
                    cout << "构造最坏情况下的原始序列:";
                    DataCreate Sepa(n, DataCreate::Sepa);
                    Data* A = Sepa.GetData();
                    //FOR(i, 0, n - 1) cout << A[i].Value << " ";
                    cout << endl << endl;

                    Data* B = new Data[n];
                    FOR(i, 0, n - 1) B[i].Init(A[i].Value);
                    MergeSort sort(n, B);
                    sort.Run();
                    cout << "排序完毕, 统计结果：";
                    worst_move = sort.CountMove();
                    worst_cmp = sort.CountCompare();
                    cout << "腾挪次数：" << worst_move << "  "
                        << "比较次数：" << worst_cmp << endl;
                    cout << endl;
                    delete[] B;
                }

                {
                    cout << "----------------------平均情况----------------------" << endl;
                    cout << "构造10次随机原始序列:" << endl;
                    long long tot_move = 0, tot_cmp = 0;
                    FOR(i, 1, 10)
                    {
                        cout << "第" << i << "次测试 : ";
                        DataCreate Random(n);
                        Data* A = Random.GetData();

                        Data* B = new Data[n];
                        FOR(i, 0, n - 1) B[i].Init(A[i].Value);
                        MergeSort sort(n, B);
                        sort.Run();
                        cout << "腾挪次数：" << sort.CountMove() << "  "
                            << "比较次数：" << sort.CountCompare() << endl;
                        tot_move += sort.CountMove();
                        tot_cmp += sort.CountCompare();
                        delete[] B;
                    }
                    avg_move = tot_move / 10;
                    avg_cmp = tot_cmp / 10;
                    cout << "平均腾挪次数：" << avg_move << "  "
                        << "平均比较次数：" << avg_cmp << endl;
                    cout << endl;
                }

                cout << "----------------------总情况----------------------" << endl;
                cout << "对于合并排序, N = " << n << endl;
                cout << "最好情况:" << "腾挪次数：" << best_move << "  "
                    << "比较次数：" << best_cmp << endl;
                cout << "最坏情况:" << "腾挪次数：" << worst_move << "  "
                    << "比较次数：" << worst_cmp << endl;
                cout << "平均情况:" << "腾挪次数：" << avg_move << "  "
                    << "比较次数：" << avg_cmp << endl;
                break;
            }
            case 2:
            {
                {
                    cout << "----------------------最好情况----------------------" << endl;
                    cout << "构造最好情况下的原始序列:";
                    DataCreate MIF(n, DataCreate::MiddleInFirst);
                    Data* A = MIF.GetData();
                    //FOR(i, 0, n - 1) cout << A[i].Value << " ";
                    cout << endl << endl;

                    Data* B = new Data[n];
                    FOR(i, 0, n - 1) B[i].Init(A[i].Value);
                    QuickSort sort(n, B);
                    sort.Run();
                    cout << "排序完毕, 统计结果：";
                    best_move = sort.CountMove();
                    best_cmp = sort.CountCompare();
                    cout << "腾挪次数：" << best_move << "  "
                        << "比较次数：" << best_cmp << endl;
                    cout << endl;
                    delete[] B;
                }

                {
                    cout << "----------------------最坏情况----------------------" << endl;
                    //cout << "构造最坏情况下的原始序列:";
                    //DataCreate SmallToLarge(n, DataCreate::SmallToLarge);
                    //Data* A = SmallToLarge.GetData();
                    //FOR(i, 0, n - 1) cout << A[i].Value << " ";
                    cout << "栈溢出！" << endl;
                    cout << endl << endl;

                    //Data* B = new Data[n];
                    //FOR(i, 0, n - 1) B[i].Init(A[i].Value);
                    //QuickSort sort(n, B);
                    //sort.Run();
                    //cout << "排序完毕, 统计结果：";
                    //worst_move = sort.CountMove();
                   // worst_cmp = sort.CountCompare();
                    cout << "腾挪次数：INF"  << "  "
                        << "比较次数：INF" << endl;
                    cout << endl;
                    //delete[] B;
                }

                {
                    cout << "----------------------平均情况----------------------" << endl;
                    cout << "构造10次随机原始序列:" << endl;
                    long long tot_move = 0, tot_cmp = 0;
                    FOR(i, 1, 10)
                    {
                        cout << "第" << i << "次测试 : ";
                        DataCreate Random(n);
                        Data* A = Random.GetData();

                        Data* B = new Data[n];
                        FOR(i, 0, n - 1) B[i].Init(A[i].Value);
                        QuickSort sort(n, B);
                        sort.Run();
                        cout << "腾挪次数：" << sort.CountMove() << "  "
                            << "比较次数：" << sort.CountCompare() << endl;
                        tot_move += sort.CountMove();
                        tot_cmp += sort.CountCompare();
                        delete[] B;
                    }
                    avg_move = tot_move / 10;
                    avg_cmp = tot_cmp / 10;
                    cout << "平均腾挪次数：" << avg_move << "  "
                        << "平均比较次数：" << avg_cmp<< endl;
                    cout << endl;
                }

                cout << "----------------------总情况----------------------" << endl;
                cout << "对于快速排序, N = " << n << endl;
                cout << "最好情况:" << "腾挪次数：" << best_move << "  "
                    << "比较次数：" << best_cmp << endl;
                cout << "最坏情况:" << "腾挪次数：" << "INF" << "  "
                    << "比较次数：" << "INF" << endl;
                cout << "平均情况:" << "腾挪次数：" << avg_move << "  "
                    << "比较次数：" << avg_cmp << endl;
                break;
            }
            default:
                break;
            }
            break;
        case 2:
            cout << "N=";
            cin >> n;
            {
                cout << "构造10次随机原始序列:" << endl;
                long long tot_move_insert = 0, tot_cmp_insert = 0,
                    tot_move_merge = 0, tot_cmp_merge = 0,
                    tot_move_quick = 0, tot_cmp_quick = 0;
                double time_insert = 0, time_merge = 0, time_quick = 0;
                FOR(i, 1, 10)
                {
                    cout << "-------------------------第" << i << "次测试-------------------------" << endl;
                    DataCreate Random(n);
                    Data* A = Random.GetData();
                    //cout << "原始序列:" << endl;
                    //FOR(i, 0, n - 1) cout << A[i].Value << " ";
                    //cout << endl;

                    Data* B = new Data[n];

                    //FOR(i, 0, n - 1) B[i].Init(A[i].Value);
                    //InsertSort insert_sort(n, B);
                    //insert_sort.Run();
                    cout << "插入排序: 腾挪次数：" << "INF" << "  "
                        << "比较次数：" << "INF" << "  "  
                        <<  "运行时间" << "INF" << endl;
                   // tot_move_insert += insert_sort.CountMove();
                    //tot_cmp_insert += insert_sort.CountCompare();
                    //time_insert += insert_sort.RunTime;

                    FOR(i, 0, n - 1) B[i].Init(A[i].Value);
                    MergeSort merge_sort(n, B);
                    merge_sort.Run();
                    cout << "合并排序: 腾挪次数：" << merge_sort.CountMove() << "  "
                        << "比较次数：" << merge_sort.CountCompare() 
                        << "运行时间" << merge_sort.RunTime << endl;
                    tot_move_merge += merge_sort.CountMove();
                    tot_cmp_merge += merge_sort.CountCompare();
                    time_merge += merge_sort.RunTime;

                    FOR(i, 0, n - 1) B[i].Init(A[i].Value);
                    QuickSort quick_sort(n, B);
                    quick_sort.Run();
                    cout << "快速排序: 腾挪次数：" << quick_sort.CountMove() << "  "
                        << "比较次数：" << quick_sort.CountCompare() 
                        << "运行时间" << quick_sort.RunTime << endl;
                    tot_move_quick += quick_sort.CountMove();
                    tot_cmp_quick += quick_sort.CountCompare();
                    time_quick += quick_sort.RunTime;

                    delete[] B;
                }
                cout << "----------------------总情况----------------------" << endl;
                cout << "插入排序：平均腾挪次数：" << "INF" << "  "
                    << "平均比较次数：" << "INF"
                    << "平均运行时间" << "INF";
                cout << "合并排序：平均腾挪次数：" << tot_move_merge / 10 << "  "
                    << "平均比较次数：" << tot_cmp_merge / 10 
                    << "平均运行时间" << time_merge / 10 << endl << endl;
                cout << "快速排序：平均腾挪次数：" << tot_move_quick / 10 << "  "
                    << "平均比较次数：" << tot_cmp_quick / 10 
                    << "平均运行时间" << time_quick / 10 << endl << endl;
                cout << endl;

            }
            break;
        default:
            break;
        }
    }
    
    
}
