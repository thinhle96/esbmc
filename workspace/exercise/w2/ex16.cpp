void f (unsigned int n)
{
    int *p = new int[n]; 
    p[n-1] = 0;
    delete[] p; 
}
