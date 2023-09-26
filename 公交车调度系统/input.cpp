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

//指令输入变量
struct Input input;

//文件输入函数
void inputFile() {
	FILE* fp = fopen("dict.dic", "r");
	if (fp == NULL) exit(1);
	char ch;
	int chstate = 0;
	while ((ch = fgetc(fp)) != EOF) {
		switch (chstate) {
		case(0):
			if (ch == '#')
				chstate = 1;
			else if (ch == 'T')
				fscanf(fp, "OTAL_STATION = %d\n", &totalStation);
			else if (ch == 'S') {
				char str[5] = "";
				fscanf(fp, "TRATEGY =  %s", str);
				if (str[0] == 'F')
					strategy = FCFS;
				else if (str[1] == 'S')
					strategy = SSTF;
				else strategy = SCAN;
				getc(fp);
			}
			else fscanf(fp, "ISTANCE = %d\n", &distance);
			break;
		case(1):
			if (ch == '\n')
				chstate = 0;
			break;
		}
	}
}

//输入指令函数
void inputCmd() {

	//scanf("%s", input.cmd);
	getInputCmd();

	if (strcmp(input.cmd, "clock") == 0 || strcmp(input.cmd, "end") == 0){
		input.num = -1;
	}
	else{
		getInputNum();
		//scanf("%d", &input.num);

	}
}


void getInputCmd() {
	int x = 900, y = 350, a = 45;
	ExMessage msg;
	while (1) {
		if (peekmessage(&msg, EM_MOUSE)) {
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
				if (panduananniu(msg.x, msg.y, x + a, y + 1.5 * a, 2 * a, a)) {//target
					strcpy(input.cmd, "target");
					printf("%s\n", input.cmd);
					return;
				}
				else if (panduananniu(msg.x, msg.y, x + 3.5 * a, y + 1.5 * a, 2 * a, a)) {//clockwise
					strcpy(input.cmd, "clockwise");
					printf("%s\n", input.cmd);
					return;
				}
				else if (panduananniu(msg.x, msg.y, x + 6 * a, y + 1.5 * a, 2 * a, a)) {//counterclockwise
					strcpy(input.cmd, "counterclockwise");
					printf("%s\n", input.cmd);
					return;
				}
				else if (panduananniu(msg.x, msg.y, x + 8.5 * a, y + 1.5 * a, 2 * a, a)) {//end
					strcpy(input.cmd, "end");
					printf("%s\n", input.cmd);
					return;
				}
				break;
			default:
				break;
			}
		}
	}
}



void getInputNum() {
	int x = 900, y = 350, a = 45;
	ExMessage msg;
	while (1) {
		if (peekmessage(&msg, EM_MOUSE)) {
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
				if (panduananniu(msg.x, msg.y, x + a, y + 4.5 * a, a, a)) {// 1
					input.num = 1;
					printf("%d\n", input.num);
					return;
				}
				else if (panduananniu(msg.x, msg.y, x + 3 * a, y + 4.5 * a, a, a)) {//2
					input.num = 2;
					printf("%d\n", input.num);
					return;
				}
				else if (panduananniu(msg.x, msg.y, x + 5 * a, y + 4.5 * a, a, a)) {//3
					input.num = 3;
					printf("%d\n", input.num);
					return;
				}
				else if (panduananniu(msg.x, msg.y, x + 7 * a, y + 4.5 * a, a, a)) {//4
					input.num = 4;
					printf("%d\n", input.num);
					return;
				}
				else if (panduananniu(msg.x, msg.y, x + 9 * a, y + 4.5 * a, a, a)) {//5
					input.num = 5;
					printf("%d\n", input.num);
					return;
				}
				else if (panduananniu(msg.x, msg.y, x + a, y + 6 * a, a, a)) {//6
					input.num = 6;
					printf("%d\n", input.num);
					return;
				}
				else if (panduananniu(msg.x, msg.y, x + 3 * a, y + 6 * a, a, a)) {//7
					input.num = 7;
					printf("%d\n", input.num);
					return;
				}
				else if (panduananniu(msg.x, msg.y, x + 5 * a, y + 6 * a, a, a)) {//8
					input.num = 8;
					printf("%d\n", input.num);
					return;
				}
				else if (panduananniu(msg.x, msg.y, x + 7 * a, y + 6 * a, a, a)) {//9
					input.num = 9;
					printf("%d\n", input.num);
					return;
				}
				else if (panduananniu(msg.x, msg.y, x + 9 * a, y + 6 * a, a, a)) {//10
					input.num = 10;
					printf("%d\n", input.num);
					return;
				}

				break;
			default:
				break;
			}
		}
	}
}

bool panduananniu(int msgx, int msgy, int x, int y, int w, int h) {
	if (msgx >= x && msgx <= x + w && msgy >= y && msgy <= y + h)
		return 1;
	else
		return 0;
}
