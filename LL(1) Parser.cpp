#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <string>
#include <queue>
#include <deque>
char START = 'E';
char END = '#';
using namespace std;
//BASIC
string to_string(char c) {
    string s(1, c);
    return s;
}
vector < string > to_vector(string s) {
    vector < string > v = {
        s
    };
    return v;
}
vector < string > split(string s, char c) {
    vector < string > v;
    string temp;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == c) {
            v.push_back(temp);
            temp = "";
        } else {
            temp += s[i];
        }
    }
    if (temp != "") v.push_back(temp);
    return v;
}
map < char, vector < string > > input() {
    cout << "REPRESENT NULL AS #" << endl;
    int n;
    cout << "Enter No. Of Productions :";
    cin >> n;
    map < char, vector < string > > m;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        vector < string > temp = split(s, '=');
        vector < string > rhs = split(temp[1], '|');
        for (auto prod: rhs)
            m[temp[0][0]].push_back(prod);
    }
    cout << "\n\nEnter Starting Production" << " : ";
    cin >> START;
    cout << endl;
    return m;
}
void print_productions(map < char, vector < string > > prod) {
    cout << endl << endl;
    for (auto lhs: prod) {
        // cout << lhs.first << " : ";
        for (auto rhs: lhs.second) {
            cout << lhs.first << " : " << rhs << endl;
        }
    }
    cout << endl << endl;
}
void print(map < char, vector < string > > prod) {
    for (auto lhs: prod) {
        cout << lhs.first << " : { ";
        for (auto rhs: lhs.second) {
            cout << rhs << ", ";
        }
        cout << " }" << endl;
    }
}
string replace(string s, int pos, string p) {
    string temp = "";
    for (int i = 0; i < s.size(); i++) {
        if (i != pos) {
            temp += s[i];
        } else temp += p;
    }
    return temp;
}
//ACTUAL
map < char, vector < string > > productions;
map < char, vector < string > > first_arr;
map < char, vector < string > > follow_arr;
map < char, map < char, string > > table;
void print_table() {
    vector < char > terminals, non_terminals;
    set < char > th;
    for (auto i: table) {
        non_terminals.push_back(i.first);
        for (auto j: productions[i.first]) {
            for (auto k: j) {
                if (productions.count(k) == 0) {
                    th.insert(k);
                }
            }
        }
    }
    for (auto i: th) terminals.push_back(i);
    for (auto & i: terminals)
        if (i == END) i = '$';
    cout << endl;
    for (int i = 0; i < terminals.size(); i++) {
        cout << " \t\t|" << "\t\t" << terminals[i];
    }
    cout << endl;
    cout << endl;
    for (int i = 0; i < non_terminals.size(); i++) {
        cout << non_terminals[i];
        for (int j = 0; j < terminals.size(); j++) {
            if (find(follow_arr[non_terminals[i]].begin(), follow_arr[non_terminals[i]].end(), to_string(terminals[j])) != follow_arr[non_terminals[i]].end()) {
                if (find(productions[non_terminals[i]].begin(), productions[non_terminals[i]].end(), to_string(END)) != productions[non_terminals[i]].end())
                    table[non_terminals[i]][terminals[j]] = '#';
            }
            cout << " \t\t|" << "\t\t" << table[non_terminals[i]][terminals[j]];
        }
        cout << endl;
        cout << endl;
    }
}
vector < string > first(char c) {
    if (!productions.count(c)) return to_vector(to_string(c));
    vector < string > arr;
    for (auto rhs: productions[c]) {
        for (auto j: rhs) {
            vector < string > temp = first(j);
            for (auto content: temp) {
                arr.push_back(content);
                table[c][content[0]] = rhs;
            }
            if (find(temp.begin(), temp.end(), to_string(END)) == temp.end())
                break;
        }
    }
    set < string > dups;
    for (auto i: arr)
        dups.insert(i);
    arr.clear();
    for (auto i: dups) arr.push_back(i);
    first_arr[c] = arr;
    return arr;
}
map < char, vector < string > > make_it(char c, map < char, vector < string > > productions) {
    map < char, vector < string > > temp;
    for (auto i: productions) {
        for (auto j: i.second) {
            if (find(j.begin(), j.end(), c) != j.end()) {
                temp[i.first].push_back(j);
            }
        }
    }
    // print_productions(temp);
    map < char, vector < string > > tempo = productions;
    map < char, vector < string > > new_ = tempo;
    for (auto & i: tempo) {
        for (auto & j: i.second) {
            string temp2 = j;
            for (int k = 0; k < temp2.size(); k++) {
                if (temp.count(temp2[k])) {
                    for (auto to_replace: temp[temp2[k]]) {
                        string add = temp2;
                        new_[i.first].push_back(replace(add, k, to_replace));
                    }
                }
            }
        }
    }
    set < char > it_has;
    for (auto i: new_) {
        for (auto j: i.second) {
            if (find(j.begin(), j.end(), c) != j.end())
                it_has.insert(i.first);
        }
    }
    set < char > it_has2;
    for (auto i: productions) {
        for (auto j: i.second) {
            if (find(j.begin(), j.end(), c) != j.end())
                it_has2.insert(i.first);
        }
    }
    // print_productions(new_);
    if (it_has.size() != it_has2.size()) {
        // cout << it_has.size() << " " << it_has2.size() << endl;
        return make_it(c, new_);
    }
    // print_productions(new_);
    return new_;
}
void follow(char start) {
    productions['Z'].push_back(to_string(start) + '$');
    for (auto wow: productions) {
        // if(wow.first=='Z') continue;
        char c = wow.first;
        // c = 'F';
        set < string > follow_arr1;
        map < char, vector < string > > temp = make_it(c, productions);
        // print_productions(temp);
        for (auto lhs: temp) {
            for (auto ch: lhs.second) {
                int pos = -1;
                for (int i = 0; i < ch.size(); i++) {
                    if (ch[i] == c) {
                        for (int j = i + 1; j < ch.size(); j++) {
                            vector < string > s;
                            if (first_arr.count(ch[j])) {
                                s = first_arr[ch[j]];
                            } else s.push_back(to_string(ch[j]));
                            for (auto k: s) {
                                follow_arr1.insert(k);
                            }
                            if (find(s.begin(), s.end(), to_string(END)) != s.end()) continue;
                            else break;
                        }
                    }
                }
            }
        }
        follow_arr1.erase(to_string(END));
        if (c != 'Z')
            for (auto rest: follow_arr1)
                follow_arr[c].push_back(rest);
    }
}
void get_first() {
    for (auto i: productions) {
        first(i.first);
    }
}
void get_follow() {
    follow(START);
}
void printq(deque < char > s) {
    while (!s.empty()) {
        cout << s.front();
        s.pop_front();
    }
}
void accept_string(string st) {
    deque < char > stack, input;
    stack.push_back('$');
    stack.push_back(START);
    for (auto i: st)
        input.push_back(i);
    input.push_back('$');
    cout << "Stack" << "\t\t|" << "Input" << "\t\t|" << "Action" << endl;
    while (1) {
        printq(stack);
        cout << "\t\t|";
        printq(input);
        if (stack.back() == '$') {
            cout << "\t\t|" << "POP" << endl;
            if (input.back() != '$') {
                cout << "NOT ACCEPTED";
                return;
            }
            break;
        }
        cout << "\t\t|";
        char one = stack.back();
        char two = input.front();
        stack.pop_back();
        if (one == two) {
            cout << "POP";
            input.pop_front();
            cout << endl;
            continue;
        }
        if (table.count(one) == 0) {
            cout << endl << "NOT ACCEPTED";
            return;
        }
        if (table[one].count(two) == 0) {
            cout << endl << "NOT ACCEPTED";
            return;
        }
        string data = table[one][two];
        cout << "one = " << data;
        if (table[one][two][0] == END) {
            cout << endl;
            continue;
        }
        reverse(data.begin(), data.end());
        for (auto i: data)
            stack.push_back(i);
        cout << endl;
    }
    cout << endl;
    cout << "STRING ACCEPTED";
}
int main() {
    productions = input();
    get_first();
    get_follow();
    cout << "\n\n------------------------------FIRST------------------------------\n\n";
    print(first_arr);
    cout << "\n\n------------------------------FOLLOW------------------------------\n\n";
    print(follow_arr);
    print_table();
    cout << endl << endl;
    string data;
    cout << "Enter String To Be Checked : ";
    cin >> data;
    accept_string(data);
    return 0;
}
/* INPUTS
5
E=TR
R=+TR|#
T=FY
Y=*FY|#
F=(E)|i
5
E=TU
U=+TU|#
T=FP
P=*FP|#
F=(E)|d
6
S=aBDh
B=cC
C=bC
D=EF
E=g|#
F=f|#
4
S=ACB|Cbb|Ba
A=da|BC
B=g|#
C=h|#
5
E=TU
U=+TU|#
T=FP
P=*FP|#
F=i|(E)
8
E=TR
R=+TR
R=#
T=FY
Y=*FY
Y=#
F=(E)
F=i
E
*/
