#include <iostream>
#include <vector>
#include <algorithm>
#include <type_traits>

using namespace std;

template <int N, typename T, typename F>
void operator | ( T (&tab)[N], F f ) {
  for( auto x : tab )
    f( x );
}

template <typename T, typename F>
void operator | ( T tab, F f ) {
  for( auto x : tab )
    f( x );
}

struct Functor {
  const static bool True = true;
};

struct Var : public Functor {
  template <typename T>
  T operator()( T valor ) {
    return valor;
  }
};
Var x;
  
template <typename Op1, typename Op2>  
struct Multiplicador {
  Op1 op1;
  Op2 op2;
  
  Multiplicador( Op1 op1, Op2 op2 ) : op1(op1), op2(op2) {}
  
  template <class T> 
  auto operator()( T valor ) -> decltype( op1( valor ) * op2( valor ) ) {
    return op1( valor ) * op2( valor );
  }
};

template <typename Op1, typename Op2>
inline Multiplicador< typename enable_if<Op1::True, Op1>::type, Op2 > 
operator * ( Op1 a, Op2 b ) {
  return Multiplicador< Op1, Op2>( a, b );
}

struct Pipe {
  template <class A, class B> 
  auto operator()( A a, B b ) -> decltype( a << b ) {
    return a << b;
  }
};

template <typename A>
inline Pipe operator | ( A a, Var b ) {
  return Pipe();
}

int main() {
  int tab[] =  { 1, 2, 3, 4 };
  vector<int> v;
    
  tab | [ &v ]( int x ) { v.push_back( x ); };
    
  auto op = x * x;
  v | [ &op ]( int n ) { cout << op( n ) << endl; };

  return 0;  
}