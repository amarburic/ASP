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

int main() {
    Red<int> test;
    test.stavi(1); 
    test.stavi(2);
    test.stavi(3);
    cout << test.skini();
    cout << test.celo();
    cout << test.brojElemenata();
    return 0;
}