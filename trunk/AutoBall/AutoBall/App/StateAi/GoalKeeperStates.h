
#pragma  once 

#include <string>
#include "State.h"




/// 全局队员状态
CREATESTATE(GlobalKeeperState,Telegram,GoalKeeper);

/// 守球门
CREATESTATE(TendGoal,Telegram,GoalKeeper);

/// 截球
CREATESTATE(InterceptBall,Telegram,GoalKeeper);

/// 归位
CREATESTATE(ReturnHome,Telegram,GoalKeeper);

/// 球传回到赛场
CREATESTATE(PutBallBackInPlay,Telegram,GoalKeeper);

