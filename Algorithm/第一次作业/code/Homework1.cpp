#include "DataCreate.h";
#include "Algorithm1.h"
#include "Algorithm2.h"
#include "Algorithm3.h"
#include "Algorithm4.h"
#include <iostream>

using namespace std;
const int test[4][7] = { {10, 100, 1000, 10000, 0, 0, 0}, 
                         {10, 100, 1000, 10000, 100000, 1000000, 0},
                         {10, 100, 1000, 10000, 100000, 1000000, 10000000},
                         {10, 100, 1000, 10000, 100000, 1000000, 10000000} };

int main()
{
    freopen("out.txt", "w", stdout);
/*    cout << "Algo1" << endl;
    for (int n = 1000; n <= 10000000; n+= 1000)
    {
        DataCreate RandData(n);
        Algorithm1 Algo(n, RandData.GetData());
        int RunTime = Algo.Run();
        if (RunTime > 500000000) break;
        cout << n << "," << RunTime / 1000000.0 << endl;
    }
    cout << "Algo2" << endl;
    for (int n = 1000; n <= 10000000; n += 1000)
    {
        DataCreate RandData(n);
        Algorithm2 Algo(n, RandData.GetData());
        int RunTime = Algo.Run();
        if (RunTime > 500000000) break;
        cout << n << "," << RunTime / 1000000.0 << endl;
    }*/
    cout << "Algo3" << endl;
    for (int n = 100; n <= 100000; n += 100)
    {
        DataCreate RandData(n);
        Algorithm3 Algo(n, RandData.GetData());
        long long RunTime = Algo.Run();
        if (RunTime > 500000000) break;
        cout << n << "," << RunTime / 1000000.0 << endl;
    }
    cout << "Algo4" << endl;
    for (int n = 100; n <= 100000; n += 100)
    {
        DataCreate RandData(n);
        Algorithm4 Algo(n, RandData.GetData());
        long long RunTime = Algo.Run();
        if (RunTime > 500000000) break;
        cout << n << "," << RunTime / 1000000.0 << endl;
    }
}
