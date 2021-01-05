#include "DataCreate.h"
#include "ustime.h"
#include "Algorithm1.h"
#include <iostream>
using namespace std;
int main()
{
    while (1)
    {
        int F, V;
        cout << "请输入花束总数F: ";
        cin >> F;
        cout << "请输入花瓶总数V: ";
        cin >> V;
        DataCreate Data(F, V);
        int** N = Data.GetData();
        Data.Print();
        Algorithm1 Algo(F, V, N);
        Algo.Run();
        Algo.Print();
    }
}

