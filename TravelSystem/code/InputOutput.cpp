#include"Header.h"


void Draw(int curPassId)
{
	BeginBatchDraw();

	int passengerAmount = passengerList.size();

	//设置字体
	LOGFONT font;
	gettextstyle(&font);// 获取当前字体设置
	font.lfWeight = FW_BOLD;
	_tcscpy_s(font.lfFaceName, _T("微软雅黑"));// 设置字体为“微软雅黑”
	settextstyle(&font);           // 设置字体样式

	std::string text;
	CString textString;

	//先贴背景图、策略图
	if (curState == NORMAL)
	{
		putimage(0, 0, &imgBackground2);
		if (passengerAmount != 0)//如果乘客列表不为空
		{
			putimage(1197, 171, &imgStrategy[passengerList[curPassId].GetStrategy()]);
			passengerList[curPassId].Print();//打印乘客相关信息

			//打印乘客表
			font.lfHeight = 36; // 设置字体高度
			settextstyle(&font);// 设置字体样式

			int height = 355;
			for (int i = 0; i < ((passengerAmount + 1) / 2); i++)//贴几次list图
			{
				putimage(999, height, &imgList);
				height += 41;
			}
			height = 355;
			for (int i = 0; i < passengerAmount; i++)
			{
				text = std::to_string(i + 1); //乘客序号
				textString = text.c_str();

				if (i == curPassId)
					settextcolor(RGB(136, 0, 27));//设置颜色,当前查询的用户其序号颜色为深红色
				else
					settextcolor(RGB(53, 81, 117));//设置颜色

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

		//贴出发地
		if (passDeparture != NOT_SET)
		{
			putimage(cities[passDeparture].GetX() - 27, cities[passDeparture].GetY() - 59, &imgHintCover, SRCPAINT);
			putimage(cities[passDeparture].GetX() - 27, cities[passDeparture].GetY() - 59, &imgDepart, SRCAND);
		}
		else
			;//do nothing
		//贴目的地
		if (passDestination != NOT_SET)
		{
			putimage(cities[passDestination].GetX() - 27, cities[passDestination].GetY() - 59, &imgHintCover, SRCPAINT);
			putimage(cities[passDestination].GetX() - 27, cities[passDestination].GetY() - 59, &imgDest, SRCAND);
		}
		else
			;//do nothing

		//贴提示选择的图
		putimage(366, 28, &imgChoose[curState % 3]);
		if ((curState == CLICK_DEST && passDeparture == passDestination)||
			(curState == CHANGE_DEST && passDestination == passDeparture && passDeparture == passengerList[curPassId].GetDepart()))
		{

			putimage(366, 28, &imgHintDest);
		}

		//贴策略图和打印相关信息
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

	//打印时间
	font.lfHeight = 42; // 设置字体高度
	settextstyle(&font);// 设置字体样式
	settextcolor(RGB(53, 81, 117));//设置颜色

	text = std::to_string(TIME) + "：00"; //全局时间
	textString = text.c_str();
	outtextxy(124, 12, textString);

	EndBatchDraw();
}

void Passenger::Print()const
{
	//贴出发地和目的地的图
	putimage(cities[departure].GetX() - 27, cities[departure].GetY() - 59, &imgHintCover, SRCPAINT);
	putimage(cities[departure].GetX() - 27, cities[departure].GetY() - 59, &imgDepart, SRCAND);

	putimage(cities[destination].GetX() - 27, cities[destination].GetY() - 59, &imgHintCover, SRCPAINT);
	putimage(cities[destination].GetX() - 27, cities[destination].GetY() - 59, &imgDest, SRCAND);

	std::string text, text2;
	CString textString;

	//设置字体
	LOGFONT font;
	gettextstyle(&font);// 获取当前字体设置
	font.lfHeight = 20; // 设置字体高度
	font.lfWeight = FW_BOLD;
	font.lfQuality = PROOF_QUALITY;    // 设置输出效果为抗锯齿 
	_tcscpy_s(font.lfFaceName, _T("微软雅黑"));
	settextstyle(&font);           // 设置字体样式

	//打印路径
	std::string trans[3];
	trans[AIR] = "飞机";
	trans[TRAIN] = "火车";
	trans[CAR] = "汽车";

	int totalHops, finishedHops;
	int unfinishedHops = curPath.size();
	int height = 333;

	//先打印已经走过的路
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
			//出发城市，到达城市，交通方式，班次
			text = cities[minRiskPath[totalHops - i - 1]->departID].GetCityName() + "->" + cities[minRiskPath[totalHops - i - 1]->destID].GetCityName() + "，"
				+ trans[minRiskPath[totalHops - i - 1]->kind] + "，" + minRiskPath[totalHops - i - 1]->number + ","
				+ std::to_string(minRiskPath[totalHops - i - 1]->departTime) + ":00-" + std::to_string(minRiskPath[totalHops - i - 1]->arrivalTime) + ":00";
			textString = text.c_str();
			outtextxy(1130, height, textString);
			height += 25;

			line(cities[minRiskPath[totalHops - i - 1]->departID].GetX(), cities[minRiskPath[totalHops - i - 1]->departID].GetY(),
				cities[minRiskPath[totalHops - i - 1]->destID].GetX(), cities[minRiskPath[totalHops - i - 1]->destID].GetY());
		}
		text2 = "无限制时间";
		break;

	case MIN_TIME:
		totalHops = minTimePath.size();
		finishedHops = totalHops - unfinishedHops;
		for (int i = 0; i < finishedHops; i++)
		{
			//出发城市，到达城市，交通方式，班次
			text = cities[minTimePath[totalHops - i - 1]->departID].GetCityName() + "->" + cities[minTimePath[totalHops - i - 1]->destID].GetCityName() + "，"
				+ trans[minTimePath[totalHops - i - 1]->kind] + "，" + minTimePath[totalHops - i - 1]->number + ","
				+ std::to_string(minTimePath[totalHops - i - 1]->departTime) + ":00-" + std::to_string(minTimePath[totalHops - i - 1]->arrivalTime) + ":00";
			textString = text.c_str();
			outtextxy(1130, height, textString);
			height += 25;

			line(cities[minTimePath[totalHops - i - 1]->departID].GetX(), cities[minTimePath[totalHops - i - 1]->departID].GetY(),
				cities[minTimePath[totalHops - i - 1]->destID].GetX(), cities[minTimePath[totalHops - i - 1]->destID].GetY());
		}
		text2 = "无限制时间";
		break;

	default:
		totalHops = limitTimeMinRiskPath.size();
		finishedHops = totalHops - unfinishedHops;
		for (int i = 0; i < finishedHops; i++)
		{
			//出发城市，到达城市，交通方式，班次
			text = cities[limitTimeMinRiskPath[totalHops - i - 1]->departID].GetCityName() + "->" + cities[limitTimeMinRiskPath[totalHops - i - 1]->destID].GetCityName() + "，"
				+ trans[limitTimeMinRiskPath[totalHops - i - 1]->kind] + "，" + limitTimeMinRiskPath[totalHops - i - 1]->number + ","
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

	//再打印还没走过的路
	settextcolor(RGB(53, 81, 117));//设置颜色
	setlinecolor(RGB(0, 168, 243));
	for (int i = unfinishedHops - 1; i >= 0; i--)
	{
		text = cities[curPath[i]->departID].GetCityName() + "->" + cities[curPath[i]->destID].GetCityName() + "，"
			+ trans[curPath[i]->kind] + "，" + curPath[i]->number + ","
			+ std::to_string(curPath[i]->departTime) + ":00-" + std::to_string(curPath[i]->arrivalTime) + ":00";;//出发城市，到达城市，交通方式，班次
		textString = text.c_str();
		outtextxy(1130, height, textString);
		height += 25;

		line(cities[curPath[i]->departID].GetX(), cities[curPath[i]->departID].GetY(),
			cities[curPath[i]->destID].GetX(), cities[curPath[i]->destID].GetY());
	}

	if (state == MOVING)//如果是移动状态,贴交通工具，并打印状态
	{
		if (x < cities[curPath.back()->destID].GetX())//下一站在当前位置的右边
		{
			putimage(x - 20, y - 30, TRANSPORT_HEIGHT, TRANSPORT_HEIGHT, &imgTransCover[curPath.back()->kind], TRANSPORT_HEIGHT, 0, SRCPAINT);
			putimage(x - 20, y - 30, TRANSPORT_HEIGHT, TRANSPORT_HEIGHT, &imgTrans[curPath.back()->kind], TRANSPORT_HEIGHT, 0, SRCAND);
		}
		else
		{
			putimage(x - 20, y - 30, TRANSPORT_HEIGHT, TRANSPORT_HEIGHT, &imgTransCover[curPath.back()->kind], 0, 0, SRCPAINT);
			putimage(x - 20, y - 30, TRANSPORT_HEIGHT, TRANSPORT_HEIGHT, &imgTrans[curPath.back()->kind], 0, 0, SRCAND);
		}

		text = "路途移动中"; //状态
	}
	else//如果是静止状态，贴人物图并打印状态
	{
		putimage(x, y - 48, &imgPersonCover[seqNum % 2], SRCPAINT);
		putimage(x, y - 48, &imgPerson[seqNum % 2], SRCAND);

		if (state == WAITING)
		{
			text = "等待出发中";
		}
		else
		{
			text = "到达目的地";
		}
	}
	font.lfHeight = 26; // 设置字体高度
	settextstyle(&font);           // 设置字体样式
	textString = text.c_str();
	outtextxy(1255, 636, textString);//把状态打印出来

	//打印路途总风险
	text = std::to_string(idleResult.idleRisk);
	textString = text.c_str();
	outtextxy(1255, 677, textString);//打印出来

	//打印路途总时间
	text = std::to_string(idleResult.idleTime);
	textString = text.c_str();
	outtextxy(1255, 717, textString);//打印出来

	//打印限定时间
	textString = text2.c_str();
	outtextxy(1255, 556, textString);

	//打印已用时间
	text = std::to_string(usedHours);
	textString = text.c_str();
	outtextxy(1255, 596, textString);//打印出来

	//打印乘客
	text = name;
	textString = text.c_str();
	outtextxy(1260, 20, textString);

	//打印出发地
	text = cities[departure].GetCityName();
	textString = text.c_str();
	outtextxy(1260, 61, textString);

	//打印目的地
	text = cities[destination].GetCityName();
	textString = text.c_str();
	outtextxy(1260, 101, textString);
}

void Passenger::PrintWhenChange()const
{
	std::string text;
	CString textString;

	//设置字体
	LOGFONT font;
	gettextstyle(&font);// 获取当前字体设置
	font.lfHeight = 26; // 设置字体高度
	font.lfWeight = FW_BOLD;
	font.lfQuality = PROOF_QUALITY;    // 设置输出效果为抗锯齿 
	_tcscpy_s(font.lfFaceName, _T("微软雅黑"));
	settextstyle(&font);  // 设置字体样式
	settextcolor(RGB(53, 81, 117));

	//打印乘客
	text = name;
	textString = text.c_str();
	outtextxy(1256, 22, textString);

	//打印出发地
	text = cities[passDeparture].GetCityName();
	textString = text.c_str();
	outtextxy(1255, 63, textString);

	//打印目的地
	text = cities[passDestination].GetCityName();
	textString = text.c_str();
	outtextxy(1255, 103, textString);

	if (state == MOVING)//如果是移动状态,贴交通工具，并打印状态
	{
		if (x < cities[curPath.back()->destID].GetX())//下一站在当前位置的右边
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
	else//如果是静止状态，贴人物图并打印状态
	{
		putimage(x, y - 48, &imgPersonCover[seqNum % 2], SRCPAINT);
		putimage(x, y - 48, &imgPerson[seqNum % 2], SRCAND);
	}
}

void Passenger::PrintLog()const
{
	int totalHops = curPath.size();
	std::string trans[3], passStates[3], timeString;
	trans[AIR] = "飞机";
	trans[TRAIN] = "火车";
	trans[CAR] = "汽车";
	passStates[WAITING - 1] = "等待出发中";
	passStates[MOVING - 1] = "路途移动中";
	passStates[ARRIVE - 1] = "到达目的地";

	//打印日志
	std::cout << "乘客姓名：" << name << "\n";
	std::cout << "乘客序号：" << seqNum + 1 << "\n";
	std::cout << "出发时间：" << printStartTIME << "\n";
	std::cout << "乘客出发地：" << cities[departure].GetCityName() << "\n";
	std::cout << "乘客目的地：" << cities[destination].GetCityName() << "\n";
	//打印到文件里
	fileLog << "乘客姓名：" << name << "\n";
	fileLog << "乘客序号：" << seqNum + 1 << "\n";
	fileLog << "出发时间：" << printStartTIME << "\n";
	fileLog << "乘客出发地：" << cities[departure].GetCityName() << "\n";
	fileLog << "乘客目的地：" << cities[destination].GetCityName() << "\n";

	switch (strategy)
	{
	case MIN_RISK:
		std::cout << "乘客旅行策略：最小风险策略\n";
		fileLog << "乘客旅行策略：最小风险策略\n";
		timeString = "无限制时间";
		break;
	case MIN_TIME:
		std::cout << "乘客旅行策略：最小时间策略\n";
		fileLog << "乘客旅行策略：最小时间策略\n";
		timeString = "无限制时间";
		break;
	default:
		std::cout << "乘客旅行策略：限时最小风险策略 （限定时间：" << expectedTime << "小时)\n";
		fileLog << "乘客旅行策略：限时最小风险策略 （限定时间：" << expectedTime << "小时)\n";
		timeString = std::to_string(expectedTime);
		if (res == -1)//若最小时间都不满足条件
		{
			//打印错误信息
			std::cout << "(该限定时间下没有可行路线，故按照最小时间安排路线)\n";
			fileLog << "(该限定时间下没有可行路线，故按照最小时间安排路线)\n";
		}
		break;
	}

	std::cout << "乘客当前状态：" << passStates[state - 1] << "\n";
	fileLog << "乘客当前状态：" << passStates[state - 1] << "\n";
	if (state != MOVING)
	{
		std::cout << "乘客当前所在城市：" << cities[curCity].GetCityName() << "\n";
		fileLog << "乘客当前所在城市：" << cities[curCity].GetCityName() << "\n";
	}
	else//MOVING也保证了totalHops不等于0
	{
		std::cout << "乘客当前乘坐交通工具：" << trans[curPath[totalHops - 1]->kind] << "\n";
		fileLog << "乘客当前乘坐交通工具：" << trans[curPath[totalHops - 1]->kind] << "\n";
	}

	if (state == ARRIVE)
	{
		std::cout << "乘客已到达目的地\n";
		fileLog << "乘客已到达目的地\n";
	}
	else//打印路线到日志上
	{
		if (totalHops == 0)
		{
			std::cout << "乘客无剩余路线\n";
			fileLog << "乘客无剩余路线\n";
		}
		else
		{
			std::cout << "乘客剩余路线如下：\n";
			fileLog << "乘客剩余路线如下：\n";
			for (int i = totalHops - 1; i >= 0; i--)
			{
				std::cout << cities[curPath[i]->departID].GetCityName() << "—>" << cities[curPath[i]->destID].GetCityName() << "，" << trans[curPath[i]->kind]
					<< "，" << curPath[i]->number << "，" << curPath[i]->departTime << ":00—" << curPath[i]->arrivalTime << ":00\n";//出发城市，到达城市，交通方式，班次,出发时间，到达时间

				fileLog << cities[curPath[i]->departID].GetCityName() << "—>" << cities[curPath[i]->destID].GetCityName() << "，" << trans[curPath[i]->kind]
					<< "，" << curPath[i]->number << "，" << curPath[i]->departTime << ":00—" << curPath[i]->arrivalTime << ":00\n";//出发城市，到达城市，交通方式，班次,出发时间，到达时间
			}
		}
	}

	std::cout << "路线总风险：" << idleResult.idleRisk << "\n";
	std::cout << "路线总耗时：" << idleResult.idleTime << "\n";
	std::cout << "限定时间：" << timeString << "\n";
	std::cout << "已用时间：" << usedHours << "\n—————————————————————————————\n\n";

	fileLog << "路线总风险：" << idleResult.idleRisk << "\n";
	fileLog << "路线总耗时：" << idleResult.idleTime << "\n";
	fileLog << "限定时间：" << timeString << "\n";
	fileLog << "已用时间：" << usedHours << "\n—————————————————————————————\n\n";
}

int InputTime()
{
	char s[10];
	int days = 0, hours = 0;
	InputBox(s, 10, _T("请输入天数：（≥0的整数）"));
	sscanf_s(s, "%d", &days);

	InputBox(s, 10, _T("请输入小时数：（0-23的整数）"));
	sscanf_s(s, "%d", &hours);

	return (days * 24 + hours);
}

std::string InputPassName()
{
	char name[20];
	InputBox(name, 20, _T("请输入乘客姓名："));

	std::string nameString = name;
	return name;
}
