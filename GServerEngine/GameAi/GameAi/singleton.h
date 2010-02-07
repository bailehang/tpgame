#pragma  once

#define GetInstObj(CLASS_NAME) \
	CSingleton<CLASS_NAME>::InstObj()

#define GetInstObjP(CLASS_NAME) \
	CSingleton<CLASS_NAME>::InstObjP();

template < class  class_name>
class CSingleton
{
	typedef	class_name   ctype;
public:
	static ctype& InstObj()
	{
		static ctype obj;
		return obj;
	}

	static ctype* InstObjP()
	{
		static ctype obj;
		return &obj;
	}
private:
	CSingleton(){}
	~CSingleton(){}
	CSingleton(const CSingleton&);
	CSingleton &operator = (const CSingleton&);          
};