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

//输出模块（图形化要修改）
void print() {
	printf("TIME:%d\n", bustime);

	printf("BUS:\n");
	printf("position:%d\n", bus.pos);

	printf("target:");
	for (int i = 0; i < totalStation; i++) {
		printf("%d", stations[i].state[2]);
	}
	printf("\n");

	printf("STATION:\n");
	printf("clockwise:");
	for (int i = 0; i < totalStation; i++) {
		printf("%d", stations[i].state[0]);
	}
	printf("\n");

	printf("counterclockwise:");
	for (int i = 0; i < totalStation; i++) {
		printf("%d", stations[i].state[1]);
	}
	printf("\n");
}
