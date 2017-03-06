#include <iostream>
#include <vector>
#include <string>
#include <functional>

using namespace std;

template <typename K, typename V>
class Pair {
public:
    K first;
    V second;
    bool DEL;
    Pair() : DEL(false) {}
    Pair(const K& first, const V& second, bool DEL = false) : first(first), second(second), DEL(DEL) {}
};

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
class HashMapa : public Mapa<K, V> {
	static const int INICIJALNA_VELICINA = 6000000;
	int brEl, prostor;
	Pair<K, V> **niz, **kraj;
	//unsigned int (*h)(const K&, unsigned int);
	function<unsigned int(const K&, unsigned int)> h;
	unsigned int HesLin(const K& kljuc, unsigned int i) const { return (h(kljuc, prostor) + i) % prostor; }
	int UmetniHash(const K& k);
	bool ObrisiHash(const K& k);
	int TraziHash(const K& k) const;
	void Prosiri();
public:
	HashMapa() : brEl(0), prostor(INICIJALNA_VELICINA), niz(new Pair<K, V>*[prostor]{}), kraj(niz + prostor) {}
	HashMapa(const HashMapa& rhs);
	HashMapa(HashMapa&& rhs);
	~HashMapa();
	V& operator[](const K& kljuc);
	V operator[](const K& kljuc) const;
	HashMapa& operator=(const HashMapa& rhs);
	HashMapa& operator=(HashMapa&& rhs);
	int brojElemenata() const { return brEl; }
	void obrisi(const K& kljuc);
	void obrisi();
	//void definisiHashFunkciju(unsigned int (*h)(const K&, unsigned int)) { this.h = h; }
	void definisiHashFunkciju(function<unsigned int(const K&, unsigned int)> h) { this->h = h; }

};

template <typename K, typename V>
int HashMapa<K, V>::UmetniHash(const K& k) {
	int j, i = 0;
	do {
		j = HesLin(k, i);
		if (niz[j] == nullptr || niz[j]->DEL) {
		    if(niz[j] != nullptr)
		        delete niz[j];
			niz[j] = new Pair<K, V>(k, V());
			brEl++;
			return j;
		}
		else
			i = i + 1;
	} while (i < prostor);
	return -1;
}

template <typename K, typename V>
bool HashMapa<K, V>::ObrisiHash(const K& k) {
	int indeks(TraziHash(k));
	if (indeks != -1) {
		brEl--;
		niz[indeks]->DEL = true;
		return true;
	}
	return false;
}

template <typename K, typename V>
int HashMapa<K, V>::TraziHash(const K& k) const {
	unsigned int indeks, i(0);
	do {
		indeks = HesLin(k, i);
		if (niz[indeks] && !niz[indeks]->DEL && niz[indeks]->first == k)
			return indeks;
		else
			i = i + 1;
	} while (niz[indeks] != nullptr && i != prostor);
	return -1;
}

template <typename K, typename V>
HashMapa<K, V>::HashMapa(const HashMapa& rhs) : brEl(rhs.brEl), prostor(rhs.prostor), niz(new Pair<K, V>*[prostor] {}), kraj(niz + prostor), h(rhs.h) {
	for (int i = 0; i < rhs.prostor; i++)
		if (rhs.niz[i]) {
			niz[i] = new Pair<K, V>(*rhs.niz[i]);
		}
}

template <typename K, typename V>
HashMapa<K, V>::HashMapa(HashMapa&& rhs) : brEl(rhs.brEl), prostor(rhs.prostor), niz(rhs.niz), kraj(rhs.kraj), h(rhs.h) {
	rhs.niz = rhs.kraj = nullptr;
}

template <typename K, typename V>
HashMapa<K, V>& HashMapa<K, V>::operator=(const HashMapa& rhs) {
	if (this == &rhs)
		return *this;
	obrisi();
	delete[] niz;
	brEl = rhs.brEl;
	prostor = rhs.prostor;
	niz = new Pair<K, V>*[prostor] {};
	kraj = niz + prostor;
	h = rhs.h;
	for (int i = 0; i < rhs.prostor; i++)
		if (rhs.niz[i]) {
			niz[i] = new Pair<K, V>(*rhs.niz[i]);
		}
	return *this;
}

template <typename K, typename V>
HashMapa<K, V>& HashMapa<K, V>::operator=(HashMapa&& rhs) {
	if (this == &rhs)
		return *this;
	obrisi();
	delete[] niz;
	brEl = rhs.brEl;
	prostor = rhs.prostor;
	niz = rhs.niz;
	kraj = rhs.kraj;
	h = rhs.h;
	rhs.niz = rhs.kraj = nullptr;
	return *this;
}

template <typename K, typename V>
void HashMapa<K, V>::Prosiri() {
	Pair<K, V> **noviNiz = new Pair<K, V>*[brEl * 2]{};
	Pair<K, V> **it, **it2;
	for (it = niz, it2 = noviNiz; it != kraj; it++)
		if (*it != nullptr)
			(*it2++) = new Pair<K, V>(**it++);
	int brEl2(brEl * 2);
	obrisi();
	delete[] niz;
	prostor = brEl = brEl2;
	kraj = it2;
	niz = noviNiz;
}

template <typename K, typename V>
V& HashMapa<K, V>::operator[](const K& kljuc) {
	if (!h)
		throw "Hash funkcija nije definisana";
	int indeks(TraziHash(kljuc));
	if (indeks == -1)
		return niz[UmetniHash(kljuc)]->second;
	else
		return niz[indeks]->second;
}

template <typename K, typename V>
V HashMapa<K, V>::operator[](const K& kljuc) const {
	if (!h)
		throw "Hash funkcija nije definisana";
	int indeks(TraziHash(kljuc));
	if (indeks == -1)
		return V();
	else
		return niz[indeks]->second;
}

template <typename K, typename V>
void HashMapa<K, V>::obrisi(const K& kljuc) {
	if (!ObrisiHash(kljuc))
		throw "Kljuc nije pronadjen";
}

template <typename K, typename V>
void HashMapa<K, V>::obrisi() {
	for (Pair<K, V> **it = niz; it != kraj; it++) {
		delete *it;
		*it = nullptr;
	}
	brEl = 0;
}

template <typename K, typename V>
HashMapa<K, V>::~HashMapa() {
	obrisi();
	delete[] niz;
}

int main() {

} 