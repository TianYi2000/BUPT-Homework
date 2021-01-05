#include <iostream>
#include <set>
#include <algorithm>
#define Reg register
#define FOR(_i,_a,_b) for (Reg int _i = (_a) ; _i <= (_b) ; _i++)
#define gmax(_a, _b) ((_a) > (_b) ? (_a) : (_b))
using namespace std;
const int maxn = 1000, maxm = 10000;
int W[maxn + 10], V[maxn + 10];
int N, M;
struct point
{
	int w, v;
	point(int _w = 0, int _v = 0) : w(_w), v(_v) {};
	bool operator<(const point& rhs) const {
		if (w == rhs.w)
			return v < rhs.v;
		return w < rhs.w;
	};
};
set <point> p, q;

void Oplus(set <point>& p, set <point>& q, int x)
{
	q.clear();
	set<point>::iterator it;
	for (it = p.begin(); it != p.end(); it++)
	{
		point n = *it;
		if (n.w + W[x] <= M)
			q.insert(point(n.w + W[x], n.v + V[x]));
	}
}

void Union(set <point>& p, set <point>& q)
{
	set<point> temp;
	set_union(begin(p), end(p), begin(q), end(q), inserter(temp, temp.begin()));
	set<point>::iterator it, jt;
	p.clear();
	for (it = temp.begin(); it != temp.end(); it++)
	{
		int ok = 1;
		point x = *it;
		for (jt = temp.begin(); jt != temp.end(); jt++)
		{
			point y = *jt;
			if (y.w <= x.w && y.v > x.v)
				ok = 0;
		}
		if (ok)
			p.insert(x);
	}
}

int main()
{
	cin >> N >> M;
	FOR(i, 1, N)
		cin >> W[i] >> V[i];
	p.clear();
	p.insert(point(0, 0));

	FOR(i, 1, N)
	{
		Oplus(p, q, i);
		Union(p, q);
	}
	set<point>::iterator it;
	int maxV = 0;
	
	for (it = p.begin(); it != p.end(); it++)
	{
		point x = *it;
		if (x.v > maxV)
			maxV = x.v;
	}
	cout << maxV;
}