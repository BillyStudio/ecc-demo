#include <bits/stdc++.h>
#include <NTL/ZZ_p.h>
#include <NTL/ZZ_pXFactoring.h>

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

void InnerProduct(ZZ_p& x, const Vec<ZZ_p>& a, const Vec<ZZ_p>& b)
{
   long n = min(a.length(), b.length());
   long i;
   ZZ accum, t;

   accum = 0;
   for (i = 0; i < n; i++) {
      mul(t, rep(a[i]), rep(b[i]));
      add(accum, accum, t);
   }

   conv(x, accum);
}

int main(int argc, char *argv[])
{
    ZZ_p::init(ZZ(5));
    Vec<ZZ_p> a;
    a.SetLength(3);
    Vec<ZZ_p> b[3];
    Vec<ZZ_p> res[3];
    long in;

    for (int i=0; i<3; ++i) {
        cin >> in;
        cout << in << endl;
        ZZ_p pin = ZZ_p(in);
        a[i] = pin;
    }
    
    cout << a << endl;
    // cin >> b;
    // cout << b << endl;
    // add(res, a, b);
    return 0;
}