#pragma once
#include<iostream>
#include<vector>
#include"Graph.h"


class Passenger
{
	int seqNum;		 //第几位乘客
	std::string name;//乘客姓名
	int startTime;	 //乘客制定计划的时间
	double preciseStartTIME;//精确到小数的乘客制定计划的时间
	int departure;	//起点城市
	int destination;//终点城市
	int strategy;	//选择的策略 1:最小风险；2:最短时间；3：限时最小风险
	int state;		//乘客的状态  1等待出发/2到达目的地/3在路上
	int x;			//在图上的x坐标
	int y;			//在图上的y坐标
	int curCity;	//旅客当前所在城市,更改方案的时候会用到
	int expectedTime; //限定的时间
	Result idleResult; //理想的结果

	int travelHours;//某一趟车已用耗时（小时
	int usedHours;//已用时间
	int printStartTIME;//打印用的出发时间
	int res;

	//存路径，终点的数组下标是0，起点在栈顶
	std::vector<Vehicle*> minRiskPath;//最少风险策略的路径
	std::vector<Vehicle*> minTimePath;//最少时间策略的路径
	std::vector<Vehicle*> limitTimeMinRiskPath;//限时最少风险策略的路径
	std::vector<Vehicle*> curPath;//当前还未完成的路径

public:
	clock_t lastXMoveTime;//上一次x移动的时间（非小时，指毫秒
	clock_t lastYMoveTime;//上一次y移动的时间（非小时，指毫秒

	Passenger(int seqNum, int startTime,double pTIME, int departure, int destination, int strategy, int expectedTime, clock_t time, std::string name);
	int GetStrategy()const;
	int GetCurCity()const;
	int GetDepart()const;
	int GetDest()const;
	int GetExpectedTime()const;
	int GetUsedHours()const;
	int GetState()const;
	std::string GetName()const;
	void ChangeDest(int newDest, int newTime);//旅客变更计划
	void ChangeStrategy(int newStrategy, int newTime);//旅客变更策略
	void UpdateHours();//更新已经用的时间
	void MakePlan(); //制定方案
	void MovePassenger();//更新乘客位置及状态
	Result MakeMinPath(int stra);//制定最少时间/风险策略的路线方案
	int MakelimitTimeMinRiskPath();//制定限时最少风险策略的路线方案
	void Print()const;//常规模式下打印乘客信息（到动画界面）
	void PrintWhenChange()const;//更改旅行方案模式下打印的乘客信息（到动画界面）
	void PrintLog()const;//打印乘客信息到日志中
	bool whetherRemake()const;
};
