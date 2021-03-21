#include"header.h"
//#include<stack>

extern std::vector<City> cities;

Passenger::Passenger(int seqNum, int startTime, double pTIME, int departure, int destination, int strategy, int expectedTime, clock_t time, std::string name)
{
	this->seqNum = seqNum;
	this->startTime = startTime;
	preciseStartTIME = pTIME;
	this->strategy = strategy;
	this->state = WAITING;
	this->departure = departure;
	this->destination = destination;
	this->expectedTime = expectedTime;
	this->name = name;
	curCity = departure;
	x = cities[departure].GetX();
	y = cities[departure].GetY();
	idleResult.idleRisk = 0;
	idleResult.idleTime = 0;

	lastXMoveTime = time;
	lastYMoveTime = time;
	travelHours = 0;
	usedHours = 0;
	printStartTIME = startTime;
	res = 0;
}


int Passenger::GetStrategy() const
{
	return strategy;
}

int Passenger::GetCurCity() const
{
	return curCity;
}

int Passenger::GetDepart() const
{
	return departure;
}

int Passenger::GetDest() const
{
	return destination;
}

int Passenger::GetExpectedTime() const
{
	return expectedTime;
}

int Passenger::GetUsedHours() const
{
	return usedHours;
}

std::string Passenger::GetName() const
{
	return name;
}

int Passenger::GetState()const
{
	return state;
}

void Passenger::ChangeDest(int newDest, int newTime) //旅客变更计划
{
	destination = newDest;
	expectedTime = newTime;
	departure = curCity;//重新设定出发城市

	int unfinishedTime = 0;
	Vehicle* curMovingPath = nullptr;
	if (state == MOVING)
	{
		curMovingPath = curPath.back();
		unfinishedTime = curMovingPath->transTime - travelHours;
		preciseStartTIME = -1;//不会涉及到错过第一趟车
	}
	else
	{
		preciseStartTIME = preciseTIME;
		state = WAITING;
	}

	if (curCity == destination)
	{
		curPath.clear();
		minRiskPath.clear();
		minTimePath.clear();
		limitTimeMinRiskPath.clear();

		if (state == MOVING)
		{
			departure = curMovingPath->departID;

			curPath.push_back(curMovingPath);
			minRiskPath.push_back(curMovingPath);
			minTimePath.push_back(curMovingPath);
			limitTimeMinRiskPath.push_back(curMovingPath);

			idleResult.idleRisk = curMovingPath->transRisk;
			idleResult.idleTime = curMovingPath->transTime;
			usedHours = travelHours;
		}
		else
		{
			state = ARRIVE;

			idleResult.idleRisk = 0;
			idleResult.idleTime = 0;
			usedHours = 0;
		}

		PrintLog();
	}
	else
	{
		startTime = TIME + unfinishedTime;
		printStartTIME = TIME;

		MakePlan();
	}
}

void Passenger::ChangeStrategy(int newStrategy, int newTime)
{
	strategy = newStrategy;
	expectedTime = newTime;
	departure = curCity;//重新设定出发城市

	int unfinishedTime = 0;
	if (state == MOVING)
	{
		unfinishedTime = curPath.back()->transTime - travelHours;
		preciseStartTIME = -1;//不会涉及到错过第一趟车
	}
	else
	{
		preciseStartTIME = preciseTIME;
		state = WAITING;
	}

	startTime = TIME + unfinishedTime;
	printStartTIME = TIME;
	
	MakePlan();
}

void Passenger::MakePlan()
{
	//curPath.clear();
	minRiskPath.clear();
	minTimePath.clear();
	limitTimeMinRiskPath.clear();
	usedHours = 0;//已用时间清零

	switch (strategy)
	{
	case MIN_RISK:
		idleResult = MakeMinPath(MIN_RISK);
		break;
	case MIN_TIME:
		idleResult = MakeMinPath(MIN_TIME);
		break;
	case LIMIT_TIME_MIN_RISK:
		res = MakelimitTimeMinRiskPath();
		break;
	default:
		break;
	}

	PrintLog();
}

void Passenger::UpdateHours()
{
	if (state == ARRIVE)
		;
	else
	{
		usedHours++;
		if (state == MOVING)
		{
			travelHours++;
		}
		else;
	}

}

void Passenger::MovePassenger()//更新乘客坐标
{
	if (curPath.size() != 0)
	{
		int destID = curPath.back()->destID;
		int destX = cities[destID].GetX();
		int destY = cities[destID].GetY();

		if (state == WAITING)//如果乘客当前状态是等待，则不移动
		{
			int departTIME = curPath.back()->departTime;
			if (TIME == departTIME)//如果到了路径第一趟的出发时间，则更改旅客状态为移动
			{
				if (curPath.back()->departID == departure && preciseStartTIME > departTIME)//如果刚刚好添加乘客的时间和
				{
					preciseStartTIME = -1;
				}
				else
				{
					state = MOVING;
					lastXMoveTime = currentTime;
					lastYMoveTime = currentTime;
				}

			}
			else
				;//do nothing
		}
		else if (state == MOVING)//如果乘客当前状态是移动中，则判断是否到达更新时间，每次更新1像素
		{
			if (currentTime - lastXMoveTime >= curPath.back()->xSpeed)
			{
				lastXMoveTime = currentTime;
				if (destX < x)//如果下一站在当前位置/站的左边，x--
				{
					x--;
				}
				else if (destX > x)//如果下一站在当前位置/站的右边，x++
				{
					x++;
				}
				else
					;//do nothing
			}
			else
				;//do nothing

			if (currentTime - lastYMoveTime >= curPath.back()->ySpeed)
			{
				lastYMoveTime = currentTime;
				if (destY < y)//如果下一站在当前位置/站的上边，y--
				{
					y--;
				}
				else if (destY > y)//如果下一站在当前位置/站的下边，y++
				{
					y++;
				}
				else
					;//do nothing
			}
			else
				;//do nothing

			//判断是否到达下一站
			if (curPath.back()->transTime == travelHours)//时间到了才算到站
			{
				x = destX;
				y = destY;
				if (destID == destination)
					state = ARRIVE;
				else
					state = WAITING;

				travelHours = 0;//到站了就清零，为下一趟车计数做准备

				curPath.pop_back();
			}
			else//没到达下一站就把下一站当做当前城市
			{
				curCity = destID;
			}
		}

		else
			;//do nothing (state == ARRIVE)
	}
	else
		;//curPath为空就不处理
}

bool Passenger::whetherRemake() const
{
	if ((curPath.size() == 1 && state == MOVING) || state == ARRIVE)
	{
		return false;
	}
	else
	{
		return true;
	}
}
