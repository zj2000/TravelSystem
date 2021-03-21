#include"Header.h"


void Draw(int curPassId)
{
	BeginBatchDraw();

	int passengerAmount = passengerList.size();

	//��������
	LOGFONT font;
	gettextstyle(&font);// ��ȡ��ǰ��������
	font.lfWeight = FW_BOLD;
	_tcscpy_s(font.lfFaceName, _T("΢���ź�"));// ��������Ϊ��΢���źڡ�
	settextstyle(&font);           // ����������ʽ

	std::string text;
	CString textString;

	//��������ͼ������ͼ
	if (curState == NORMAL)
	{
		putimage(0, 0, &imgBackground2);
		if (passengerAmount != 0)//����˿��б�Ϊ��
		{
			putimage(1197, 171, &imgStrategy[passengerList[curPassId].GetStrategy()]);
			passengerList[curPassId].Print();//��ӡ�˿������Ϣ

			//��ӡ�˿ͱ�
			font.lfHeight = 36; // ��������߶�
			settextstyle(&font);// ����������ʽ

			int height = 355;
			for (int i = 0; i < ((passengerAmount + 1) / 2); i++)//������listͼ
			{
				putimage(999, height, &imgList);
				height += 41;
			}
			height = 355;
			for (int i = 0; i < passengerAmount; i++)
			{
				text = std::to_string(i + 1); //�˿����
				textString = text.c_str();

				if (i == curPassId)
					settextcolor(RGB(136, 0, 27));//������ɫ,��ǰ��ѯ���û��������ɫΪ���ɫ
				else
					settextcolor(RGB(53, 81, 117));//������ɫ

				if (i % 2 == 1)
				{
					outtextxy(1050, height, textString);
					height += 41;
				}
				else
				{
					outtextxy(1010, height, textString);
				}
			}
		}
		else
			;// do nothing
	}
	else
	{
		putimage(0, 0, &imgBackground3);

		//��������
		if (passDeparture != NOT_SET)
		{
			putimage(cities[passDeparture].GetX() - 27, cities[passDeparture].GetY() - 59, &imgHintCover, SRCPAINT);
			putimage(cities[passDeparture].GetX() - 27, cities[passDeparture].GetY() - 59, &imgDepart, SRCAND);
		}
		else
			;//do nothing
		//��Ŀ�ĵ�
		if (passDestination != NOT_SET)
		{
			putimage(cities[passDestination].GetX() - 27, cities[passDestination].GetY() - 59, &imgHintCover, SRCPAINT);
			putimage(cities[passDestination].GetX() - 27, cities[passDestination].GetY() - 59, &imgDest, SRCAND);
		}
		else
			;//do nothing

		//����ʾѡ���ͼ
		putimage(366, 28, &imgChoose[curState % 3]);
		if ((curState == CLICK_DEST && passDeparture == passDestination)||
			(curState == CHANGE_DEST && passDestination == passDeparture && passDeparture == passengerList[curPassId].GetDepart()))
		{

			putimage(366, 28, &imgHintDest);
		}

		//������ͼ�ʹ�ӡ�����Ϣ
		if (curState >= CLICK_STRATEGY)
		{
			putimage(1197, 171, &imgStrategy[passStrategy]);
			if (curState != CLICK_STRATEGY)
			{
				passengerList[curPassId].PrintWhenChange();
			}
			else;
		}
		else
			;//do nothing
	}

	//��ӡʱ��
	font.lfHeight = 42; // ��������߶�
	settextstyle(&font);// ����������ʽ
	settextcolor(RGB(53, 81, 117));//������ɫ

	text = std::to_string(TIME) + "��00"; //ȫ��ʱ��
	textString = text.c_str();
	outtextxy(124, 12, textString);

	EndBatchDraw();
}

void Passenger::Print()const
{
	//�������غ�Ŀ�ĵص�ͼ
	putimage(cities[departure].GetX() - 27, cities[departure].GetY() - 59, &imgHintCover, SRCPAINT);
	putimage(cities[departure].GetX() - 27, cities[departure].GetY() - 59, &imgDepart, SRCAND);

	putimage(cities[destination].GetX() - 27, cities[destination].GetY() - 59, &imgHintCover, SRCPAINT);
	putimage(cities[destination].GetX() - 27, cities[destination].GetY() - 59, &imgDest, SRCAND);

	std::string text, text2;
	CString textString;

	//��������
	LOGFONT font;
	gettextstyle(&font);// ��ȡ��ǰ��������
	font.lfHeight = 20; // ��������߶�
	font.lfWeight = FW_BOLD;
	font.lfQuality = PROOF_QUALITY;    // �������Ч��Ϊ����� 
	_tcscpy_s(font.lfFaceName, _T("΢���ź�"));
	settextstyle(&font);           // ����������ʽ

	//��ӡ·��
	std::string trans[3];
	trans[AIR] = "�ɻ�";
	trans[TRAIN] = "��";
	trans[CAR] = "����";

	int totalHops, finishedHops;
	int unfinishedHops = curPath.size();
	int height = 333;

	//�ȴ�ӡ�Ѿ��߹���·
	settextcolor(RGB(143, 143, 143));
	setlinestyle(PS_DASH | PS_ENDCAP_FLAT, 4);
	setlinecolor(RGB(143, 143, 143));
	switch (strategy)
	{
	case MIN_RISK:
		totalHops = minRiskPath.size();
		finishedHops = totalHops - unfinishedHops;
		for (int i = 0; i < finishedHops; i++)
		{
			//�������У�������У���ͨ��ʽ�����
			text = cities[minRiskPath[totalHops - i - 1]->departID].GetCityName() + "->" + cities[minRiskPath[totalHops - i - 1]->destID].GetCityName() + "��"
				+ trans[minRiskPath[totalHops - i - 1]->kind] + "��" + minRiskPath[totalHops - i - 1]->number + ","
				+ std::to_string(minRiskPath[totalHops - i - 1]->departTime) + ":00-" + std::to_string(minRiskPath[totalHops - i - 1]->arrivalTime) + ":00";
			textString = text.c_str();
			outtextxy(1130, height, textString);
			height += 25;

			line(cities[minRiskPath[totalHops - i - 1]->departID].GetX(), cities[minRiskPath[totalHops - i - 1]->departID].GetY(),
				cities[minRiskPath[totalHops - i - 1]->destID].GetX(), cities[minRiskPath[totalHops - i - 1]->destID].GetY());
		}
		text2 = "������ʱ��";
		break;

	case MIN_TIME:
		totalHops = minTimePath.size();
		finishedHops = totalHops - unfinishedHops;
		for (int i = 0; i < finishedHops; i++)
		{
			//�������У�������У���ͨ��ʽ�����
			text = cities[minTimePath[totalHops - i - 1]->departID].GetCityName() + "->" + cities[minTimePath[totalHops - i - 1]->destID].GetCityName() + "��"
				+ trans[minTimePath[totalHops - i - 1]->kind] + "��" + minTimePath[totalHops - i - 1]->number + ","
				+ std::to_string(minTimePath[totalHops - i - 1]->departTime) + ":00-" + std::to_string(minTimePath[totalHops - i - 1]->arrivalTime) + ":00";
			textString = text.c_str();
			outtextxy(1130, height, textString);
			height += 25;

			line(cities[minTimePath[totalHops - i - 1]->departID].GetX(), cities[minTimePath[totalHops - i - 1]->departID].GetY(),
				cities[minTimePath[totalHops - i - 1]->destID].GetX(), cities[minTimePath[totalHops - i - 1]->destID].GetY());
		}
		text2 = "������ʱ��";
		break;

	default:
		totalHops = limitTimeMinRiskPath.size();
		finishedHops = totalHops - unfinishedHops;
		for (int i = 0; i < finishedHops; i++)
		{
			//�������У�������У���ͨ��ʽ�����
			text = cities[limitTimeMinRiskPath[totalHops - i - 1]->departID].GetCityName() + "->" + cities[limitTimeMinRiskPath[totalHops - i - 1]->destID].GetCityName() + "��"
				+ trans[limitTimeMinRiskPath[totalHops - i - 1]->kind] + "��" + limitTimeMinRiskPath[totalHops - i - 1]->number + ","
				+ std::to_string(limitTimeMinRiskPath[totalHops - i - 1]->departTime) + ":00-" + std::to_string(limitTimeMinRiskPath[totalHops - i - 1]->arrivalTime) + ":00";
			textString = text.c_str();
			outtextxy(1130, height, textString);
			height += 25;

			line(cities[limitTimeMinRiskPath[totalHops - i - 1]->departID].GetX(), cities[limitTimeMinRiskPath[totalHops - i - 1]->departID].GetY(),
				cities[limitTimeMinRiskPath[totalHops - i - 1]->destID].GetX(), cities[limitTimeMinRiskPath[totalHops - i - 1]->destID].GetY());
		}
		text2 = std::to_string(expectedTime);
		if (res == -1)
		{
			putimage(366, 28, &imgHintOuttime);
		}
		else {}

		break;
	}

	//�ٴ�ӡ��û�߹���·
	settextcolor(RGB(53, 81, 117));//������ɫ
	setlinecolor(RGB(0, 168, 243));
	for (int i = unfinishedHops - 1; i >= 0; i--)
	{
		text = cities[curPath[i]->departID].GetCityName() + "->" + cities[curPath[i]->destID].GetCityName() + "��"
			+ trans[curPath[i]->kind] + "��" + curPath[i]->number + ","
			+ std::to_string(curPath[i]->departTime) + ":00-" + std::to_string(curPath[i]->arrivalTime) + ":00";;//�������У�������У���ͨ��ʽ�����
		textString = text.c_str();
		outtextxy(1130, height, textString);
		height += 25;

		line(cities[curPath[i]->departID].GetX(), cities[curPath[i]->departID].GetY(),
			cities[curPath[i]->destID].GetX(), cities[curPath[i]->destID].GetY());
	}

	if (state == MOVING)//������ƶ�״̬,����ͨ���ߣ�����ӡ״̬
	{
		if (x < cities[curPath.back()->destID].GetX())//��һվ�ڵ�ǰλ�õ��ұ�
		{
			putimage(x - 20, y - 30, TRANSPORT_HEIGHT, TRANSPORT_HEIGHT, &imgTransCover[curPath.back()->kind], TRANSPORT_HEIGHT, 0, SRCPAINT);
			putimage(x - 20, y - 30, TRANSPORT_HEIGHT, TRANSPORT_HEIGHT, &imgTrans[curPath.back()->kind], TRANSPORT_HEIGHT, 0, SRCAND);
		}
		else
		{
			putimage(x - 20, y - 30, TRANSPORT_HEIGHT, TRANSPORT_HEIGHT, &imgTransCover[curPath.back()->kind], 0, 0, SRCPAINT);
			putimage(x - 20, y - 30, TRANSPORT_HEIGHT, TRANSPORT_HEIGHT, &imgTrans[curPath.back()->kind], 0, 0, SRCAND);
		}

		text = "·;�ƶ���"; //״̬
	}
	else//����Ǿ�ֹ״̬��������ͼ����ӡ״̬
	{
		putimage(x, y - 48, &imgPersonCover[seqNum % 2], SRCPAINT);
		putimage(x, y - 48, &imgPerson[seqNum % 2], SRCAND);

		if (state == WAITING)
		{
			text = "�ȴ�������";
		}
		else
		{
			text = "����Ŀ�ĵ�";
		}
	}
	font.lfHeight = 26; // ��������߶�
	settextstyle(&font);           // ����������ʽ
	textString = text.c_str();
	outtextxy(1255, 636, textString);//��״̬��ӡ����

	//��ӡ·;�ܷ���
	text = std::to_string(idleResult.idleRisk);
	textString = text.c_str();
	outtextxy(1255, 677, textString);//��ӡ����

	//��ӡ·;��ʱ��
	text = std::to_string(idleResult.idleTime);
	textString = text.c_str();
	outtextxy(1255, 717, textString);//��ӡ����

	//��ӡ�޶�ʱ��
	textString = text2.c_str();
	outtextxy(1255, 556, textString);

	//��ӡ����ʱ��
	text = std::to_string(usedHours);
	textString = text.c_str();
	outtextxy(1255, 596, textString);//��ӡ����

	//��ӡ�˿�
	text = name;
	textString = text.c_str();
	outtextxy(1260, 20, textString);

	//��ӡ������
	text = cities[departure].GetCityName();
	textString = text.c_str();
	outtextxy(1260, 61, textString);

	//��ӡĿ�ĵ�
	text = cities[destination].GetCityName();
	textString = text.c_str();
	outtextxy(1260, 101, textString);
}

void Passenger::PrintWhenChange()const
{
	std::string text;
	CString textString;

	//��������
	LOGFONT font;
	gettextstyle(&font);// ��ȡ��ǰ��������
	font.lfHeight = 26; // ��������߶�
	font.lfWeight = FW_BOLD;
	font.lfQuality = PROOF_QUALITY;    // �������Ч��Ϊ����� 
	_tcscpy_s(font.lfFaceName, _T("΢���ź�"));
	settextstyle(&font);  // ����������ʽ
	settextcolor(RGB(53, 81, 117));

	//��ӡ�˿�
	text = name;
	textString = text.c_str();
	outtextxy(1256, 22, textString);

	//��ӡ������
	text = cities[passDeparture].GetCityName();
	textString = text.c_str();
	outtextxy(1255, 63, textString);

	//��ӡĿ�ĵ�
	text = cities[passDestination].GetCityName();
	textString = text.c_str();
	outtextxy(1255, 103, textString);

	if (state == MOVING)//������ƶ�״̬,����ͨ���ߣ�����ӡ״̬
	{
		if (x < cities[curPath.back()->destID].GetX())//��һվ�ڵ�ǰλ�õ��ұ�
		{
			putimage(x - 20, y - 30, TRANSPORT_HEIGHT, TRANSPORT_HEIGHT, &imgTransCover[curPath.back()->kind], TRANSPORT_HEIGHT, 0, SRCPAINT);
			putimage(x - 20, y - 30, TRANSPORT_HEIGHT, TRANSPORT_HEIGHT, &imgTrans[curPath.back()->kind], TRANSPORT_HEIGHT, 0, SRCAND);
		}
		else
		{
			putimage(x - 20, y - 30, TRANSPORT_HEIGHT, TRANSPORT_HEIGHT, &imgTransCover[curPath.back()->kind], 0, 0, SRCPAINT);
			putimage(x - 20, y - 30, TRANSPORT_HEIGHT, TRANSPORT_HEIGHT, &imgTrans[curPath.back()->kind], 0, 0, SRCAND);
		}
	}
	else//����Ǿ�ֹ״̬��������ͼ����ӡ״̬
	{
		putimage(x, y - 48, &imgPersonCover[seqNum % 2], SRCPAINT);
		putimage(x, y - 48, &imgPerson[seqNum % 2], SRCAND);
	}
}

void Passenger::PrintLog()const
{
	int totalHops = curPath.size();
	std::string trans[3], passStates[3], timeString;
	trans[AIR] = "�ɻ�";
	trans[TRAIN] = "��";
	trans[CAR] = "����";
	passStates[WAITING - 1] = "�ȴ�������";
	passStates[MOVING - 1] = "·;�ƶ���";
	passStates[ARRIVE - 1] = "����Ŀ�ĵ�";

	//��ӡ��־
	std::cout << "�˿�������" << name << "\n";
	std::cout << "�˿���ţ�" << seqNum + 1 << "\n";
	std::cout << "����ʱ�䣺" << printStartTIME << "\n";
	std::cout << "�˿ͳ����أ�" << cities[departure].GetCityName() << "\n";
	std::cout << "�˿�Ŀ�ĵأ�" << cities[destination].GetCityName() << "\n";
	//��ӡ���ļ���
	fileLog << "�˿�������" << name << "\n";
	fileLog << "�˿���ţ�" << seqNum + 1 << "\n";
	fileLog << "����ʱ�䣺" << printStartTIME << "\n";
	fileLog << "�˿ͳ����أ�" << cities[departure].GetCityName() << "\n";
	fileLog << "�˿�Ŀ�ĵأ�" << cities[destination].GetCityName() << "\n";

	switch (strategy)
	{
	case MIN_RISK:
		std::cout << "�˿����в��ԣ���С���ղ���\n";
		fileLog << "�˿����в��ԣ���С���ղ���\n";
		timeString = "������ʱ��";
		break;
	case MIN_TIME:
		std::cout << "�˿����в��ԣ���Сʱ�����\n";
		fileLog << "�˿����в��ԣ���Сʱ�����\n";
		timeString = "������ʱ��";
		break;
	default:
		std::cout << "�˿����в��ԣ���ʱ��С���ղ��� ���޶�ʱ�䣺" << expectedTime << "Сʱ)\n";
		fileLog << "�˿����в��ԣ���ʱ��С���ղ��� ���޶�ʱ�䣺" << expectedTime << "Сʱ)\n";
		timeString = std::to_string(expectedTime);
		if (res == -1)//����Сʱ�䶼����������
		{
			//��ӡ������Ϣ
			std::cout << "(���޶�ʱ����û�п���·�ߣ��ʰ�����Сʱ�䰲��·��)\n";
			fileLog << "(���޶�ʱ����û�п���·�ߣ��ʰ�����Сʱ�䰲��·��)\n";
		}
		break;
	}

	std::cout << "�˿͵�ǰ״̬��" << passStates[state - 1] << "\n";
	fileLog << "�˿͵�ǰ״̬��" << passStates[state - 1] << "\n";
	if (state != MOVING)
	{
		std::cout << "�˿͵�ǰ���ڳ��У�" << cities[curCity].GetCityName() << "\n";
		fileLog << "�˿͵�ǰ���ڳ��У�" << cities[curCity].GetCityName() << "\n";
	}
	else//MOVINGҲ��֤��totalHops������0
	{
		std::cout << "�˿͵�ǰ������ͨ���ߣ�" << trans[curPath[totalHops - 1]->kind] << "\n";
		fileLog << "�˿͵�ǰ������ͨ���ߣ�" << trans[curPath[totalHops - 1]->kind] << "\n";
	}

	if (state == ARRIVE)
	{
		std::cout << "�˿��ѵ���Ŀ�ĵ�\n";
		fileLog << "�˿��ѵ���Ŀ�ĵ�\n";
	}
	else//��ӡ·�ߵ���־��
	{
		if (totalHops == 0)
		{
			std::cout << "�˿���ʣ��·��\n";
			fileLog << "�˿���ʣ��·��\n";
		}
		else
		{
			std::cout << "�˿�ʣ��·�����£�\n";
			fileLog << "�˿�ʣ��·�����£�\n";
			for (int i = totalHops - 1; i >= 0; i--)
			{
				std::cout << cities[curPath[i]->departID].GetCityName() << "��>" << cities[curPath[i]->destID].GetCityName() << "��" << trans[curPath[i]->kind]
					<< "��" << curPath[i]->number << "��" << curPath[i]->departTime << ":00��" << curPath[i]->arrivalTime << ":00\n";//�������У�������У���ͨ��ʽ�����,����ʱ�䣬����ʱ��

				fileLog << cities[curPath[i]->departID].GetCityName() << "��>" << cities[curPath[i]->destID].GetCityName() << "��" << trans[curPath[i]->kind]
					<< "��" << curPath[i]->number << "��" << curPath[i]->departTime << ":00��" << curPath[i]->arrivalTime << ":00\n";//�������У�������У���ͨ��ʽ�����,����ʱ�䣬����ʱ��
			}
		}
	}

	std::cout << "·���ܷ��գ�" << idleResult.idleRisk << "\n";
	std::cout << "·���ܺ�ʱ��" << idleResult.idleTime << "\n";
	std::cout << "�޶�ʱ�䣺" << timeString << "\n";
	std::cout << "����ʱ�䣺" << usedHours << "\n����������������������������������������������������������\n\n";

	fileLog << "·���ܷ��գ�" << idleResult.idleRisk << "\n";
	fileLog << "·���ܺ�ʱ��" << idleResult.idleTime << "\n";
	fileLog << "�޶�ʱ�䣺" << timeString << "\n";
	fileLog << "����ʱ�䣺" << usedHours << "\n����������������������������������������������������������\n\n";
}

int InputTime()
{
	char s[10];
	int days = 0, hours = 0;
	InputBox(s, 10, _T("����������������0��������"));
	sscanf_s(s, "%d", &days);

	InputBox(s, 10, _T("������Сʱ������0-23��������"));
	sscanf_s(s, "%d", &hours);

	return (days * 24 + hours);
}

std::string InputPassName()
{
	char name[20];
	InputBox(name, 20, _T("������˿�������"));

	std::string nameString = name;
	return name;
}
