#pragma once
#include<iostream>
#include<vector>

const int CITY_NUM = 16;
const int NOT_SET = -1;
const int MAX_COST = 999999999;

typedef struct result
{
	double idleRisk;//·���ܷ���
	int idleTime;//·���ܺ�ʱ
}Result;

typedef struct stackinfo
{
	int cityID;//���еı��
	int wayFromLast;//����һ�����е�·�ı��
	double riskFromLast;//����һ�����еķ���ֵ
	int TimeFromlast;//����һ�����еĺ�ʱ
}StackInfo;

class Vertex
{
public:
	bool isMarked;		//��ʾ�ó����Ƿ񱻱�ǣ��ܹ�16������
	int minWayIndex;	//�ó��еĶ��ر��б�ѡ�е������ߵı��
	int preVertex;		//��Ǹó�����·���е�ǰһ������
	int startWaitTime;	//��һ�����е���������е�ʱ��
	double riskToDepart;//������ʼ��Ĵ���
	int timeToDepart;	//������ʼ���ʱ��

	Vertex();
	void setVertex(bool isMarked, int preVertex, double riskToDepart, int timeToDepart, int startWaitTime);
};

class Vehicle
{
public:
	int kind;		//��ͨ��ʽ
	int risk;		//�ý�ͨ���ߵķ���
	int destID;		//�յ�վ
	int departID;	//��ʼվ
	int departTime; //����ʱ��
	int arrivalTime;//����ʱ��
	int days;		//ʼ��ʱ��͵�վʱ��֮���Խ������
	
	int transTime;  //·;����ʱ��
	double transRisk;//·;����

	int xSpeed;//�������ٺ���x�������һ��
	int ySpeed;//�������ٺ���y�������һ��

	std::string number;//���

	Vehicle(int kind, int departID, int departTime, int destID, int arrivalTime, int days, std::string number);
};

class City
{
	int id;			//�������
	std::string name; //������
	int x;			//�ó����ڵ�ͼ�ϵ�x����
	int y;			//�ó����ڵ�ͼ�ϵ�y����
	double risk;	//������еķ���ֵ����/��/�ͣ�

public:
	std::vector<Vehicle> neighbor[CITY_NUM];//�ó��е��ھӵ�·����¼�����Ըó���Ϊ��㣬�ھ�Ϊ�յ�������

	City(int id, std::string name, int x, int y, double risk);//������Ĺ��캯��
	int GetID() const;//���س��е�ID
	int GetX()const;//���س��е�x����
	int GetY()const;//���س��е�y����
	double GetRisk()const;//���س��еķ���ֵ
	std::string GetCityName()const;//���س��е�����
	void AddNeighbor(int kind, int departID, int departTime, int destID, int arrivalTime, int days, std::string number);//����Ըó���Ϊ��㣬�ھ�Ϊ�յ�������
};