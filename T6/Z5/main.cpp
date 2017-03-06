#include <iostream>
#include <vector>

using namespace std;

struct Student { string ime, prezime, emailAdresa; int brojIndeksa; };

bool operator<=(const Student &lhs, const Student &rhs) {
    return lhs.prezime + lhs.ime <= rhs.prezime + rhs.ime;
}
bool operator>=(const Student &lhs, const Student &rhs) {
    return lhs.prezime + lhs.ime >= rhs.prezime + rhs.ime;
}

int foo(vector<Student> niz) {
    int pocetak(0), kraj(niz.size() - 1);
    while(pocetak < kraj) {
        int srednji((pocetak + kraj) / 2);
        if(niz[srednji] <= niz[srednji + 1] && niz[srednji] <= niz[srednji - 1])
            return srednji;
        if(niz[srednji] >= niz[srednji + 1] && niz[srednji] <= niz[srednji - 1])
            pocetak = srednji + 1;
        else 
            kraj = srednji - 1;
    } 
    return -1;
}

int main() {
    
    return 0;
}