//δ��ɣ��ļ����뺯��,���غ���,����ģ��{��ʼ��ԭʼ������к���������Ŀ�꣬Ŀ��仯�����Ŀ�꣬����������}
//ͼ�λ�Ҫ�޸ĵĲ��֣�void print(); void bus_position_process();
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

int totalStation=5;   //վ̨����
int distance=2;		  //վ̨���
int bustime;
int strategy=FCFS;   //����
int totalNo;   //������ˮ��
IMAGE img1,img2;

void panduanchonghe();//�����ӣ��ж�ָ���Ƿ������վ���غ�

//��ʼ��
void init() {
	//��ʼ��ʱ��
	bustime = 0;

	//��ʼ��������
	bus.state = FREE;
	bus.pos = 0;
	bus.goal = -1;
	//��ʼ����ͼ��վ��

	for (int i = 0; i < totalStation; i++) {
		stations[i].state[0] = 0;
		stations[i].state[1] = 0;
		stations[i].state[2] = 0;
		stations[i].pos = i * distance;
	}

	//��ʼ�����ȶ���
	totalNo = 0;
	queueLength = 0;
	memset(queue, 127, sizeof(queue));
	flagofPassingstation = -1;
	
	//���ر���ͼƬ
	loadimage(&img1,"./bk.jpg",1400,700);
	//��������
	initgraph(1400, 700);
	setbkcolor(WHITE);
	cleardevice();
	drawAll();
	//������
	loadimage(&img2, "./bus.jpg", 30, 30);//250,250�ֱ�ΪͼƬ������Ŀ�Ⱥ͸߶�
	//putimage(100,150, &img);
	bus.x=position[bus.pos].x ,bus.y=position[bus.pos].y ;
	putimage(bus.x- 15,bus.y- 15 ,&img2); //�޷�͸�� 500��100Ϊ���Ͻ�����
	//putimage(position[3].x - 15, position[3].y - 15, &img);
	 
}

//�����߳�
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

//���ˢ���߳�
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

//���غ���
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
