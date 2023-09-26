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

	setfillcolor(BLUE);//按钮底色
	setlinecolor(RGB(135, 206, 235));//按钮边线
	setlinestyle(PS_SOLID, 7);//按钮边框粗细
	fillroundrect(x, y, x + w, y + h, 10, 10);//按钮方框

	settextstyle(35, 0, "黑体");//按钮字体
	setbkmode(TRANSPARENT);
	settextcolor(RGB(135, 206, 235));//按钮字体颜色
	char text[50];
	strcpy(text, text_);
	int tx = x + (w - textwidth(text)) / 2;
	int ty = y + (h - textheight(text)) / 2;

	outtextxy(tx, ty, text);

}

void showinputarea() {
	setlinestyle(PS_DASH); //边框样式
 	setlinecolor(BLACK);      //边框颜色
 	rectangle(900, 350, 1400, 700);//矩形 左上角 右下角 
 	settextstyle(20, 0, "黑体");//大小,字体
    setbkmode(TRANSPARENT);// 字体背景模式：透明
    settextcolor(BLACK); //字体颜色:黑
    outtextxy(900, 350, "输入区"); //输出 字符串
    
	int x = 900, y = 350, a = 45;
	
	//4个指令按钮
	button(x + 3.5 * a, y + 0.2 * a, 5 * a, 0.9 * a, "指令类型");
	button(x + a, y + 1.5 * a, 2 * a, a, "T");
	button(x + 3.5 * a, y + 1.5 * a, 2 * a, a, "C");
	button(x + 6 * a, y + 1.5 * a, 2 * a, a, "U");
	button(x + 8.5 * a, y + 1.5 * a, 2 * a, a, "END");
	//10个站台按钮
	button(x + 3.5 * a, y + 3.1 * a, 5 * a, 0.9 * a, "站点号");
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

	settextstyle(20, 0, "黑体");//大小,字体
	setbkmode(TRANSPARENT);// 字体背景模式：透明
	settextcolor(BLACK); //字体颜色:黑
	outtextxy(900, 0, "输出区"); //输出 字符串

	setlinestyle(PS_DASH); //边框样式
	setlinecolor(BLACK);      //边框颜色
	rectangle(900, 0, 1400, 350);//矩形 左上角 右下角 
	
	//输出按钮
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
	//动画区文字
	settextstyle(20, 0, "黑体");//大小,字体
	setbkmode(TRANSPARENT);// 字体背景模式：透明
	settextcolor(BLACK); //字体颜色:黑
	outtextxy(400, 350, "动画区"); //输出 字符串
	//动画区轨道
	setlinestyle(PS_SOLID); //边框样式
	setlinecolor(BLACK);      //边框颜色
	rectangle(100, 150, 800, 550);//矩形 左上角 右下角 
	
								  
	//***动画区站点 700 400
	setfillcolor(BLUE);   //填充颜色
	//solidcircle(position[i].x, position[i].y, 5); //圆心 半径 填充
	int i = 0;
	double point_num = (double)totalStation * distance;
	int numx = (point_num / 11 * 7) / 2 + 1;
	int numy = point_num / 2 - numx + 2;
	double position_distance1 = 700 / (numx - 1);
	double position_distance2 = 400 / (numy - 1);

	//公交站
	IMAGE img_U;
	IMAGE img_D;
	IMAGE img_R; 
	IMAGE img_L;
	loadimage(&img_U, "./stationU.jpg", 50,75 );//250,250分别为图片拉扯后的宽度和高度
	loadimage(&img_D, "./stationD.jpg", 50, 75);
	loadimage(&img_R, "./stationR.jpg", 75, 50);
	loadimage(&img_L, "./stationL.jpg", 75, 50);
	//putimage(100,150, &img);
	//putimage(position[bus.x].x - 15, position[bus.x].y - 15, &img); //无法透视 500，100为左上角坐标
	
		//公交站编号
	char num[50];
	settextstyle(20, 0, "黑体");//大小,字体
	setbkmode(TRANSPARENT);// 字体背景模式：透明
	settextcolor(RED); //字体颜色:黑
	
	//动画区画图
	for (i = 0; i < numx; i++) {
		position[i].x = 100 + i * position_distance1;
		position[i].y = 150;
		solidcircle(position[i].x, position[i].y, 5); //圆心 半径 填充
		if ((i ) % distance == 0) {
			putimage(position[i].x - 45, position[i].y - 75, &img_U);

			_itoa(i/distance + 1, num,10);                              //整型转字符型
			outtextxy(position[i].x - 25, position[i].y - 30, num);      //公交站文字
		}
	}
	for (; i < point_num / 2 ; i++) {

		position[i].x = 800;
		position[i].y = 150 + (i - numx + 1) * position_distance2;
		solidcircle(position[i].x, position[i].y, 5); //圆心 半径 填充
		if ((i) % distance == 0) {
			putimage(position[i].x+5, position[i].y - 45, &img_R);

			_itoa(i / distance + 1, num, 10);                              //整型转字符型
			outtextxy(position[i].x - 25, position[i].y - 30, num);      //公交站文字
		}
	}
	
	for (; (i - (point_num / 2 )) < numx; i++) {
		position[i].x = 800 -( i - (point_num / 2 ) ) * position_distance1;
		position[i].y = 550;
		solidcircle(position[i].x, position[i].y, 5); //圆心 半径 填充
		if ((i) % distance == 0) {
			putimage(position[i].x-5 , position[i].y+2 , &img_D);

			_itoa(i / distance + 1, num, 10);                              //整型转字符型
			outtextxy(position[i].x - 25, position[i].y - 30, num);      //公交站文字
		}
	}

	for (; (i - (point_num / 2 )) < point_num / 2 ; i++) {

		position[i].x = 100;
		position[i].y = 550 - ((i - (point_num / 2 )) - numx + 1) * position_distance2;
		solidcircle(position[i].x, position[i].y, 5); //圆心 半径 填充
		if ((i) % distance == 0) {
			putimage(position[i].x -75, position[i].y -5 , &img_L);

			_itoa(i / distance + 1, num, 10);                              //整型转字符型
			outtextxy(position[i].x - 25, position[i].y - 30, num);      //公交站文字
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
