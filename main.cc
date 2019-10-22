#include "ecc.h"

using namespace std;
using namespace NTL;

const NTL::ZZ GF = NTL::ZZ(23);

ostream& operator<< (ostream &output, Point &point) {
    output<<"("<<point.x<<","<<point.y<<")";
    return output;
}

ECC::ECC(long _p, long _a, long _b) : p(_p), a(_a), b(_b) {
    factors.SetMaxLength(4);
    SetCoeff(factors, long(0), _b);
    SetCoeff(factors, long(1), _a);
    SetCoeff(factors, long(2), long(0));
    SetCoeff(factors, long(3));
    dynamic = new Point[p];
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
    lambda = (3*G.x*G.x+a) * InvMod((2*G.y)%GF, GF) % GF;
    add_sum.x = (lambda*lambda - 2*G.x) % GF;
    add_sum.y = (lambda * (G.x - add_sum.x) - G.y) % GF; 
    return add_sum;
}

Point ECC::addPoint(Point A, Point B) {
    Point add_P;
    ZZ lambda;

    if (A.x==0 && A.y==0) return B;
    if (B.x==0 && B.y==0) return A;

    if (A.x < B.x) {
        lambda = (B.y - A.y) * InvMod((B.x-A.x)%GF, GF) % GF;
        add_P.x = (lambda*lambda - A.x - B.x) % GF;
        add_P.y = (lambda*(A.x-add_P.x) - A.y) % GF;
    } else if (A.x > B.x) {
        lambda = (A.y - B.y) * InvMod((A.x-B.x)%GF, GF) % GF;
        add_P.x = (lambda*lambda - B.x - A.x) % GF;
        add_P.y = (lambda*(B.x-add_P.x) - B.y) % GF;
    } else if (A.y == B.y){    // A.x == B.x, A.y == B.y
        add_P = doublePoint(A);
    } else {    // A.x == B.x, (A.y)^2 == (B.y)^2 mod GF
        add_P.x = 0;
        add_P.y = 0;
    }

    return add_P;
}

Point ECC::mulBasePoint(long k)
{
    Point kG;
    long i;
    
    kG.Zero();
    if (k == 0) return kG;
    if (k == 1) return G;

    //long max_loop = NumBits(k);
    i = 0;
    while (k>0) {
        if (k&1) {
            if (i == 0) {
                kG = dynamic[i];
            } else {
                kG = addPoint(kG, dynamic[i]);
            }
            // cout << "dynamic["<<i<<"]="<<dynamic[i].x<<","<<dynamic[i].y<<endl;
            // cout << "kG="<<kG.x<<","<<kG.y<<endl;
        }
        k >>= 1;
        i++;
    }

    return kG;
}

Point ECC::mulPoint(long k, Point P)
{
    Point kP, pow2P;
    long i;
    bool neg_flag = false;
    
    kP.Zero();
    if (k == 0) return kP;
    if (k == 1) return P;
    if (k < 0) {
        k = -k;
        neg_flag = true;
    }

    pow2P = P;
    while (k>0) {
        if (k&1) kP = addPoint(kP, pow2P);
        k >>= 1;
        pow2P = doublePoint(pow2P);
    }

    if (neg_flag == true) kP.y = GF-kP.y;
    return kP;
}

long ECC::findStage() {
    long stage, base2pow, dp_i;
    Point kG = G;

    stage = 1;
    base2pow = 2;
    dynamic[0] = G;
    dp_i = 1;
    while (kG.y!=0 && kG.x!=0) {
        ++ stage;
        kG = addPoint(kG, G);
        // cout << kG.x << "," << kG.y << endl;
        if (stage == base2pow) {
            dynamic[dp_i++] = kG;
            base2pow <<= 1;
        }
        if (kG.x == G.x && kG.y == G.y) {
            break;
        }
    }
    return stage;
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
    long a = 13, b = 22, l_GF;
    Point G;
    long k;
    std::srand(std::time(nullptr)); // use current time as seed

    conv(l_GF, GF);
    ECC example = ECC(l_GF, a, b);
    G.x = 10;
    G.y = 5;
    if (example.setBasePoint(G) == 0) {
        Point M;
        long n = example.findStage();
        
        cout << "Alice inputs private key:";
        cin >> k;
        k = k % l_GF;
        Point pubK = example.mulBasePoint(k);
        cout << "Alice send Bob:(p,a,b,K,G)=("<<l_GF<<",";
        cout<<a<<",";
        cout<<b<<",";
        cout<<pubK<<",",
        cout << "("<<G.x<<","<<G.y<<") )" << endl;
        
        long r;
        while ((r=rand()%n)==0);
        cout<<"Bob generate rand r=" << r << endl;
        cout << "Bob input M.x M.y:";
        cin >> M.x >> M.y;
        cout <<"M=" << M << endl;
        Point C1 = example.addPoint(M, example.mulPoint(r, pubK));
        cout << "Bob calculates C1 = M+rK = "<< C1 <<endl;
        Point C2 = example.mulBasePoint(r);
        cout << "Bob calculates C2 = " << C2 << endl;
        cout << "Bob sends Alice: (C1,C2)=" << C1<<","<< C2 << endl; 
        Point M2;
        M2 = example.addPoint(C1, example.mulPoint(-k, C2));
        cout << "Alice decrypt M=C1-kC2=" << M2 << endl;
    } else {
        cout << "set base point false" << endl;
    }
}
