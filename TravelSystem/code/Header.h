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
#define TIME_DURATION 10000 //每个时间单位（小时）对应的毫秒数，即每十秒为一小时
#define UPDATE_INTEVAL 50 //更新画面间隔

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
extern int TIME;//全局时间（显示的时间）
extern double preciseTIME;//全局时间，精确小数
extern std::vector<City> cities;
extern std::vector<Passenger> passengerList;
extern IMAGE imgBackground1, imgBackground2, imgBackground3;
extern IMAGE imgDepart;
extern IMAGE imgDest;
extern IMAGE imgHintOuttime;
extern IMAGE imgHintDest;
extern IMAGE imgHintCover;
extern IMAGE imgList;
extern IMAGE imgTrans[3];//交通工具
extern IMAGE imgTransCover[3];//交通工具掩码
extern IMAGE imgStrategy[3];
extern IMAGE imgPerson[2];
extern IMAGE imgPersonCover[2];
extern IMAGE imgChoose[3];

extern clock_t startTime;//开始时间
extern clock_t stopTime;//暂停时间
extern clock_t endStopTime;//结束暂停的时间（点击继续的时间）
extern clock_t stopInterval;//暂停间隔
extern clock_t totStopInterval;//总共暂停时间
extern clock_t currentTime;//当前时间

extern int passStartTime;	//乘客出发时间
extern int passDeparture;	//起点城市
extern int passDestination;//终点城市
extern int passStrategy;	//选择的策略 1:最小风险；2:最短时间；3：限时最小风险
extern int passExpectedTime; //限定的时间


void InitTable();	//用于初始化时刻表
void InitCityList();//用于初始化城市列表
int ToCityID(std::string name);//
void Continue();//
void Stop();//
int InputTime();//
std::string InputPassName();//
void StartSystem();//
void TransferSystemState();//
void Update();//
void LoadImg();		//加载图片
void InitCanvas();	//初始化画布
void Draw(int curPassId);//
bool Between(int left, int right, int top, int bottom, int x, int y);//
int MatchCity(int x, int y);//
int MatchStrategy(int x, int y);//

#endif // !HEADER_H