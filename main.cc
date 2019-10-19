#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

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

ZZ PowerMod(const ZZ& a, const ZZ& e, const ZZ& n)
{
   if (e == 0) return ZZ(1);

   long k = NumBits(e);

   ZZ res;
   res = 1;

   for (long i = k-1; i >= 0; i--) {
      res = (res*res) % n;
      if (bit(e, i) == 1) res = (res*a) % n;
   }

   if (e < 0)
      return InvMod(res, n);
   else
      return res;
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

int main()
{
   ZZ a, b, c;

   cin >> a;
   unsigned char *res1 = conv_num2str(a, NumBytes(a));
   cout << "Number of bytes: " << NumBytes(a) << endl;
   for (int i = 0; i < NumBytes(a); ++i) {
       printf("%d", res1[i]);
   }
   printf("\n");
   // cout << c << "\n";
}