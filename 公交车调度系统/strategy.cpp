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

struct Queue queue[31];
int queueLength;
int busdirection;


//调度队列生成
void dispatchqueueInsert() {
    int cmdtype;
    if (strcmp(input.cmd, "clockwise") == 0)
        cmdtype = 0;
    else if (strcmp(input.cmd, "counterclockwise") == 0)
        cmdtype = 1;
    else cmdtype = 2;
    switch (strategy) {
    case FCFS:
        //去重
        for (int i = 0; i < queueLength; i++) {
            if (queue[i].stationSubscript == input.num - 1 && queue[i].requestType == cmdtype) {
                return;
            }
        }
        //无重复则插入
        queue[queueLength].stationSubscript = input.num - 1;
        queue[queueLength].requestType = cmdtype;
        queueLength++;
        break;
    case SSTF:
        //去重,录入请求流水号,站点下表和请求类型
        if (queue[(input.num - 1) * 3 + cmdtype].stationSubscript == INF) {

            queue[(input.num - 1) * 3 + cmdtype].No = totalNo;
            queue[(input.num - 1) * 3 + cmdtype].stationSubscript = input.num - 1;
            queue[(input.num - 1) * 3 + cmdtype].requestType = cmdtype;
            queueLength++;
            totalNo++;
        }
        break;
    case SCAN: {
        //queue[input.num - 1].stationSubscript = input.num - 1;
        queue[input.num - 1].requestType = 1;//请求统一为1（有请求）
        queueLength++;
        break;
    }
    }
}

void dispatchqueueErase() {
    if (queueLength == 0)
        return;
    switch (strategy) {
    case FCFS:
        for (int i = 0; i < queueLength - 1; i++) {
            queue[i] = queue[i + 1];
        }
        queueLength--;
        break;
    case SSTF:
        for (int i = 0; i < 3; i++) {
            if (queue[bus.goal * 3 + i].stationSubscript != INF) {
                queue[bus.goal * 3 + i].stationSubscript = INF;
                queueLength--;
            }
        }
        break;
    case SCAN: {
        queue[bus.goal].requestType = 0;
        queueLength--;
        break;
    }
    }
}

void getgoal() {
    if ((queueLength == 0 || bus.goal != -1)&&strategy!=SCAN)//姜杨皓加
        return;
    switch (strategy) {
    case FCFS:
        bus.goal = queue[0].stationSubscript;
        bus.goal_state = queue[0].requestType;
        break;
    case SSTF: {
        int min_distance = INF, mini = 0;
        for (int i = 0; i < 31; i++) {
            if (queue[i].stationSubscript != INF) {
                //顺时针方向搜索
                int xi = queue[i].stationSubscript * distance;
                if (bus.pos - xi >= 0) {
                    if (xi + totalStation * distance - bus.pos < min_distance) {
                        min_distance = xi + totalStation * distance - bus.pos;
                        mini = i;
                    }
                    else if (xi + totalStation * distance - bus.pos == min_distance && queue[i].No < queue[mini].No) {
                        mini = i;
                    }
                }
                else {
                    if (xi - bus.pos < min_distance) {
                        min_distance = xi - bus.pos;
                        mini = i;
                    }
                    else if (xi - bus.pos == min_distance && queue[i].No < queue[mini].No) {
                        mini = i;
                    }
                }
            }
        }
        for (int i = 0; i < 31; i++) {
            if (queue[i].stationSubscript != INF) {
                //逆时针方向搜索
                int xi = queue[i].stationSubscript * distance;
                if (bus.pos - xi >= 0) {
                    if (bus.pos - xi < min_distance) {
                        min_distance = bus.pos - xi;
                        mini = i;
                    }
                    else if (bus.pos - xi == min_distance && queue[i].No < queue[mini].No) {
                        mini = i;
                    }
                }
                else {
                    if (bus.pos + totalStation * distance - xi < min_distance) {
                        min_distance = bus.pos + totalStation * distance - xi;
                        mini = i;
                    }
                    else if (bus.pos + totalStation * distance - xi == min_distance && queue[i].No < queue[mini].No) {
                        mini = i;
                    }
                }
            }
        }
        bus.goal = queue[mini].stationSubscript;
        bus.goal_state = queue[mini].requestType;
        break;
    }
    case SCAN: {
        getgoalSCAN();
        break;
    }
    }
}

int isgoal() {
    if (bus.pos == stations[bus.goal].pos)
        return 1;
    return 0;
}

void finishgoal() {
    if (isgoal()) {
        dispatchqueueErase();
        bus.goal = -1;
        getgoal();
    }
}

int ispassingstation() {
    if (strategy != SSTF || isgoal())
        return 0;
    for (int i = 0; i < 10; i++) {
        if (bus.pos == stations[i].pos) {
            if (bus.state == CLCW && queue[3 * i].stationSubscript != INF) {
                return 1;
            }
            if (bus.state == CCLCW && queue[3 * i + 1].stationSubscript != INF) {
                return 1;
            }
            if (queue[3 * i + 2].stationSubscript != INF) {
                return 1;
            }
        }
    }
    return 0;
}

void finishpassingstation() {
    switch (strategy) {
    case SSTF:
        for (int i = 0; i < 10; i++) {
            if (bus.pos == stations[i].pos) {
                if (flagofPassingstation == CLCW && queue[3 * i].stationSubscript != INF) {
                    queue[3 * i].stationSubscript = INF;
                    queueLength--;
                }
                if (flagofPassingstation == CCLCW && queue[3 * i + 1].stationSubscript != INF) {
                    queue[3 * i + 1].stationSubscript = INF;
                    queueLength--;
                }
                if (queue[3 * i + 2].stationSubscript != INF) {
                    queue[3 * i + 2].stationSubscript = INF;
                    queueLength--;
                }
            }
        }
        break;
    default:
        return;
    }
}

//bus.goal是站台下标
void getgoalSCAN() {
    //暂无方向
    int huoqv = 0;
    if (busdirection == 0) {
        //设目标找方向
        for (int i = 0; i < totalStation+1; i++) {
            if (queue[fixnum_station(i + bus.pos / distance)].requestType == 1) {
                bus.goal = fixnum_station(i + bus.pos / distance);
                busdirection = 1;
                break;
            }
            if (queue[fixnum_station(bus.pos / distance - i)].requestType == 1) {
                bus.goal = fixnum_station(bus.pos / distance - i);
                busdirection = -1;
                break;
            }
        }
    }

    //顺时针走
    else if (busdirection == 1) {
        //遍览顺时针请求的一半，若无，换方向
        if (bus.pos % distance != 0) {
            int a = bus.pos / distance + 1;
            for (int i = 0; i < fixnum_station(bus.goal - a); i++) {
                if (queue[fixnum_station(a + i)].requestType == 1) {
                    bus.goal = fixnum_station(a + i);
                    break;
                }
            }
        }
        else if (bus.pos % distance == 0) {
            int a = bus.pos / distance;
            for (int i = 0; i <= totalStation / 2; i++) {
                if (queue[fixnum_station(a + i)].requestType == 1) {
                    bus.goal = fixnum_station(a + i);
                    huoqv = 1;
                    break;
                }
            }
            if (huoqv == 0) {
                busdirection = -1;
                for (int i = 0; i <= totalStation / 2; i++) {
                    if (queue[fixnum_station(a - i)].requestType == 1) {
                        bus.goal = fixnum_station(a - i);
                        huoqv = 1;
                        break;
                    }
                }
            }
            if (huoqv == 0) {
                bus.goal = -1;
            }
        }
    }

    //逆时针走
    else if (busdirection == -1) {
        //遍览逆时针请求的一半，若无，换方向
        if (bus.pos % distance != 0) {
            int a = bus.pos / distance;
            for (int i = 0; i < fixnum_station(a - bus.goal); i++) {
                if (queue[fixnum_station(a - i)].requestType == 1) {
                    bus.goal = fixnum_station(a - i);
                    break;
                }
            }
        }
        else if (bus.pos % distance == 0) {
            int a = bus.pos / distance;
            for (int i = 0; i <= totalStation / 2; i++) {
                if (queue[fixnum_station(a - i)].requestType == 1) {
                    bus.goal = fixnum_station(a - i);
                    huoqv = 1;
                    break;
                }
            }
            if (huoqv == 0) {
                busdirection = 1;
                for (int i = 0; i <= totalStation / 2; i++) {
                    if (queue[fixnum_station(a + i)].requestType == 1) {
                        bus.goal = fixnum_station(a + i);
                        huoqv = 1;
                        break;
                    }
                }
            }
            if (huoqv == 0) { 
                bus.goal = -1;
            }
        }
    }
    else
        printf("error\n");
}


int fixnum_station(int num) {
    if (0 <= num && totalStation > num)
        return num;
    else if (num < 0) {
        return num + totalStation;
    }
    else {
        return num - totalStation;
    }
}
