#include <iostream>
#include <algorithm>
#include <time.h>

#include "PainterEngine.h"

using namespace std;
 
const int gz_gs = 9;  //个数
const int gz_dx = 70; //大小
char shudu[10][10]; //存储数独
int flag = false;
 
void drawboard() {   //绘制棋盘
    PainterEngine_DrawRect()
	setlinecolor(RGB(128,0,0));   //边框
	setlinestyle(PS_SOLID, 10);
	rectangle(0, 0, 630, 630);  
 
	setlinecolor(BLACK);
	for (int i = 0; i < gz_gs; i++) {
		int x = i * gz_dx;
		int y = i * gz_dx;
		if (i % 3 == 0) {
			setlinestyle(PS_SOLID, 3);  // 每3行/列绘制粗线条
		}
		else {
			setlinestyle(PS_SOLID, 1);  // 其他线条为细线条
		}
		line(x, 0, x, gz_gs * gz_dx);  // 垂直线条
		line(0, y, gz_gs * gz_dx, y);  // 水平线条
	}
}
 
bool Check(int n, int key)
{
	for (int i = 0; i < 9; i++)  //判断n所在横列是否合法 
	{
		int j = n / 9;
		if (shudu[j][i] == key + '0')  return false;
	}
	
	for (int i = 0; i < 9; i++)  //判断n所在竖列是否合法 
	{
		int j = n % 9;
		if (shudu[i][j] == key + '0') return false;
	}
 
	int x = n / 9 / 3 * 3;   //x为n所在的小九宫格左顶点竖坐标 
	int y = n % 9 / 3 * 3;   // y为n所在的小九宫格左顶点横坐标 
	
	for (int i = x; i < x + 3; i++) // 判断n所在的小九宫格是否合法 
	{
		for (int j = y; j < y + 3; j++)
		{
			if (shudu[i][j] == key + '0') {
				return false;
			}
		}
	}
	return true;  // 全部合法，返回正确 
}
 
int DFS(int n)
{   
	if (n > 80)  // 所有的都符合，退出递归 
	{
		flag = true;
		return 0;
	}
 
	if (shudu[n / 9][n % 9] != '0')  //当前位不为0时跳过 
	{
		DFS(n + 1);
	}
	else
	{
		for (int i = 1; i <= 9; i++) // 放入1—9进行测试
		{
			if (Check(n, i) == true)  // 填入数字判断是否满足条件
			{
				shudu[n / 9][n % 9] = i + '0';
				DFS(n + 1);  // 递归深度搜索
				if (flag == true) {  // 返回时如果构造成功，则直接退出，剪枝 
					return 0;
				}
				shudu[n / 9][n % 9] = '0'; //回溯
			}
		}
	}
}
 
void product_shudu() {   //随机生成数独
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 9; i++) {  //先全体置成字符‘0’
		for (int j = 0; j < 9; j++) {
			shudu[i][j] = '0';
		}
	}
 
	int RorL = rand() % 10;
	if (RorL % 2 == 1) {
		for (int i = 0; i < 9; i++) { // 将1—9填入首列
			shudu[0][i] = i + 1 + '0';
		}
		for (int i = 1; i <= 3; i++) { //随机打乱
			swap(shudu[0][rand() % 4 + 5], shudu[0][rand() % 5]);
		}
	}
	else {
		for (int i = 0; i < 9; i++) { // 将1—9填入首行
			shudu[i][0] = i + 1 + '0';
		}
		for (int i = 1; i <= 3; i++) { //随机打乱
			swap(shudu[rand() % 4 + 5][0], shudu[rand() % 4][0]);
		}
	}
 
}
 
void  hollow() {  // 随机挖空格子
	srand((unsigned int)time(NULL));
	for (int i = 10; i > 0;i--) {  
		int row = rand() % 9;
		int col = rand() % 9;
		if (shudu[row][col] != '0') {
			shudu[row][col] = '0';
		}
	}
}
 
void clozeboard() { //初始化棋盘
	settextstyle(30, 0, _T("微软雅黑"));  // 设置字体大小和样式
	settextcolor(BLACK);
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (shudu[i][j] != '0') {
				outtextxy(i * gz_dx + 30, j * gz_dx + 20, shudu[i][j]);
			}
		}
	}
}
 
 
bool full_YN() { //检查是否填完了,填完自动提交
	int flag = 0;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (shudu[i][j] == '0'){
				flag = 1;
			}
		}
	}
	if (flag == 1) return false;
	else return true;
}
 
bool rightness(){  //判断数独是否正确
 
	for (int i = 0; i < 9; i++) {  //判断行列上是否合法
		int arr[10] = { 0 }, brr[10] = { 0 };
		for (int j = 0; j < 9; j++) {
			arr[shudu[i][j] - '0']++;
			brr[shudu[j][i] - '0']++;
			if (arr[j] > 1 || brr[j] > 1) {
				return false;
			}
		}
	}
 
	for (int i = 0; i < 9; i = i + 3) {  //判断九宫格上是否合法
		for (int j = 0; j < 9; j = j + 3) {
			int arr[10] = { 0 };
			for (int x = i; x < i + 3; x++) {
				for (int y = j; y < j + 3; y++) {
					arr[shudu[x][y] - '0']++;
					if (arr[y] > 1) {
						return false;
					}
				}
			}
		}
	}
	return true;
}
 
int main() {
	//窗口搭建:
	initgraph(630, 630);
	setbkcolor(WHITE);
	cleardevice();
 
	//函数调用：
	drawboard();
	product_shudu();
	if (DFS(0) == 0) {
		hollow();
		clozeboard();
	}
 
	ExMessage msg;
	ExMessage keyMsg;
	while (1) {
		keyMsg = getmessage(EX_KEY);  // 修改为传入指针
		msg = getmessage(EX_MOUSE);
 
		int i = min(gz_dx * gz_gs - 1, msg.x) / gz_dx;
		int j = min(gz_dx * gz_gs - 1, msg.y) / gz_dx;
 
		if (keyMsg.message == WM_KEYDOWN) {  // 检查是否有键盘按下
			int num = keyMsg.vkcode;  
			if (num >= '0' && num <= '9') {  // 检查是否是数字键
				shudu[i][j] = num;
			}
		}
 
		if (full_YN() == true) {  //填完数字解数独
			if (rightness()==true) {
				cout << "恭喜通关！" << endl;
			}
			else cout << "game over" << endl;
			break;
		}
 
		//调用函数反复刷新窗口:
		cleardevice();
		drawboard();
		clozeboard();
		FlushBatchDraw();
	}
 
	EndBatchDraw();
	closegraph();
	return 0;
}
 