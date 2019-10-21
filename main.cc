#include <NTL/ZZ.h>
#include <NTL/ZZ_pXFactoring.h>

using namespace std;
using namespace NTL;

const ZZ GF = ZZ(23);

typedef struct Point {
    ZZ x;
    unsigned char y_bit;
    void Zero();
    Point operator+(const Point& b) {
        Point sum_P;
        long lamb;
        sum_P.x = this->x + b.x;
        return sum_P;
    }
} Point;

void Point::Zero() {
    x = 0;
    y_bit = 0;
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
    ZZ x;
    ZZ_pX f;

    cin >> f;
    // for (int i=0; i<f.rep.length(); i++){
    //     cout << f[i] << endl;
    // }
    cin >> x;   // input x mod 23
    ZZ res = calc_poly(f, x);
    cout << "result=" << res << endl;
//    unsigned char *res1 = conv_num2str(a, NumBytes(a));
//    cout << "Number of bytes: " << NumBytes(a) << endl;
//    for (int i = 0; i < NumBytes(a); ++i) {
//        printf("%d", res1[i]);
//    }
//    printf("\n");
   // cout << c << "\n";
}