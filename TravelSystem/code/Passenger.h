#pragma once
#include<iostream>
#include<vector>
#include"Graph.h"


class Passenger
{
	int seqNum;		 //�ڼ�λ�˿�
	std::string name;//�˿�����
	int startTime;	 //�˿��ƶ��ƻ���ʱ��
	double preciseStartTIME;//��ȷ��С���ĳ˿��ƶ��ƻ���ʱ��
	int departure;	//������
	int destination;//�յ����
	int strategy;	//ѡ��Ĳ��� 1:��С���գ�2:���ʱ�䣻3����ʱ��С����
	int state;		//�˿͵�״̬  1�ȴ�����/2����Ŀ�ĵ�/3��·��
	int x;			//��ͼ�ϵ�x����
	int y;			//��ͼ�ϵ�y����
	int curCity;	//�ÿ͵�ǰ���ڳ���,���ķ�����ʱ����õ�
	int expectedTime; //�޶���ʱ��
	Result idleResult; //����Ľ��

	int travelHours;//ĳһ�˳����ú�ʱ��Сʱ
	int usedHours;//����ʱ��
	int printStartTIME;//��ӡ�õĳ���ʱ��
	int res;

	//��·�����յ�������±���0�������ջ��
	std::vector<Vehicle*> minRiskPath;//���ٷ��ղ��Ե�·��
	std::vector<Vehicle*> minTimePath;//����ʱ����Ե�·��
	std::vector<Vehicle*> limitTimeMinRiskPath;//��ʱ���ٷ��ղ��Ե�·��
	std::vector<Vehicle*> curPath;//��ǰ��δ��ɵ�·��

public:
	clock_t lastXMoveTime;//��һ��x�ƶ���ʱ�䣨��Сʱ��ָ����
	clock_t lastYMoveTime;//��һ��y�ƶ���ʱ�䣨��Сʱ��ָ����

	Passenger(int seqNum, int startTime,double pTIME, int departure, int destination, int strategy, int expectedTime, clock_t time, std::string name);
	int GetStrategy()const;
	int GetCurCity()const;
	int GetDepart()const;
	int GetDest()const;
	int GetExpectedTime()const;
	int GetUsedHours()const;
	int GetState()const;
	std::string GetName()const;
	void ChangeDest(int newDest, int newTime);//�ÿͱ���ƻ�
	void ChangeStrategy(int newStrategy, int newTime);//�ÿͱ������
	void UpdateHours();//�����Ѿ��õ�ʱ��
	void MakePlan(); //�ƶ�����
	void MovePassenger();//���³˿�λ�ü�״̬
	Result MakeMinPath(int stra);//�ƶ�����ʱ��/���ղ��Ե�·�߷���
	int MakelimitTimeMinRiskPath();//�ƶ���ʱ���ٷ��ղ��Ե�·�߷���
	void Print()const;//����ģʽ�´�ӡ�˿���Ϣ�����������棩
	void PrintWhenChange()const;//�������з���ģʽ�´�ӡ�ĳ˿���Ϣ�����������棩
	void PrintLog()const;//��ӡ�˿���Ϣ����־��
	bool whetherRemake()const;
};
