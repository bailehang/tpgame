
#pragma  once 


enum  eRangetype
{
	eRange_No,
	eRange_Circ,
	eRange_Point,
	eRange_Team,
	eRange_Seror,
};

enum Range_Circ
{

};

enum Range_Point
{
	eRangPoint,
};



class range
{	
public:
	range( skillparam * param ) { }

protected:
	skillparam  *m_param;
};

class circ1 : public  range 
{
public:
	circ1( skillparam * param ) : range( param )
	{

	}	

private:

};

class circ2 : public  range 
{
public:
	circ2( skillparam * param ) : range( param )
	{

	}	

private:

};


class point : public range
{
public:
	point( skillparam * param , POSITION pos) : range ( param )
	{
		m_pos = pos;
	}

	void  Execute( std::list<CMoveShape*> &listobj)
	{				  
		if ( m_pos == Pos_Self )
		{
			//return m_param->pFire;
			listobj.push_back( m_param->pFire );
		}

	}

private:
	POSITION	m_pos;
};

