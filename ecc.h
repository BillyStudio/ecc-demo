#ifndef _ECC_51748_H
#define _ECC_51748_H

#include <NTL/ZZ.h>
#include <NTL/ZZ_pXFactoring.h>
#include <NTL/ZZXFactoring.h>

const NTL::ZZ GF = NTL::ZZ(23);

typedef struct Point {
    NTL::ZZ x;
    //unsigned char y_bit;
    NTL::ZZ y;
    void Zero();
    Point operator+(const Point& b) {
        Point sum_P;
        long lamb;
        sum_P.x = this->x + b.x;
        return sum_P;
    }
} Point;

class ECC {
public:
    ECC(NTL::ZZ p, long a, long b);
    int setBasePoint(const Point& G);
    int setBasePoint(const NTL::ZZ x, const NTL::ZZ y);
private:
    NTL::ZZ p, a, b;
    Point G;
    NTL::ZZ_pX factors;
    ECC();
    bool checkPoint(const NTL::ZZ x, const NTL::ZZ y);
    NTL::ZZ calc_poly(NTL::ZZ_pX factors, NTL::ZZ x);
};

#endif
