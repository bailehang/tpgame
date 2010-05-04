# include <iostream>
# include <cstdlib>
using namespace std;


class B
{
public:
	  void print()
	  {
		  cout <<" this is B "<<endl;
	  }
	  virtual B * clone()
	  {
		  return new B(*this);
	  }
};

class D : public B
{
  public:
	  void print()
	  {
		  cout <<" this is D "<<endl;
	  }
	  virtual B * clone()
	  {
		  return new D(*this);
	  }    

};

int main()
{
    
    D * d = new D;
    d->print();
    B * b = d->clone();
    b->print();
    D * k = (D*)d->clone();
    k->print();
    
	cout << typeid(*b).name <<endl;

	cout << typeid(B).name <<endl;

	if (typeid(*b) == typeid(D))
	{
		cout <<" aa " <<endl;
	}
	//typeid(*k) == typeid(D);
   // B * b = new B;
   // b->print();
    system("pause");
}
