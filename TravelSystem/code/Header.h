#ifndef HEADER_H
#define HEADER_H

#include<iostream>
#include<string>
#include<cstdlib>
#include<vector>
#include<fstream>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <atlstr.h>

#include"Graph.h"
#include"Passenger.h"


#define CAR 0 
#define TRAIN 1
#define AIR 2
#define HIGH 0.9
#define MID 0.5
#define LOW 0.2
#define TIME_DURATION 10000 //ÿ��ʱ�䵥λ��Сʱ����Ӧ�ĺ���������ÿʮ��ΪһСʱ
#define UPDATE_INTEVAL 50 //���»�����

#define MIN_RISK 0
#define MIN_TIME 1
#define LIMIT_TIME_MIN_RISK 2

#define WAITING 1
#define MOVING 2
#define ARRIVE 3

#define CLICK_DEPART 0
#define CLICK_DEST 1
#define CLICK_STRATEGY 2
#define NORMAL 3
#define CHANGE_DEST 4
#define CHANGE_STRA 5

#define CANVAS_WIDTH 1436
#define CANVAS_HEIGHT 758
#define TRANSPORT_WIDTH 80
#define TRANSPORT_HEIGHT 40
#define HINT_WIDTH 55
#define HINT_HEIGHT 36

extern std::ofstream fileLog;
extern bool isStop;
extern int curState;
extern int TIME;//ȫ��ʱ�䣨��ʾ��ʱ�䣩
extern double preciseTIME;//ȫ��ʱ�䣬��ȷС��
extern std::vector<City> cities;
extern std::vector<Passenger> passengerList;
extern IMAGE imgBackground1, imgBackground2, imgBackground3;
extern IMAGE imgDepart;
extern IMAGE imgDest;
extern IMAGE imgHintOuttime;
extern IMAGE imgHintDest;
extern IMAGE imgHintCover;
extern IMAGE imgList;
extern IMAGE imgTrans[3];//��ͨ����
extern IMAGE imgTransCover[3];//��ͨ��������
extern IMAGE imgStrategy[3];
extern IMAGE imgPerson[2];
extern IMAGE imgPersonCover[2];
extern IMAGE imgChoose[3];

extern clock_t startTime;//��ʼʱ��
extern clock_t stopTime;//��ͣʱ��
extern clock_t endStopTime;//������ͣ��ʱ�䣨���������ʱ�䣩
extern clock_t stopInterval;//��ͣ���
extern clock_t totStopInterval;//�ܹ���ͣʱ��
extern clock_t currentTime;//��ǰʱ��

extern int passStartTime;	//�˿ͳ���ʱ��
extern int passDeparture;	//������
extern int passDestination;//�յ����
extern int passStrategy;	//ѡ��Ĳ��� 1:��С���գ�2:���ʱ�䣻3����ʱ��С����
extern int passExpectedTime; //�޶���ʱ��


void InitTable();	//���ڳ�ʼ��ʱ�̱�
void InitCityList();//���ڳ�ʼ�������б�
int ToCityID(std::string name);//
void Continue();//
void Stop();//
int InputTime();//
std::string InputPassName();//
void StartSystem();//
void TransferSystemState();//
void Update();//
void LoadImg();		//����ͼƬ
void InitCanvas();	//��ʼ������
void Draw(int curPassId);//
bool Between(int left, int right, int top, int bottom, int x, int y);//
int MatchCity(int x, int y);//
int MatchStrategy(int x, int y);//

#endif // !HEADER_H