#include <cstdio>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <stack>
#include <queue>
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
#define UINT unsigned int
using namespace std;
string BeginSym;
set <string> termin, non_termin;
map <string, set<string> > first, follow;
set <string> created_first, created_follow;
struct Proce;
vector<Proce> proce;
map <string, vector<Proce> > proce_index;
int tot_pro = 0;
vector<string> boostsplit(const std::string & input)
{
    std::vector <std::string> fields;
    boost::split(fields, input, boost::is_any_of(" "));
    return fields;
}
struct Proce
{
    int id;
    string ori;
    string left;
    vector<string> right;
    Proce() {};
    Proce(const string& _p)
    {
        id = tot_pro++;
        ori = _p;
        vector<string> syms = boostsplit(_p);
        left = syms[0];
        int l = syms.size();
        FOR(i, 2, l - 1)
            right.push_back(syms[i]);
        proce_index[left].push_back(*this);
    }   
    bool operator< (const Proce& rhs) const
    {
        return ori < rhs.ori;
    }
    bool operator== (const Proce& rhs) const
    {
        return ori == rhs.ori;
    }

};
struct Item
{
    Proce pro;
    int dot, tot_len, belong;
    Item() {};
    Item(Proce _p, int _dot)
    {
        pro = _p;
        dot = _dot;
        tot_len = _p.right.size();
    }
    bool operator< (const Item& rhs) const
    {
        return (pro == rhs.pro && dot < rhs.dot) || pro < rhs.pro;
    }
    bool operator== (const Item& rhs) const
    {
        return pro == rhs.pro && dot == rhs.dot;
    }
    
};

struct ItemSet
{
    set<Item> item_set;
    ItemSet() {};
    ItemSet(set<Item> _item_set) : item_set(_item_set) {};
    void extend()
    {
        int ori_num = 0;
         while (item_set.size() > (UINT)ori_num)
        {
            ori_num = item_set.size();
            for (set<Item>::iterator it1 = item_set.begin(); it1 != item_set.end(); it1++)
            { 
                Item I = *it1;
                Proce Pa = I.pro;
                
                if ((UINT)I.dot < Pa.right.size() && non_termin.find(Pa.right[I.dot]) != non_termin.end())
                {
                    
                    string B = Pa.right[I.dot];
                    
                    for (vector<Proce>::iterator it2 = proce_index[B].begin(); it2 != proce_index[B].end(); it2++)
                    {
                        
                        Proce Pb = *it2;
                        Item new_i = Item(Pb, 0);
                        
                        item_set.insert(new_i);
                        
                        
                    }
                    
                }
                
            }
        }
    }
    
    bool operator== (const ItemSet& rhs) const
    {
        if (item_set.size() != rhs.item_set.size()) return false;
        for (set<Item>::iterator it1 = item_set.begin(); it1 != item_set.end(); it1++)
        {
            if (rhs.item_set.find(*it1) == rhs.item_set.end()) return false;
        }
        return true;
    }
};

struct ActionOp
{
    string type;
    int val;
    ActionOp() {};
    ActionOp(string _type, int _val) : type(_type), val(_val) {};
};

vector<ItemSet> item_sets;
map<string, int>go[1000];
vector <map<string, ActionOp> > Action;
vector <map<string, int> > Goto;

void Extend()
{
    non_termin.insert("S");
    Proce P = Proce("S -> " + BeginSym);
    BeginSym = "S";
    proce.push_back(P);
    set<Item> its;
    its.insert(Item(P, 0));
    ItemSet its0 = ItemSet(its);
    its0.extend();
    item_sets.push_back(its0);
}

void init()
{
    proce.push_back(Proce("E -> E + T"));
    proce.push_back(Proce("E -> E - T"));
    proce.push_back(Proce("E -> T"));
    proce.push_back(Proce("T -> T * F"));
    proce.push_back(Proce("T -> T / F"));
    proce.push_back(Proce("T -> F"));
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
    non_termin.insert("T");
    non_termin.insert("F");

    BeginSym = "E";

    Extend();

    first[BeginSym]= set<string>{"(", "num"};
    first["E"] = set<string>{ "(", "num" };
    first["T"] = set<string>{ "(", "num" };
    first["F"] = set<string>{ "(", "num" };

    follow[BeginSym] = set<string>{ "$" };
    follow["E"] = set<string>{ "+", "-", "$", ")" };
    follow["T"] = set<string>{ "+", "-", "*", "/", "$", ")" };
    follow["F"] = set<string>{ "+", "-", "*", "/", "$", ")" };
}



void CreateGo()
{
    queue<int> Q;
    Q.push(0);
    while (!Q.empty())
    {
        ItemSet IS = item_sets[Q.front()];
        Q.pop();

        vector<ItemSet>::iterator it0 = find(item_sets.begin(), item_sets.end(), IS);
        int id = it0 - item_sets.begin();

        for (set<string>::iterator it1 = termin.begin(); it1 != termin.end(); it1++)
        {
            string a = *it1;
            set<Item> its;
            for (set<Item>::iterator it2 = IS.item_set.begin(); it2 != IS.item_set.end(); it2++)
            {
                Item I = *it2;
                Proce P = I.pro;
                if ((UINT)I.dot < P.right.size() && P.right[I.dot] == a)
                    its.insert(Item(P, I.dot + 1));
            }
            if (its.empty()) continue;
            ItemSet n_IS(its);
            n_IS.extend();
            vector<ItemSet>::iterator it3;
            for (it3 = item_sets.begin(); it3 != item_sets.end(); it3++) if (*it3 == n_IS) break;
            int n_id = it3 - item_sets.begin();
            if (it3 == item_sets.end())
            {
                item_sets.push_back(n_IS);
                Q.push(n_id);
                go[id][a] = n_id;
            }
            else
            {
                go[id][a] = n_id;
            }
        }

        for (set<string>::iterator it1 = non_termin.begin(); it1 != non_termin.end(); it1++)
        {
            string a = *it1;
            set<Item> its;
            for (set<Item>::iterator it2 = IS.item_set.begin(); it2 != IS.item_set.end(); it2++)
            {
                Item I = *it2;
                Proce P = I.pro;
                if ((UINT)I.dot < P.right.size() && P.right[I.dot] == a)
                    its.insert(Item(P, I.dot + 1));
            }
            if (its.empty()) continue;
            ItemSet n_IS(its);
            n_IS.extend();
            vector<ItemSet>::iterator it3;
            for (it3 = item_sets.begin(); it3 != item_sets.end(); it3++) if (*it3 == n_IS) break;
            int n_id = it3 - item_sets.begin();
            if (it3 == item_sets.end())
            {                
                item_sets.push_back(n_IS);
                Q.push(n_id);
                go[id][a] = n_id;
            }
            else
            {
                go[id][a] = n_id;
            }
        }
    }
}


void CreateTable()
{
    Action.resize(item_sets.size());
    Goto.resize(item_sets.size());
    FOR(i, 0, (int)(item_sets.size() - 1))
    {
        ItemSet IS = item_sets[i];
        for (set<Item>::iterator it1 = IS.item_set.begin(); it1 != IS.item_set.end(); it1++)
        {
            Item I = *it1;
            Proce P = I.pro;
            if ((UINT)I.dot < P.right.size())
            {
                string a = P.right[I.dot];
                if (termin.find(a) != termin.end())
                    Action[i][a] = ActionOp("SHIFT", go[i][a]);
                else
                    Goto[i][a] = go[i][a];
            }
            else
            {
                string A = P.left;
                if (A == BeginSym)
                    Action[i]["$"] = ActionOp("ACC", 0);
                else
                {
                    for (set<string>::iterator it2 = follow[A].begin(); it2 != follow[A].end(); it2++)
                    {
                        string a = *it2;
                        int pid = P.id;
                        Action[i][a] = ActionOp("REDUCE", pid);
                    }
                }
            }
        }
    }
}

void error(string w, int pointer)
{
    cout << "出现错误！错误出现在字符串'" << w.substr(0, pointer) << "'结尾处" << endl;
}

void print_stack_sym(stack<string> S)
{
    string str;
    while (!S.empty())
    {
        string s = S.top();
        S.pop();
        str = s + str;
        FOR(i, s.size() + 1, 4)
            str = " " + str;
    }
    cout << setw(30) << left << str << endl;
}

void print_stack_state(stack<int> S)
{
    string str;
    while (!S.empty())
    {
        int s = S.top();
        S.pop();
        string ss = to_string(s);
        str = ss + str;
        FOR(i, ss.size() + 1, 4)
            str = " " + str;
    }
    cout << setw(30) << left << str << "   ";
}

void Prase(string w)
{
    int step = 0;
    w += "$";
    int pointer = 0;
    stack<int> state_stack;
    stack<string> sym_stack;
    state_stack.push(0);
    sym_stack.push(BeginSym);  
    while (!state_stack.empty())
    {
        cout << setw(3) <<left << ++step << " ";
        print_stack_state(state_stack);
        cout << setw(10) <<right << w.substr(pointer) << "   ";
        int S = state_stack.top();
        string a = w.substr(pointer, 1);
        if (w[pointer] >= '0' && w[pointer] <= '9')
        {
            a = "num";
            while (w[pointer + 1] >= '0' && w[pointer + 1] <= '9') pointer++;
        }
        if (Action[S][a].type == "SHIFT")
        {
            cout << "移进" << a << endl;
            cout << setw(4) << " ";
            print_stack_sym(sym_stack);
            state_stack.push(Action[S][a].val);
            sym_stack.push(a);
            pointer++;
        }
        else if (Action[S][a].type == "REDUCE")
        {
            Proce P = proce[Action[S][a].val];
            cout << "使用产生式" << P.ori << "规约" << endl;
            cout << setw(4) << " ";
            print_stack_sym(sym_stack);
            FOR(i, 1, (int)P.right.size())
            {
                state_stack.pop();
                sym_stack.pop();
            }
            sym_stack.push(P.left);
            int n_state = state_stack.top();
            state_stack.push(Goto[n_state][P.left]);
            
        }
        else if (Action[S][a].type == "ACC")
        {
            cout << "分析完成" << endl;
            cout << setw(4) << " ";
            print_stack_sym(sym_stack);
            return;
        }
        else
            error(w, pointer);
        
    }
}



int main()
{
    init();
    CreateGo();
    FOR(i, 0, (int)(item_sets.size() - 1))
    {
        cout << "I" << i << ":" << endl;
        for (set<Item>::iterator it = item_sets[i].item_set.begin(); it != item_sets[i].item_set.end(); it++)
        {
            Item I = *it;
            Proce P = I.pro;
            cout << P.left << "->";
            FOR(j, 0, I.dot - 1)
                cout << P.right[j];
            cout << "·";
            FOR(j, I.dot, (int)(P.right.size() - 1))
                cout << P.right[j];
            cout << endl;
        }
        cout << endl;
    }
    FOR(i, 0, (int)(item_sets.size() - 1))
    {
        for (set<string>::iterator it = termin.begin(); it != termin.end(); it++)
        {
            string a = *it;
            if (go[i].find(a) != go[i].end())
            {
                cout << "GO[" << i << " , " << a << "] = " << go[i][a] << endl;
            }
        }
        for (set<string>::iterator it = non_termin.begin(); it != non_termin.end(); it++)
        {
            string a = *it;
            if (go[i].find(a) != go[i].end())
            {
                cout << "GO[" << i << " , " << a << "] = " << go[i][a] << endl;
            }
        }
        cout << endl;
    }

    CreateTable();
    FOR(i, 0, int(Action.size() - 1))
    {
        for (map<string, ActionOp>::iterator it = Action[i].begin(); it != Action[i].end(); it ++)
        {
            ActionOp Op = it->second;
            cout << "Action[" << i << " , " << it->first << "] = ";
            if (Op.type == "ACC") cout << "ACC";
            else
            {
                if (Op.type == "SHIFT") cout << "S";
                if (Op.type == "REDUCE") cout << "R";
                cout << Op.val;
            }
            cout << endl;
        }
    }

    FOR(i, 0, int(Goto.size() - 1))
    {
        for (map<string, int>::iterator it = Goto[i].begin(); it != Goto[i].end(); it++)
            cout << "Goto[" << i << " , " << it->first << "] = " << it->second << endl;
    }

    string input_string;
    while (1)
    {
        cin >> input_string;
        Prase(input_string);
    }
}
