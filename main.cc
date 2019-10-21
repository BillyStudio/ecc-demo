#include "ecc.h"

using namespace std;
using namespace NTL;

ECC::ECC(ZZ _p, long _a, long _b) : p(_p), a(_a), b(_b) {
    factors.SetMaxLength(4);
    SetCoeff(factors, long(0), _b);
    SetCoeff(factors, long(1), _a);
    SetCoeff(factors, long(2), long(0));
    SetCoeff(factors, long(3));
}

int ECC::setBasePoint(const ZZ x, const ZZ y) {
    if (checkPoint(x, y) == true) {
        G.x = x;
        G.y = y;
        return 0;
    }
    return -1;
}

int ECC::setBasePoint(const Point& base_P) {
    if (checkPoint(base_P.x, base_P.y) == true) {
        G = base_P;
        return 0;
    }
    return -1;
}

bool ECC::checkPoint(const ZZ x, const ZZ y) {
    cout << "x = " << x << ", f(x)=" << calc_poly(factors, x) << ", y=" << y << endl;
    if (y*y%GF == calc_poly(factors, x))
        return true;
    return false;
}

ZZ ECC::calc_poly(ZZ_pX factors, ZZ x)
{
    ZZ res;
    res = 0;

    for (long i=0; i<factors.rep.length(); i++){
        ZZ_p item = factors[i];
        res += item._ZZ_p__rep * PowerMod(x, i, GF);
    }
    return res % GF;
}

Point ECC::doublePoint(const Point& G) {
    Point add_sum;
    ZZ lambda;
    lambda = (3*G.x*G.x+a) * InvMod(2*G.y, GF) % GF;
    add_sum.x = (lambda*lambda - 2*G.x) % GF;
    add_sum.y = (lambda * (G.x - add_sum.x) - G.y) % GF; 
    return add_sum;
}

Point ECC::addPoint(Point A, Point B) {
    Point add_P;
    ZZ lambda;
    if (A.x == B.x) return doublePoint(A);
    if (A.x > B.x) {add_P = A; A = B; B=add_P;}
    lambda = (B.y - A.y) * InvMod(B.x - A.x, GF) % GF;
    add_P.x = (lambda*lambda - A.x - B.x) % GF;
    add_P.y = (lambda*(A.x-add_P.x) - add_P.y) % GF;
    return add_P;
}

Point ECC::mulPoint(long k, const Point& G)
{
    Point kG;
    long i;
    
    kG.Zero();
    if (k == 0) return kG;

    long max_loop = NumBits(k);
    kG = G;
    for (i=k-1; i>=0; --i) {
        kG = doublePoint(kG);
        if (bit(k, i) == 1) kG = addPoint(kG, G);
    }

    return kG;
}

unsigned char *conv_num2str(ZZ num, size_t n_bytes) {
    unsigned char *res;
    ZZ N;
    int byte;

    res = (unsigned char *)malloc(n_bytes);
    N = 1<<8;
    byte = 0;
    while (num.size() > 0) {
        ZZ m = num % N;
        for (int i=0; i<8; ++i) {
            res[byte] |= (bit(m, i) << i);
        }
        byte = byte + 1;
        num = num / N;
    }
    return res;
}

// ZZ conv_str2num(unsigned char* str, size_t n_bytes) {
//     ZZ res;
//     long byte;

//     res = 0;
//     for (byte=0; byte<n_bytes; ++byte) {
//         ZZ temp;
//         for (int i=0; i<8; ++i) {
//             temp = temp + (str[byte] | (1<<i));
//         }
//         res = res + temp;
//         res = res << long(1<<8);
//     }
//     return res;
// }

// char *conv_pointarr2str(Point kG, int k) {
// }

int main()
{
    ZZ_p::init(GF);
    long a = 13, b = 22;
    ECC example = ECC(GF, a, b);
    Point G;
    G.x = ZZ(10);
    G.y = ZZ(5);
    if (example.setBasePoint(G) == 0) {
        cout << "set true" << endl;
        // test the stage n of G(10, 5)
        for (long i=1; i<=7; ++i) {
            Point kG = example.mulPoint(i, G);
            cout << "(" << kG.x << "," << kG.y << ")" << endl;
        }
        
        // char *m = "H";
        // Point kG[strlen(m)];
        // convert m string to number
        // for (int i=0; i<strlen(m); ++i) {
        //     kG[i] = MulPoint(m[i], example.getBasePoint());
        // }
        // conv_pointarr2str(kG, strlen(m));
    } else {
        cout << "set false" << endl;
    }
}
