#include <iostream>
#include <list>

using namespace std;

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
void funkcija(Red<Tip>& red) {
    Tip suma(*(new Tip()));
    int size = red.brojElemenata();
    for(int i = 0; i < size; i++) {
        suma += red.celo();
        red.stavi(red.skini());
    }
    suma /= size;
    for(int i = 0; i < size; i++) {
        if(red.celo() > suma)
            red.skini();
        else 
            red.stavi(red.skini());
    }
}

int main() {
    Red<int> r;
    r.stavi(1);
    r.stavi(2);
    r.stavi(3);
    r.stavi(6);
    r.stavi(10);
    funkcija(r);
    while(r.brojElemenata() > 0)
        cout << r.skini() << endl;
    return 0;
}