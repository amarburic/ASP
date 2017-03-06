#include <iostream>

using namespace std;

/*
void hanoi(int n, int s, int d, int t) {
    if(n == 1) {
        cout << "Preseli disk sa stapa " << s << " na stap " << d << endl;
        getchar();
        return;
    }
    hanoi(n - 1, s, t, d);
    cout << "Preseli disk sa stapa " << s << " na stap " << d << endl;
    getchar();
    hanoi(n - 1, t, d, s);
}
*/

int nzd(int x, int y) {
    if(y == 0)
        return x;
    return nzd(y, x % y);
}

int fib2_0(int n, int prosli = 1, int pretprosli = 0) {
    if(n == 0) return pretprosli;
    return fib2_0(n - 1, prosli + pretprosli, prosli);
}

int main() {
    //hanoi(4, 1, 3, 2);
    for(int i = 0; i <= 10; i++)
        cout << fib2_0(i) << endl;
     return 0;
}