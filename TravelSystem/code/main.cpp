#include"Header.h"

std::vector<City> cities;//保存所有的城市
std::vector<Passenger> passengerList;//保存所有添加进系统的乘客
std::ofstream fileLog;

bool isStop = false;
int curState = NORMAL;
int TIME = 0;//全局时间（显示的时间）
double preciseTIME = 0;//全局时间，精确小数
clock_t startTime = 0;//开始时间
clock_t stopTime = 0;//暂停时间
clock_t endStopTime = 0;//结束暂停的时间
clock_t stopInterval = 0;//暂停间隔
clock_t totStopInterval = 0;//总共暂停时间
clock_t currentTime = 0;//当前时间

int passStartTime = NOT_SET;	//乘客出发时间
double passPreciseTIME = NOT_SET;//精确到小数的乘客出发时间
int passDeparture = NOT_SET;	//起点城市
int passDestination = NOT_SET;//终点城市
int passStrategy = MIN_RISK;	//选择的策略 1:最小风险；2:最短时间；3：限时最小风险
int passExpectedTime = MAX_COST; //限定的时间

clock_t lastUpdateTime = 0;
bool isStart = false;
int curPassID = 0;//当前界面上显示的乘客的序号
int lastDest = NOT_SET;
int lastStra = NOT_SET;
int lastExpeTime = MAX_COST;

int main()
{
	InitCityList();
	InitTable();
	
	LoadImg();
	InitCanvas();

	while (true)
	{
		if (isStart)//已经启动系统
		{
			currentTime = clock();

			TransferSystemState();
			Update();
		}
		else//尚未启动系统
		{
			StartSystem();
		}
		
	}

	fileLog.close();
	_getch();
}

void StartSystem()
{
	if (MouseHit())
	{
		MOUSEMSG mouse = GetMouseMsg();
		if (mouse.mkLButton)
		{
			isStart = true;
			startTime = clock();

			Draw(curPassID);

			fileLog.open("旅行系统日志.txt", std::ios::out);//打开输出文件

			//输出到命令行
			std::cout << "*******************************************************************************\n";
			std::cout << "                             当前时间 TIME ：" << TIME << "：00\n";
			std::cout << "*******************************************************************************\n\n";
			//输出到文件
			fileLog << "*******************************************************************************\n";
			fileLog << "                             当前时间 TIME ：" << TIME << "：00\n";
			fileLog << "*******************************************************************************\n\n";
		}
	}
	else
		;
}

void TransferSystemState()
{
	if (MouseHit())
	{
		MOUSEMSG mouse = GetMouseMsg();
		//如果鼠标左键按下，则判断是点击哪个按钮并执行相关操作
		if (mouse.mkLButton)
		{
			mouse = GetMouseMsg();
			switch (curState)
			{
				case NORMAL:
					if (Between(968, 1086, 13, 51, mouse.x, mouse.y))// 当点击的是“添加乘客”按钮，更改了临时变量passStartTime
					{
						Stop();//计时暂停
						curState = CLICK_DEPART;

						//初始化临时变量为之后做准备
						passStartTime = TIME;
						passPreciseTIME = preciseTIME;
						passDeparture = NOT_SET;	//起点城市
						passDestination = NOT_SET;//终点城市
						passStrategy = MIN_RISK;	//选择的策略
						passExpectedTime = MAX_COST; //限定的时间

						//打印日志
						std::cout << "事件：添加乘客\n";
						fileLog << "事件：添加乘客\n";
					}
					else if (Between(1338, 1417, 139, 168, mouse.x, mouse.y) && (passengerList.size() != 0))//当点击更改目的地按钮(只有在有乘客的情况下才有效)
					{
						Stop();//计时暂停
						curState = CHANGE_DEST;
						passDestination = passengerList[curPassID].GetDest();
						lastDest = passDestination;

						passStrategy = passengerList[curPassID].GetStrategy();//方便贴图，在该阶段不更改
						passDeparture = passengerList[curPassID].GetCurCity();
						passExpectedTime = MAX_COST;

						//打印日志
						std::cout << "事件：更改" << curPassID + 1 << "号乘客" << passengerList[curPassID].GetName() << "的目的地\n";
						fileLog << "事件：更改" << curPassID + 1 << "号乘客" << passengerList[curPassID].GetName() << "的目的地\n";
					}
					else if (Between(1338, 1417, 291, 320, mouse.x, mouse.y) && (passengerList.size() != 0))//当点击更改策略按钮(只有在有乘客的情况下才有效)
					{
						Stop();//计时暂停
						curState = CHANGE_STRA;
						passStrategy = passengerList[curPassID].GetStrategy();
						lastStra = passStrategy;
						lastExpeTime = passengerList[curPassID].GetExpectedTime() - passengerList[curPassID].GetUsedHours();

						//初始化变量
						passDeparture = passengerList[curPassID].GetCurCity();
						passDestination = passengerList[curPassID].GetDest();
						passExpectedTime = MAX_COST;

						//打印日志
						std::cout << "事件：更改" << curPassID + 1 << "号乘客" << passengerList[curPassID].GetName() << "的旅行策略\n";
						fileLog << "事件：更改" << curPassID + 1 << "号乘客" << passengerList[curPassID].GetName() << "的旅行策略\n";
					}
					else if (Between(1003, 1075, 359, 745, mouse.x, mouse.y))//如果点击的是乘客列表
					{
						//先判断点的哪个乘客，在更新当前界面的乘客的序号
						unsigned int passid = ((mouse.x - 1003) / 38) /*+ 1*/ + ((mouse.y - 359) / 41) * 2;
						if (passid < passengerList.size())
						{
							curPassID = passid;

							//打印日志
							std::cout << "事件：查询" << curPassID + 1 << "号乘客" << passengerList[curPassID].GetName() << "的信息\n";
							std::cout << "——————————查询的乘客信息如下——————————\n";
							fileLog << "事件：查询" << curPassID + 1 << "号乘客" << passengerList[curPassID].GetName() << "的信息\n";
							fileLog << "——————————查询的乘客信息如下——————————\n";
							passengerList[curPassID].PrintLog();
						}
						else
							;
					}
					else
						;
					break;

				case CLICK_DEPART:
					if (Between(992, 1083, 61, 97, mouse.x, mouse.y) && passDeparture != NOT_SET)//如果点击“确认”按钮
					{
						curState = CLICK_DEST;//进入下一阶段

						//打印日志
						std::cout << "事件：鼠标输入乘客出发地：" << cities[passDeparture].GetCityName() << "\n";
						fileLog << "事件：鼠标输入乘客出发地：" << cities[passDeparture].GetCityName() << "\n";
					}
					else if (Between(992, 1083, 101, 137, mouse.x, mouse.y))//如果点击“取消”按钮
					{
						Continue();

						//打印日志
						std::cout << "事件：取消添加乘客\n\n";
						fileLog << "事件：取消添加乘客\n\n";
					}
					else
					{
						int match = MatchCity(mouse.x, mouse.y);
						if (match != -1)//如果点击的位置是一个城市
						{
							passDeparture = match;
						}
						else
							;//do nothing
					}
					break;

				case CLICK_DEST:
					if (Between(992, 1083, 61, 97, mouse.x, mouse.y) && passDestination != NOT_SET)//如果点击“确认”按钮
					{
						if (passDestination == passDeparture)
						{
							//打印日志
							std::cout << "事件：鼠标输入乘客目的地：" << cities[passDestination].GetCityName() << "，与出发地相同，请重新选择\n";
							fileLog << "事件：鼠标输入乘客目的地：" << cities[passDestination].GetCityName() << "，与出发地相同，请重新选择\n";
						}
						else
						{
							curState = CLICK_STRATEGY;//进入下一阶段

						//打印日志
							std::cout << "事件：鼠标输入乘客目的地：" << cities[passDestination].GetCityName() << "\n";
							fileLog << "事件：鼠标输入乘客目的地：" << cities[passDestination].GetCityName() << "\n";
						}
					}
					else if (Between(992, 1083, 101, 137, mouse.x, mouse.y))//如果点击“取消”按钮
					{
						Continue();

						//打印日志
						std::cout << "事件：取消添加乘客\n\n";
						fileLog << "事件：取消添加乘客\n\n";
					}
					else
					{
						int match = MatchCity(mouse.x, mouse.y);
						if (match != -1)//如果点击的位置是一个城市
						{
							passDestination = match;
						}
						else
							;//do nothing
					}
					break;

				case CLICK_STRATEGY:
					if (Between(992, 1083, 61, 97, mouse.x, mouse.y))//如果点击“确认”按钮，因为默认策略是最小风险，所以可以直接点确认
					{
						//这时候新的乘客信息录入完毕
						int seq = passengerList.size();
						curPassID = seq;
						std::string passName = InputPassName();//输入乘客姓名


						switch (passStrategy)
						{
						case LIMIT_TIME_MIN_RISK:
							passExpectedTime = InputTime();//输入限定时间
							std::cout << "事件：输入乘客旅行策略：限时最小风险策略 （限定时间：" << passExpectedTime << "小时)\n";//打印日志
							fileLog << "事件：输入乘客旅行策略：限时最小风险策略 （限定时间：" << passExpectedTime << "小时)\n";
							break;

						case MIN_RISK:
							std::cout << "事件：鼠标输入乘客旅行策略：最小风险策略\n";//打印日志
							fileLog << "事件：鼠标输入乘客旅行策略：最小风险策略\n";
							break;
						default://最小时间策略
							std::cout << "事件：鼠标输入乘客旅行策略：最小时间策略\n";//打印日志
							fileLog << "事件：鼠标输入乘客旅行策略：最小时间策略\n";
							break;
						}
						std::cout << "事件：键盘输入乘客姓名：" << passName << "\n";//打印日志
						std::cout << "—————————新乘客添加成功，信息如下————————\n";
						fileLog << "事件：键盘输入乘客姓名：" << passName << "\n";//打印日志
						fileLog << "—————————新乘客添加成功，信息如下————————\n";

						Passenger newPassenger(seq, passStartTime, passPreciseTIME, passDeparture, passDestination, passStrategy, passExpectedTime, currentTime, passName);//利用信息生成一个乘客
						passengerList.push_back(newPassenger);//将该乘客存入列表里

						passengerList[curPassID].MakePlan();//规划路线

						Continue();//添加乘客完毕，回复正常状态，取消时间暂停
					}
					else if (Between(992, 1083, 101, 137, mouse.x, mouse.y))//如果点击“取消”按钮
					{
						Continue();

						//打印日志
						std::cout << "事件：取消添加乘客\n\n";
						fileLog << "事件：取消添加乘客\n\n";
					}
					else
					{
						int match = MatchStrategy(mouse.x, mouse.y);
						if (match != -1)
						{
							passStrategy = match;
						}
						else
							;//do nothing
					}
					break;

				case CHANGE_DEST:
					if (Between(992, 1083, 61, 97, mouse.x, mouse.y))//如果点击“确认”按钮
					{
						if (passDestination == passDeparture && passDeparture == passengerList[curPassID].GetDepart())//初始出发地和暂定现出发地相同且目的地与初始出发地相同
						{
							//打印日志
							std::cout << "事件：鼠标输入乘客目的地：" << cities[passDestination].GetCityName() << "，与初始出发地相同，请重新选择\n";
							fileLog << "事件：鼠标输入乘客目的地：" << cities[passDestination].GetCityName() << "，与初始出发地相同，请重新选择\n";
						}
						else
						{
							if (passDestination != lastDest)//如果目的城市变了，才重新制定方案
							{
								//打印日志
								std::cout << "事件：鼠标输入乘客新目的地：" << cities[passDestination].GetCityName() << "\n";
								fileLog << "事件：鼠标输入乘客新目的地：" << cities[passDestination].GetCityName() << "\n";

								if (passStrategy == LIMIT_TIME_MIN_RISK)//如果是方案3，需要输入时间
								{
									passExpectedTime = InputTime();
									std::cout << "事件：输入乘客新限定时间：" << passExpectedTime << "小时\n";//打印日志
									fileLog << "事件：输入乘客新限定时间：" << passExpectedTime << "小时\n";
								}
								else
									;//do nothing
								std::cout << "——————————该乘客新信息如下——————————\n";
								fileLog << "——————————该乘客新信息如下——————————\n";
								passengerList[curPassID].ChangeDest(passDestination, passExpectedTime);//重新制定方案

							}
							else //城市没有变，不处理
							{
								std::cout << "事件：鼠标输入乘客目的地，不需要更改路线，按原路线继续旅行\n\n";
								fileLog << "事件：鼠标输入乘客目的地，不需要更改路线，按原路线继续旅行\n\n";
							}

							//点了确认就恢复正常状态，并重新初始化临时变量
							Continue();

						}
						
					}
					else if (Between(992, 1083, 101, 137, mouse.x, mouse.y))//如果点击“取消”按钮
					{
						Continue();

						//打印日志
						std::cout << "事件：取消更改乘客目的地\n\n";
						fileLog << "事件：取消更改乘客目的地\n\n";
					}
					else
					{
						int match = MatchCity(mouse.x, mouse.y);
						if (match != -1)//如果点击的位置是一个城市
						{
							passDestination = match;
						}
						else
							;//do nothing
					}
					break;

				case CHANGE_STRA:
					if (Between(992, 1083, 61, 97, mouse.x, mouse.y))//如果点击“确认”按钮
					{
						if ((passStrategy != lastStra || (lastStra == LIMIT_TIME_MIN_RISK && passExpectedTime != lastExpeTime))
							&& passengerList[curPassID].whetherRemake())//如果策略变了且没到终点且不在移动最后一趟，才重新制定方案
						{
							switch (passStrategy)
							{
							case LIMIT_TIME_MIN_RISK:
								//passExpectedTime = InputTime();//输入限定时间
								std::cout << "事件：输入乘客新旅行策略：限时最小风险策略 （限定时间：" << passExpectedTime << "小时)\n";//打印日志
								fileLog << "事件：输入乘客新旅行策略：限时最小风险策略 （限定时间：" << passExpectedTime << "小时)\n";
								break;

							case MIN_RISK:
								std::cout << "事件：鼠标输入乘客新旅行策略：最小风险策略\n";//打印日志
								fileLog << "事件：鼠标输入乘客新旅行策略：最小风险策略\n";
								break;
							default://最小时间策略
								std::cout << "事件：鼠标输入乘客新旅行策略：最小时间策略\n";//打印日志
								fileLog << "事件：鼠标输入乘客新旅行策略：最小时间策略\n";
								break;
							}
							std::cout << "——————————该乘客新信息如下——————————\n";
							fileLog << "——————————该乘客新信息如下——————————\n";
							passengerList[curPassID].ChangeStrategy(passStrategy, passExpectedTime);
						}
						else
						{
							std::cout << "事件：鼠标输入乘客旅行策略，不需要更改路线，按原路线继续旅行\n\n";
							fileLog << "事件：鼠标输入乘客旅行策略，不需要更改路线，按原路线继续旅行\n\n";
						}

						Continue();
					}
					else if (Between(992, 1083, 101, 137, mouse.x, mouse.y))//如果点击“取消”按钮
					{
						Continue();

						//打印日志
						std::cout << "事件：取消更改乘客旅行策略\n\n";
						fileLog << "事件：取消更改乘客旅行策略\n\n";
					}
					else
					{
						int match = MatchStrategy(mouse.x, mouse.y);
						if (match != -1)
						{
							passStrategy = match;
							if (passStrategy == LIMIT_TIME_MIN_RISK)//如果选中的是限时最小风险策略
							{
								passExpectedTime = InputTime();
							}
							else
								passExpectedTime = MAX_COST;
						}
						else
							;//do nothing
					}
					break;

				default:
					break;
			}
		}
	}
}

void Update()
{
	//如果暂停了,更新暂停间隔
	if (isStop == true)
	{
		stopInterval = currentTime - stopTime;
	}
	else;

	int newTIME = ((currentTime - startTime - totStopInterval - stopInterval) / TIME_DURATION) % 24;
	preciseTIME = 1.0 * (static_cast<double>(currentTime) - startTime - totStopInterval - stopInterval) / TIME_DURATION;
	if (preciseTIME > 24)
	{
		preciseTIME -= 24;
	}
	else
		;

	bool isUpdate = false;
	int size = passengerList.size();
	if (newTIME == ((TIME + 1) % 24))//如果TIME更新了
	{
		TIME = newTIME;
		isUpdate = true;

		//日志更新
		std::cout << "*******************************************************************************\n";
		std::cout << "                             当前时间 TIME ：" << TIME << "：00\n";
		std::cout << "*******************************************************************************\n\n";
		fileLog << "*******************************************************************************\n";
		fileLog << "                             当前时间 TIME ：" << TIME << "：00\n";
		fileLog << "*******************************************************************************\n\n";

		//为每一位乘客更新当前列车/航班的已用时间(hours)
		for (int i = 0; i < size; i++)
		{
			passengerList[i].UpdateHours();
		}
	}
	else
		;

	if (isStop == false)
	{
		for (unsigned int i = 0; i < passengerList.size(); i++)//更新乘客坐标
		{
			passengerList[i].MovePassenger();
		}
	}
	else
		;

	if (isUpdate && size > 0)
	{
		std::cout << "——————————查询的乘客信息如下——————————\n";
		fileLog << "——————————查询的乘客信息如下——————————\n";
		passengerList[curPassID].PrintLog();//打印当前乘客的日志信息
	}

	if (currentTime - lastUpdateTime >= UPDATE_INTEVAL)//如果到了更新时间间隔，就更新画面
	{
		lastUpdateTime = currentTime;

		Draw(curPassID);
	}
	else
		;//do nothing
}

void Continue()
{
	endStopTime = clock();
	//endStopTime = currentTime;
	stopInterval = endStopTime - stopTime;
	totStopInterval += stopInterval;

	for (unsigned int i = 0; i < passengerList.size(); i++)
	{
		passengerList[i].lastXMoveTime += stopInterval;
		passengerList[i].lastYMoveTime += stopInterval;
	}

	stopInterval = 0;
	isStop = false; 

	curState = NORMAL;
}

void Stop()
{
	stopTime = currentTime;
	isStop = true;
}

bool Between(int left, int right, int top, int bottom, int x, int y)
{
	if ((x >= left && x <= right) && (y <= bottom && y >= top))
		return true;
	else
		return false;
}

int MatchCity(int x, int y)
{
	int id = -1;
	for (int i = 0; i < CITY_NUM && id == -1; i++)
	{
		if (Between(cities[i].GetX() - 15, cities[i].GetX() + 15, cities[i].GetY() - 45, cities[i].GetY() + 7, x, y))
			id = i;
	}
	return id;
}

int MatchStrategy(int x, int y)
{
	int stra = -1;
	if (Between(1199, 1416, 175, 207, x, y))
		stra = MIN_RISK;
	else if (Between(1199, 1416, 213, 245, x, y))
		stra = MIN_TIME;
	else if (Between(1199, 1416, 251, 283, x, y))
		stra = LIMIT_TIME_MIN_RISK;
	else
		;

	return stra;
}
