#include <cassert>
char s[] = "abc";
void main(void)
{
 char *p = s;
 p[1] = 'y';
 assert (s[1]=='y');
}