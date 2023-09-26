#define CLCW 0
#define CCLCW 1
#define FREE 2

//公交车变量
struct Bus
{
	int state;
	int pos;
	int goal;  //目标
	int goal_state; //记录当前目标对应的请求类型 
	double x,y;  //像素坐标 
};

struct Station
{
	int pos;
	int state[3];
};

struct POSITION {
 	double x;
 	double y;
};

void gettime();
void state_machine();
void bus_position_process();
void bus_changing();
void station_changing();

extern struct Bus bus;
extern struct Station stations[10];
extern int flagofPassingstation;
extern int  busderection ;//公交车行驶方向
extern struct POSITION position[32];
