#include <iostream>
#define gmax(_a, _b) ((_a) > (_b) ? (_a) : (_b))
using namespace std;
const int maxn = 1000, maxm = 10000;
int W[maxn + 10], V[maxn + 10], X[maxn + 10], F[maxn][maxm];
int main()
{
	int N, M;
    double x;
	cin >> N >> x;
	M = x * 100;
	for (int i = 1; i <= N; i++)
	{
		cin >> x >> V[i];
		W[i] = x * 100;
	}
	for (int i = 1; i <= N; i++)
		for (int j = 0; j <= M; j++)
		{
			F[i][j] = F[i-1][j];
			if (j >= W[i])
				F[i][j] = gmax(F[i][j], F[i - 1][j - W[i]] + V[i]);
		}
    for (int i = N, j = M; i >= 1; i--)
    {
        if (F[i][j] > F[i-1][j])
        {
            X[i] = 1;
            j -= W[i];
        }
        else
            X[i] = 0;
    }
	cout << F[N][M] << endl;
    for (int i = 1; i <= N; ++i) cout << X[i] << " ";
	return 0;
}