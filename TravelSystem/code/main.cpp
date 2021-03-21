#include"Header.h"

std::vector<City> cities;//�������еĳ���
std::vector<Passenger> passengerList;//����������ӽ�ϵͳ�ĳ˿�
std::ofstream fileLog;

bool isStop = false;
int curState = NORMAL;
int TIME = 0;//ȫ��ʱ�䣨��ʾ��ʱ�䣩
double preciseTIME = 0;//ȫ��ʱ�䣬��ȷС��
clock_t startTime = 0;//��ʼʱ��
clock_t stopTime = 0;//��ͣʱ��
clock_t endStopTime = 0;//������ͣ��ʱ��
clock_t stopInterval = 0;//��ͣ���
clock_t totStopInterval = 0;//�ܹ���ͣʱ��
clock_t currentTime = 0;//��ǰʱ��

int passStartTime = NOT_SET;	//�˿ͳ���ʱ��
double passPreciseTIME = NOT_SET;//��ȷ��С���ĳ˿ͳ���ʱ��
int passDeparture = NOT_SET;	//������
int passDestination = NOT_SET;//�յ����
int passStrategy = MIN_RISK;	//ѡ��Ĳ��� 1:��С���գ�2:���ʱ�䣻3����ʱ��С����
int passExpectedTime = MAX_COST; //�޶���ʱ��

clock_t lastUpdateTime = 0;
bool isStart = false;
int curPassID = 0;//��ǰ��������ʾ�ĳ˿͵����
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
		if (isStart)//�Ѿ�����ϵͳ
		{
			currentTime = clock();

			TransferSystemState();
			Update();
		}
		else//��δ����ϵͳ
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

			fileLog.open("����ϵͳ��־.txt", std::ios::out);//������ļ�

			//�����������
			std::cout << "*******************************************************************************\n";
			std::cout << "                             ��ǰʱ�� TIME ��" << TIME << "��00\n";
			std::cout << "*******************************************************************************\n\n";
			//������ļ�
			fileLog << "*******************************************************************************\n";
			fileLog << "                             ��ǰʱ�� TIME ��" << TIME << "��00\n";
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
		//������������£����ж��ǵ���ĸ���ť��ִ����ز���
		if (mouse.mkLButton)
		{
			mouse = GetMouseMsg();
			switch (curState)
			{
				case NORMAL:
					if (Between(968, 1086, 13, 51, mouse.x, mouse.y))// ��������ǡ���ӳ˿͡���ť����������ʱ����passStartTime
					{
						Stop();//��ʱ��ͣ
						curState = CLICK_DEPART;

						//��ʼ����ʱ����Ϊ֮����׼��
						passStartTime = TIME;
						passPreciseTIME = preciseTIME;
						passDeparture = NOT_SET;	//������
						passDestination = NOT_SET;//�յ����
						passStrategy = MIN_RISK;	//ѡ��Ĳ���
						passExpectedTime = MAX_COST; //�޶���ʱ��

						//��ӡ��־
						std::cout << "�¼�����ӳ˿�\n";
						fileLog << "�¼�����ӳ˿�\n";
					}
					else if (Between(1338, 1417, 139, 168, mouse.x, mouse.y) && (passengerList.size() != 0))//���������Ŀ�ĵذ�ť(ֻ�����г˿͵�����²���Ч)
					{
						Stop();//��ʱ��ͣ
						curState = CHANGE_DEST;
						passDestination = passengerList[curPassID].GetDest();
						lastDest = passDestination;

						passStrategy = passengerList[curPassID].GetStrategy();//������ͼ���ڸý׶β�����
						passDeparture = passengerList[curPassID].GetCurCity();
						passExpectedTime = MAX_COST;

						//��ӡ��־
						std::cout << "�¼�������" << curPassID + 1 << "�ų˿�" << passengerList[curPassID].GetName() << "��Ŀ�ĵ�\n";
						fileLog << "�¼�������" << curPassID + 1 << "�ų˿�" << passengerList[curPassID].GetName() << "��Ŀ�ĵ�\n";
					}
					else if (Between(1338, 1417, 291, 320, mouse.x, mouse.y) && (passengerList.size() != 0))//��������Ĳ��԰�ť(ֻ�����г˿͵�����²���Ч)
					{
						Stop();//��ʱ��ͣ
						curState = CHANGE_STRA;
						passStrategy = passengerList[curPassID].GetStrategy();
						lastStra = passStrategy;
						lastExpeTime = passengerList[curPassID].GetExpectedTime() - passengerList[curPassID].GetUsedHours();

						//��ʼ������
						passDeparture = passengerList[curPassID].GetCurCity();
						passDestination = passengerList[curPassID].GetDest();
						passExpectedTime = MAX_COST;

						//��ӡ��־
						std::cout << "�¼�������" << curPassID + 1 << "�ų˿�" << passengerList[curPassID].GetName() << "�����в���\n";
						fileLog << "�¼�������" << curPassID + 1 << "�ų˿�" << passengerList[curPassID].GetName() << "�����в���\n";
					}
					else if (Between(1003, 1075, 359, 745, mouse.x, mouse.y))//���������ǳ˿��б�
					{
						//���жϵ���ĸ��˿ͣ��ڸ��µ�ǰ����ĳ˿͵����
						unsigned int passid = ((mouse.x - 1003) / 38) /*+ 1*/ + ((mouse.y - 359) / 41) * 2;
						if (passid < passengerList.size())
						{
							curPassID = passid;

							//��ӡ��־
							std::cout << "�¼�����ѯ" << curPassID + 1 << "�ų˿�" << passengerList[curPassID].GetName() << "����Ϣ\n";
							std::cout << "����������������������ѯ�ĳ˿���Ϣ���¡�������������������\n";
							fileLog << "�¼�����ѯ" << curPassID + 1 << "�ų˿�" << passengerList[curPassID].GetName() << "����Ϣ\n";
							fileLog << "����������������������ѯ�ĳ˿���Ϣ���¡�������������������\n";
							passengerList[curPassID].PrintLog();
						}
						else
							;
					}
					else
						;
					break;

				case CLICK_DEPART:
					if (Between(992, 1083, 61, 97, mouse.x, mouse.y) && passDeparture != NOT_SET)//��������ȷ�ϡ���ť
					{
						curState = CLICK_DEST;//������һ�׶�

						//��ӡ��־
						std::cout << "�¼����������˿ͳ����أ�" << cities[passDeparture].GetCityName() << "\n";
						fileLog << "�¼����������˿ͳ����أ�" << cities[passDeparture].GetCityName() << "\n";
					}
					else if (Between(992, 1083, 101, 137, mouse.x, mouse.y))//��������ȡ������ť
					{
						Continue();

						//��ӡ��־
						std::cout << "�¼���ȡ����ӳ˿�\n\n";
						fileLog << "�¼���ȡ����ӳ˿�\n\n";
					}
					else
					{
						int match = MatchCity(mouse.x, mouse.y);
						if (match != -1)//��������λ����һ������
						{
							passDeparture = match;
						}
						else
							;//do nothing
					}
					break;

				case CLICK_DEST:
					if (Between(992, 1083, 61, 97, mouse.x, mouse.y) && passDestination != NOT_SET)//��������ȷ�ϡ���ť
					{
						if (passDestination == passDeparture)
						{
							//��ӡ��־
							std::cout << "�¼����������˿�Ŀ�ĵأ�" << cities[passDestination].GetCityName() << "�����������ͬ��������ѡ��\n";
							fileLog << "�¼����������˿�Ŀ�ĵأ�" << cities[passDestination].GetCityName() << "�����������ͬ��������ѡ��\n";
						}
						else
						{
							curState = CLICK_STRATEGY;//������һ�׶�

						//��ӡ��־
							std::cout << "�¼����������˿�Ŀ�ĵأ�" << cities[passDestination].GetCityName() << "\n";
							fileLog << "�¼����������˿�Ŀ�ĵأ�" << cities[passDestination].GetCityName() << "\n";
						}
					}
					else if (Between(992, 1083, 101, 137, mouse.x, mouse.y))//��������ȡ������ť
					{
						Continue();

						//��ӡ��־
						std::cout << "�¼���ȡ����ӳ˿�\n\n";
						fileLog << "�¼���ȡ����ӳ˿�\n\n";
					}
					else
					{
						int match = MatchCity(mouse.x, mouse.y);
						if (match != -1)//��������λ����һ������
						{
							passDestination = match;
						}
						else
							;//do nothing
					}
					break;

				case CLICK_STRATEGY:
					if (Between(992, 1083, 61, 97, mouse.x, mouse.y))//��������ȷ�ϡ���ť����ΪĬ�ϲ�������С���գ����Կ���ֱ�ӵ�ȷ��
					{
						//��ʱ���µĳ˿���Ϣ¼�����
						int seq = passengerList.size();
						curPassID = seq;
						std::string passName = InputPassName();//����˿�����


						switch (passStrategy)
						{
						case LIMIT_TIME_MIN_RISK:
							passExpectedTime = InputTime();//�����޶�ʱ��
							std::cout << "�¼�������˿����в��ԣ���ʱ��С���ղ��� ���޶�ʱ�䣺" << passExpectedTime << "Сʱ)\n";//��ӡ��־
							fileLog << "�¼�������˿����в��ԣ���ʱ��С���ղ��� ���޶�ʱ�䣺" << passExpectedTime << "Сʱ)\n";
							break;

						case MIN_RISK:
							std::cout << "�¼����������˿����в��ԣ���С���ղ���\n";//��ӡ��־
							fileLog << "�¼����������˿����в��ԣ���С���ղ���\n";
							break;
						default://��Сʱ�����
							std::cout << "�¼����������˿����в��ԣ���Сʱ�����\n";//��ӡ��־
							fileLog << "�¼����������˿����в��ԣ���Сʱ�����\n";
							break;
						}
						std::cout << "�¼�����������˿�������" << passName << "\n";//��ӡ��־
						std::cout << "�������������������³˿���ӳɹ�����Ϣ���¡���������������\n";
						fileLog << "�¼�����������˿�������" << passName << "\n";//��ӡ��־
						fileLog << "�������������������³˿���ӳɹ�����Ϣ���¡���������������\n";

						Passenger newPassenger(seq, passStartTime, passPreciseTIME, passDeparture, passDestination, passStrategy, passExpectedTime, currentTime, passName);//������Ϣ����һ���˿�
						passengerList.push_back(newPassenger);//���ó˿ʹ����б���

						passengerList[curPassID].MakePlan();//�滮·��

						Continue();//��ӳ˿���ϣ��ظ�����״̬��ȡ��ʱ����ͣ
					}
					else if (Between(992, 1083, 101, 137, mouse.x, mouse.y))//��������ȡ������ť
					{
						Continue();

						//��ӡ��־
						std::cout << "�¼���ȡ����ӳ˿�\n\n";
						fileLog << "�¼���ȡ����ӳ˿�\n\n";
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
					if (Between(992, 1083, 61, 97, mouse.x, mouse.y))//��������ȷ�ϡ���ť
					{
						if (passDestination == passDeparture && passDeparture == passengerList[curPassID].GetDepart())//��ʼ�����غ��ݶ��ֳ�������ͬ��Ŀ�ĵ����ʼ��������ͬ
						{
							//��ӡ��־
							std::cout << "�¼����������˿�Ŀ�ĵأ�" << cities[passDestination].GetCityName() << "�����ʼ��������ͬ��������ѡ��\n";
							fileLog << "�¼����������˿�Ŀ�ĵأ�" << cities[passDestination].GetCityName() << "�����ʼ��������ͬ��������ѡ��\n";
						}
						else
						{
							if (passDestination != lastDest)//���Ŀ�ĳ��б��ˣ��������ƶ�����
							{
								//��ӡ��־
								std::cout << "�¼����������˿���Ŀ�ĵأ�" << cities[passDestination].GetCityName() << "\n";
								fileLog << "�¼����������˿���Ŀ�ĵأ�" << cities[passDestination].GetCityName() << "\n";

								if (passStrategy == LIMIT_TIME_MIN_RISK)//����Ƿ���3����Ҫ����ʱ��
								{
									passExpectedTime = InputTime();
									std::cout << "�¼�������˿����޶�ʱ�䣺" << passExpectedTime << "Сʱ\n";//��ӡ��־
									fileLog << "�¼�������˿����޶�ʱ�䣺" << passExpectedTime << "Сʱ\n";
								}
								else
									;//do nothing
								std::cout << "���������������������ó˿�����Ϣ���¡�������������������\n";
								fileLog << "���������������������ó˿�����Ϣ���¡�������������������\n";
								passengerList[curPassID].ChangeDest(passDestination, passExpectedTime);//�����ƶ�����

							}
							else //����û�б䣬������
							{
								std::cout << "�¼����������˿�Ŀ�ĵأ�����Ҫ����·�ߣ���ԭ·�߼�������\n\n";
								fileLog << "�¼����������˿�Ŀ�ĵأ�����Ҫ����·�ߣ���ԭ·�߼�������\n\n";
							}

							//����ȷ�Ͼͻָ�����״̬�������³�ʼ����ʱ����
							Continue();

						}
						
					}
					else if (Between(992, 1083, 101, 137, mouse.x, mouse.y))//��������ȡ������ť
					{
						Continue();

						//��ӡ��־
						std::cout << "�¼���ȡ�����ĳ˿�Ŀ�ĵ�\n\n";
						fileLog << "�¼���ȡ�����ĳ˿�Ŀ�ĵ�\n\n";
					}
					else
					{
						int match = MatchCity(mouse.x, mouse.y);
						if (match != -1)//��������λ����һ������
						{
							passDestination = match;
						}
						else
							;//do nothing
					}
					break;

				case CHANGE_STRA:
					if (Between(992, 1083, 61, 97, mouse.x, mouse.y))//��������ȷ�ϡ���ť
					{
						if ((passStrategy != lastStra || (lastStra == LIMIT_TIME_MIN_RISK && passExpectedTime != lastExpeTime))
							&& passengerList[curPassID].whetherRemake())//������Ա�����û���յ��Ҳ����ƶ����һ�ˣ��������ƶ�����
						{
							switch (passStrategy)
							{
							case LIMIT_TIME_MIN_RISK:
								//passExpectedTime = InputTime();//�����޶�ʱ��
								std::cout << "�¼�������˿������в��ԣ���ʱ��С���ղ��� ���޶�ʱ�䣺" << passExpectedTime << "Сʱ)\n";//��ӡ��־
								fileLog << "�¼�������˿������в��ԣ���ʱ��С���ղ��� ���޶�ʱ�䣺" << passExpectedTime << "Сʱ)\n";
								break;

							case MIN_RISK:
								std::cout << "�¼����������˿������в��ԣ���С���ղ���\n";//��ӡ��־
								fileLog << "�¼����������˿������в��ԣ���С���ղ���\n";
								break;
							default://��Сʱ�����
								std::cout << "�¼����������˿������в��ԣ���Сʱ�����\n";//��ӡ��־
								fileLog << "�¼����������˿������в��ԣ���Сʱ�����\n";
								break;
							}
							std::cout << "���������������������ó˿�����Ϣ���¡�������������������\n";
							fileLog << "���������������������ó˿�����Ϣ���¡�������������������\n";
							passengerList[curPassID].ChangeStrategy(passStrategy, passExpectedTime);
						}
						else
						{
							std::cout << "�¼����������˿����в��ԣ�����Ҫ����·�ߣ���ԭ·�߼�������\n\n";
							fileLog << "�¼����������˿����в��ԣ�����Ҫ����·�ߣ���ԭ·�߼�������\n\n";
						}

						Continue();
					}
					else if (Between(992, 1083, 101, 137, mouse.x, mouse.y))//��������ȡ������ť
					{
						Continue();

						//��ӡ��־
						std::cout << "�¼���ȡ�����ĳ˿����в���\n\n";
						fileLog << "�¼���ȡ�����ĳ˿����в���\n\n";
					}
					else
					{
						int match = MatchStrategy(mouse.x, mouse.y);
						if (match != -1)
						{
							passStrategy = match;
							if (passStrategy == LIMIT_TIME_MIN_RISK)//���ѡ�е�����ʱ��С���ղ���
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
	//�����ͣ��,������ͣ���
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
	if (newTIME == ((TIME + 1) % 24))//���TIME������
	{
		TIME = newTIME;
		isUpdate = true;

		//��־����
		std::cout << "*******************************************************************************\n";
		std::cout << "                             ��ǰʱ�� TIME ��" << TIME << "��00\n";
		std::cout << "*******************************************************************************\n\n";
		fileLog << "*******************************************************************************\n";
		fileLog << "                             ��ǰʱ�� TIME ��" << TIME << "��00\n";
		fileLog << "*******************************************************************************\n\n";

		//Ϊÿһλ�˿͸��µ�ǰ�г�/���������ʱ��(hours)
		for (int i = 0; i < size; i++)
		{
			passengerList[i].UpdateHours();
		}
	}
	else
		;

	if (isStop == false)
	{
		for (unsigned int i = 0; i < passengerList.size(); i++)//���³˿�����
		{
			passengerList[i].MovePassenger();
		}
	}
	else
		;

	if (isUpdate && size > 0)
	{
		std::cout << "����������������������ѯ�ĳ˿���Ϣ���¡�������������������\n";
		fileLog << "����������������������ѯ�ĳ˿���Ϣ���¡�������������������\n";
		passengerList[curPassID].PrintLog();//��ӡ��ǰ�˿͵���־��Ϣ
	}

	if (currentTime - lastUpdateTime >= UPDATE_INTEVAL)//������˸���ʱ�������͸��»���
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
