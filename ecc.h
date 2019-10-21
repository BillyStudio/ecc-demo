#ifndef _ECC_51748_H
#define _ECC_51748_H

#include <NTL/ZZ.h>
#include <NTL/ZZ_pXFactoring.h>
#include <NTL/ZZXFactoring.h>

const NTL::ZZ GF = NTL::ZZ(23);

typedef struct Point {
    NTL::ZZ x;
    NTL::ZZ y;
    void Zero(){x=0; y=0;}
} Point;

class ECC {
public:
    ECC(NTL::ZZ p, long a, long b);
    int setBasePoint(const Point& G);
    int setBasePoint(const NTL::ZZ x, const NTL::ZZ y);
    Point getBasePoint() {return G;};
    
    Point mulPoint(long k, const Point& G);
    int encrypt(char *);
    int decrypt();
private:
    NTL::ZZ p, a, b;
    Point G;
    NTL::ZZ_pX factors;
    ECC();
    bool checkPoint(const NTL::ZZ x, const NTL::ZZ y);
    NTL::ZZ calc_poly(NTL::ZZ_pX factors, NTL::ZZ x);
    Point addPoint(Point, Point);
    Point doublePoint(const Point&);
    
};

#endif
