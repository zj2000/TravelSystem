#include"Header.h"

IMAGE imgBackground1, imgBackground2, imgBackground3;
IMAGE imgDepart;
IMAGE imgDest;
IMAGE imgHintOuttime;
IMAGE imgHintDest;
IMAGE imgHintCover;
IMAGE imgList;
IMAGE imgTrans[3];//��ͨ����
IMAGE imgTransCover[3];//��ͨ��������
IMAGE imgStrategy[3];
IMAGE imgPerson[2];
IMAGE imgPersonCover[2];
IMAGE imgChoose[3];//ѡ��ʱ����ʾ

void InitCityList()
{
	cities.push_back(City(0, "����", 711, 315, HIGH));
	cities.push_back(City(1, "����", 788, 523, HIGH));
	cities.push_back(City(2, "����", 692, 612, HIGH));
	cities.push_back(City(3, "����", 523, 589, HIGH));
	cities.push_back(City(4, "������", 822, 206, HIGH));
	cities.push_back(City(5, "�人", 678, 481, HIGH));

	cities.push_back(City(6, "����", 762, 580, MID));
	cities.push_back(City(7, "�ɶ�", 528, 483, MID));
	cities.push_back(City(8, "����", 728, 385, MID));
	cities.push_back(City(9, "��ɳ", 676, 536, MID));
	cities.push_back(City(10, "���ͺ���", 635, 323, MID));

	cities.push_back(City(11, "�Ϻ�", 796, 466, LOW));
	cities.push_back(City(12, "����", 609, 424, LOW));
	cities.push_back(City(13, "��³ľ��", 333, 247, LOW));
	cities.push_back(City(14, "����", 352, 490, LOW));
	cities.push_back(City(15, "����", 614, 626, LOW));
}

void InitTable()
{
	std::string number;//���
	std::string startCity;//�׷�վ
	std::string endCity;//�յ�վ
	int startHour;//�׷�ʱ��
	int endHour;//��վʱ��
	int days; //��վ���׷�ʱ��֮�����������
	int kind; //��ͨ����

	std::ifstream fileIn;//���ļ�����ʱ���
	fileIn.open("./res/table.txt", std::ios::in);//���ļ�
	if (fileIn.is_open())//����򿪳ɹ�
	{
		while (!fileIn.eof())
		{
			fileIn >> kind >> startCity >> endCity >> number >> startHour >> endHour >> days;

			//����������ת�������Ӧ��ID��
			int startID = ToCityID(startCity);
			int endID = ToCityID(endCity);
			
			cities[startID].AddNeighbor(kind, startID, startHour, endID, endHour, days, number);//���׷�վ�����ھ�
		}

	}
	fileIn.close();//�ر��ļ�
}

void LoadImg()
{
	loadimage(&imgBackground1, _T("./res/bg.png"), CANVAS_WIDTH, CANVAS_HEIGHT);
	loadimage(&imgBackground2, _T("./res/bg2-2.png"), CANVAS_WIDTH, CANVAS_HEIGHT);
	loadimage(&imgBackground3, _T("./res/bg2-3.png"), CANVAS_WIDTH, CANVAS_HEIGHT);
	loadimage(&imgDepart, _T("./res/departure.png"), HINT_WIDTH, HINT_HEIGHT);
	loadimage(&imgDest, _T("./res/destination.png"), HINT_WIDTH, HINT_HEIGHT);
	loadimage(&imgHintOuttime, _T("./res/hint3.png"), 463, 41);
	loadimage(&imgHintCover, _T("./res/hintcover.png"), HINT_WIDTH, HINT_HEIGHT);
	loadimage(&imgList, _T("./res/list.png"), 80, 39);

	loadimage(&imgTrans[AIR], _T("./res/air.png"), TRANSPORT_WIDTH, TRANSPORT_HEIGHT);
	loadimage(&imgTransCover[AIR], _T("./res/aircover.png"), TRANSPORT_WIDTH, TRANSPORT_HEIGHT);
	loadimage(&imgTrans[TRAIN], _T("./res/train.png"), TRANSPORT_WIDTH, TRANSPORT_HEIGHT);
	loadimage(&imgTransCover[TRAIN], _T("./res/traincover.png"), TRANSPORT_WIDTH, TRANSPORT_HEIGHT);
	loadimage(&imgTrans[CAR], _T("./res/bus.png"), TRANSPORT_WIDTH, TRANSPORT_HEIGHT);
	loadimage(&imgTransCover[CAR], _T("./res/buscover.png"), TRANSPORT_WIDTH, TRANSPORT_HEIGHT);

	loadimage(&imgStrategy[0], _T("./res/strategy1.png"), 223, 116);
	loadimage(&imgStrategy[1], _T("./res/strategy2.png"), 223, 116);
	loadimage(&imgStrategy[2], _T("./res/strategy3.png"), 223, 116);

	loadimage(&imgPerson[0], _T("./res/passenger1.png"), 32, 48);
	loadimage(&imgPerson[1], _T("./res/passenger2.png"), 32, 48);
	loadimage(&imgPersonCover[0], _T("./res/passengerCover1.png"), 32, 48);
	loadimage(&imgPersonCover[1], _T("./res/passengerCover2.png"), 32, 48);

	loadimage(&imgChoose[0], _T("./res/choose1.png"), 381, 41);
	loadimage(&imgChoose[1], _T("./res/choose2.png"), 381, 41);
	loadimage(&imgChoose[2], _T("./res/choose3.png"), 381, 41);
	loadimage(&imgHintDest, _T("./res/hint1.png"), 381, 41);
}

void InitCanvas()
{
	initgraph(CANVAS_WIDTH, CANVAS_HEIGHT, SHOWCONSOLE);

	putimage(0, 0, &imgBackground1);

	setbkmode(TRANSPARENT);//�������屳��Ϊ͸��
}

int ToCityID(std::string city)
{
	int num = 0;

	if (city == "Beijing")
		num = 0;
	else if (city == "Wenzhou")
		num = 1;
	else if (city == "Guangzhou")
		num = 2;
	else if (city == "Kunming")
		num = 3;
	else if (city == "Haerbin")
		num = 4;
	else if (city == "Wuhan")
		num = 5;
	else if (city == "Xiamen")
		num = 6;
	else if (city == "Chengdu")
		num = 7;
	else if (city == "Jinan")
		num = 8;
	else if (city == "Changsha")
		num = 9;
	else if (city == "Huhehaote")
		num = 10;
	else if (city == "Shanghai")
		num = 11;
	else if (city == "Xi'an")
		num = 12;
	else if (city == "Wulumuqi")
		num = 13;
	else if (city == "Lasa")
		num = 14;
	else if (city == "Nanning")
		num = 15;
	else
		;

	return num;
}
