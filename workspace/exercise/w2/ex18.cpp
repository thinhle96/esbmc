#include <cassert>
int nondet_int();
int x, y;
int main ()
{
 x = nondet_int();
 y = x+1;
 assert (y>x);
 return 0;
}