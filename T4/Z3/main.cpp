#include <iostream>
#include <list>

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
void funkcija(Red<Tip>& r, DvostrukaLista<Tip> l, int n) {
	int red = 1;
	int pocetnaVelicina(r.brojElemenata());
	while(pocetnaVelicina && l.brojElemenata()) {
		if(red) {
			int limit = min(n, pocetnaVelicina);
			for(int i = 0; i < limit; i++, pocetnaVelicina--)
				r.stavi(r.skini());
		} else {
			int limit = min(n, l.brojElemenata());
			for(int i = 0; i < limit; i++) {
				r.stavi(l.trenutni());
				l.obrisi();
			}
		}
		red = 1 - red;
	}
	while(pocetnaVelicina)
		r.stavi(r.skini());
	while(l.brojElemenata()) {
		r.stavi(l.trenutni());
		l.obrisi();
	}
}

int main() {
    DvostrukaLista<int> l;
    Red<int> r;
    for(int i = 0; i < 10; i++)
    	l.dodajIspred(i), r.stavi(i + 10);
    funkcija(r, l, 2);
    while(r.brojElemenata())
    	cout << r.skini() << " ";
    return 0;
}

/*
Napisati funkciju koja prima red, listu i varijablu n tipa int i modifikuje red tako da elementi iz
liste i reda budu naizmjenično poredani gdje je svaki isprepleteni dio reda i liste veličine n a počinje
se sa elementima reda. U slučaju razlike u dužini reda i liste, višak elemenata se postavlja na kraj
reda. Primjer 1: Neka je dat red sa elementima {1, 2, 3, 4} gdje je element 1 na čelu reda i lista sa
elementima {1, 2, 3, 4} i neka je n=1. Rezultantni red je {1, 1, 2, 2, 3, 3, 4, 4}. Primjer 2: Neka je
dat red sa elementima {A, A, A, A, A, A, A} gdje je element A na čelu reda i lista sa elementima
{B, B, B, B} i neka je n=2. Rezultantni red je {A, A, B, B, A, A, B, B, A, A, A}.
*/