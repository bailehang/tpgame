#pragma  once

#define GetInst(CLASS_NAME) \
	CSingleton<CLASS_NAME>::Inst()

template < class  class_name>
class CSingleton
{
	typedef	class_name   ctype;
public:
	static ctype& Inst()
	{
		static ctype obj;
		return obj;
	}
private:
	CSingleton(){}
	~CSingleton(){}
	CSingleton(const CSingleton&);
	CSingleton &operator = (const CSingleton&);          
};