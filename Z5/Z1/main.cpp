#include <iostream>
#include <limits>
#include <vector>
#include <queue>
#include <list>
#include <string>
#include <functional>


using namespace std;

//1

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
class HashMapaLan : public Mapa<K, V> {
	static const int INICIJALNA_VELICINA = 6000;
	int brEl;
	vector<list<pair<K, V>>> mapa;
	function<unsigned int(const K&, unsigned int)> h;
	typename list<pair<K, V>>::iterator Trazi(const K& kljuc);
	typename list<pair<K, V>>::const_iterator Trazi(const K& kljuc) const;
	int Velicina() const { return mapa.size(); }
public:
	HashMapaLan() : brEl(0), mapa(INICIJALNA_VELICINA, list<pair<K, V>>()) {}
	V& operator[](const K& kljuc);
	V operator[](const K& kljuc) const;
	int brojElemenata() const { return brEl; }
	void obrisi(const K& kljuc);
	void obrisi() {
		for(auto& l : mapa)
			l.clear(); 
		brEl = 0;
	}
	void definisiHashFunkciju(function<unsigned int(const K&, unsigned int)> h) { this->h = h; }

};

template <typename K, typename V>
typename list<pair<K, V>>::iterator HashMapaLan<K, V>::Trazi(const K& kljuc) {
	if (!h)
		throw "Hash funkcija nije definisana";
	int hes(h(kljuc, Velicina()));
	auto it(mapa[hes].begin());
	while (it != mapa[hes].end() && kljuc > it->first)
		it++;
	return it;
}

template <typename K, typename V>
typename list<pair<K, V>>::const_iterator HashMapaLan<K, V>::Trazi(const K& kljuc) const {
	if (!h)
		throw "Hash funkcija nije definisana";
	int hes(h(kljuc, Velicina()));
	auto it(mapa[hes].begin());
	while (it != mapa[hes].end() && kljuc > it->first)
		it++;
	return it;
}

template <typename K, typename V>
V& HashMapaLan<K, V>::operator[](const K& kljuc) {
	auto it(Trazi(kljuc));
	int hes(h(kljuc, Velicina()));
	if (it == mapa[hes].end() || it->first != kljuc) {
		it = mapa[hes].insert(it, pair<K, V>(kljuc, V()));
		brEl++;
	}
	return it->second;
}

template <typename K, typename V>
V HashMapaLan<K, V>::operator[](const K& kljuc) const {
	auto it(Trazi(kljuc));
	int hes(h(kljuc, Velicina()));
	if (it == mapa[hes].end() || it->first != kljuc)
		return V();
	return it->second;
}

template <typename K, typename V>
void HashMapaLan<K, V>::obrisi(const K& kljuc) {
	auto it(Trazi(kljuc));
	int hes(h(kljuc, Velicina()));
	if (it == mapa[hes].end() || it->first != kljuc)
		throw "Kljuc nije pronadjen";
	auto itp(it++);
	mapa[hes].erase(itp, it);
	brEl--;

}

//1.b

template <typename T>
class Grana;
template <typename T>
class Cvor;
template <typename T>
class GranaIterator;

template <typename T>
class UsmjereniGraf {
protected: 
	UsmjereniGraf() = default;
public:
	UsmjereniGraf(int) {};
	virtual ~UsmjereniGraf() {};

	virtual int dajBrojCvorova() const = 0;
	virtual void postaviBrojCvorova(int) = 0;

	virtual void dodajGranu(int, int, float = 0) = 0;
	virtual void obrisiGranu(int, int) = 0;
	virtual void postaviTezinuGrane(int, int, float = 0) = 0;
	virtual float dajTezinuGrane(int, int) const = 0;
	virtual bool postojiGrana(int, int) const = 0;

	virtual void postaviOznakuCvora(int, T) = 0;
	virtual T dajOznakuCvora(int) const = 0;
	virtual void postaviOznakuGrane(int, int, T) = 0;
	virtual T dajOznakuGrane(int, int) const = 0;

	virtual Grana<T> dajGranu(int, int) = 0;
	virtual const Grana<T> dajGranu(int, int) const = 0;
	virtual Cvor<T> dajCvor(int) = 0;
	virtual const Cvor<T> dajCvor(int) const = 0;

	GranaIterator<T> dajGranePocetak() {
		return ++GranaIterator<T>(this, 0, -1);
	}
	const GranaIterator<T> dajGranePocetak() const {
		return ++GranaIterator<T>(const_cast<UsmjereniGraf*>(this), 0, -1); 
	}																		
																				
	GranaIterator<T> dajGraneKraj() {										
		return GranaIterator<T>(this, dajBrojCvorova(), 0);
	}
	const GranaIterator<T> dajGraneKraj() const {
		return GranaIterator<T>(const_cast<UsmjereniGraf*>(this), dajBrojCvorova(), 0);
	}
};

template <typename T>
class Grana {
	UsmjereniGraf<T>* graf;
	int polazni, dolazni;
public:
	Grana(UsmjereniGraf<T>* graf, int polazni, int dolazni) : graf(graf),
		polazni(polazni), dolazni(dolazni) {}
	float dajTezinu() const { return graf->dajTezinuGrane(polazni, dolazni); }
	void postaviTezinu(float tezina) { graf->postaviTezinuGrane(polazni, dolazni, tezina); }
	T dajOznaku() const { return graf->dajOznakuGrane(polazni, dolazni); }
	void postaviOznaku(T oznaka) { graf->postaviOznakuGrane(polazni, dolazni, oznaka); }
	Cvor<T> dajPolazniCvor() { return graf->dajCvor(polazni); }
	const Cvor<T> dajPolazniCvor() const { return graf->dajCvor(polazni); }
	Cvor<T> dajDolazniCvor() { return graf->dajCvor(dolazni); }
	const Cvor<T> dajDolazniCvor() const { return graf->dajCvor(dolazni); }
};

template <typename T>
class Cvor {
	UsmjereniGraf<T>* graf;
	int rbroj;
public:
	Cvor(UsmjereniGraf<T>* graf, int rbroj) : graf(graf),
		rbroj(rbroj) {}
	T dajOznaku() const { return graf->dajOznakuCvora(rbroj); }
	void postaviOznaku(T oznaka) { graf->postaviOznakuCvora(rbroj, oznaka); }
	int dajRedniBroj() const { return rbroj; }
};

template <typename T>
class GranaIterator {
	UsmjereniGraf<T>* graf;
	int i, j;
	void inkrementuj(int &i, int &j) {
		j + 1 >= graf->dajBrojCvorova() ? j = 0, i++ : j++;
	}
public:
	GranaIterator(UsmjereniGraf<T>* graf, int i, int j) :
		graf(graf), i(i), j(j) {}
	Grana<T> operator*() { return graf->dajGranu(i, j); }
	const Grana<T> operator*() const { return graf->dajGranu(i, j); }
	bool operator==(const GranaIterator &iter) const { return i == iter.i && j == iter.j && graf == iter.graf; }
	bool operator!=(const GranaIterator &iter) const { return !(*this == iter); }
	GranaIterator& operator++();
	GranaIterator operator++(int);
};

template <typename T>
GranaIterator<T>& GranaIterator<T>::operator++() {
	do
		inkrementuj(i, j);
	while (i < graf->dajBrojCvorova() && !graf->postojiGrana(i, j));
	return *this;
}

template <typename T>
GranaIterator<T> GranaIterator<T>::operator++(int) {
	auto kopija(*this);
	++(*this);
	return kopija;
}

template <typename T>
class ListaGraf : public UsmjereniGraf<T> {
	struct Element {
		int cvor;
		float tezina;
		T oznaka;
		Element(int cvor, float tezina = 0, T oznaka = T()) :
			cvor(cvor), tezina(tezina), oznaka(oznaka) {}
	};

	vector<list<Element>> listaSusjedstva;
	vector<T> oznakeCvorova;

	void validirajCvor(int) const;
	void validirajGranu(int, int) const;
	void validirajPostojanjeGrane(int, int) const;

	typename list<Element>::iterator pronadjiElement(int i, int j);
	typename list<Element>::const_iterator pronadjiElement(int i, int j) const;
public:
	void ispisi() {
		for (int i = 0; i < listaSusjedstva.size(); i++) {
			cout << i << ": ";
			for (Element e : listaSusjedstva[i])
				cout << e.cvor << " ";
			cout << endl;
		}
	}
public:
	ListaGraf(int brojCvorova);

	int dajBrojCvorova() const { return listaSusjedstva.size(); }
	void postaviBrojCvorova(int);

	void dodajGranu(int, int, float = 0);
	void obrisiGranu(int, int);
	void postaviTezinuGrane(int, int, float = 0);
	float dajTezinuGrane(int, int) const;
	bool postojiGrana(int, int) const;

	void postaviOznakuCvora(int, T);
	T dajOznakuCvora(int) const;
	void postaviOznakuGrane(int, int, T);
	T dajOznakuGrane(int, int) const;

	Grana<T> dajGranu(int, int);
	const Grana<T> dajGranu(int, int) const;
	Cvor<T> dajCvor(int);
	const Cvor<T> dajCvor(int) const;
};


template <typename T>
typename list<typename ListaGraf<T>::Element>::iterator ListaGraf<T>::pronadjiElement(int i, int j) {
	typename list<Element>::iterator it;
	for (it = listaSusjedstva[i].begin(); it !=
		listaSusjedstva[i].end(); it++) {
		if (j == it->cvor)
			return it;
		else if (j < it->cvor)
			return listaSusjedstva[i].end();
	}
	return it;
}

template <typename T>
typename list<typename ListaGraf<T>::Element>::const_iterator ListaGraf<T>::pronadjiElement(int i, int j) const {
	typename list<Element>::const_iterator it;
	for (it = listaSusjedstva[i].begin(); it !=
		listaSusjedstva[i].end(); it++) {
		if (j == it->cvor)
			return it;
		else if (j < it->cvor)
			return listaSusjedstva[i].end();
	}
	return it;
}

template <typename T>
void ListaGraf<T>::validirajCvor(int indeks) const {
	if (indeks < 0 || indeks >= dajBrojCvorova())
		throw "Neispravan broj cvora";
}

template <typename T>
void ListaGraf<T>::validirajGranu(int i, int j) const {
	validirajCvor(i);
	validirajCvor(j);
}

template <typename T>
void ListaGraf<T>::validirajPostojanjeGrane(int i, int j) const {
	validirajGranu(i, j);
	if (!postojiGrana(i, j))
		throw "Grana ne postoji";
}

template <typename T>
ListaGraf<T>::ListaGraf(int brojCvorova) {
	if (brojCvorova < 0)
		throw "Broj cvorova ne moze biti negativan";
	listaSusjedstva = vector<list<Element>>(brojCvorova, list<Element>());
	oznakeCvorova = vector<T>(brojCvorova);
}

template <typename T>
void ListaGraf<T>::postaviBrojCvorova(int noviBrojCvorova) {
	if (noviBrojCvorova < dajBrojCvorova())
		throw "Novi broj cvorova ne moze biti manji od trenutnog";
	listaSusjedstva.resize(noviBrojCvorova);
	oznakeCvorova.resize(noviBrojCvorova);
}

template <typename T>
void ListaGraf<T>::dodajGranu(int i, int j, float w) {
	validirajGranu(i, j);
	typename list<Element>::iterator it;
	for (it = listaSusjedstva[i].begin();
		it != listaSusjedstva[i].end() && j > it->cvor;
		it++);
	listaSusjedstva[i].insert(it, Element(j, w));
}

template <typename T>
void ListaGraf<T>::obrisiGranu(int i, int j) {
	validirajPostojanjeGrane(i, j);
	listaSusjedstva[i].erase(pronadjiElement(i, j));
}

template <typename T>
void ListaGraf<T>::postaviTezinuGrane(int i, int j, float w) {
	validirajPostojanjeGrane(i, j);
	pronadjiElement(i, j)->tezina = w;
}

template <typename T>
float ListaGraf<T>::dajTezinuGrane(int i, int j) const {
	validirajPostojanjeGrane(i, j);
	return pronadjiElement(i, j)->tezina;
}

template <typename T>
bool ListaGraf<T>::postojiGrana(int i, int j) const {
	validirajGranu(i, j);
	return pronadjiElement(i, j) != listaSusjedstva[i].end();
}

template <typename T>
void ListaGraf<T>::postaviOznakuCvora(int i, T oznaka) {
	validirajCvor(i);
	oznakeCvorova[i] = oznaka;
}

template <typename T>
T ListaGraf<T>::dajOznakuCvora(int i) const {
	validirajCvor(i);
	return oznakeCvorova[i];
}

template <typename T>
void ListaGraf<T>::postaviOznakuGrane(int i, int j, T oznaka) {
	validirajPostojanjeGrane(i, j);
	pronadjiElement(i, j)->oznaka = oznaka;

}

template <typename T>
T ListaGraf<T>::dajOznakuGrane(int i, int j) const {
	validirajPostojanjeGrane(i, j);
	return pronadjiElement(i, j)->oznaka;
}

template <typename T>
Grana<T> ListaGraf<T>::dajGranu(int i, int j) {
	validirajPostojanjeGrane(i, j);
	return Grana<T>(this, i, j);
}

template <typename T>
const Grana<T> ListaGraf<T>::dajGranu(int i, int j) const {
	validirajPostojanjeGrane(i, j);
	return Grana<T>(const_cast<ListaGraf*>(this), i, j);
}

template <typename T>
Cvor<T> ListaGraf<T>::dajCvor(int i) {
	validirajCvor(i);
	return Cvor<T>(this, i);
}

template <typename T>
const Cvor<T> ListaGraf<T>::dajCvor(int i) const {
	validirajCvor(i);
	return Cvor<T>(const_cast<ListaGraf*>(this), i);
}


template <typename TipOznake>
void dfs(UsmjereniGraf<TipOznake> *graf, vector<Cvor<TipOznake> > &dfs_obilazak,
	Cvor<TipOznake> cvor) {
	cvor.postaviOznaku(1);
	dfs_obilazak.push_back(cvor);
	for (GranaIterator<TipOznake> it = graf->dajGranePocetak(); it != graf->dajGraneKraj(); ++it) {
		Cvor<TipOznake> susjedni = (*it).dajDolazniCvor();
		if ((*it).dajPolazniCvor().dajRedniBroj() == cvor.dajRedniBroj() &&
			susjedni.dajOznaku() != 1)
			dfs(graf, dfs_obilazak, susjedni);
	}
}

template <typename TipOznake>
void bfs(UsmjereniGraf<TipOznake> *graf, vector<Cvor<TipOznake> > &bfs_obilazak,
	Cvor<TipOznake> pocetni_cvor) {
	pocetni_cvor.postaviOznaku(1);
	bfs_obilazak.push_back(pocetni_cvor);
	queue<Cvor<TipOznake>> red;
	red.push(pocetni_cvor);
	while (!red.empty()) {
		Cvor<TipOznake> c = red.front();
		red.pop();
		for (GranaIterator<TipOznake> it = graf->dajGranePocetak(); it != graf->dajGraneKraj(); ++it) {
			Cvor<TipOznake> susjedni = (*it).dajDolazniCvor();
			if ((*it).dajPolazniCvor().dajRedniBroj() == c.dajRedniBroj() &&
				susjedni.dajOznaku() != 1) {
				susjedni.postaviOznaku(1);
				bfs_obilazak.push_back(susjedni);
				red.push(susjedni);
			}
		}

	}

}


int main() {

UsmjereniGraf<bool> *g = new ListaGraf<bool>(6);
  g->dodajGranu(0, 1, 2.5);
  g->dodajGranu(1, 2, 1.2);
  g->dodajGranu(1, 3, 0.1);
  g->dodajGranu(3, 3, -4.0);
  g->dodajGranu(2, 4, 3.14);
  g->dodajGranu(2, 3, 2.73);
  g->dodajGranu(3, 5, 1);
  g->dodajGranu(5, 2, -5.0);
  for (int i = 0; i < 6; i++)
    for (int j = 0; j < 6; j++)
      cout << g->postojiGrana(i, j);
  delete g;
}