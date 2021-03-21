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

void Passenger::ChangeDest(int newDest, int newTime) //�ÿͱ���ƻ�
{
	destination = newDest;
	expectedTime = newTime;
	departure = curCity;//�����趨��������

	int unfinishedTime = 0;
	Vehicle* curMovingPath = nullptr;
	if (state == MOVING)
	{
		curMovingPath = curPath.back();
		unfinishedTime = curMovingPath->transTime - travelHours;
		preciseStartTIME = -1;//�����漰�������һ�˳�
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
	departure = curCity;//�����趨��������

	int unfinishedTime = 0;
	if (state == MOVING)
	{
		unfinishedTime = curPath.back()->transTime - travelHours;
		preciseStartTIME = -1;//�����漰�������һ�˳�
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
	usedHours = 0;//����ʱ������

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

void Passenger::MovePassenger()//���³˿�����
{
	if (curPath.size() != 0)
	{
		int destID = curPath.back()->destID;
		int destX = cities[destID].GetX();
		int destY = cities[destID].GetY();

		if (state == WAITING)//����˿͵�ǰ״̬�ǵȴ������ƶ�
		{
			int departTIME = curPath.back()->departTime;
			if (TIME == departTIME)//�������·����һ�˵ĳ���ʱ�䣬������ÿ�״̬Ϊ�ƶ�
			{
				if (curPath.back()->departID == departure && preciseStartTIME > departTIME)//����ոպ���ӳ˿͵�ʱ���
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
		else if (state == MOVING)//����˿͵�ǰ״̬���ƶ��У����ж��Ƿ񵽴����ʱ�䣬ÿ�θ���1����
		{
			if (currentTime - lastXMoveTime >= curPath.back()->xSpeed)
			{
				lastXMoveTime = currentTime;
				if (destX < x)//�����һվ�ڵ�ǰλ��/վ����ߣ�x--
				{
					x--;
				}
				else if (destX > x)//�����һվ�ڵ�ǰλ��/վ���ұߣ�x++
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
				if (destY < y)//�����һվ�ڵ�ǰλ��/վ���ϱߣ�y--
				{
					y--;
				}
				else if (destY > y)//�����һվ�ڵ�ǰλ��/վ���±ߣ�y++
				{
					y++;
				}
				else
					;//do nothing
			}
			else
				;//do nothing

			//�ж��Ƿ񵽴���һվ
			if (curPath.back()->transTime == travelHours)//ʱ�䵽�˲��㵽վ
			{
				x = destX;
				y = destY;
				if (destID == destination)
					state = ARRIVE;
				else
					state = WAITING;

				travelHours = 0;//��վ�˾����㣬Ϊ��һ�˳�������׼��

				curPath.pop_back();
			}
			else//û������һվ�Ͱ���һվ������ǰ����
			{
				curCity = destID;
			}
		}

		else
			;//do nothing (state == ARRIVE)
	}
	else
		;//curPathΪ�վͲ�����
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
