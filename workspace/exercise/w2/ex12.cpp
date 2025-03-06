#include <cassert>
void f (int a)
{
 if (a == 0)
   assert (1);
 else f (a - 1);
}
int main ()
{
 f(5);
 return 0;
}
