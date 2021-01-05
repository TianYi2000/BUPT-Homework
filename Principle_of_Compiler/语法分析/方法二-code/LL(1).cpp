#include <cstdio>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <cmath>
#include <stack>
#include <set>
#include <map>
#include <boost/algorithm/string.hpp>
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
const int maxn = 1000;
string BeginSym;
set <string> termin, non_termin;
map <string, set<string> > first, follow;
set <string> created_first, created_follow;
vector<string> boostsplit(const string & input);
struct Proce;
vector<Proce> proce;
map <string, vector<Proce> > proce_index;
map <string, map<string, Proce> > Table;
stack<string> Stack;
struct Proce
{
    string ori;
    string left;
    vector<string> right;
    Proce() {};
    Proce(const string & _p)
    {
        ori = _p;
        vector<string> syms = boostsplit(_p);
        left = syms[0];
        int l = syms.size();
        FOR(i, 2, l - 1)
            right.push_back(syms[i]);
        proce_index[left].push_back(*this);
    }
};
void init()
{
    proce.push_back(Proce("E -> T E'"));
    proce.push_back(Proce("E' -> + T E'"));
    proce.push_back(Proce("E' -> - T E'"));
    proce.push_back(Proce("E' -> epsilon"));
    proce.push_back(Proce("T -> F T'"));
    proce.push_back(Proce("T' -> * F T'"));
    proce.push_back(Proce("T' -> / F T'"));
    proce.push_back(Proce("T' -> epsilon"));
    proce.push_back(Proce("F -> ( E )"));
    proce.push_back(Proce("F -> num"));

    termin.insert("num");
    termin.insert("+");
    termin.insert("-");
    termin.insert("*");
    termin.insert("/");
    termin.insert("(");
    termin.insert(")");
    termin.insert("epsilon");
    termin.insert("$");

    non_termin.insert("E");
    non_termin.insert("E'");
    non_termin.insert("T");
    non_termin.insert("T'");
    non_termin.insert("F");

    BeginSym = "E";

}

vector<string> boostsplit(const std::string& input)
{
    std::vector <std::string> fields;
    boost::split(fields, input, boost::is_any_of(" "));
    return fields;
}

void CreateFirst(const string & sym)
{
    vector<Proce> & Pv = proce_index[sym];
    
    for (vector<Proce>::iterator it = Pv.begin(); it != Pv.end(); ++it)
    {
        Proce &P = (*it);
        string left = P.left;
        bool all_epsilon = true;
        for (vector<string>::iterator j = P.right.begin(); j != P.right.end(); j++)
        {
            
            if (termin.find(*j) != termin.end())
            {
                first[left].insert(*j);
                break;
            }

            if (created_first.find(*j) == created_first.end())
            {
                CreateFirst(*j);
            }

            first[left].insert(first[*j].begin(), first[*j].end());

            if (first[*j].find("epsilon") == first[*j].end())
            {
                all_epsilon = false;
                break;
            }
            else
                first[left].erase("epsilon");

            if (all_epsilon)
                first[left].insert("epsilon");
        }
    }
    created_first.insert(sym);
}

void CreateFollow()
{
    follow[BeginSym].insert("$");
    bool changed = true;
    while (changed)
    {
        changed = false;
        for (vector<Proce>::iterator it1 = proce.begin(); it1 != proce.end(); it1++)      //遍历所有产生式
        {
            Proce& P = *it1;
            for (vector<string>::iterator it2 = P.right.begin(); it2 != P.right.end(); it2++)
            {
                string X = *it2;
                int old_size = follow[X].size();
                bool all_epsilon = true;
                for (vector<string>::iterator it3 = it2 + 1; it3 != P.right.end() && all_epsilon; it3++)
                {
                    string b = *it3;
                    bool show_epsilon = false;
                    for (set<string>::iterator it4 = first[b].begin(); it4 != first[b].end(); it4++)
                    {
                        string a = *it4;
                        if (a != "epsilon")
                            follow[X].insert(a);
                        else
                            show_epsilon = true;
                    }
                    all_epsilon &= show_epsilon;
                }
                if (all_epsilon)
                    for (set<string>::iterator it5 = follow[P.left].begin(); it5 != follow[P.left].end(); it5++)
                        follow[X].insert(*it5);
                if (follow[X].size() > old_size)
                    changed = true;
            }
        }
    }
}

void CreateTable()
{
    for (vector<Proce>::iterator it1 = proce.begin(); it1 != proce.end(); it1++)
    {
        Proce& P = *it1;
        bool show_epsilon = false;
        for (set<string>::iterator it2 = first[P.right[0]].begin(); it2 != first[P.right[0]].end(); it2++)
        {
            string a = *it2;
            if (a != "epsilon")
                Table[P.left][a] = P;
            else
                show_epsilon = true;
        }
        if (show_epsilon)
            for (set<string>::iterator it3 = follow[P.left].begin(); it3 != follow[P.left].end(); it3++)
            {
                string b = *it3;
                Table[P.left][b] = P;
            }   
    }
}

void error(string w, int pointer)
{
    cout << "出现错误！错误出现在字符串'" << w.substr(0, pointer) << "'结尾处" << endl;
}

void Prase(string w)
{
    w += "$";
    int pointer = 0;
    while (!Stack.empty())
        Stack.pop();
    Stack.push("$");
    Stack.push(BeginSym);
    while (!Stack.empty())
    {
        string X = Stack.top();
        string a = w.substr(pointer, 1);
        if (w[pointer] >= '0' && w[pointer] <= '9')
        {
            a = "num";
            while (w[pointer + 1] >= '0' && w[pointer + 1] <= '9') pointer++;
        }
        if (termin.find(X) != termin.end())
        {
            if (X == a)
            {
                Stack.pop();
                pointer++;
            }          
            else
            {
                error(w, pointer);
                return;
            }
                
        }
        else
        {
            if (Table.find(X) != Table.end() && Table[X].find(a) != Table[X].end())
            {
                Stack.pop();
                Proce P = Table[X][a];
                int length = P.right.size();
                for (int i = length - 1; i >= 0; i--)
                {
                    string Y = P.right[i];
                    if (Y != "epsilon")
                        Stack.push(Y);
                }
                cout << P.ori << endl;
            }
            else
            {
                error(w, pointer);
                return;
            }        
        }   
    }
    cout << "分析完成！" << endl;
}

int main()
{
    init();
    cout << "First:" << endl;
    for (set<string>::iterator it = non_termin.begin(); it != non_termin.end(); it++)
    {
        CreateFirst(*it);
        cout << *it << ": ";
        for (set<string>::iterator j = first[*it].begin(); j != first[*it].end(); j++)
            cout << *j << " ";
        cout << endl;
    }
    for (set<string>::iterator it = termin.begin(); it != termin.end(); it++)
    {
        first[*it].insert(*it);
    }

    CreateFollow();
    cout << "Follow:" << endl;
    for (set<string>::iterator it = non_termin.begin(); it != non_termin.end(); it++)
    {
        cout << *it << ": ";
        for (set<string>::iterator j = follow[*it].begin(); j != follow[*it].end(); j++)
            cout << *j << " ";
        cout << endl;
    }

    CreateTable();

    for (set<string>::iterator it1 = non_termin.begin(); it1 != non_termin.end(); it1++)
    {
        string A = *it1;
        if (Table.find(A) != Table.end())
        {
            for (set<string>::iterator it2 = termin.begin(); it2 != termin.end(); it2++)
            {
                string a = *it2;
                if (Table[A].find(a) != Table[A].end())
                {
                    cout << "M[" << A << " , " << a << "] = " << Table[A][a].ori << endl;
                }
            }
        }
        
    }
    string input_string;
    while (1)
    {
        cin >> input_string;
        Prase(input_string);
    }
    
}