#include <iostream>
#include <vector>
#include <utility>
#include <string>

using namespace std;

template <typename K, typename V>
class Mapa {
public:
    Mapa() {}
    virtual ~Mapa() {}
    virtual V& operator[](const K&) = 0;
    virtual V operator[](const K&) const = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const K&) = 0;
    
};

template <typename K,typename V>
class NizMapa : public Mapa<K, V> {
    static const int INICIJALNA_VELICINA = 32;
    int brEl, prostor;
    pair<K, V> **niz, **kraj;
    void Prosiri();
public:
    NizMapa() : brEl(0), prostor(INICIJALNA_VELICINA), niz(new pair<K, V>*[prostor]{}), kraj(niz) {}
    NizMapa(const NizMapa& rhs);
    NizMapa(NizMapa&& rhs);
    ~NizMapa();
    V& operator[](const K& kljuc);
    V operator[](const K& kljuc) const;
    NizMapa& operator=(const NizMapa& rhs);
    NizMapa& operator=(NizMapa&& rhs);
    int brojElemenata() const { return brEl; }
    void obrisi(const K& kljuc);
    void obrisi();
};

template <typename K, typename V>
NizMapa<K, V>::NizMapa(const NizMapa& rhs) : brEl(rhs.brEl), prostor(rhs.prostor), niz(new pair<K, V>*[prostor]{}), kraj(niz) {
    for(pair<K, V> **it = rhs.niz, **it2 = niz; it != rhs.kraj; it++) 
        if(*it) {
            (*it2++) = new pair<K, V>(**it);
            kraj++;
        }
}

template <typename K, typename V>
NizMapa<K, V>::NizMapa(NizMapa&& rhs) : brEl(rhs.brEl), prostor(rhs.prostor), niz(rhs.niz), kraj(rhs.kraj) {
    rhs.niz = rhs.kraj = nullptr;
}

template <typename K, typename V>
NizMapa<K, V>& NizMapa<K, V>::operator=(const NizMapa& rhs) {
    if(this == &rhs)
        return *this;
    obrisi();
    delete[] niz;
    brEl = rhs.brEl;
    prostor = rhs.prostor;
    niz = new pair<K, V>*[prostor]{};
    kraj = niz;
    for(pair<K, V> **it = rhs.niz, **it2 = niz; it != rhs.kraj; it++) 
        if(*it) {
            (*it2++) = new pair<K, V>(**it);
            kraj++;
        }
    return *this;
}

template <typename K, typename V>
NizMapa<K, V>& NizMapa<K, V>::operator=(NizMapa&& rhs) {
    if(this == &rhs)
        return *this;
    obrisi();
    delete[] niz;
    brEl = rhs.brEl;
    prostor = rhs.prostor;
    niz = rhs.niz;
    kraj = rhs.kraj;
    rhs.niz = rhs.kraj = nullptr;
    return *this;
}

template <typename K, typename V>
void NizMapa<K, V>::Prosiri() {
    pair<K, V> **noviNiz = new pair<K, V>*[brEl * 2]{};
    pair<K, V> **it, **it2;
    for(it = niz, it2 = noviNiz; it != kraj; it++)
        if(*it != nullptr)
            (*it2++) = new pair<K, V>(**it++);
    int brEl2(brEl * 2);
    obrisi();
    delete[] niz;
    prostor = brEl = brEl2;
    kraj = it2;
    niz = noviNiz;
}

template <typename K, typename V>
V& NizMapa<K, V>::operator[](const K& kljuc) {
    bool pronadjen(false);
    pair<K, V> **it;
    for(it = niz; it != kraj; it++)
        if(*it && (*it)->first == kljuc) { 
            pronadjen = true;
            break;
        }
    if(pronadjen)
        return (*it)->second;
    else {
        if(kraj - niz == prostor)
            Prosiri();
        *kraj = new pair<K, V>(kljuc, V());
        brEl++;
        return (*kraj++)->second;
    }
}

template <typename K, typename V>    
V NizMapa<K, V>::operator[](const K& kljuc) const {
    bool pronadjen(false);
    pair<K, V> **it;
    for(it = niz; it != kraj; it++)
        if(*it && (*it)->first == kljuc) { 
            pronadjen = true;
            break;
        }
    if(pronadjen)
        return (*it)->second;
    else 
        return V();
}

template <typename K, typename V>    
void NizMapa<K, V>::obrisi(const K& kljuc) {
    bool pronadjen(false);
    for(pair<K, V> **it = niz; it != kraj; it++)
        if(*it && (*it)->first == kljuc) {
            delete *it;
            *it = nullptr;
            if(it + 1 == kraj)
                kraj = it;
            brEl--;
            pronadjen = true;
            break;
        }
    if(!pronadjen)
        throw "Kljuc nije pronadjen";
}

template <typename K, typename V>    
void NizMapa<K, V>::obrisi() {
    for(pair<K, V> **it = niz; it != kraj; it++) {
        delete *it;
        *it = nullptr;
    }
    brEl = 0;
    kraj = niz;
}

template <typename K, typename V>    
NizMapa<K, V>::~NizMapa() {
    obrisi();
    delete[] niz;
}

int main() {
    // Test performansi NizMape
NizMapa <int, int> m;
for (int i(1000); i>-1000; i--)
    m[i] = i*i;
cout << m[-100];
    return 0;
}
