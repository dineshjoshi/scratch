/**
 * This program sends g++ in a tizzy. 
 * Original source:
 * http://miek.nl/downloads/2010/c++-talk.pdf?we-require-more-discussions
 *
 */

struct a{typedef int foo;};struct a1:a{};struct a2:a{};

#define X(b,a) struct a##1:b##1,b##2{};struct a##2:b##1,b##2{};

X(a,b)X(b,c)X(c,d)X(d,e)X(e,f)X(f,g)X(g,h)X(h,i)X(i,j)X(j,k)X(k,l)

X(l,m)X(m,n) n1::foo main(){}
