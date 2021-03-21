#pragma once
#include<iostream>
#include<vector>

const int CITY_NUM = 16;
const int NOT_SET = -1;
const int MAX_COST = 999999999;

typedef struct result
{
	double idleRisk;//路线总风险
	int idleTime;//路线总耗时
}Result;

typedef struct stackinfo
{
	int cityID;//城市的编号
	int wayFromLast;//到上一个城市的路的标号
	double riskFromLast;//到上一个城市的风险值
	int TimeFromlast;//到上一个城市的耗时
}StackInfo;

class Vertex
{
public:
	bool isMarked;		//表示该城市是否被标记，总共16个城市
	int minWayIndex;	//该城市的多重边中被选中的那条边的编号
	int preVertex;		//标记该城市在路线中的前一个城市
	int startWaitTime;	//上一个城市到达这个城市的时间
	double riskToDepart;//距离起始点的代价
	int timeToDepart;	//距离起始点的时间

	Vertex();
	void setVertex(bool isMarked, int preVertex, double riskToDepart, int timeToDepart, int startWaitTime);
};

class Vehicle
{
public:
	int kind;		//交通方式
	int risk;		//该交通工具的风险
	int destID;		//终点站
	int departID;	//起始站
	int departTime; //出发时间
	int arrivalTime;//到达时间
	int days;		//始发时间和到站时间之间跨越的天数
	
	int transTime;  //路途花费时间
	double transRisk;//路途风险

	int xSpeed;//经过多少毫秒x坐标更新一次
	int ySpeed;//经过多少毫秒y坐标更新一次

	std::string number;//班次

	Vehicle(int kind, int departID, int departTime, int destID, int arrivalTime, int days, std::string number);
};

class City
{
	int id;			//城市序号
	std::string name; //城市名
	int x;			//该城市在地图上的x坐标
	int y;			//该城市在地图上的y坐标
	double risk;	//这个城市的风险值（高/中/低）

public:
	std::vector<Vehicle> neighbor[CITY_NUM];//该城市到邻居的路，记录所有以该城市为起点，邻居为终点的有向边

	City(int id, std::string name, int x, int y, double risk);//城市类的构造函数
	int GetID() const;//返回城市的ID
	int GetX()const;//返回城市的x坐标
	int GetY()const;//返回城市的y坐标
	double GetRisk()const;//返回城市的风险值
	std::string GetCityName()const;//返回城市的名字
	void AddNeighbor(int kind, int departID, int departTime, int destID, int arrivalTime, int days, std::string number);//添加以该城市为起点，邻居为终点的有向边
};