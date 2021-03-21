#include"header.h"

Result Passenger::MakeMinPath(int stra)
{
	Vertex vertexes[CITY_NUM];

	vertexes[departure].setVertex(true, departure, static_cast<double>(0), 0, startTime);

	std::vector<int> markedCity;//��¼�Ѿ��̶��ĵ�
	markedCity.push_back(departure);

	double waitRisk = NOT_SET; //�ȴ�����
	double totalRisk = NOT_SET; //�ܷ��� = �ȴ�����+·;����
	int totalTime = NOT_SET; //��ʱ�� = �ȴ�ʱ��+·;ʱ��

	bool isFound = false; //�Ƿ�̶��յ�
	int countMark = 1;


	while (countMark < CITY_NUM && isFound == false)//��ǳ���С���ܳ��������յ�û�б��̶��Ž���ѭ��
	{
		int minTime = MAX_COST;//������Ѱ��ʱ����С�ı�
		double minRisk = MAX_COST;//������Ѱ�з�����С�ı�
		int nextMarkIndex = departure; //��һ��Ҫ�̶��ĵ����ţ���������С���Ǹ�������
		int curMarked;

		//��ÿ��mark���ж����б�����Ѱ��Ŀǰ�ɴ����̵�һ��·�������Ѹ�·����ĩ�����mark��
		for (unsigned int i = 0; i < markedCity.size(); i++)
		{
			curMarked = markedCity[i];
			//�Ը�mark���е������ھӽ��б���	
			for (int curNeighbor = 0; curNeighbor < CITY_NUM; curNeighbor++)
			{
				int totalWays = cities[curMarked].neighbor[curNeighbor].size(); //��mark���и�����ĳ�����ھ�֮�������0�����·��
				if (vertexes[curNeighbor].isMarked == false && totalWays > 0)//����Ƿǹ̶��ĵ��Ҹõ���·���Ž��м���
				{
					int minWayIndex = 0;		  //���ж���·������̵�����·�����
					double minWayRisk = MAX_COST; //��̵�����·������
					int minWayTime = MAX_COST; //��̵�����·������

					//��ÿ��·�����б���,Ȼ��ѡ��������С������
					for (int curWay = 0; curWay < totalWays; curWay++)
					{
						int time1 = cities[curMarked].neighbor[curNeighbor][curWay].departTime;
						int waitTime = time1 - vertexes[curMarked].startWaitTime;
						if (waitTime < 0 || (curMarked == departure && waitTime == 0 && preciseStartTIME > static_cast<double>(time1)))
						{
							//���ȴ�ʱ��С��0�����������ˣ�����24Сʱ���߿�ʼʱ���ĳ�˳���ͬ����ʱ���Ѿ������ˣ�ֻ�ܵȵڶ�������˳�
							waitTime += 24;
						}
						waitRisk = waitTime * cities[curMarked].GetRisk(); //�ȴ����� = �ȴ�ʱ��*���з���
						totalRisk = waitRisk + cities[curMarked].neighbor[curNeighbor][curWay].transRisk;//�ܷ��� = �ȴ�����+·;����
						totalTime = waitTime + cities[curMarked].neighbor[curNeighbor][curWay].transTime;//��ʱ�� = �ȴ�ʱ��+·;ʱ��

						if (stra == MIN_RISK)//���ǲ���1
						{
							if (totalRisk < minWayRisk)//ѡ�����·��������¼���
							{
								minWayRisk = totalRisk;
								minWayIndex = curWay;
								minWayTime = totalTime;
							}
						}
						if (stra == MIN_TIME)//���ǲ��Զ�
						{
							if (totalTime < minWayTime)//ѡ�����·��������¼���
							{
								minWayTime = totalTime;
								minWayIndex = curWay;
								minWayRisk = totalRisk;
							}
						}

					}

					//����Ը�mark�������м�ڵ�����и�С�Ŀ����������
					if (stra == MIN_RISK)//���ǲ���1
					{
						if ((vertexes[curMarked].riskToDepart + minWayRisk) < vertexes[curNeighbor].riskToDepart)
						{
							vertexes[curNeighbor].riskToDepart = vertexes[curMarked].riskToDepart + minWayRisk;
							vertexes[curNeighbor].timeToDepart = vertexes[curMarked].timeToDepart + minWayTime;
							vertexes[curNeighbor].preVertex = curMarked;
							vertexes[curNeighbor].startWaitTime = cities[curMarked].neighbor[curNeighbor][minWayIndex].arrivalTime;
							vertexes[curNeighbor].minWayIndex = minWayIndex;
						}
						else
							;

						if (vertexes[curNeighbor].riskToDepart < minRisk)
						{
							minRisk = vertexes[curNeighbor].riskToDepart;
							nextMarkIndex = curNeighbor;
						}
						else
							;
					}
					else//���ǲ��Զ�
					{
						if ((vertexes[curMarked].timeToDepart + minWayTime) < vertexes[curNeighbor].timeToDepart)
						{
							vertexes[curNeighbor].timeToDepart = vertexes[curMarked].timeToDepart + minWayTime;
							vertexes[curNeighbor].riskToDepart = vertexes[curMarked].riskToDepart + minWayRisk;
							vertexes[curNeighbor].preVertex = curMarked;
							vertexes[curNeighbor].startWaitTime = cities[curMarked].neighbor[curNeighbor][minWayIndex].arrivalTime;
							vertexes[curNeighbor].minWayIndex = minWayIndex;
						}

						if (vertexes[curNeighbor].timeToDepart < minTime)
						{
							minTime = vertexes[curNeighbor].timeToDepart;
							nextMarkIndex = curNeighbor;
						}

					}

				}//end if

				else//����ǹ̶��ĵ㣬������
					;
			}//end for (������mark���е��ھ�)

		}//end for (��������mark����)

		//�̶���С�����ĵ�,����ɶ�Ӧ����
		if (nextMarkIndex != departure)
		{
			markedCity.push_back(nextMarkIndex);
			vertexes[nextMarkIndex].isMarked = true;
			countMark++;
		}

		//����¹̶��ĵ����յ㣬�����˳�ѭ��
		if (nextMarkIndex == destination)
		{
			isFound = true;
		}
	}//end while


	Vehicle* unfinishedPath = NULL;
	int unfinishedTime = 0;
	double unfinishedRisk = 0;
	if (state == MOVING)
	{
		unfinishedPath = curPath.back();
		unfinishedTime = unfinishedPath->transTime - travelHours;
		unfinishedRisk = static_cast<double>(unfinishedTime) * unfinishedPath->risk * cities[unfinishedPath->departID].GetRisk();
	}
	curPath.clear();

	//��·��������
	int curVertex = destination;
	while (curVertex != departure)
	{
		int preVertex = vertexes[curVertex].preVertex;
		int way = vertexes[curVertex].minWayIndex;

		if (stra == MIN_RISK)
			minRiskPath.push_back(&cities[preVertex].neighbor[curVertex][way]);
		else
			minTimePath.push_back(&cities[preVertex].neighbor[curVertex][way]);

		curPath.push_back(&cities[preVertex].neighbor[curVertex][way]);
		curVertex = preVertex;
	}

	Result res;
	res.idleRisk = vertexes[destination].riskToDepart + unfinishedRisk;
	res.idleTime = vertexes[destination].timeToDepart + unfinishedTime;
	if (state == MOVING)
	{
		curPath.push_back(unfinishedPath);

		if (stra == MIN_RISK)
			minRiskPath.push_back(unfinishedPath);
		else
			minTimePath.push_back(unfinishedPath);
	}

	return res;
}

int Passenger::MakelimitTimeMinRiskPath()
{
	double upperRiskBound; // �������ֵ���Ͻ�
	double minRisk;    //���������ķ���ֵ
	int timeOfMinRisk; //���������ĺ�ʱ
	double curRisk = 0;
	int curTime = 0;

	StackInfo lastPop;//��һ��������ջ��Ԫ��
	lastPop.cityID = 0;
	lastPop.wayFromLast = -1;

	bool whetherHasNext = false; // �Ƿ�����һ���ڵ�
	bool isVistiedCity[CITY_NUM];//ĳ�������Ƿ񱻷��ʹ�
	int startWaitTime[CITY_NUM]; // ����ó��е�ʱ��
	for (int i = 0; i < CITY_NUM; i++) //��ʼ��
	{
		startWaitTime[i] = 0;
		isVistiedCity[i] = false;
	}

	Vehicle* unfinishedPath = NULL;
	int unfinishedTime = 0;//�ƶ��е��г�/����δ��ɵ�ʱ��
	double unfinishedRisk = 0;
	if (state == MOVING)
	{
		unfinishedPath = curPath.back();
		unfinishedTime = unfinishedPath->transTime - travelHours;
		unfinishedRisk = static_cast<double>(unfinishedTime) * unfinishedPath->risk * cities[unfinishedPath->departID].GetRisk();
	}

	std::vector<StackInfo> tempSolution; //����ջʵ����ȱ�������ջ�д�ŵĵ����� �����ýڵ���ھӽڵ�ʱ�ܹ����ַ���Լ���Ŀ��н�
	std::vector<StackInfo> finalSolution;

	Result tempRes;

	tempRes = MakeMinPath(MIN_TIME);
	upperRiskBound = tempRes.idleRisk - unfinishedRisk;//����Сʱ������������ֵ������,������ƶ�״̬���ǵü�ȥû������Ƕ�·
	if (tempRes.idleTime > expectedTime)//����Сʱ����Ե���ʱ����������ֵ�����޽�
	{
		limitTimeMinRiskPath.assign(minTimePath.begin(), minTimePath.end());//�����ʱ���·�߸��Ƶ���ʱ·���У��Ѿ��������ƶ�δ��������
		idleResult = tempRes;

		return -1;
	}

	tempRes = MakeMinPath(MIN_RISK);   //����С���ղ��������ʼ��
	minRisk = tempRes.idleRisk - unfinishedRisk;//��ʼ��ķ���ֵ,������ƶ�״̬���ǵü�ȥû������Ƕ�·
	timeOfMinRisk = tempRes.idleTime;//��ʼ��ĺ�ʱ
	if (timeOfMinRisk <= expectedTime) //�Ƚ��г����жϣ�����С���ղ��Եĺ�ʱ����ʱ��Լ����������ý�������Ž�
	{
		limitTimeMinRiskPath.assign(minRiskPath.begin(), minRiskPath.end());
		idleResult = tempRes;
		return 1;
	}
	else
	{
		StackInfo newStackElem;
		newStackElem.cityID = departure;
		newStackElem.wayFromLast = NOT_SET;
		tempSolution.push_back(newStackElem);// �Ȱ������ջ

		startWaitTime[departure] = startTime;
		isVistiedCity[departure] = true;

		while (!tempSolution.empty()) // ��ջ��Ϊ�գ������ѭ��
		{
			int curCity = tempSolution.back().cityID;//��ǰҪ�����Ľڵ㣨��Ϊ���ڵ㣩Ϊջ��
			bool whetherContinue = true;//����Ƿ�Ҫ���������ҵ���һ���㼰���Ӧ�ı���ջ�����˳�ѭ��

			//�Ըñ����ڵ������û�����������ھӽڵ���б���
			for (int curNeighbor = 0; curNeighbor < CITY_NUM && whetherContinue; curNeighbor++)
			{
				if (curNeighbor >= lastPop.cityID && isVistiedCity[curNeighbor] == false)// С����һ��������ջ�������õ㱻��������
				{
					int totalWays = cities[curCity].neighbor[curNeighbor].size(); //��mark���и�����ĳ�����ھ�֮������ж���·��

					//��ÿ��·�����б���
					for (int curWay = 0; curWay < totalWays && whetherContinue; curWay++)
					{
						if ((curNeighbor != lastPop.cityID) || (curNeighbor == lastPop.cityID && curWay > lastPop.wayFromLast))//С�ڵ�����һ��������ջ�������ñ߱���������
						{
							int time1 = cities[curCity].neighbor[curNeighbor][curWay].departTime;
							int waitTime = time1 - startWaitTime[curCity];//�ȴ�ʱ��
							if (waitTime < 0 || (curCity == departure && waitTime == 0 && preciseStartTIME > static_cast<double>(time1)))//���ȴ�ʱ��С��0�����������ˣ�����24Сʱ
							{
								waitTime += 24;
							}
							double waitRisk = waitTime * cities[curCity].GetRisk(); //�ȴ����� = �ȴ�ʱ��*���з���
							double totalRisk = waitRisk + cities[curCity].neighbor[curNeighbor][curWay].transRisk;//�ܷ��� = �ȴ�����+·;����
							int totalTime = waitTime + cities[curCity].neighbor[curNeighbor][curWay].transTime;//��ʱ�� = �ȴ�ʱ��+·;ʱ��

							if (curRisk + totalRisk > upperRiskBound || curTime + totalTime > (expectedTime - unfinishedTime))
								;  //��������������֦����������
							else
							{
								//����������
								startWaitTime[curNeighbor] = cities[curCity].neighbor[curNeighbor][curWay].arrivalTime;
								curRisk += totalRisk;
								curTime += totalTime;

								//���õ�ñ߽�ջ
								newStackElem.cityID = curNeighbor;
								newStackElem.wayFromLast = curWay;
								newStackElem.riskFromLast = totalRisk;
								newStackElem.TimeFromlast = totalTime;
								tempSolution.push_back(newStackElem);
								whetherContinue = false;//����ѭ��

								if (curNeighbor == destination)//�жϸ��ֽڵ��Ƿ�Ϊ�յ�,�����ӽڵ�Ϊ�յ㣬��ôα������һ�����������ĸ��ųɱ�·�������·���ֵ�Ͻ�
								{
									minRisk = curRisk;
									upperRiskBound = minRisk;
									timeOfMinRisk = curTime;

									finalSolution.clear();
									finalSolution.assign(tempSolution.begin(), tempSolution.end());
								}
								else //�����յ�
								{
									whetherHasNext = true; //�ҵ�����һ���ڵ�
									isVistiedCity[curNeighbor] = true;//�õ���Ϊ�Ѿ�������
									lastPop.cityID = -1;
									lastPop.wayFromLast = -1;
								}

							}

						}//end of if(curWay is not visited)
					}//end of for (curWay)
				}
			}//end of for(curNeighbor)

			if (whetherHasNext == false) //�ôα���û�еõ���һ�ڵ�
			{
				lastPop = tempSolution.back();
				tempSolution.pop_back();
				if (!tempSolution.empty())
				{
					curCity = tempSolution.back().cityID;
					isVistiedCity[lastPop.cityID] = false;
					curRisk -= lastPop.riskFromLast;
					curTime -= lastPop.TimeFromlast;
				}
				else
					;//do nothing
			}
			else
				;//do nothing
			whetherHasNext = false;
		}//end of while

		curPath.clear();

		//������·������
		while (finalSolution.back().cityID != departure)
		{
			int curVertex = finalSolution.back().cityID;
			int way = finalSolution.back().wayFromLast;
			finalSolution.pop_back();
			//if()
			int preVertex = finalSolution.back().cityID;

			limitTimeMinRiskPath.push_back(&cities[preVertex].neighbor[curVertex][way]);
			curPath.push_back(&cities[preVertex].neighbor[curVertex][way]);//???????
		}
		if (state == MOVING)//������ƶ�״̬���ǵ�û������Ƕ�·
		{
			curPath.push_back(unfinishedPath);
			limitTimeMinRiskPath.push_back(unfinishedPath);
		}
		else
			;//do nothing

		idleResult.idleRisk = minRisk + unfinishedRisk;
		idleResult.idleTime = timeOfMinRisk + unfinishedTime;

		return 3;
	}//end of else

}

Vehicle::Vehicle(int kind, int departID, int departTime, int destID, int arrivalTime, int days, std::string number)
{
	this->kind = kind;

	this->departID = departID;
	this->destID = destID;
	this->departTime = departTime;
	this->arrivalTime = arrivalTime;
	this->days = days;
	this->number = number;

	switch (kind)
	{
	case CAR:
		risk = 2;
		break;
	case TRAIN:
		risk = 5;
		break;
	case AIR:
		risk = 9;
	default:
		break;
	}

	transTime = arrivalTime - departTime + days * 24;
	transRisk = static_cast<double>(transTime) * risk * cities[departID].GetRisk();

	int xDistance = abs(cities[departID].GetX() - cities[destID].GetX());
	int yDistance = abs(cities[departID].GetY() - cities[destID].GetY());

	xSpeed = transTime * TIME_DURATION / xDistance;
	ySpeed = transTime * TIME_DURATION / yDistance;

}


Vertex::Vertex()
{
	isMarked = false;
	minWayIndex = 0;
	preVertex = NOT_SET;
	riskToDepart = MAX_COST;
	startWaitTime = NOT_SET;
	timeToDepart = MAX_COST;
}
void Vertex::setVertex(bool isMarked, int preVertex, double riskToDepart, int timeToDepart, int startWaitTime)
{
	this->isMarked = isMarked;
	this->preVertex = preVertex;
	this->riskToDepart = riskToDepart;
	this->timeToDepart = timeToDepart;
	this->startWaitTime = startWaitTime;
}


City::City(int id, std::string name, int x, int y, double risk)
{
	this->id = id;
	this->name = name;
	this->x = x;
	this->y = y;
	this->risk = risk;
}
int City::GetID() const
{
	return id;
}
int City::GetX()const
{
	return x;
}
int City::GetY()const
{
	return y;
}
double City::GetRisk() const
{
	return risk;
}
std::string City::GetCityName() const
{
	return name;
}
void City::AddNeighbor(int kind, int departID, int departTime, int destID, int arrivalTime, int days, std::string number)
{
	Vehicle newNeighbor{ kind, departID, departTime, destID, arrivalTime, days, number };
	neighbor[newNeighbor.destID].push_back(newNeighbor);
}