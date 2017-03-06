#include <iostream>
#include <queue>
#include <string>
#include <utility>

using namespace std;

class StampacServer {
public:
    enum Prioritet {Sef, Radnik, Pripravnik};
private:
  class comparator {
  public:
    bool operator()(const pair<Prioritet, string>& left, const pair<Prioritet, string>& right)  { return left.first > right.first; }
  };  
  priority_queue<pair<StampacServer::Prioritet, string>, std::vector<pair<StampacServer::Prioritet, string>>, comparator> prioritetniRed;
public:
  void posaljiDokument(Prioritet p, string sadrzaj) {
    prioritetniRed.push(make_pair(p, sadrzaj));
  }
  string dajDokument() {
    if(prioritetniRed.size()) {
      auto temp(prioritetniRed.top());
      prioritetniRed.pop();
      return temp.second;
    }
    else 
      return "";
  }
};

int main() {
    StampacServer ss;
    ss.posaljiDokument(StampacServer::Pripravnik, "abc");
    ss.posaljiDokument(StampacServer::Radnik, "cdf");
    ss.posaljiDokument(StampacServer::Sef, "sdf");
    cout << ss.dajDokument() << endl;
    cout << ss.dajDokument() << endl;
    cout << ss.dajDokument() << endl;
    return 0;
}

/*
Razviti klasu StampacServer koja obavlja funkciju servera za štampanje u jednoj kompaniji.
Klasa implementira funkciju void posaljiDokument(Prioritet p, string sadrzaj).
Prioritet je tip enum {Sef, Radnik, Pripravnik}. Ovaj enum se treba nalaziti u javnom
interfejsu klase. Sadržaj dokumenta za printanje je pojednostavljeno samo tekst - pa je tipa string.
Ova funkcija treba da prihvati dokument sa odgovarajućim prioritetom i pohrani ga za kasnije 
štampanje.
Osim toga, ova klasa treba da implementira funkciju string dajDokument() koja od
dokumenata koji su poslati za štampanje vraća neodštampani dokument sa najvišim prioritetom, a
ako ima više dokumenata sa istim prioritetom, onda najstariji. Logično, najviši prioriet je Sef, a
zatim Radnik i najniži Pripravnik. Ako ne postoji niti jedan neodštampani dokument, treba
vratiti prazan string.
Klasa bi trebala biti korištena tako što će driver za štampače na računarima uposlenika pozivati
funkciju posaljiDokuement, a štampači čim završe sa prethodnim dokumentom, pozivati
funkciju dajDokument. Ovakvo ponašanje treba simulirati u main programu.
*/