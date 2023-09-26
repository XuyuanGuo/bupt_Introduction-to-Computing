#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<conio.h>
#include<graphics.h>
#include<easyx.h>
#include"input.h"
#include"output.h"
#include"state.h"
#include"strategy.h"
#include"main.h"
#include"cartoon.h" 
using namespace std;

//Ŀ��վ�ж�
int flagofPassingstation;

//����������
struct Bus bus;

//վ�����
struct Station stations[10];

struct POSITION position[32]; 

void gettime() {
	printf("%d/n", bustime);
}

//״̬������ ��main����ѭ���У�ÿ����һ��clock ����һ�Ρ���Ҫ֪����ǰbus�����꣬Ŀ���վ���ţ���Ŵ�0��ʼ����������վ������վ������
void state_machine() {
	//û��Ŀ��ʱ���ı�״̬
	if (bus.goal == -1)
		return;
	//����˳����ʻ����ʱ�䣨������ͣ�ٵȴ�ʱ�䣩
	int goal_x = bus.goal * distance;
	int max_x = totalStation * distance - 1;
	int time_clkw;
	int time_cclkw;
	if (goal_x > bus.pos) {
		time_clkw = goal_x - bus.pos;
		time_cclkw = max_x - time_clkw + 1;
	}
	else if (goal_x < bus.pos) {
		time_cclkw = bus.pos - goal_x;
		time_clkw = max_x - time_cclkw + 1;
	}
	else {
		time_clkw = time_cclkw = 0;
	}
	//������״̬�仯
	switch (bus.state) {
	case FREE:
	{
		if (~flagofPassingstation) {
			station_changing();
			finishpassingstation();
			flagofPassingstation = -1;
		}
		else if ((time_clkw == time_cclkw && time_clkw != 0) || time_clkw < time_cclkw) {
			bus.state = CLCW;
		}
		else if (time_clkw > time_cclkw) {
			bus.state = CCLCW;
		}
		else {
			//�ı�Ŀ��
			if (queueLength > 0) {
				station_changing();
				finishgoal();
				state_machine();
			}
		}
	}
	break;
	case CLCW:
	{
		bus.pos++;
		bus_position_process();
		bus.x=position[bus.pos].x;
		bus.y=position[bus.pos].y;
		if (isgoal()) {
			bus.state = FREE;
		}
		else if (ispassingstation()) {
			flagofPassingstation = CLCW;
			bus.state = FREE;
		}
	}
	break;
	case CCLCW:
	{
		bus.pos--;
		bus_position_process();
		bus.x=position[bus.pos].x;
		bus.y=position[bus.pos].y;
		if (isgoal()) {
			bus.state = FREE;
		}
		else if (ispassingstation()) {
			flagofPassingstation = CCLCW;
			bus.state = FREE;
		}
	}
	break;
	}
}

//���������괦��ͼ�λ�Ҫ�޸ģ�
void bus_position_process() {
	if (bus.pos == totalStation * distance) {
		bus.pos = 0;
	}
	if (bus.pos == -1) {
		bus.pos = totalStation * distance - 1;
	}
}

//�ӿڣ����� state_machine(),bus_position_process()
void bus_changing() {
	state_machine();
	bus_position_process();

}


//վ��״̬�仯
void station_changing() {
	//����������
	if (strcmp(input.cmd, "clockwise") == 0) {
		stations[input.num - 1].state[0] = 1;
	}
	else if (strcmp(input.cmd, "counterclockwise") == 0) {
		stations[input.num - 1].state[1] = 1;
	}
	else if (strcmp(input.cmd, "target") == 0) {
		stations[input.num - 1].state[2] = 1;
	}
	//����Ŀ��,����ǲ���1�������Ŀ����������ͣ�����վ���״̬�ı�
	else switch (strategy) {
	case FCFS:
		//ѭ���������������������������ͬվ������
		while (bus.goal * distance == bus.pos) {
			stations[bus.goal].state[bus.goal_state] = 0;
			finishgoal();
		}
		break;
	case SSTF:
		//Ŀ��վ����
		if (bus.goal * distance == bus.pos) {
			for (int i = 0; i < 3; i++) {
				stations[bus.goal].state[i] = 0;
			}
		}
		else {
			//˳��վ����
			if (~flagofPassingstation) {
				for (int i = 0; i < 10; i++) {
					if (bus.pos == stations[i].pos) {
						stations[i].state[flagofPassingstation] = 0;
						stations[i].state[2] = 0;
						break;
					}
				}
			}
		}
		break;
	case SCAN://������
		//ѭ���������������������������ͬվ������
		if (bus.goal * distance == bus.pos) {
			for(int i=0;i<3;i++)
			stations[bus.goal].state[i] = 0;
			//finishgoal();
		}
		break;
	}

}

int  busderection = 0;//��������ʻ�������ӣ�0�޷��� 1˳ -1��
