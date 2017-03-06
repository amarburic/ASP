#include <iostream>
#include <stdexcept>
#include <ctime>
#include <algorithm>

using namespace std;

template <class Tip>
class Lista {
public:
    Lista() {}
    virtual ~Lista() {}
    virtual int brojElemenata() const = 0;
    virtual Tip& trenutni() = 0;
    virtual Tip trenutni() const = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const Tip&) = 0;
    virtual void dodajIza(const Tip&) = 0;
    virtual Tip& operator[](int) = 0;
    virtual Tip operator[](int) const = 0;
};
template <typename Tip>
class Iterator; 

//operator dodjele i konstruktor kopije
template <class Tip>
class NizLista : public Lista<Tip> {
    int alociranoMjesta, brojEl, trenIndeks;
    Tip* niz;
    void Prosiri();
    void TestIndeksa(int) const;
    void TestPrazneListe() const;
public: 
    NizLista() : alociranoMjesta(1), 
        brojEl(0), trenIndeks(-1), 
        niz(new Tip[alociranoMjesta]{}) {}
    NizLista(const NizLista& rhs) : alociranoMjesta(rhs.alociranoMjesta), 
        brojEl(rhs.brojEl), trenIndeks(rhs.trenIndeks), niz(new Tip[alociranoMjesta]{}) {
            niz = new Tip[alociranoMjesta];
            for(int i = 0; i < brojEl; i++)
                niz[i] = rhs.niz[i];
    }
    NizLista(NizLista &&rhs) :  alociranoMjesta(rhs.alociranoMjesta), 
        brojEl(rhs.brojEl), trenIndeks(rhs.trenIndeks), niz(rhs.niz) {rhs.niz = nullptr; }
    ~NizLista() { delete[] niz; }
    int brojElemenata() const { return brojEl; }
    Tip& trenutni() { TestPrazneListe(); return niz[trenIndeks]; }
    Tip trenutni() const { TestPrazneListe(); return niz[trenIndeks]; }
    bool prethodni() { 
        TestPrazneListe(); 
        return (trenIndeks == 0) ? false : (trenIndeks-- > 0); 
    }
    bool sljedeci() { 
        TestPrazneListe(); 
        return (trenIndeks ==  brojEl - 1) ? false : (trenIndeks++ > 0); 
    }
    void pocetak() { TestPrazneListe(); trenIndeks = 0; }
    void kraj() { TestPrazneListe(); trenIndeks = brojEl - 1; }
    void obrisi();
    void dodajIspred(const Tip&);
    void dodajIza(const Tip&);
    Tip& operator[](int i) { TestIndeksa(i); return niz[i]; }
    Tip operator[](int i) const { TestIndeksa(i); return niz[i]; }
    Tip& operator=(const NizLista& rhs);
    Tip& operator=(NizLista&& rhs);
    
    friend class Iterator<Tip>;
};

template <class Tip>
void NizLista<Tip>::Prosiri() {
    Tip* temp(new Tip[2 * alociranoMjesta]);
    for(int i = 0; i < brojEl; i++)
        temp[i] = niz[i];
    delete[] niz;
    niz = temp;
    alociranoMjesta *= 2;
}

template <class Tip>
void NizLista<Tip>::TestIndeksa(int i) const {
    if(i < 0 || i >= brojEl)
        throw domain_error("Indeks van granica");
}

template <class Tip>
void NizLista<Tip>::TestPrazneListe() const {
    if(brojEl == 0)
        throw domain_error("Lista je prazna");
}

template <class Tip>
void NizLista<Tip>::obrisi() {
    TestPrazneListe();
    for(int i = trenIndeks + 1; i < brojEl; i++)
        niz[i - 1] = niz[i];
    if(trenIndeks == brojEl - 1)
        trenIndeks--;
    brojEl--;
}

template <class Tip>
void NizLista<Tip>::dodajIspred(const Tip& el) {
    if(brojEl == alociranoMjesta)
        Prosiri();
    if(brojEl == 0) 
        niz[0] = el;
    else {
        for(int i = brojEl; i > trenIndeks; i--)
            niz[i] = niz[i - 1];
        niz[trenIndeks] = el;
    }
    trenIndeks++; brojEl++;
}

template <class Tip>
void NizLista<Tip>::dodajIza(const Tip& el) {
    if(brojEl == alociranoMjesta)
        Prosiri();
    if(brojEl == 0) 
        trenIndeks++, niz[0] = el;
    else {
        for(int i = brojEl; i > trenIndeks + 1; i--)
            niz[i] = niz[i - 1];
        niz[trenIndeks + 1] = el;        
    }
    brojEl++;
}

template <class Tip>
Tip& NizLista<Tip>::operator=(const NizLista<Tip>& rhs) {
    delete[] niz; niz = nullptr;
    alociranoMjesta = rhs.alociranoMjesta;
    brojEl = rhs.brojEl;
    trenIndeks = rhs.trenIndeks;
    niz = new Tip[alociranoMjesta];
    for(int i = 0; i < brojEl; i++)
        niz[i] = rhs.niz[i];
    return this;
}

template <class Tip>
Tip& NizLista<Tip>::operator=(NizLista<Tip>&& rhs) {
    delete[] niz; niz = nullptr;
    alociranoMjesta = rhs.alociranoMjesta;
    brojEl = rhs.brojEl;
    trenIndeks = rhs.trenIndeks;
    niz = rhs.niz;
    rhs.niz = nullptr;
    return this;
}

template <class Tip>
class JednostrukaLista : public Lista<Tip> {
    struct Cvor {
        Tip element;
        Cvor* sljedeci;
        Cvor(const Tip& element, Cvor* sljedeci) :
            element(element), sljedeci(sljedeci) {};
    };
    int lduzina, dduzina;
    Cvor *pokPocetak, *pokKraj, *pokTrenutni;
    void TestPrazneListe() const;
    void TestIndeksa(int) const;
public:
    JednostrukaLista() : lduzina(0), dduzina(0), 
        pokPocetak(nullptr), pokKraj(nullptr), pokTrenutni(nullptr) {}
    JednostrukaLista(const JednostrukaLista &rhs);
    JednostrukaLista(JednostrukaLista &&rhs);
    ~JednostrukaLista();
    int brojElemenata() const { return lduzina + dduzina; }
    Tip& trenutni(); 
    Tip trenutni() const;
    bool prethodni();
    bool sljedeci();
    void pocetak();
    void kraj();
    void obrisi();
    void dodajIspred(const Tip&);
    void dodajIza(const Tip&);
    
    void izbaciSvakiNTi(int n);
    
    Tip& operator[](int);
    Tip operator[](int) const;
    Tip& operator=(const JednostrukaLista& rhs);
    Tip& operator=(JednostrukaLista&& rhs);
    
    friend class Iterator<Tip>;
};

template <class Tip>
void JednostrukaLista<Tip>::TestPrazneListe() const {
    if(lduzina + dduzina == 0)
        throw domain_error("Lista je prazna");
}

template <class Tip>
void JednostrukaLista<Tip>::TestIndeksa(int i) const {
    if(i < 0 || i >= dduzina + lduzina)
        throw domain_error("Indeks van granica");
}

template <class Tip>
JednostrukaLista<Tip>::JednostrukaLista(const JednostrukaLista<Tip> &rhs) : 
    lduzina(rhs.lduzina), dduzina(rhs.dduzina), pokPocetak(nullptr), 
    pokKraj(nullptr), pokTrenutni(nullptr) {
    Cvor* iter(rhs.pokPocetak);
    Cvor* prethodni = nullptr;
    while(iter != nullptr) {
        if(iter == rhs.pokPocetak()) {
            prethodni = new Cvor(iter->element, nullptr);
            pokPocetak = prethodni;
        } else {
            prethodni->sljedeci = new Cvor(iter->element, nullptr);
            prethodni = prethodni->sljedeci;
        }
        if(iter == rhs.pokTrenutni)
            pokTrenutni = prethodni;
        if(iter == rhs.pokKraj)
            pokKraj = prethodni;
        iter = iter->sljedeci;
    }
}

template <class Tip>
JednostrukaLista<Tip>::JednostrukaLista(JednostrukaLista<Tip> &&rhs) : 
    lduzina(rhs.lduzina), dduzina(rhs.dduzina), pokPocetak(rhs.pokPocetak), 
    pokKraj(rhs.pokKraj), pokTrenutni(rhs.pokTrenutni) { rhs.pokPocetak = nullptr; }

template <class Tip>
JednostrukaLista<Tip>::~JednostrukaLista() {
    Cvor* iter(pokPocetak);
    while(iter != nullptr) {
        Cvor* temp(iter->sljedeci);
        delete iter;
        iter = temp;
    }
}

template <class Tip>
Tip& JednostrukaLista<Tip>::trenutni() {
    TestPrazneListe();
    if(lduzina == 0)
        return pokPocetak->element;
    return pokTrenutni->sljedeci->element;
}
template <class Tip>
Tip JednostrukaLista<Tip>::trenutni() const {
    TestPrazneListe();
    if(lduzina == 0)
        return pokPocetak->element;
    return pokTrenutni->sljedeci->element;
}
template <class Tip>
bool JednostrukaLista<Tip>::prethodni() {
    TestPrazneListe();
    if(lduzina == 0)
        return false;
    if(lduzina == 1)
        pokTrenutni = nullptr;
    else {
        Cvor* iter(pokPocetak);
        while(iter->sljedeci != pokTrenutni)
            iter = iter->sljedeci;
        pokTrenutni = iter;
    }
    lduzina--; dduzina++;
    return true;
}
template <class Tip>
bool JednostrukaLista<Tip>::sljedeci() {
    TestPrazneListe();
    if(dduzina == 1)
        return false;
    if(lduzina == 0) 
        pokTrenutni = pokPocetak;
    else 
        pokTrenutni = pokTrenutni->sljedeci;
    lduzina++; dduzina--;
    return true;
}
template <class Tip>
void JednostrukaLista<Tip>::pocetak() {
    TestPrazneListe();
    pokTrenutni = nullptr;
    dduzina += lduzina;
    lduzina = 0;
}
template <class Tip>
void JednostrukaLista<Tip>::kraj() {
    TestPrazneListe();
    pokTrenutni = pokKraj;
    lduzina += dduzina - 1;
    dduzina = 1;
}
template <class Tip>
void JednostrukaLista<Tip>::obrisi() {
    TestPrazneListe();
    if(lduzina == 0) {
        Cvor* temp(pokPocetak->sljedeci);
        delete pokPocetak;
        pokPocetak = temp;
    } else {
        Cvor* temp(pokTrenutni->sljedeci->sljedeci);
        //cout << "vr:" << (temp == nullptr) << endl;
        delete pokTrenutni->sljedeci;
        pokTrenutni->sljedeci = temp;
    }
    dduzina--;
    if(dduzina == 0) {
        if(lduzina == 0)
            pokPocetak = pokKraj = pokTrenutni = nullptr;
        else {
            prethodni();
            pokKraj = pokTrenutni;
        }
    }
}

template <class Tip>
void JednostrukaLista<Tip>::dodajIspred(const Tip& el) {
        Cvor* p = new Cvor(el, nullptr);
        if(lduzina == 0) {
            p->sljedeci = pokPocetak;
            pokPocetak = p;
            if(dduzina != 0)
                pokTrenutni = pokPocetak;
            if(dduzina == 1)
                pokKraj = pokTrenutni;
        } else {
            p->sljedeci = pokTrenutni->sljedeci;
            pokTrenutni->sljedeci = p;
            pokTrenutni = p;
            if(dduzina == 1)
                pokKraj = p;
        }
        if(brojElemenata() == 0)
            dduzina++;
        else lduzina++;
}
template <class Tip>
void JednostrukaLista<Tip>::dodajIza(const Tip& el) {
        if(brojElemenata() == 0) {
            dodajIspred(el);
            return;
        }
        Cvor* p = new Cvor(el, nullptr);
        if(lduzina == 0) {
            p->sljedeci = pokPocetak->sljedeci;
            //cout << "p->sljedeci je null: " << (p->sljedeci == nullptr) << endl;
            pokPocetak->sljedeci = p;
            if(dduzina == 1)
                pokKraj = pokPocetak;
        } else {
            p->sljedeci = pokTrenutni->sljedeci->sljedeci;
            pokTrenutni->sljedeci->sljedeci = p;
            if(dduzina == 1)
                pokKraj = pokTrenutni->sljedeci;
            if(dduzina == 2)
                pokKraj = p;
        }
        dduzina++; 
}

template <class Tip>
void JednostrukaLista<Tip>::izbaciSvakiNTi(int n) {
    if(n < 1 || n > brojElemenata())
        throw domain_error("Ne moze se izbaciti nijedan element");
    Cvor* iter = pokPocetak;
    int indeks = n - 1;
    while(indeks < brojElemenata()) {
        for(int i = 0; i < indeks - 1; i++)
            iter = iter->sljedeci;
        Cvor* temp = nullptr;
        if(indeks == 0) {
            temp = pokPocetak;
            pokPocetak = pokPocetak->sljedeci;
        } else {
            temp = iter->sljedeci;
            iter->sljedeci = temp->sljedeci;
        }
        if(temp == pokKraj) {
            if(indeks == 0)
                pokKraj = nullptr;
            else
                pokKraj = iter;
        } else if(temp == pokKraj->sljedeci) {
            
        }
    }
}

template <class Tip> // optimizovati
Tip& JednostrukaLista<Tip>::operator[](int i) {
    TestIndeksa(i);
    if(i == 0)
        return pokPocetak->element;
    Cvor* iter(pokPocetak);
    for(int j = 0; j < i - 1; j++)
        iter = iter->sljedeci;
    return iter->sljedeci->element;
}
template <class Tip>
Tip  JednostrukaLista<Tip>::operator[](int i) const {
    TestIndeksa(i);
    if(i == 0)
        return pokPocetak->element;
    Cvor* iter(pokPocetak);
    for(int j = 0; j < i - 1; j++)
        iter = iter->sljedeci;
    return iter->sljedeci->element;
}

template <class Tip>
Tip& JednostrukaLista<Tip>::operator=(const JednostrukaLista& rhs) {
    Cvor* iter(pokPocetak);
    while(iter != nullptr) {
        Cvor* temp(iter->sljedeci);
        delete iter;
        iter = temp;
    }
    lduzina = rhs.lduzina; dduzina = rhs.dduzina; 
    pokPocetak = nullptr;  
    pokKraj = nullptr; 
    pokTrenutni = nullptr;
    iter = rhs.pokPocetak;
    Cvor* prethodni = nullptr;
    while(iter != nullptr) {
        if(iter == rhs.pokPocetak()) {
            prethodni = new Cvor(iter->element, nullptr);
            pokPocetak = prethodni;
        } else {
            prethodni->sljedeci = new Cvor(iter->element, nullptr);
            prethodni = prethodni->sljedeci;
        }
        if(iter == rhs.pokTrenutni)
            pokTrenutni = prethodni;
        if(iter == rhs.pokKraj)
            pokKraj = prethodni;
        iter = iter->sljedeci;
    }
    return *this;
}
template <class Tip>
Tip& JednostrukaLista<Tip>::operator=(JednostrukaLista&& rhs) {
    Cvor* iter(pokPocetak);
    while(iter != nullptr) {
        Cvor* temp(iter->sljedeci);
        delete iter;
        iter = temp;
    }
    lduzina = rhs.lduzina; dduzina = rhs.dduzina; 
    pokPocetak = rhs.pokPocetak;  
    pokKraj = rhs.pokKraj; 
    pokTrenutni = rhs.pokTrenutni;
    rhs.pokPocetak = nullptr;
    return *this;
}

template <class Tip>
class Iterator {
    const NizLista<Tip> *niz;
    const JednostrukaLista<Tip> *lista;
    int trenutniNiz;
    typename JednostrukaLista<Tip>::Cvor *trenutniLista;
public:
    Iterator(const NizLista<Tip> niz) : 
        niz(niz), lista(nullptr), trenutniNiz(0), trenutniLista(nullptr) {}
    Iterator(const JednostrukaLista<Tip> lista) : 
        niz(nullptr), lista(lista), trenutniNiz(0), trenutniLista(lista.pokPocetak) {}
    Tip trenutni();
    bool prethodni();
    bool sljedeci();
    void pocetak();
    void kraj();
};

template <class Tip>
Tip Iterator<Tip>::trenutni() {
    if(niz == nullptr) {
        lista->TestPrazneListe();
        return trenutniLista->element;
        
    } else {
        niz->TestPrazneListe();
        return (*niz)[trenutniNiz];
    }
}
template <class Tip>
bool Iterator<Tip>::prethodni() {
    if(niz == nullptr) {
        lista->TestPrazneListe();
        if(lista->pokPocetak == trenutniLista)
            return false;
       else {
        typename JednostrukaLista<Tip>::Cvor* iter = lista->pokPocetak;
        while(iter->sljedeci != trenutniLista)
            iter = iter->sljedeci;
        trenutniLista = iter;
        return true;
    } 
    } else {
        niz->TestPrazneListe();
        return (trenutniNiz == 0) ? false : (trenutniNiz-- > 0);
    }
}
template <class Tip>
bool Iterator<Tip>::sljedeci() {
    if(niz == nullptr) {
        lista->TestPrazneListe();
        if(trenutniLista->sljedeci == nullptr)
            return false;
        else {
            trenutniLista = trenutniLista->sljedeci;
            return true;
        } 
    } else {
        niz->TestPrazneListe();
        return (trenutniNiz == niz->brojElemenata - 1) ? false : (trenutniNiz++ > 0);
    }
}
template <class Tip>
void Iterator<Tip>::pocetak() {
    if(niz == nullptr) {
        lista->TestPrazneListe();
        trenutniLista = lista->pokPocetak;
    } else {
        niz->TestPrazneListe();
        trenutniNiz = 0;
    }
}
template <class Tip>
void Iterator<Tip>::kraj() {
    if(niz == nullptr) {
        lista->TestPrazneListe();
        if(lista->brojElemenata() == 1)
            trenutniLista = lista->pokPocetak;
        else
            trenutniLista = lista->pokKraj->sljedeci();
    } else {
        niz->TestPrazneListe();
        trenutniNiz = niz->brojElemenata() - 1;
    }
}

template<typename Tip>
Tip dajMaksimum(const NizLista<Tip>& n) {
    auto iter(n);
    Tip maximum(iter->trenutni());
    while(iter->sljedeci())
        maximum = max(iter->trenutni(), maximum);
    return maximum;
}
template<typename Tip>
Tip dajMaksimum(const JednostrukaLista<Tip>& n) {
    auto iter(n);
    Tip maximum(iter->trenutni());
    while(iter->sljedeci())
        maximum = max(iter->trenutni(), maximum);
    return maximum;
}

int main() {
    
}