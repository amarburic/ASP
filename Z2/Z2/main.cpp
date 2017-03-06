#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

template <typename Tip>
class Stek {
    vector<Tip> v;
    void TestPraznogSteka() {
        if(v.empty())
            throw "Stek je prazan";
    }
public:
    Stek() : v() {}
    Stek(const Stek& rhs) : v(rhs.v) {}
    Stek(Stek&& rhs) : v(move(rhs.v)) {}
    Stek(initializer_list<Tip> rhs) : v(rhs) {}
    void brisi() { v.clear(); }
    void stavi(const Tip& el) { v.push_back(el); }
    Tip skini() { 
        TestPraznogSteka();
        auto ret(v.back());
        v.pop_back();
        return ret;
    }
    Tip& vrh() {
        TestPraznogSteka();
        return v.back();
    }
    Tip vrh() const {
        TestPraznogSteka();
        return v.back();
    }
    int brojElemenata() const { return v.size(); }
    Stek& operator=(const Stek& rhs) { 
        if(this == &rhs)
            return *this;
        v = rhs.v;
        return *this;
    }
    Stek& operator=(Stek&& rhs) {       
        if(this == &rhs)
            return *this;
        v = move(rhs.v);
        return *this;}
};


void pretraga(Stek<vector<int>>& s, int trazeni) {
    if(!s.brojElemenata()) {
        cout << "Nema elementa";
        return;
    }
    auto tmp(s.skini());
    if(tmp.empty() || trazeni < tmp[0]) 
        pretraga(s, trazeni);
    else if(trazeni > tmp[tmp.size() - 1])
        cout << "Nema elementa";
    else {
        int dno(0), vrh(tmp.size() - 1);
        bool pronadjen(false);
        while(vrh >= dno && !pronadjen) {
            int srednji((vrh + dno) / 2); 
            if(tmp[srednji] == trazeni) {
                cout << srednji << " " << s.brojElemenata();
                pronadjen = true;
            }
            else if(tmp[srednji] > trazeni)
                vrh = srednji - 1;
            else
                dno = srednji + 1;
        }
        if(!pronadjen) 
            cout << "Nema elementa";
    }
    s.stavi(tmp);
}

int main() {
Stek<vector<int> > s;
vector<int> a;
for (int i(10); i<60; i++)
a.push_back(i);
vector<int> b;
b.push_back(6);b.push_back(8);
vector<int> c;
c.push_back(9);c.push_back(10);c.push_back(130);
vector<int> d;
d.push_back(157);d.push_back(226);d.push_back(3424);
s.stavi(a);
s.stavi(b);
s.stavi(c);
s.stavi(d);
pretraga(s,1);
}