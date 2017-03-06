#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#define rand() 1

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

template <typename K, typename V>
class Cvor {
public:
    K kljuc;
    V vrijednost;
    Cvor *roditelj, *lijevo, *desno;
    Cvor(const K& kljuc, const V& vrijednost, Cvor* roditelj = nullptr, 
    Cvor* lijevo = nullptr, Cvor* desno = nullptr) :
    kljuc(kljuc), vrijednost(vrijednost), roditelj(roditelj), 
    lijevo(lijevo), desno(desno) {}
};

template <typename K, typename V>
class BinStabloMapa : public Mapa<K, V> {
    mutable Cvor<K, V> *korijen;
    int velicina;
    Cvor<K, V>* Trazi(const K& kljuc, Cvor<K, V>* &cvor) const;
    Cvor<K, V>* Umetni(const K& kljuc, const V& vrijednost, Cvor<K, V> *&cvor, Cvor<K, V> * const& roditelj = nullptr);
    void BrisiCvor(const K& kljuc, Cvor<K, V>* &cvor);
    void Brisi(Cvor<K, V> *&cvor);
    void Kopiraj(const Cvor<K, V>* stari, Cvor<K, V>* &novi, Cvor<K, V>* roditelj = nullptr);
public:
    BinStabloMapa() : korijen(nullptr), velicina(0) {}
    BinStabloMapa(const BinStabloMapa& rhs);
    BinStabloMapa(BinStabloMapa&& rhs);
    ~BinStabloMapa();
    V& operator[](const K&);
    V operator[](const K&) const;
    BinStabloMapa& operator=(const BinStabloMapa& rhs);
    BinStabloMapa& operator=(BinStabloMapa&& rhs);
    int brojElemenata() const { return velicina; }
    void obrisi() { Brisi(korijen); velicina = 0; }
    void obrisi(const K& kljuc) { BrisiCvor(kljuc, korijen); }
};

template <typename K, typename V>
Cvor<K, V>* BinStabloMapa<K, V>::Trazi(const K& kljuc, Cvor<K, V>* &cvor) const {
    if(cvor == nullptr || cvor->kljuc == kljuc)
        return cvor;
    if(cvor->kljuc < kljuc)
        return Trazi(kljuc, cvor->desno);
    else if(cvor->kljuc > kljuc) 
        return Trazi(kljuc, cvor->lijevo);
}

template <typename K, typename V>
Cvor<K, V> *BinStabloMapa<K, V>::Umetni(const K& kljuc, const V& vrijednost, Cvor<K, V> *&cvor, Cvor<K, V> * const& roditelj) {
    if(cvor == nullptr) {
        velicina++;
        return cvor = new Cvor<K, V>(kljuc, vrijednost, roditelj);
    }
    if(cvor->kljuc < kljuc)
        return Umetni(kljuc, vrijednost, cvor->desno, cvor);
    if(cvor->kljuc > kljuc)
        return Umetni(kljuc, vrijednost, cvor->lijevo, cvor);
}

template <typename K, typename V>
void BinStabloMapa<K, V>::BrisiCvor(const K& kljuc, Cvor<K, V>* &cvor) {
    Cvor<K, V> *p(Trazi(kljuc, korijen)), *rp(nullptr);
    if (p == nullptr)
        return;
    if (p->lijevo == nullptr)
        rp = p->desno;
    else if (p->desno == nullptr)
        rp = p->lijevo;
    else {
        rp = p->lijevo;
        while (rp->desno != nullptr)
            rp = rp->desno;
        if (rp->roditelj != p) {
            rp->roditelj->desno = rp->lijevo;
            rp->lijevo = p->lijevo;
        }
        rp->desno = p->desno;
    }
    if (p->roditelj == nullptr)
        korijen = rp;
    else if (p == p->roditelj->lijevo)
    p->roditelj->lijevo = rp;
    else
    p->roditelj->desno = rp;
    velicina--;
    delete p;
    p = nullptr;
}

template <typename K, typename V>
void BinStabloMapa<K, V>::Brisi(Cvor<K, V> *&cvor) {
    if(cvor != nullptr) {
        Brisi(cvor->desno);
        Brisi(cvor->lijevo);
        delete cvor;
        cvor = nullptr;
    }
}

template <typename K, typename V>
void BinStabloMapa<K, V>::Kopiraj(const Cvor<K, V>* stari, Cvor<K, V>* &novi, Cvor<K, V>* roditelj) {
    if(stari == nullptr)
        return;
    novi = new Cvor<K, V>(stari->kljuc, stari->vrijednost, roditelj);
    Kopiraj(stari->lijevo, novi->lijevo, novi);
    Kopiraj(stari->desno, novi->desno, novi);
}

template <typename K, typename V>
BinStabloMapa<K, V>::BinStabloMapa(const BinStabloMapa& rhs) 
: velicina(rhs.velicina), korijen(nullptr) {
    Kopiraj(rhs.korijen, korijen);
}

template <typename K, typename V>
BinStabloMapa<K, V>::BinStabloMapa(BinStabloMapa&& rhs) 
: velicina(rhs.velicina), korijen(rhs.korijen) {
    rhs.korijen = nullptr;
}

template <typename K, typename V>
BinStabloMapa<K, V>::~BinStabloMapa() {
    Brisi(korijen);
}

template <typename K, typename V>
V& BinStabloMapa<K, V>::operator[](const K& kljuc) {
   
    Cvor<K, V>* rezultat(Trazi(kljuc, korijen));
    if(rezultat != nullptr)
         return rezultat->vrijednost;
    return Umetni(kljuc, V(), korijen)->vrijednost;
}

template <typename K, typename V>
V BinStabloMapa<K, V>::operator[](const K& kljuc) const {
    auto rezultat(Trazi(kljuc, korijen));
    if(rezultat != nullptr)
        return rezultat->vrijednost;
    return V();
}

template <typename K, typename V>
BinStabloMapa<K, V>& BinStabloMapa<K, V>::operator=(const BinStabloMapa<K, V>& rhs) {
    if(this == &rhs)
        return *this;
    Brisi(korijen);
    velicina = rhs.velicina;
    korijen = nullptr;
    Kopiraj(rhs.korijen, korijen);
    return *this;
}

template <typename K, typename V>
BinStabloMapa<K, V>& BinStabloMapa<K, V>::operator=(BinStabloMapa<K, V>&& rhs) {
    if(this == &rhs)
        return *this;
    Brisi(korijen);
    velicina = rhs.velicina;
    korijen = rhs.korijen;
    rhs.korijen = nullptr;
    return *this;
}


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
    srand(0);
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

double prosjecnoVrijeme(clock_t t1, clock_t t2, int broj_el) {
    return ((t2 - t1) / (CLOCKS_PER_SEC / 1000.)) / broj_el;
}



int main()
{
    
BinStabloMapa <int, int> m;
int vel(1000000);
for (int i(-vel); i<vel; i++) {
    m[i] = i;
}

cout << "OK";

    NizMapa<int, int> niz_test;
    BinStabloMapa<int, int> stablo_test;
    vector<int> kljucevi;
    int broj_el = 10000;
    for(int i = 0; i < broj_el; i++)
        kljucevi.push_back(rand());
    clock_t vrijeme1 = clock();
    for(auto k : kljucevi)
        niz_test[k];
    clock_t vrijeme2 = clock();
    cout << "Prosjecno vrijeme umetanja u niz mapu: " << prosjecnoVrijeme(vrijeme1, vrijeme2, broj_el) << " ms." << endl;
    vrijeme1 = clock();
    for(auto k : kljucevi)
        stablo_test[k];
    vrijeme2 = clock();
    cout << "Prosjecno vrijeme umetanja u binarno stablo mapu: " << prosjecnoVrijeme(vrijeme1, vrijeme2, broj_el) << " ms." << endl;
    vrijeme1 = clock();
    for(auto k : kljucevi)
        niz_test[k];
    vrijeme2 = clock();
    cout << "Prosjecno vrijeme citanja niz mape: " << prosjecnoVrijeme(vrijeme1, vrijeme2, broj_el) << " ms." << endl;
    vrijeme1 = clock();
    for(auto k : kljucevi)
        stablo_test[k];
    vrijeme2 = clock();
    cout << "Prosjecno vrijeme citanja binarno stablo mape: " << prosjecnoVrijeme(vrijeme1, vrijeme2, broj_el) << " ms." << endl;
   
    //Citanje i umetanje u niz mapu u najgorem slucaju je O(n), 
    //a prosjecni slucaj je O(n/2)
    //jer je potrebno linerano pretraziti citav niz da bi se pronasao
    //element sa poklapajucim kljucem, ukoliko postoji, prije nego se doda novi 
    //u O(1) vremenu
    
    //Citanje i umetanje u binarno stablo u najgorem slucaju je O(n),
    //ali ukoliko je stablo bolje balansirano, kompleknost moze biti 
    //O(logn)
    
    //Umetanje i citanje je ocigledno optimalnije pri koristenju 
    //binarnog stabla, sto se poklapa sa ocekivanjima. 
}