#include <cassert>
int main ()
{
    char c;
    long l;
    int i;
    l = c = i;
    assert (l==i);
    return 0;
}