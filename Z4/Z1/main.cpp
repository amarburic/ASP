
//#define DEBUG
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>

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
	int balans;
	Cvor *roditelj, *lijevo, *desno;
	Cvor(const K& kljuc, const V& vrijednost, Cvor* roditelj = nullptr, int balans = 0,
		Cvor* lijevo = nullptr, Cvor* desno = nullptr) :
		kljuc(kljuc), vrijednost(vrijednost), balans(balans), roditelj(roditelj),
		lijevo(lijevo), desno(desno) {}
};

template <typename K, typename V>
class AVLStabloMapa : public Mapa<K, V> {
#ifdef DEBUG
public: //ispravi
#endif
	Cvor<K, V> *korijen;
	int velicina;
	Cvor<K, V>* Trazi(const K& kljuc, Cvor<K, V>* cvor) const;
	void  Umetni(const K& kljuc, const V& vrijednost, Cvor<K, V> *&cvor, Cvor<K, V> *roditelj = nullptr);
	void BrisiCvor(const K& kljuc, Cvor<K, V>* &cvor);
	void Brisi(Cvor<K, V> *&cvor);
	void Kopiraj(Cvor<K, V>* const& stari, Cvor<K, V>* &novi, Cvor<K, V>* roditelj = nullptr);
	void AzurirajBalans(Cvor<K, V> *cvor, bool lijevo, bool brisanje = false);
	void LijevaRotacija(Cvor<K, V>* cvor);
	void DesnaRotacija(Cvor<K, V>* cvor);
	int OdrediBalans(Cvor <K, V>* cvor);

	void Preorder(Cvor<K, V> const *, int level = 0) const;
public:
	void Preorder() const;
	AVLStabloMapa() : korijen(nullptr), velicina(0) {}
	AVLStabloMapa(const AVLStabloMapa& rhs);
	AVLStabloMapa(AVLStabloMapa&& rhs);
	~AVLStabloMapa();
	V& operator[](const K&);
	V operator[](const K&) const;
	AVLStabloMapa& operator=(const AVLStabloMapa& rhs);
	AVLStabloMapa& operator=(AVLStabloMapa&& rhs);
	int brojElemenata() const { return velicina; }
	void obrisi() { Brisi(korijen); velicina = 0; }
	void obrisi(const K& kljuc) { BrisiCvor(kljuc, korijen); }
};

template <typename K, typename V>
void AVLStabloMapa<K, V>::Preorder() const {
	cout << "Preorder: " << endl;
	Preorder(korijen);
}

template <typename K, typename V>
void AVLStabloMapa<K, V>::Preorder(Cvor<K, V> const * cvor, int level) const {
	if(cvor == nullptr)
		
		cout << string(level, ' ') << "Empty" << endl;
	else {
		cout << string(level, ' ') << cvor->kljuc << " : " << cvor->vrijednost << " " << cvor->balans 
		<< " Roditelj: ";
		if(cvor->roditelj)
			cout << (cvor->roditelj->kljuc);
		else cout << "None";
		cout << endl;
		Preorder(cvor->lijevo, level + 2);
		Preorder(cvor->desno, level + 2);
	}
}

template <typename K, typename V>
Cvor<K, V>* AVLStabloMapa<K, V>::Trazi(const K& kljuc, Cvor<K, V>* cvor) const {
	if (cvor == nullptr || cvor->kljuc == kljuc)
		return cvor;
	if (cvor->kljuc < kljuc)
		return Trazi(kljuc, cvor->desno);
	return Trazi(kljuc, cvor->lijevo);
}

template <typename K, typename V>
void AVLStabloMapa<K, V>::Umetni(const K& kljuc, const V& vrijednost, Cvor<K, V> *&cvor, Cvor<K, V> *roditelj) {
	#ifdef DEBUG
	cout << kljuc << endl; //ispravi
	#endif
	if (cvor == nullptr) {
		velicina++;
		cvor = new Cvor<K, V>(kljuc, vrijednost, roditelj);
		if (cvor->roditelj) {
			if (cvor->roditelj->lijevo && cvor->roditelj->desno)
				cvor->roditelj->balans = 0;
			else {
				if(cvor == cvor->roditelj->lijevo)
					AzurirajBalans(cvor->roditelj, true);
				else
					AzurirajBalans(cvor->roditelj, false);
			}
		}
		if (roditelj == nullptr)
			korijen = cvor;
	}
	else if (cvor->kljuc < kljuc) {
		#ifdef DEBUG
		cout << "kljuc veci" << endl; // ispravi
		#endif
		Umetni(kljuc, vrijednost, cvor->desno, cvor);
	}
	else if (cvor->kljuc > kljuc) {
		#ifdef DEBUG
		cout << "kljuc manji" << endl; //ispravi
		#endif
		Umetni(kljuc, vrijednost, cvor->lijevo, cvor);
	}
}

template <typename K, typename V>
void AVLStabloMapa<K, V>::BrisiCvor(const K& kljuc, Cvor<K, V>* &cvor) {
	Cvor<K, V> *p(Trazi(kljuc, korijen)), *rp(nullptr), *sr(nullptr);
	bool lijevo = false;
	if (p == nullptr)
		return;
	if (p->lijevo == nullptr) {
		rp = p->desno;
		sr = rp;
		if(p->desno == nullptr)
			sr = p->roditelj;
	}
	else if (p->desno == nullptr) {
		rp = p->lijevo;
		sr = rp;
		lijevo = true;
	}
	else {
		rp = p->lijevo;
		while (rp->desno != nullptr)
			rp = rp->desno;
		if (rp->roditelj != p) {

			sr = rp->roditelj;
			rp->roditelj->desno = rp->lijevo;
			if(rp->lijevo)
				rp->lijevo->roditelj = rp->roditelj->desno;
			rp->lijevo = p->lijevo;
			if(rp->lijevo)
				rp->lijevo->roditelj = rp;
		} else
			sr = rp;
		rp->desno = p->desno;
		rp->desno->roditelj = rp;
	}
	if (p->roditelj == nullptr) {
		korijen = rp;
	}
	else if (p == p->roditelj->lijevo)
		p->roditelj->lijevo = rp;
	else
		p->roditelj->desno = rp;
	if(rp) {
		rp->roditelj = p->roditelj;
		rp->balans = p->balans;
	}
	velicina--;
	delete p;
	p = nullptr;
	#ifdef DEBUG
	
//	cout << sr << endl;
	#endif

	if(sr) {
		if(sr->lijevo || sr->desno)
			sr->balans += (sr->lijevo ? 1 : -1);
		else 
			AzurirajBalans(sr, lijevo, true);
	}
}

template <typename K, typename V>
void AVLStabloMapa<K, V>::Brisi(Cvor<K, V> *&cvor) {
	if (cvor != nullptr) {
		Brisi(cvor->desno);
		Brisi(cvor->lijevo);
		delete cvor;
		cvor = nullptr;
	}
}

template <typename K, typename V>
void AVLStabloMapa<K, V>::Kopiraj(Cvor<K, V>* const& stari, Cvor<K, V>* &novi, Cvor<K, V>* roditelj) {
	if (stari == nullptr)
		return;
	novi = new Cvor<K, V>(stari->kljuc, stari->vrijednost, roditelj, stari->balans); //dodan balans u konst
	Kopiraj(stari->lijevo, novi->lijevo, novi);
	Kopiraj(stari->desno, novi->desno, novi);
}

template <typename K, typename V>
void AVLStabloMapa<K, V>::AzurirajBalans(Cvor<K, V> *cvor, bool lijevo, bool brisanje) {
	#ifdef DEBUG
	cout << "Azuriram " << cvor->kljuc << " " << (lijevo ? "Lijevo" : "Desno") << endl;
	#endif
	if(lijevo)
		brisanje ? cvor->balans-- : cvor->balans++;
	else 
		brisanje ? cvor->balans++ : cvor->balans--;
	if(cvor->balans < -1) {
		if(cvor->desno->balans == -1)
			LijevaRotacija(cvor);
		else {
			DesnaRotacija(cvor->desno);
			LijevaRotacija(cvor);
		}
	} else if(cvor->balans > 1) {
		if(cvor->lijevo->balans == 1) {
			DesnaRotacija(cvor);
		} else {
			LijevaRotacija(cvor->lijevo);
			DesnaRotacija(cvor);
		}
	} else {
		if(cvor->roditelj) {
			if(cvor == cvor->roditelj->lijevo)
				AzurirajBalans(cvor->roditelj, true, brisanje);
			else
				AzurirajBalans(cvor->roditelj, false, brisanje);
		}
	}
	#ifdef DEBUG
	cout << "Zavrsio " << cvor->kljuc << " " << (lijevo ? "Lijevo" : "Desno") << endl;
	#endif
}

template <typename K, typename V>
void AVLStabloMapa<K, V>::DesnaRotacija(Cvor<K, V>* cvor) {
	#ifdef DEBUG
		cout << "Desno rotiram: " << cvor->kljuc << endl;
	#endif
	Cvor<K, V>** pcvor;
	if(cvor->roditelj) {
		if(cvor->roditelj->lijevo && cvor->roditelj->lijevo->kljuc == cvor->kljuc)
			pcvor = &cvor->roditelj->lijevo;
		else
			pcvor = &cvor->roditelj->desno;
	}
	else 
		pcvor = &korijen;
	*pcvor = cvor->lijevo;
	(*pcvor)->roditelj = cvor->roditelj;
	cvor->lijevo = (*pcvor)->desno;
	if(cvor->lijevo)
		cvor->lijevo->roditelj = cvor;
	(*pcvor)->desno = cvor;
	cvor->roditelj = (*pcvor);
	cvor->balans -= 1 + max(0, (*pcvor)->balans);
	(*pcvor)->balans -= 1 - min(0, cvor->balans);
}

template <typename K, typename V>
void AVLStabloMapa<K, V>::LijevaRotacija(Cvor<K, V>* cvor) {
	#ifdef DEBUG
		cout << "Lijevo rotiram: " << cvor->kljuc << endl;
	#endif
		Cvor<K, V>** pcvor;
	if(cvor->roditelj) {
		if(cvor->roditelj->lijevo && cvor->roditelj->lijevo->kljuc == cvor->kljuc)
			pcvor = &cvor->roditelj->lijevo;
		else
			pcvor = &cvor->roditelj->desno;
	}
	else 
		pcvor = &korijen;
	*pcvor = cvor->desno;
	(*pcvor)->roditelj = cvor->roditelj;
	cvor->desno = (*pcvor)->lijevo;
	if(cvor->desno)
		cvor->desno->roditelj = cvor;
	(*pcvor)->lijevo = cvor;
	cvor->roditelj = (*pcvor);

	cvor->balans -= -1 + min(0, (*pcvor)->balans);
	(*pcvor)->balans -= -1 - max(0, cvor->balans);
	
}

template <typename K, typename V>
int AVLStabloMapa<K, V>::OdrediBalans(Cvor <K, V>* cvor) {
	if(!cvor->lijevo && !cvor->desno)
		return 0;
	if(!cvor->lijevo)
		return -abs(cvor->desno->balans) - 1;
	if(!cvor->desno)
		return abs(cvor->lijevo->balans) + 1;
	return abs(cvor->lijevo->balans) - abs(cvor->desno->balans);
}

template <typename K, typename V>
AVLStabloMapa<K, V>::AVLStabloMapa(const AVLStabloMapa& rhs)
	: velicina(rhs.velicina), korijen(nullptr) {
	Kopiraj(rhs.korijen, korijen);
}

template <typename K, typename V>
AVLStabloMapa<K, V>::AVLStabloMapa(AVLStabloMapa&& rhs)
	: velicina(rhs.velicina), korijen(rhs.korijen) {
	rhs.korijen = nullptr;
}

template <typename K, typename V>
AVLStabloMapa<K, V>::~AVLStabloMapa() {
	Brisi(korijen);
}

template <typename K, typename V>
V& AVLStabloMapa<K, V>::operator[](const K& kljuc) {
	auto rezultat(Trazi(kljuc, korijen));
	if (rezultat != nullptr)
		return rezultat->vrijednost;
	Umetni(kljuc, V(), korijen);
	return Trazi(kljuc, korijen)->vrijednost;
	
}

template <typename K, typename V>
V AVLStabloMapa<K, V>::operator[](const K& kljuc) const {
	auto rezultat(Trazi(kljuc, korijen));
	if (rezultat != nullptr)
		return rezultat->vrijednost;
	return V();
}

template <typename K, typename V>
AVLStabloMapa<K, V>& AVLStabloMapa<K, V>::operator=(const AVLStabloMapa<K, V>& rhs) {
	if (this == &rhs)
		return *this;
	Brisi(korijen);
	velicina = rhs.velicina;
	korijen = nullptr;
	Kopiraj(rhs.korijen, korijen);
	return *this;
}

template <typename K, typename V>
AVLStabloMapa<K, V>& AVLStabloMapa<K, V>::operator=(AVLStabloMapa<K, V>&& rhs) {
	if (this == &rhs)
		return *this;
	Brisi(korijen);
	velicina = rhs.velicina;
	korijen = rhs.korijen;
	rhs.korijen = nullptr;
	return *this;
}

int main()
{


}



