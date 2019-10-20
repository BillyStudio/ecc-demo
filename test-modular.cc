#include <NTL/ZZ_p.h>

using namespace std;
using namespace NTL;

void add(Vec<ZZ_p>& x, const Vec<ZZ_p>& a, const Vec<ZZ_p>& b)
{
   long n = a.length();
   if (b.length() != n) Error("vector add: dimension mismatch");

   x.SetLength(n);
   long i;
   for (i = 0; i < n; i++)
      add(x[i], a[i], b[i]);
}

int main(int argc, char *argv[])
{
    Vec<ZZ_p> a, b, res;
    ZZ_p in = ZZ_p();

    for (int i=0; i<3; ++i) {
        cin >> in;
        cout << in << endl;
        a.append(in);
        b.append(in);
    }
    
    cout << a << endl;
    cin >> b;
    cout << b << endl;
    add(res, a, b);
    return 0;
}