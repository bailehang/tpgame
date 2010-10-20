
#pragma  once 

/// 关于是否准备进行下一次踢球
class TimeCount
{

public:

	TimeCount(double Updates)  ;

	bool isReadyOK();

private:

	/// 2次更新间隔
	double m_dUpdateNum;

	/// 下一次更新时间
	DWORD m_dwNextUpdateTime;
};
