# include <iostream>
# include <cstdlib>
# include <fstream>

using namespace std;

#define THISTESTSING
#define GetInst(CLASS_NAME) \
		Singleton<CLASS_NAME>::Inst()

template<typename T>
class THISTESTSING Singleton
{
      public:
			 static T & Inst()
			 {
				 static T obj;
				 return obj;
			 }
             int operator() () 
             {
                  cout << "this is test " <<std::endl;
				  return 0;
			 }
      private:
		  Singleton(){}
		  ~Singleton(){}
		  Singleton(const Singleton&);
		  Singleton &operator = (const Singleton&);          
};


template<typename T>
class CMTSingleton
{
	
	public:
		static T & Inst()
	    {
			 static T obj;
			 return obj;
		}
	private:
		CMTSingleton(){}
		~CMTSingleton(){};
		CMTSingleton(const CMTSingleton &);
		CMTSingleton &operator = (const CMTSingleton &);

		class CInstCreator
		{
			public:
			  /// 保证在main进入之前，Inst()已经被调用.
			  CInstCreator(void)	{ CMTSingleton<T>::Inst() ;}
			  inline  void DoNothing()(void) const {}
		};

	static CInstCreator creator;
};

template<typename T>
typename CMTSingleton<T>::CInstCreator CMTSingleton<T>::creator;


class  test
{
public:
	int num;

};

int main()
{
    Singleton<test>::Inst().num =5;
    //t();
    cout << GetInst(test).num <<" " << &GetInst(test).num <<endl;
	GetInst(test).num =10;
	cout << GetInst(test).num <<" " << &GetInst(test).num <<endl;
    system("pause");   
    return 0;
}
