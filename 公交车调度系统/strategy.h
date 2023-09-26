struct Queue {
	int stationSubscript;
	int requestType;
	int No;
	int requestTime;
};



void dispatchqueueInsert();
void dispatchqueueErase();
void getgoal();
int isgoal();
void finishgoal();
int ispassingstation();
void finishpassingstation();
void getgoalSCAN();
int fixnum_station(int num);


extern struct Queue queue[31];
extern int queueLength;
