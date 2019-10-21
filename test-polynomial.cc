#include <ecc.h>

using namespace std;
using namespace NTL;

ZZ calc_poly(ZZ_pX factors, ZZ x)
{
    ZZ res;
    res = 0;

    for (long i=0; i<factors.rep.length(); i++){
        ZZ_p item = factors[i];
        res += item._ZZ_p__rep * PowerMod(x, i, GF);
    }

    return res;
}

int main()
{
    ZZ_p::init(GF);
    ZZ x;
    ZZ_pX f;

    cin >> f;
    ZZ res = calc_poly(f, x);
    cout << "result=" << res << endl;
}
