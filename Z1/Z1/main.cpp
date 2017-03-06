#include <iostream>
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

template <class Tip>
class DvostrukaLista : public Lista<Tip> {
	struct Cvor {
		Tip element;
		Cvor* prethodni;
		Cvor* sljedeci;
		Cvor(const Tip& element, Cvor* prethodni, Cvor* sljedeci = nullptr) :
			element(element), prethodni(prethodni), sljedeci(sljedeci) {};
	};
	int lduzina, dduzina;
	Cvor *pokPocetak, *pokKraj, *pokTrenutni;
	void TestPrazneListe() const { 
		if(lduzina + dduzina == 0)
			throw "Lista je prazna"; 
	}
	void TestIndeksa(int i) const {
		if(i < 0 || i >= lduzina + dduzina)
			throw "Indeks van raspona";
	}
	void Obrisi();
	void Kopiraj(const DvostrukaLista&);
public:
	DvostrukaLista() : lduzina(0), dduzina(0),
		pokPocetak(nullptr), pokKraj(nullptr), pokTrenutni(nullptr) {}
	DvostrukaLista(const DvostrukaLista &rhs) : lduzina(0), dduzina(0),
		pokPocetak(nullptr), pokKraj(nullptr), pokTrenutni(nullptr) { Kopiraj(rhs); }
	DvostrukaLista(DvostrukaLista &&rhs);
	~DvostrukaLista() { Obrisi(); }
	int brojElemenata() const { return lduzina + dduzina; }
	Tip& trenutni() {
		TestPrazneListe();
		return pokTrenutni->element;
	}
	Tip trenutni() const {
		TestPrazneListe();
		return pokTrenutni->element;
	}
	bool prethodni();
	bool sljedeci();
	void pocetak();
	void kraj();
	void obrisi();
	void dodajIspred(const Tip&);
	void dodajIza(const Tip&);
	Tip& operator[](int);
	Tip operator[](int) const;
	DvostrukaLista& operator=(const DvostrukaLista& rhs) { 
		if(this == &rhs)
			return *this;
		Kopiraj(rhs); 
		return *this;
	}
	DvostrukaLista& operator=(DvostrukaLista&& rhs);

	friend class Iterator<Tip>;
};

template <typename Tip>
void DvostrukaLista<Tip>::Obrisi() {
	while(pokPocetak != nullptr) {
		Cvor* temp(pokPocetak->sljedeci);
		delete pokPocetak;
		pokPocetak = temp;
	}
	pokKraj = pokTrenutni = nullptr;
	lduzina = dduzina = 0;
}

template <typename Tip>
void DvostrukaLista<Tip>::Kopiraj(const DvostrukaLista<Tip>& rhs) {
	Obrisi();
	Cvor* iter(rhs.pokPocetak), *prethodni(nullptr);
	while(iter != nullptr) {
		if(prethodni == nullptr) 
			prethodni = pokKraj = pokPocetak = new Cvor(iter->element, nullptr);
		else {
			prethodni->sljedeci = new Cvor(iter->element, prethodni);
			pokKraj = prethodni = prethodni->sljedeci;
		}
		if(rhs.pokTrenutni == iter)
			pokTrenutni = prethodni;
		iter = iter->sljedeci;
	}
	lduzina = rhs.lduzina; dduzina = rhs.dduzina;
}

template <typename Tip>
DvostrukaLista<Tip>::DvostrukaLista(DvostrukaLista<Tip>&& rhs): lduzina(0), dduzina(0),
		pokPocetak(rhs.pokPocetak), pokKraj(rhs.pokKraj), pokTrenutni(rhs.pokTrenutni) {
	rhs.pokPocetak = nullptr;
}  

template <typename Tip>
bool DvostrukaLista<Tip>::prethodni() {
	if(lduzina > 0) {
		pokTrenutni = pokTrenutni->prethodni;
		lduzina--; dduzina++;
		return true;
	}
	return false;
}

template <typename Tip>
bool DvostrukaLista<Tip>::sljedeci() {
	if(dduzina > 1) {
		pokTrenutni = pokTrenutni->sljedeci;
		lduzina++; dduzina--;
		return true;
	}
	return false;
}

template <typename Tip>
void DvostrukaLista<Tip>::pocetak() {
	TestPrazneListe();
	pokTrenutni = pokPocetak;
	dduzina += lduzina;
	lduzina = 0;
}
template <typename Tip>
void DvostrukaLista<Tip>::kraj() {
	TestPrazneListe();
	pokTrenutni = pokKraj;
	lduzina += dduzina - 1;
	dduzina = 1;
}

template <typename Tip>
void DvostrukaLista<Tip>::obrisi() {
	TestPrazneListe();
	if(pokPocetak == pokTrenutni && pokKraj == pokTrenutni) {
		delete pokTrenutni;
		pokPocetak = pokKraj = pokTrenutni = nullptr;
		lduzina = dduzina = 0;
	}
	else if(pokPocetak == pokTrenutni) {
		Cvor* temp = pokTrenutni->sljedeci;
		delete pokTrenutni;
		pokPocetak = pokTrenutni = temp;
		pokTrenutni->prethodni = nullptr;
		dduzina--;
	} else if(pokKraj == pokTrenutni) {
		Cvor* temp = pokTrenutni->prethodni;
		delete pokTrenutni;
		pokKraj = pokTrenutni = temp;
		pokTrenutni->sljedeci = nullptr;
		lduzina--;
	} else {
		Cvor* temp = pokTrenutni->sljedeci;
		pokTrenutni->prethodni->sljedeci = pokTrenutni->sljedeci;
		pokTrenutni->sljedeci->prethodni = pokTrenutni->prethodni;
		delete pokTrenutni;
		pokTrenutni = temp;
		dduzina--;
	}
}

template <typename Tip>
void DvostrukaLista<Tip>::dodajIza(const Tip& el) {
	if(pokTrenutni == nullptr)
		pokPocetak = pokTrenutni = pokKraj = new Cvor(el, nullptr);
	else if(pokTrenutni == pokKraj)
		pokKraj = pokTrenutni->sljedeci = new Cvor(el, pokTrenutni);
	else {
		Cvor* temp = new Cvor(el, pokTrenutni, pokTrenutni->sljedeci);
		pokTrenutni->sljedeci->prethodni = temp;
		pokTrenutni->sljedeci = temp;
	}
	dduzina++;
}

template <typename Tip>
void DvostrukaLista<Tip>::dodajIspred(const Tip& el) {
	if(pokTrenutni == nullptr) {
		dodajIza(el); 
		return;
	}
	else if(pokTrenutni == pokPocetak)
		pokPocetak = pokTrenutni->prethodni = new Cvor(el, nullptr, pokTrenutni);
	else {
		Cvor* temp = new Cvor(el, pokTrenutni->prethodni, pokTrenutni);
		pokTrenutni->prethodni->sljedeci = temp;
		pokTrenutni->prethodni = temp;
	}
	lduzina++;	
}

template <typename Tip>
Tip& DvostrukaLista<Tip>::operator[](int i) {
	TestPrazneListe();
	TestIndeksa(i);
	Cvor* iter(pokPocetak);
	for(int j = 0; j < i; j++)
		iter = iter->sljedeci;
	return iter->element;
}

template <typename Tip>
Tip DvostrukaLista<Tip>::operator[](int i) const {
	TestPrazneListe();
	TestIndeksa(i);
	Cvor* iter(pokPocetak);
	for(int j = 0; j < i; j++)
		iter = iter->sljedeci;
	return iter->element;
}

template <typename Tip>
DvostrukaLista<Tip>& DvostrukaLista<Tip>::operator=(DvostrukaLista&& rhs) {
	if(this == &rhs)
		return *this;
	Obrisi();
	lduzina = rhs.lduzina;
	dduzina = rhs.dduzina;
	pokPocetak = rhs.pokPocetak;
	pokKraj = rhs.pokKraj;
	pokTrenutni = rhs.pokTrenutni;
	rhs.pokPocetak = nullptr;
	return *this;
}

template <class Tip>
class Iterator {
    const DvostrukaLista<Tip> *lista;
    typename DvostrukaLista<Tip>::Cvor *trenutniPok;
public:
    Iterator(const DvostrukaLista<Tip>& lista) : lista(&lista), trenutniPok(lista.pokPocetak) {}
    Tip trenutni();
    bool prethodni();
    bool sljedeci();
    void pocetak();
    void kraj();
};

template <class Tip>
Tip Iterator<Tip>::trenutni() {
    lista->TestPrazneListe();
    return trenutniPok->element;
}

template <class Tip>
bool Iterator<Tip>::prethodni() {
    lista->TestPrazneListe();
    if(lista->pokPocetak == trenutniPok)
        return false;
    else { 
		trenutniPok = trenutniPok->prethodni;
        return true;
    } 
}

template <class Tip>
bool Iterator<Tip>::sljedeci() {
    lista->TestPrazneListe();
    if(trenutniPok->sljedeci == nullptr)
        return false;
    else {
        trenutniPok = trenutniPok->sljedeci;
        return true;
    } 
}

template <class Tip>
void Iterator<Tip>::pocetak() {
    lista->TestPrazneListe();
    trenutniPok = lista->pokPocetak;
}

template <class Tip>
void Iterator<Tip>::kraj() {
    lista->TestPrazneListe();
    trenutniPok = lista->pokKraj;
}

template<typename Tip>
Tip dajMaksimum(const Lista<Tip>& n) {
    if(n.brojElemenata() == 0)
    	return Tip();
    Iterator<Tip> iter(dynamic_cast<const DvostrukaLista<Tip>&>(n));
    Tip maximum(iter.trenutni());
    while(iter.sljedeci())
        maximum = max(iter.trenutni(), maximum);
    return maximum;
}

int main() {

Lista<int>* l;
l = new DvostrukaLista<int>;
for (int i(0); i<100000; i++)
    l->dodajIspred(i);
auto iter(dynamic_cast<const DvostrukaLista<int>&>(*l));


std::cout << iter.trenutni();
delete l;

    return 0;
}