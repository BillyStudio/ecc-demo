#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

int main()
{
   ZZ a, b, c;

   a = conv<ZZ>("A");
   b = conv<ZZ>("B");
   c = (a+1)*(b+1);
   cout << c << "\n";
}
