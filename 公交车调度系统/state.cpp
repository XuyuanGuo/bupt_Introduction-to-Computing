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

//目标站判定
int flagofPassingstation;

//公交车变量
struct Bus bus;

//站点变量
struct Station stations[10];

struct POSITION position[32]; 

void gettime() {
	printf("%d/n", bustime);
}

//状态机函数 再main（）循环中，每出现一次clock 调用一次。需要知道当前bus的坐标，目标的站点编号（编号从0开始计数），总站点数，站点间隔。
void state_machine() {
	//没有目标时不改变状态
	if (bus.goal == -1)
		return;
	//计算顺逆行驶所需时间（不包括停顿等待时间）
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
	//公交车状态变化
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
			//改变目标
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

//公交车坐标处理（图形化要修改）
void bus_position_process() {
	if (bus.pos == totalStation * distance) {
		bus.pos = 0;
	}
	if (bus.pos == -1) {
		bus.pos = totalStation * distance - 1;
	}
}

//接口，调用 state_machine(),bus_position_process()
void bus_changing() {
	state_machine();
	bus_position_process();

}


//站点状态变化
void station_changing() {
	//出现新请求
	if (strcmp(input.cmd, "clockwise") == 0) {
		stations[input.num - 1].state[0] = 1;
	}
	else if (strcmp(input.cmd, "counterclockwise") == 0) {
		stations[input.num - 1].state[1] = 1;
	}
	else if (strcmp(input.cmd, "target") == 0) {
		stations[input.num - 1].state[2] = 1;
	}
	//到达目标,如果是策略1，则根据目标的请求类型，进行站点的状态改变
	else switch (strategy) {
	case FCFS:
		//循环处理，解决特殊情况（多个连续的同站点请求）
		while (bus.goal * distance == bus.pos) {
			stations[bus.goal].state[bus.goal_state] = 0;
			finishgoal();
		}
		break;
	case SSTF:
		//目标站处理
		if (bus.goal * distance == bus.pos) {
			for (int i = 0; i < 3; i++) {
				stations[bus.goal].state[i] = 0;
			}
		}
		else {
			//顺便站处理
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
	case SCAN://姜杨皓加
		//循环处理，解决特殊情况（多个连续的同站点请求）
		if (bus.goal * distance == bus.pos) {
			for(int i=0;i<3;i++)
			stations[bus.goal].state[i] = 0;
			//finishgoal();
		}
		break;
	}

}

int  busderection = 0;//公交车行驶方向姜杨皓加；0无方向 1顺 -1逆
