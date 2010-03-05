#pragma  once

/// 得到当前时间，精确到毫秒
inline  ulong GetCurrSystemTime()
{
	SYSTEMTIME  sys;
	GetLocalTime(&sys);

	tm      time;
	time.tm_year = sys.wYear - 1900;
	time.tm_mon  = sys.wMonth-1;
	time.tm_mday = sys.wDay;
	time.tm_hour = sys.wHour;
	time.tm_min  = sys.wMinute;
	time.tm_sec  = sys.wSecond;
	return (ulong)(_mktime64(&time) * 1000 + sys.wMilliseconds);
}

/// 定时器指针结点
struct ListNode
{
	ListNode *next,*prev;
};

///
/// 定时器类型
/// 
enum eTimerType
{
	eTimer1 = 10,
	eTimer2 ,
	eTimer3 
};

/// 
/// 定时器结点,tlist表示结点的指针,expires循环周期时间
/// etime 触发周期时间,pFun触发函数.
/// 
struct timernode
{
	ListNode    tlist;
	ulong       expires;
	ulong       etime;
	void        *pFun;
	eTimerType  eType;
};


/// 游戏事件基类
class   CGameEvent
{
public:
	virtual  long  TimeOut( eTimerType type) = 0;
};