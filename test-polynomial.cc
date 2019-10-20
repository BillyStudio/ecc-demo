#include <NTL/ZZXFactoring.h>

using namespace std;
using namespace NTL;

int main()
{
   ZZX f;

   cin >> f;

   Vec< Pair< ZZX, long > > factors;
   ZZ c;

   factor(c, factors, f);

   cout << c << "\n";
   cout << factors << "\n";
}
