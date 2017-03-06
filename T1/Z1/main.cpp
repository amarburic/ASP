#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

struct Student {
    std::string ime, prezime;
    int brojIndexa;
};

string imeIPrezime(const Student& s) {
    return s.ime + s.prezime;
}

class Compare {
    public:
    bool operator() (const Student& p1, const Student& p2) const {
        string s1(imeIPrezime(p1)), s2(imeIPrezime(p2));
        if(s1 == s2)
            throw "";
        return s1 < s2;
    }
};

bool istoImePrezime(std::vector<Student> studenti) {
    try {
        sort(studenti.begin(), studenti.end(), Compare());
        for(auto it = studenti.begin(); it < studenti.end() - 1; it++) {
            if(imeIPrezime(*it) == imeIPrezime(*(it + 1)))
                return true;
        }
    } catch(...) {
        return true;
    }
    return false;
}

int main() {
    vector<Student> v{{"aa", "bb", 1}, {"ab", "ba", 2}, {"aa", "ba", 3}, {"ba", "bb", 4}, {"ab", "bb", 5}};
    cout << istoImePrezime(v);
}