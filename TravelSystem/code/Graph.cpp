#include"header.h"

Result Passenger::MakeMinPath(int stra)
{
	Vertex vertexes[CITY_NUM];

	vertexes[departure].setVertex(true, departure, static_cast<double>(0), 0, startTime);

	std::vector<int> markedCity;//记录已经固定的点
	markedCity.push_back(departure);

	double waitRisk = NOT_SET; //等待风险
	double totalRisk = NOT_SET; //总风险 = 等待风险+路途风险
	int totalTime = NOT_SET; //总时间 = 等待时间+路途时间

	bool isFound = false; //是否固定终点
	int countMark = 1;


	while (countMark < CITY_NUM && isFound == false)//标记城市小于总城市数且终点没有被固定才进入循环
	{
		int minTime = MAX_COST;//本轮搜寻中时间最小的边
		double minRisk = MAX_COST;//本轮搜寻中风险最小的边
		int nextMarkIndex = departure; //下一个要固定的点的序号，即距离最小的那个点的序号
		int curMarked;

		//对每个mark城市都进行遍历，寻找目前可达的最短的一条路径，并把该路径的末点加入mark里
		for (unsigned int i = 0; i < markedCity.size(); i++)
		{
			curMarked = markedCity[i];
			//对该mark城市的所有邻居进行遍历	
			for (int curNeighbor = 0; curNeighbor < CITY_NUM; curNeighbor++)
			{
				int totalWays = cities[curMarked].neighbor[curNeighbor].size(); //该mark城市跟它的某个下邻居之间可能有0或多条路径
				if (vertexes[curNeighbor].isMarked == false && totalWays > 0)//如果是非固定的点且该点有路径才进行计算
				{
					int minWayIndex = 0;		  //若有多条路径，最短的哪条路径序号
					double minWayRisk = MAX_COST; //最短的那条路径长度
					int minWayTime = MAX_COST; //最短的那条路径长度

					//对每条路径进行遍历,然后选出风险最小的那条
					for (int curWay = 0; curWay < totalWays; curWay++)
					{
						int time1 = cities[curMarked].neighbor[curNeighbor][curWay].departTime;
						int waitTime = time1 - vertexes[curMarked].startWaitTime;
						if (waitTime < 0 || (curMarked == departure && waitTime == 0 && preciseStartTIME > static_cast<double>(time1)))
						{
							//若等待时间小于0，则代表跨天了，加上24小时或者开始时间跟某趟车相同但那时候已经发车了，只能等第二天的那趟车
							waitTime += 24;
						}
						waitRisk = waitTime * cities[curMarked].GetRisk(); //等待风险 = 等待时间*城市风险
						totalRisk = waitRisk + cities[curMarked].neighbor[curNeighbor][curWay].transRisk;//总风险 = 等待风险+路途风险
						totalTime = waitTime + cities[curMarked].neighbor[curNeighbor][curWay].transTime;//总时间 = 等待时间+路途时间

						if (stra == MIN_RISK)//若是策略1
						{
							if (totalRisk < minWayRisk)//选择最短路径，并记录标号
							{
								minWayRisk = totalRisk;
								minWayIndex = curWay;
								minWayTime = totalTime;
							}
						}
						if (stra == MIN_TIME)//若是策略二
						{
							if (totalTime < minWayTime)//选择最短路径，并记录标号
							{
								minWayTime = totalTime;
								minWayIndex = curWay;
								minWayRisk = totalRisk;
							}
						}

					}

					//如果以该mark城市做中间节点可以有更小的开销，则更新
					if (stra == MIN_RISK)//若是策略1
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
					else//若是策略二
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

				else//如果是固定的点，则跳过
					;
			}//end for (遍历该mark城市的邻居)

		}//end for (遍历所有mark城市)

		//固定最小开销的点,并完成对应更新
		if (nextMarkIndex != departure)
		{
			markedCity.push_back(nextMarkIndex);
			vertexes[nextMarkIndex].isMarked = true;
			countMark++;
		}

		//如果新固定的点是终点，可以退出循环
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

	//把路径存起来
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
	double upperRiskBound; // 理想风险值的上界
	double minRisk;    //满足条件的风险值
	int timeOfMinRisk; //满足条件的耗时
	double curRisk = 0;
	int curTime = 0;

	StackInfo lastPop;//上一个被弹出栈的元素
	lastPop.cityID = 0;
	lastPop.wayFromLast = -1;

	bool whetherHasNext = false; // 是否有下一个节点
	bool isVistiedCity[CITY_NUM];//某个城市是否被访问过
	int startWaitTime[CITY_NUM]; // 到达该城市的时间
	for (int i = 0; i < CITY_NUM; i++) //初始化
	{
		startWaitTime[i] = 0;
		isVistiedCity[i] = false;
	}

	Vehicle* unfinishedPath = NULL;
	int unfinishedTime = 0;//移动中的列车/航班未完成的时间
	double unfinishedRisk = 0;
	if (state == MOVING)
	{
		unfinishedPath = curPath.back();
		unfinishedTime = unfinishedPath->transTime - travelHours;
		unfinishedRisk = static_cast<double>(unfinishedTime) * unfinishedPath->risk * cities[unfinishedPath->departID].GetRisk();
	}

	std::vector<StackInfo> tempSolution; //利用栈实现深度遍历，该栈中存放的点满足 遍历该节点的邻居节点时能够发现符合约束的可行解
	std::vector<StackInfo> finalSolution;

	Result tempRes;

	tempRes = MakeMinPath(MIN_TIME);
	upperRiskBound = tempRes.idleRisk - unfinishedRisk;//用最小时间策略求出风险值的上限,如果是移动状态，记得减去没走完的那段路
	if (tempRes.idleTime > expectedTime)//若最小时间策略的用时都大于期望值，则无解
	{
		limitTimeMinRiskPath.assign(minTimePath.begin(), minTimePath.end());//把最短时间的路线复制到限时路线中（已经包含了移动未走完的情况
		idleResult = tempRes;

		return -1;
	}

	tempRes = MakeMinPath(MIN_RISK);   //用最小风险策略求出初始解
	minRisk = tempRes.idleRisk - unfinishedRisk;//初始解的风险值,如果是移动状态，记得减去没走完的那段路
	timeOfMinRisk = tempRes.idleTime;//初始解的耗时
	if (timeOfMinRisk <= expectedTime) //先进行初步判断，若最小风险策略的耗时满足时间约束条件，则该解就是最优解
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
		tempSolution.push_back(newStackElem);// 先把起点入栈

		startWaitTime[departure] = startTime;
		isVistiedCity[departure] = true;

		while (!tempSolution.empty()) // 若栈不为空，则进行循环
		{
			int curCity = tempSolution.back().cityID;//当前要遍历的节点（称为父节点）为栈顶
			bool whetherContinue = true;//标记是否要继续，若找到了一个点及其对应的边入栈，则退出循环

			//对该遍历节点的所有没被遍历过的邻居节点进行遍历
			for (int curNeighbor = 0; curNeighbor < CITY_NUM && whetherContinue; curNeighbor++)
			{
				if (curNeighbor >= lastPop.cityID && isVistiedCity[curNeighbor] == false)// 小于上一个被弹出栈的则代表该点被遍历过了
				{
					int totalWays = cities[curCity].neighbor[curNeighbor].size(); //该mark城市跟它的某个下邻居之间可能有多条路径

					//对每条路径进行遍历
					for (int curWay = 0; curWay < totalWays && whetherContinue; curWay++)
					{
						if ((curNeighbor != lastPop.cityID) || (curNeighbor == lastPop.cityID && curWay > lastPop.wayFromLast))//小于等于上一个被弹出栈的则代表该边被遍历过了
						{
							int time1 = cities[curCity].neighbor[curNeighbor][curWay].departTime;
							int waitTime = time1 - startWaitTime[curCity];//等待时间
							if (waitTime < 0 || (curCity == departure && waitTime == 0 && preciseStartTIME > static_cast<double>(time1)))//若等待时间小于0，则代表跨天了，加上24小时
							{
								waitTime += 24;
							}
							double waitRisk = waitTime * cities[curCity].GetRisk(); //等待风险 = 等待时间*城市风险
							double totalRisk = waitRisk + cities[curCity].neighbor[curNeighbor][curWay].transRisk;//总风险 = 等待风险+路途风险
							int totalTime = waitTime + cities[curCity].neighbor[curNeighbor][curWay].transTime;//总时间 = 等待时间+路途时间

							if (curRisk + totalRisk > upperRiskBound || curTime + totalTime > (expectedTime - unfinishedTime))
								;  //不满足条件，剪枝，不做处理
							else
							{
								//若满足条件
								startWaitTime[curNeighbor] = cities[curCity].neighbor[curNeighbor][curWay].arrivalTime;
								curRisk += totalRisk;
								curTime += totalTime;

								//将该点该边进栈
								newStackElem.cityID = curNeighbor;
								newStackElem.wayFromLast = curWay;
								newStackElem.riskFromLast = totalRisk;
								newStackElem.TimeFromlast = totalTime;
								tempSolution.push_back(newStackElem);
								whetherContinue = false;//跳出循环

								if (curNeighbor == destination)//判断该字节点是否为终点,若该子节点为终点，则该次遍历获得一条符合条件的更优成本路径，更新风险值上界
								{
									minRisk = curRisk;
									upperRiskBound = minRisk;
									timeOfMinRisk = curTime;

									finalSolution.clear();
									finalSolution.assign(tempSolution.begin(), tempSolution.end());
								}
								else //不是终点
								{
									whetherHasNext = true; //找到了下一个节点
									isVistiedCity[curNeighbor] = true;//该点标记为已经遍历过
									lastPop.cityID = -1;
									lastPop.wayFromLast = -1;
								}

							}

						}//end of if(curWay is not visited)
					}//end of for (curWay)
				}
			}//end of for(curNeighbor)

			if (whetherHasNext == false) //该次遍历没有得到下一节点
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

		//将最终路径保存
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
		if (state == MOVING)//如果是移动状态，记得没走完的那段路
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