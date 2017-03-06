#include <iostream>
#include <algorithm>
#include <ctime>
#include <string>

using namespace std;

template <typename Tip>
class Stek {
    struct Cvor {
        Cvor* prethodni;
        Tip element;
        Cvor(const Tip& element, Cvor* prethodni) :
           prethodni(prethodni), element(element) {}
    };
    int velicina;
    Cvor* pocetak;
    void Obrisi();
    void Kopiraj(const Stek&);
    void TestPraznogSteka() {
        if(pocetak == nullptr)
            throw "Stek je prazan";
    }
public:
    Stek() : velicina(0), pocetak(nullptr) {}
    Stek(const Stek& rhs) : velicina(0), pocetak(nullptr) { Kopiraj(rhs); }
    Stek(Stek&& rhs) : pocetak(rhs.pocetak), velicina(rhs.velicina) { rhs.pocetak = nullptr; }
    ~Stek() { Obrisi(); }
    void brisi() { Obrisi(); }
    void stavi(const Tip& el) {
            pocetak = new Cvor(el, pocetak);
            velicina++; 
    }
    Tip skini();
    Tip& vrh() {
        TestPraznogSteka();
        return pocetak->element;
    }
    Tip vrh() const {
        TestPraznogSteka();
        return pocetak->element;
    }
    int brojElemenata() const { return velicina; };
    Stek& operator=(const Stek&);
    Stek& operator=(Stek&&);
};

template <typename Tip>
void Stek<Tip>::Obrisi() {
    while(pocetak != nullptr) {
        Cvor* temp = pocetak->prethodni;
        delete pocetak;
        pocetak = temp;
    }
    velicina = 0;
}

template <typename Tip>
void Stek<Tip>::Kopiraj(const Stek& rhs) {
    Cvor* temp(nullptr);
    if(rhs.velicina > velicina) {
        Cvor* noviVrh(nullptr), *iter(rhs.pocetak);
        for(int i = 0; i < rhs.velicina - velicina; i++) {
            if(i == 0) {
                noviVrh = new Cvor(iter->element, nullptr);
                temp = noviVrh;
            } else {
                temp->prethodni = new Cvor(iter->element, nullptr);
                temp = temp->prethodni;
            }
            iter = iter->prethodni;
        }
        temp->prethodni = pocetak;
        temp = pocetak;
        pocetak = noviVrh;
    }
    else {
        Cvor* cpy;
        temp = pocetak;
        for(int i = 0; i < rhs.velicina - velicina; i++) {
            cpy = temp->prethodni;
            delete pocetak;
            temp = cpy;
        }
    }
    Cvor* iterTrenutni(temp), *iterNovi(rhs.pocetak);
    for(int i = 0; i < min(rhs.velicina, velicina); i++) {
        iterTrenutni->element = iterNovi->element;
        iterTrenutni = iterTrenutni->prethodni;
        iterNovi = iterNovi->prethodni;
    }
    velicina = rhs.velicina;
}

template <typename Tip>
Tip Stek<Tip>::skini() {
    TestPraznogSteka();
    Tip ret = pocetak->element;
    Cvor* temp = pocetak->prethodni;
    delete pocetak;
    pocetak = temp;
    velicina--;
    return ret; 
}

template <typename Tip>
Stek<Tip>& Stek<Tip>::operator=(const Stek<Tip>& rhs) {
    if(this == &rhs)
        return *this;
    Kopiraj(rhs);
    return *this;
}

template <typename Tip>
Stek<Tip>& Stek<Tip>::operator=(Stek<Tip>&& rhs) {
    if(this == &rhs)
        return *this;
    Kopiraj(rhs);
    rhs.pocetak = nullptr;
    return *this;
}

bool provjeri_zagrade(string tekst) {
    Stek<char> stek;
    const string zagrade[3]{"()", "[]", "{}"};
    for(int i = 0; i < tekst.size(); i++) {
        int j = 0;
        for(; j < 3; j++) {
            if(tekst[i] == zagrade[j][0]) {
                stek.stavi(tekst[i]);
                break;
            } else if(tekst[i] == zagrade[j][1]) {
                if(stek.brojElemenata() == 0 || stek.vrh() != zagrade[j][0])
                    return false;
                stek.skini();
                break;
            }
        }
        if(j == 3) return false;
    }
    if(!stek.brojElemenata() == 0)
        return false;
    return true;
}


int main() {
    
        string input = "";
    if(provjeri_zagrade(input))
        cout << "Balansirane" << endl;
    else
        cout << "Nisu balansirane" << endl;
    return 0;

    return 0;
}