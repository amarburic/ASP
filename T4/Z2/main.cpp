#include <iostream>
#include <vector>
#include <list>

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

template <typename Tip>
class Red {
    list<Tip> lista;
    void TestPrazneListe() {
        if(lista.empty())
            throw "Lista je prazna";
    }
public:
    Red() = default;
    Red(const Red& rhs) : lista(rhs.lista) {}
    Red(Red&& rhs) : lista(move(rhs.lista)) {}
    Red& operator= (const Red& rhs) {
        lista = rhs.lista;
        return *this;
    }
    Red& operator=(Red&& rhs) {
        lista = move(rhs.lista);
        return *this;
    }
    void brisi() { lista.clear(); }
    void stavi(const Tip& el) { lista.push_back(el); }
    Tip skini() { 
        TestPrazneListe();
        auto temp(lista.front());
        lista.pop_front();
        return temp;
    }
    Tip& celo() {
        TestPrazneListe();
        return lista.front();
    }
    Tip celo() const {
        TestPrazneListe();
        return lista.front();
    }
    int brojElemenata() const {
        return lista.size();
    }
};


template <typename Tip>
void funkcija(Stek<Tip>& s, Red<Tip>& r, int pocetnaDuzinaReda) {
 
    for(int i = 0; i < pocetnaDuzinaReda; i++) {
        s.stavi(r.celo());
        r.stavi(r.skini());
    }
    for(int i = 0; i < 2 * pocetnaDuzinaReda; i++)
        r.stavi(s.skini());
    for(int i = 0; i < pocetnaDuzinaReda; i++)
        s.stavi(r.skini());
    for(int i = 0; i < pocetnaDuzinaReda; i++)
        r.skini();
    for(int i = 0; i < pocetnaDuzinaReda; i++)
        s.stavi(r.skini());
    for(int i = 0; i < pocetnaDuzinaReda; i++)
        r.stavi(s.skini());
    while(r.brojElemenata())
        s.stavi(r.skini());
    
}


int main() {
    Stek<int> s; 
    Red<int> r;
    s.stavi(5);
    s.stavi(6);
    s.stavi(7);
    s.stavi(8);
    
    r.stavi(1);
    r.stavi(2);
    r.stavi(3);
    r.stavi(4);
    funkcija(s, r, s.brojElemenata());
    
    while(s.brojElemenata())
        cout << s.skini() << endl;
    std::cout << "Tutorijal 4, Zadatak 2";
    return 0;
}
/*
Napisati funkciju koja prima stek, red i varijablu pocetnaDuzinaReda tipa int koja će
elemente iz reda ubaciti na dno steka u obrnutom redoslijedu. Primjer: Neka je dat red sa
elementima {1, 2, 3, 4} gdje je element 1 na čelu reda i stek sa elementima {5, 6, 7, 8} gdje je
element 8 na vrhu steka. Rezultantni stek se sastoji od elemenata {1, 2, 3, 4, 5, 6, 7, 8} gdje je
element 8 na vrhu steka. Ustanoviti koja je vremenska kompleksnost rješenja.
*/


