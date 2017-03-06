#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

template <typename T>
class Grana;
template <typename T>
class Cvor;
template <typename T>
class GranaIterator;

template <typename T>
class UsmjereniGraf {
protected: 
	UsmjereniGraf() {}
public:
	UsmjereniGraf(int) {}
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
class MatricaGraf : public UsmjereniGraf<T> {
	const float INF = std::numeric_limits<float>::infinity();
	vector<vector<float>> m;
	vector<T> oznakeCvorova;
	vector<vector<T>> oznakeGrana;

	void validirajCvor(int) const;
	void validirajGranu(int, int) const;
	void validirajPostojanjeGrane(int, int) const;
public:
	MatricaGraf(int brojCvorova);

	int dajBrojCvorova() const { return m.size(); }
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
void MatricaGraf<T>::validirajCvor(int indeks) const {
	if (indeks < 0 || indeks >= dajBrojCvorova())
		throw "Neispravan broj cvora";
}

template <typename T>
void MatricaGraf<T>::validirajGranu(int i, int j) const {
	validirajCvor(i);
	validirajCvor(j);
}

template <typename T>
void MatricaGraf<T>::validirajPostojanjeGrane(int i, int j) const {
	validirajGranu(i, j);
	if (!postojiGrana(i, j))
		throw "Grana ne postoji";
}

template <typename T>
MatricaGraf<T>::MatricaGraf(int brojCvorova) {
	if (brojCvorova < 0)
		throw "Broj cvorova ne moze biti negativan";
	m = vector<vector<float>>(brojCvorova, vector<float>(brojCvorova, INF));
	oznakeCvorova = vector<T>(brojCvorova);
	oznakeGrana = vector<vector<T>>(brojCvorova, vector<T>(brojCvorova));
}

template <typename T>
void MatricaGraf<T>::postaviBrojCvorova(int noviBrojCvorova) {
	if (noviBrojCvorova < dajBrojCvorova())
		throw "Novi broj cvorova ne moze biti manji od trenutnog";
	m.resize(noviBrojCvorova);
	for (auto& vf : m)
		vf.resize(noviBrojCvorova, INF);
	oznakeCvorova.resize(noviBrojCvorova);
	oznakeGrana.resize(noviBrojCvorova);
	for (auto& vt : oznakeGrana)
		vt.resize(noviBrojCvorova);
}

template <typename T>
void MatricaGraf<T>::dodajGranu(int i, int j, float w) {
	validirajGranu(i, j);
	m[i][j] = w;
}

template <typename T>
void MatricaGraf<T>::obrisiGranu(int i, int j) {
	validirajPostojanjeGrane(i, j);
	m[i][j] = INF;
}

template <typename T>
void MatricaGraf<T>::postaviTezinuGrane(int i, int j, float w) {
	validirajPostojanjeGrane(i, j);
	m[i][j] = w;
}

template <typename T>
float MatricaGraf<T>::dajTezinuGrane(int i, int j) const {
	validirajPostojanjeGrane(i, j);
	return m[i][j];
}

template <typename T>
bool MatricaGraf<T>::postojiGrana(int i, int j) const {
	validirajGranu(i, j);
	return m[i][j] != INF;
}

template <typename T>
void MatricaGraf<T>::postaviOznakuCvora(int i, T oznaka) {
	validirajCvor(i);
	oznakeCvorova[i] = oznaka;
}

template <typename T>
T MatricaGraf<T>::dajOznakuCvora(int i) const {
	validirajCvor(i);
	return oznakeCvorova[i];
}

template <typename T>
void MatricaGraf<T>::postaviOznakuGrane(int i, int j, T oznaka) {
	validirajPostojanjeGrane(i, j);
	oznakeGrana[i][j] = oznaka;

}

template <typename T>
T MatricaGraf<T>::dajOznakuGrane(int i, int j) const {
	validirajPostojanjeGrane(i, j);
	return oznakeGrana[i][j];
}

template <typename T>
Grana<T> MatricaGraf<T>::dajGranu(int i, int j) {
	validirajPostojanjeGrane(i, j);
	return Grana<T>(this, i, j);
}

template <typename T>
const Grana<T> MatricaGraf<T>::dajGranu(int i, int j) const {
	validirajPostojanjeGrane(i, j);
	return Grana<T>(const_cast<MatricaGraf*>(this), i, j);
}

template <typename T>
Cvor<T> MatricaGraf<T>::dajCvor(int i) {
	validirajCvor(i);
	return Cvor<T>(this, i);
}

template <typename T>
const Cvor<T> MatricaGraf<T>::dajCvor(int i) const {
	validirajCvor(i);
	return Cvor<T>(const_cast<MatricaGraf*>(this), i);
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
UsmjereniGraf<bool> *g = new MatricaGraf<bool>(6);
    g->dodajGranu(0, 1, 2.5);
    g->dodajGranu(1, 2, 1.2);
    g->dodajGranu(1, 3, 0.1);
    g->dodajGranu(2, 4, 3.14);
    g->dodajGranu(2, 3, 2.73);
    g->dodajGranu(3, 5, 1);
    g->dodajGranu(5, 2, -5.0);
    std::vector<Cvor<bool> > bfs_obilazak;
    bfs(g, bfs_obilazak, g->dajCvor(0));
    for (int i = 0; i < bfs_obilazak.size(); i++)
      cout << bfs_obilazak[i].dajRedniBroj() << ",";
  delete g;

}