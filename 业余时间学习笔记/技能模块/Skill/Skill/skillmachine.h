

template <typename T>
class  skillmachine
{
	typedef  T  type;

public:

	void  Enter();

	void  Exit();

	void  Execute();

	void  OnEvent();

private:

	type		*m_type;
	
};