//未完成：文件输入函数,主控函数,策略模块{初始化原始请求队列函数，生成目标，目标变化（完成目标，出现新请求）}
//图形化要修改的部分：void print(); void bus_position_process();
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
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

int totalStation=5;   //站台总数
int distance=2;		  //站台间距
int bustime;
int strategy=FCFS;   //策略
int totalNo;   //请求流水号
IMAGE img1,img2;

void panduanchonghe();//姜杨皓加，判断指令是否和所在站点重合

//初始化
void init() {
	//初始化时间
	bustime = 0;

	//初始化公交车
	bus.state = FREE;
	bus.pos = 0;
	bus.goal = -1;
	//初始化地图，站点

	for (int i = 0; i < totalStation; i++) {
		stations[i].state[0] = 0;
		stations[i].state[1] = 0;
		stations[i].state[2] = 0;
		stations[i].pos = i * distance;
	}

	//初始化调度队列
	totalNo = 0;
	queueLength = 0;
	memset(queue, 127, sizeof(queue));
	flagofPassingstation = -1;
	
	//加载背景图片
	loadimage(&img1,"./bk.jpg",1400,700);
	//创建窗口
	initgraph(1400, 700);
	setbkcolor(WHITE);
	cleardevice();
	drawAll();
	//公交车
	loadimage(&img2, "./bus.jpg", 30, 30);//250,250分别为图片拉扯后的宽度和高度
	//putimage(100,150, &img);
	bus.x=position[bus.pos].x ,bus.y=position[bus.pos].y ;
	putimage(bus.x- 15,bus.y- 15 ,&img2); //无法透视 500，100为左上角坐标
	//putimage(position[3].x - 15, position[3].y - 15, &img);
	 
}

//输入线程
void* Input(void*){
	while(strcmp(input.cmd, "end") != 0){
		inputCmd();
		//panduanchonghe();
		if(strategy==SCAN&&bus.state==FREE&&(input.num-1)*distance==bus.pos)
			continue;
		getgoal();
		dispatchqueueInsert();
		station_changing();
		bus_move();
		cleardevice();
		drawAll();
		putimage(bus.x- 15,bus.y- 15,&img2);
		if(!strcmp(input.cmd, "end"))
			break;
		strcpy(input.cmd,"clock");
	}
} 

//输出刷新线程
void* Output(void*){
	clock_t start_t,end_t;
	start_t=end_t=clock();
	while(strcmp(input.cmd, "end") != 0){
		start_t=clock();
		while(end_t/CLOCKS_PER_SEC-start_t/CLOCKS_PER_SEC<Clock){
			clock_t temp_t=clock();
			while(1.0*end_t/CLOCKS_PER_SEC-1.0*temp_t/CLOCKS_PER_SEC<1.0*Clock/frequency) {
				end_t=clock();
			}
			bus_move();
			cleardevice();
			drawAll();
			putimage(bus.x- 15,bus.y- 15,&img2);
			FlushBatchDraw();
		}
		getgoal();
		bustime++;
		bus_changing();
		bus_move();
		cleardevice();
		drawAll();
		putimage(bus.x- 15,bus.y- 15,&img2);
		FlushBatchDraw();
		//print();
	}
} 

//主控函数
int main() {
	inputFile();
	init();
	BeginBatchDraw();
	pthread_t th1,th2;
	pthread_create(&th1,NULL,Input,NULL);
	pthread_create(&th2,NULL,Output,NULL);
	pthread_join(th1,NULL);
	pthread_join(th2,NULL);
	EndBatchDraw();
	return 0;
}

void panduanchonghe() {
	if (strategy != SCAN)
		return;
	if (bus.state != FREE)
		return;
	if (input.num == -1)
		return;
	if((input.num-1)*distance==bus.pos){
		//inputCmd();
		panduanchonghe(); 
	}
}
