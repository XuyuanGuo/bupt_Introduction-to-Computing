#define CLCW 0
#define CCLCW 1
#define FREE 2

//����������
struct Bus
{
	int state;
	int pos;
	int goal;  //Ŀ��
	int goal_state; //��¼��ǰĿ���Ӧ���������� 
	double x,y;  //�������� 
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
extern int  busderection ;//��������ʻ����
extern struct POSITION position[32];
