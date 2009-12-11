/*
 *	 Ai系统的公共部分定义
 */

#pragma  once

#include "stdafx.h"

/*
 *	 对于2维的行走坐标
 */
const  int	RunWay_X [9] = { 0,-1,-1,-1, 0, 1, 1, 1, 0 };
const  int	RunWay_Y [9] = { 1, 1, 0,-1,-1,-1, 0, 1, 0 };

/*
 *	 对于一个攻击目标，方向最大值
 */
const  int  Max_DirSize =  8;

/*
 *	　Ai 操作返回
 */
enum OPERATE_RESULT
{
	OR_OK								=  0,	// 成功
	OR_ERROR							= -1,	// 未知错误
	OR_DIE								= -2,	// 你已死亡
	OR_TARGET_DIE						= -3,	// 目标已死亡
	OR_LACK_MANA						= -4,	// 条件不足
	OR_NO_TARGET						= -5,	// 没有目标
	OR_INVALID_TARGET					= -6,	// 无效目标
	OR_OUT_RANGE						= -7,	// 超出范围
	OR_IN_RANGE							= -8,	// 距离太近
	OR_BUSY								= -9,	// 正在做其它事情
};

//  求出目标nToX,nToY的方向
inline int  GetDirByPos(int nFromX, int nFromY, int nToX, int nToY)
{
	if(nFromX < nToX)
	{
		if(nFromY < nToY)
			return 7;
		else if(nFromY > nToY)
			return 5;
		else
			return 6;
	}
	else if(nFromX > nToX)
	{
		if(nFromY < nToY)
			return 2;
		else if(nFromY > nToY)
			return 4;
		else
			return 3;
	}
	else // if(nFromX == nToX)
	{
		if(nFromY < nToY)
			return 0;
		else if(nFromY > nToY)
			return 4;
	}
	return Max_DirSize;
}
