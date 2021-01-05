#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <ctime>
#include <cmath>
#include <stack>
#define Reg register
#define gmax(_a, _b) ((_a) > (_b) ? (_a) : (_b))
#define gmin(_a, _b) ((_a) < (_b) ? (_a) : (_b))
#define cmax(_a, _b) (_a < (_b) ? _a = (_b) : 0)
#define cmin(_a, _b) (_a > (_b) ? _a = (_b) : 0)
#define FOR(_i,_a,_b) for (Reg int _i = (_a) ; _i <= (_b) ; _i++)
#define REP(_i,_a,_b) for (Reg int _i = (_a) ; _i >= (_b) ; _i--)
#define FOR_LETTER(_i) for (Reg int _i = 'A'; _i <= 'z'; _i != 'Z' ? (_i++) : (_i = 'a'))
#define ll long long
using namespace std;
const int INF = 9999999;
const int MAXN = 100;
int W[MAXN + 10][MAXN + 10];
int X[MAXN + 10], bestX[MAXN + 10];
int bestw, N;
int Wmin[MAXN + 10];

int Rest(int i)
{
    int val = 0;
    FOR(j, i + 1, N)    //遍历还未经过的结点
        val += Wmin[X[j]];      //加上连接它的最小权值路径
    return val + Wmin[1];       //最后还要再经过一次结点1
}

void BacktrackTSP(int i, int cw)
{
    if (i == N)     //已经搜索到叶节点，即到达最后1个城市
    {
        if (W[X[N - 1]][X[N]] != INF && W[X[N]][1] != INF)  //最后1个城市与与第1个城市相连
        {
            cw = cw + W[X[N - 1]][X[N]] + W[X[N]][1];
            if (cw < bestw) //当前回路更优，更新最优搜索结果
            {
                cout << "Update:";
                bestw = cw;
                
                FOR(i, 1, N)//更新路径
                {
                    bestX[i] = X[i];
                    cout << bestX[i] << " ";
                }   
                cout << "bestw =" << bestw << endl;   
            }
        }
        return;
    }
    FOR(j, i, N)
    {
        swap(X[i], X[j]);
        int rest = Rest(i); //求出剩余节点的预估下界
        if (cw + W[X[i - 1]][X[i]] + rest >= bestw)
        {
            swap(X[i], X[j]);   //撤回此序列
            continue;
        }

        BacktrackTSP(i + 1, cw + W[X[i - 1]][X[i]]);
        swap(X[i], X[j]);   //回溯
    }
}

int greedy()
{
    int val = 0, now = 1;
    int V[MAXN + 10] = { 0 };   //V代表已经经过的节点
    V[1] = 1;       //初始只经过1
    bestX[1] = 1;
    FOR(i, 2, N)
    {
        int to, Min = INF;
        FOR(j, 1, N)    //遍历还未访问过的节点，寻找最小权值路径
            if (W[now][j] < Min && !V[j])
            {
                Min = W[now][j];
                to = j;
            }
        bestX[i] = to;
        V[to] = 1;      //标记转移节点
        now = to;       //转移到该节点
        // cout << Min << endl;
        val += Min;     //加上权值
    }
    return val + W[now][1]; //最后还要再经过一次结点1
}

void init()
{
    cin >> N;
    
    FOR(i, 1, N)
    {
        Wmin[i] = INF;
        FOR(j, 1, N)
        {
            cin >> W[i][j];
            if (i == j)
                W[i][j] = INF;
            else
                Wmin[i] = gmin(W[i][j], Wmin[i]);
        }
    }
    
    FOR(i, 1, N) X[i] = i;
    bestw = greedy();
    cout << "Initial Greedy:";
    FOR(i, 1, N)
        cout << bestX[i] << " "; 
    cout << "bestw = " << bestw << endl;   
    // cout << bestw << endl;
}

int main()
{
    freopen("in.txt", "r", stdin);
    init();
    BacktrackTSP(2, 0);
    FOR(i, 1, N)
        cout << bestX[i] << " ";
    cout << "1" << endl;
    cout << bestw << endl;
}