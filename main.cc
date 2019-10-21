#include <ecc.h>

using namespace std;
using namespace NTL;

void Point::Zero() {
    x = 0;
    y_bit = 0;
}

ECC::ECC(ZZ _p, ZZ _a, ZZ _b) : p(_p), a(_a), b(_b) {
    factors.SetMaxLength(4);
    long long_a;
    conv(long_a, a);
    SetCoeff(factors, long(0), long_a);

}

int ECC::setBasePoint(const Point& base_P) {
    if (checkPoint(base_P.x, base_P.y) == true) {
        G = base_P;
    }
    return 0;
}

bool ECC::checkPoint(const ZZ x, const ZZ y) {
    if (y == calc_poly(factors, x))
        return true;
    return false;
}

Point MulPoint(long k, const Point& G)
{
    Point kG;
    long i;
    
    kG.Zero();
    if (k == 0) return kG;

    long max_loop = NumBits(k);
    Point dynamic[max_loop];
    for (i=0; i<max_loop; ++i) {
        if (i > 0) {
            dynamic[i] = dynamic[i-1] + dynamic[i-1];
        } else {
            dynamic[0] = G;
        }
    }

    i = 0;
    while (k > 0) {
        if (k&1) {
            kG = kG + dynamic[i];
        }
        k >>= 1;
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

ZZ conv_str2num(unsigned char* str, size_t n_bytes) {
    ZZ res;
    long byte;

    res = 0;
    for (byte=0; byte<n_bytes; ++byte) {
        ZZ temp;
        for (int i=0; i<8; ++i) {
            temp = temp + (str[byte] | (1<<i));
        }
        res = res + temp;
        res = res << long(1<<8);
    }
    return res;
}

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
    long a = 13, b = 22;
    ECC example = ECC(GF, a, b);
}
