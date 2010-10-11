#include <iostream>
#include <vector>

using namespace std;

template < typename Cont>
struct  ContainerTraits
{
	typedef typename Cont::Elem  Elem;
	typedef typename Cont::Temp  Temp;
	typedef typename Cont::Ptr   Ptr;
};

template < typename T>
struct  ContainerTraits<T *>
{
	typedef T  Elem;
	typedef T  Temp;
	typedef T  *Ptr;
};

template < class T>
struct  ContainerTraits< std::vector<T> >
{
	typedef typename std::vector<T>::value_type  Elem;
	typedef typename std::iterator_traits<typename std::vector<T>::iterator>::value_type  Temp;
	typedef typename std::iterator_traits<typename std::vector<T>::iterator>::pointer   Ptr;
};

template <typename Container>
typename ContainerTraits<Container>::Elem  process( Container & c , int size )
{
	typename ContainerTraits<Container>::Temp  temp = typename ContainerTraits<Container>::Temp();

	for ( int i = 0 ; i < size ; i++ )
	{
		temp += c[i];
	}
	return temp;
}


int main()
{
	std::vector<std::string> string2;

	//string aString = process(  string2 , string2.size() );

	double   reading [100];

	//double  r = process( reading,100);
	return 0;
}