struct Input
{
	char cmd[20];
	int num;
};

void inputFile();
void inputCmd();
void getInputCmd();
void getInputNum();
bool panduananniu(int msgx, int msgy, int x, int y, int w, int h);
extern struct Input input;
