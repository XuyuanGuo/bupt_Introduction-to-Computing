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

void drawAll(){
	putimage(0,0,&img1);
 	showinputarea();
 	output_graph();
 	pathway_graph();
}

void button(int x, int y, int w, int h, const char* text_) {

	setfillcolor(BLUE);//��ť��ɫ
	setlinecolor(RGB(135, 206, 235));//��ť����
	setlinestyle(PS_SOLID, 7);//��ť�߿��ϸ
	fillroundrect(x, y, x + w, y + h, 10, 10);//��ť����

	settextstyle(35, 0, "����");//��ť����
	setbkmode(TRANSPARENT);
	settextcolor(RGB(135, 206, 235));//��ť������ɫ
	char text[50];
	strcpy(text, text_);
	int tx = x + (w - textwidth(text)) / 2;
	int ty = y + (h - textheight(text)) / 2;

	outtextxy(tx, ty, text);

}

void showinputarea() {
	setlinestyle(PS_DASH); //�߿���ʽ
 	setlinecolor(BLACK);      //�߿���ɫ
 	rectangle(900, 350, 1400, 700);//���� ���Ͻ� ���½� 
 	settextstyle(20, 0, "����");//��С,����
    setbkmode(TRANSPARENT);// ���屳��ģʽ��͸��
    settextcolor(BLACK); //������ɫ:��
    outtextxy(900, 350, "������"); //��� �ַ���
    
	int x = 900, y = 350, a = 45;
	
	//4��ָ�ť
	button(x + 3.5 * a, y + 0.2 * a, 5 * a, 0.9 * a, "ָ������");
	button(x + a, y + 1.5 * a, 2 * a, a, "T");
	button(x + 3.5 * a, y + 1.5 * a, 2 * a, a, "C");
	button(x + 6 * a, y + 1.5 * a, 2 * a, a, "U");
	button(x + 8.5 * a, y + 1.5 * a, 2 * a, a, "END");
	//10��վ̨��ť
	button(x + 3.5 * a, y + 3.1 * a, 5 * a, 0.9 * a, "վ���");
	button(x + a, y + 4.5 * a, a, a, "1");
	button(x + 3 * a, y + 4.5 * a, a, a, "2");
	button(x + 5 * a, y + 4.5 * a, a, a, "3");
	button(x + 7 * a, y + 4.5 * a, a, a, "4");
	button(x + 9 * a, y + 4.5 * a, a, a, "5");
	button(x + a, y + 6 * a, a, a, "6");
	button(x + 3 * a, y + 6 * a, a, a, "7");
	button(x + 5 * a, y + 6 * a, a, a, "8");
	button(x + 7 * a, y + 6 * a, a, a, "9");
	button(x + 9 * a, y + 6 * a, a, a, "10");

}

void intscpy(char text[10],int n) {
	for (int i = 0; i < totalStation; i++)
		text[i] = stations[i].state[n]+'0';
	text[totalStation] = 0;
}

void output_graph() {
	
	int x = 850, y = 0, a = 40;

	settextstyle(20, 0, "����");//��С,����
	setbkmode(TRANSPARENT);// ���屳��ģʽ��͸��
	settextcolor(BLACK); //������ɫ:��
	outtextxy(900, 0, "�����"); //��� �ַ���

	setlinestyle(PS_DASH); //�߿���ʽ
	setlinecolor(BLACK);      //�߿���ɫ
	rectangle(900, 0, 1400, 350);//���� ���Ͻ� ���½� 
	
	//�����ť
	char text[11] ;
	button(x + 2 * a, y + a, 5 * a, a, "TIME");//TIME
	_itoa(bustime, text, 10);
	button(x + 8 * a, y + a, 5 * a, a, text);
	
	button(x + 2 * a, y + 2.5 * a, 5 * a, a, "position");//position
	_itoa(bus.pos, text, 10);
	button(x + 8 * a, y + 2.5 * a, 5 * a, a, text);

	button(x + 2 * a, y + 4 * a, 5 * a, a, "target");//target
	intscpy(text,2);
	button(x + 8 * a, y + 4 * a, 5 * a, a, text);

	button(x + 2 * a, y + 5.5 * a, 5 * a, a, "clockwise");//clockwise
	intscpy(text, 0);
	button(x + 8 * a, y + 5.5 * a, 5 * a, a, text);

	button(x + 2 * a, y + 7 * a, 5 * a, a, "CCW");//counterclockwise
	intscpy(text, 1);
	button(x + 8 * a, y + 7 * a, 5 * a, a, text);

	


}


void pathway_graph() {
	//����������
	settextstyle(20, 0, "����");//��С,����
	setbkmode(TRANSPARENT);// ���屳��ģʽ��͸��
	settextcolor(BLACK); //������ɫ:��
	outtextxy(400, 350, "������"); //��� �ַ���
	//���������
	setlinestyle(PS_SOLID); //�߿���ʽ
	setlinecolor(BLACK);      //�߿���ɫ
	rectangle(100, 150, 800, 550);//���� ���Ͻ� ���½� 
	
								  
	//***������վ�� 700 400
	setfillcolor(BLUE);   //�����ɫ
	//solidcircle(position[i].x, position[i].y, 5); //Բ�� �뾶 ���
	int i = 0;
	double point_num = (double)totalStation * distance;
	int numx = (point_num / 11 * 7) / 2 + 1;
	int numy = point_num / 2 - numx + 2;
	double position_distance1 = 700 / (numx - 1);
	double position_distance2 = 400 / (numy - 1);

	//����վ
	IMAGE img_U;
	IMAGE img_D;
	IMAGE img_R; 
	IMAGE img_L;
	loadimage(&img_U, "./stationU.jpg", 50,75 );//250,250�ֱ�ΪͼƬ������Ŀ�Ⱥ͸߶�
	loadimage(&img_D, "./stationD.jpg", 50, 75);
	loadimage(&img_R, "./stationR.jpg", 75, 50);
	loadimage(&img_L, "./stationL.jpg", 75, 50);
	//putimage(100,150, &img);
	//putimage(position[bus.x].x - 15, position[bus.x].y - 15, &img); //�޷�͸�� 500��100Ϊ���Ͻ�����
	
		//����վ���
	char num[50];
	settextstyle(20, 0, "����");//��С,����
	setbkmode(TRANSPARENT);// ���屳��ģʽ��͸��
	settextcolor(RED); //������ɫ:��
	
	//��������ͼ
	for (i = 0; i < numx; i++) {
		position[i].x = 100 + i * position_distance1;
		position[i].y = 150;
		solidcircle(position[i].x, position[i].y, 5); //Բ�� �뾶 ���
		if ((i ) % distance == 0) {
			putimage(position[i].x - 45, position[i].y - 75, &img_U);

			_itoa(i/distance + 1, num,10);                              //����ת�ַ���
			outtextxy(position[i].x - 25, position[i].y - 30, num);      //����վ����
		}
	}
	for (; i < point_num / 2 ; i++) {

		position[i].x = 800;
		position[i].y = 150 + (i - numx + 1) * position_distance2;
		solidcircle(position[i].x, position[i].y, 5); //Բ�� �뾶 ���
		if ((i) % distance == 0) {
			putimage(position[i].x+5, position[i].y - 45, &img_R);

			_itoa(i / distance + 1, num, 10);                              //����ת�ַ���
			outtextxy(position[i].x - 25, position[i].y - 30, num);      //����վ����
		}
	}
	
	for (; (i - (point_num / 2 )) < numx; i++) {
		position[i].x = 800 -( i - (point_num / 2 ) ) * position_distance1;
		position[i].y = 550;
		solidcircle(position[i].x, position[i].y, 5); //Բ�� �뾶 ���
		if ((i) % distance == 0) {
			putimage(position[i].x-5 , position[i].y+2 , &img_D);

			_itoa(i / distance + 1, num, 10);                              //����ת�ַ���
			outtextxy(position[i].x - 25, position[i].y - 30, num);      //����վ����
		}
	}

	for (; (i - (point_num / 2 )) < point_num / 2 ; i++) {

		position[i].x = 100;
		position[i].y = 550 - ((i - (point_num / 2 )) - numx + 1) * position_distance2;
		solidcircle(position[i].x, position[i].y, 5); //Բ�� �뾶 ���
		if ((i) % distance == 0) {
			putimage(position[i].x -75, position[i].y -5 , &img_L);

			_itoa(i / distance + 1, num, 10);                              //����ת�ַ���
			outtextxy(position[i].x - 25, position[i].y - 30, num);      //����վ����
		}
	}
	
}
 
void bus_move(){
	int temp1;
	switch(bus.state){
		case CLCW:
			temp1=bus.pos+1;
			if(temp1>=totalStation*distance) temp1=0;
			if(position[temp1].x-position[bus.pos].x>5){
				bus.x+=(position[temp1].x-position[bus.pos].x)/frequency;
			}else if(position[temp1].y-position[bus.pos].y>5){
				bus.y+=(position[temp1].y-position[bus.pos].y)/frequency;
			}else if(position[bus.pos].x-position[temp1].x>5){
				bus.x-=(position[bus.pos].x-position[temp1].x)/frequency;
			}else if(position[bus.pos].y-position[temp1].y>5){
				bus.y-=(position[bus.pos].y-position[temp1].y)/frequency;
			}
			break;
		case CCLCW:
			temp1=bus.pos-1;
			if(temp1<0) temp1=totalStation*distance-1;
			if(position[temp1].x-position[bus.pos].x>5){
				bus.x+=(position[temp1].x-position[bus.pos].x)/frequency;
			}else if(position[temp1].y-position[bus.pos].y>5){
				bus.y+=(position[temp1].y-position[bus.pos].y)/frequency;
			}else if(position[bus.pos].x-position[temp1].x>5){
				bus.x-=(position[bus.pos].x-position[temp1].x)/frequency;
			}else if(position[bus.pos].y-position[temp1].y>5){
				bus.y-=(position[bus.pos].y-position[temp1].y)/frequency;
			}
			break;
		default:
			return;
	}
}
