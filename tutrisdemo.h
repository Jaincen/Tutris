/* Filename:   tutrisdemo.h
 * Description: .....
 * Author: xxx
 * Date: 2018-08-25 11:25:50
 */
#ifndef __TUTRISDEMO_H__
#define __TURRISDEMO_H__

#include "list.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <errno.h>
#include <time.h>
#include <conio.h>
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")
#include <pthread.h>
#define  LEFT_MARGIN   16
#define  TOP_MARGIN    0

#define  WIDTH    10
#define  HEIGHT   20

#define  GROUPS   7
#define  STATUS   4

#define  True    1
#define  False   0

#define   WHITE   (BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE)
#define   RED     (BACKGROUND_RED)
#define   GREEN   (BACKGROUND_GREEN)
#define   BLUE    (BACKGROUND_BLUE)
#define   YELLOW  (BACKGROUND_RED|BACKGROUND_GREEN)
#define   PINK    (BACKGROUND_RED|BACKGROUND_BLUE)
#define   PURPLE  (BACKGROUND_GREEN|BACKGROUND_BLUE)

#define   FWHITE   (FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE)
#define   FRED     (FOREGROUND_RED)
#define   FGREEN   (FOREGROUND_GREEN)
#define   FBLUE    (FOREGROUND_BLUE)
#define   FYELLOW  (FOREGROUND_RED|FOREGROUND_GREEN)
#define   FPINK    (FOREGROUND_RED|FOREGROUND_BLUE)
#define   FPURPLE  (FOREGROUND_GREEN|FOREGROUND_BLUE)


#define   D_DOWN   0
#define   D_UP     1
#define   D_LEFT   2
#define   D_RIGHT  3
#define   D_NONE   -1

typedef  int  BOOL;

static int starttime;
static int finishtime;
static int blockArray[GROUPS][STATUS] = {
    {0x02010027, 0x01020232, 0x02010072, 0x01020131}, //T
    {0x01020223, 0x02010074, 0x01020311, 0x02010017}, //L
    {0x01020113, 0x02010047, 0x01020322, 0x02010071}, //J
    {0x01020132, 0x02010063, 0x01020132, 0x02010063}, //z
    {0x01020231, 0x02010036, 0x01020231, 0x02010036}, //s
    {0x00031111, 0x0300000F, 0x00031111, 0x0300000F}, //I
    {0x02020033, 0x02020033, 0x02020033, 0x02020033}  //ì?
};
static  int  MColor[] = {
    WHITE,RED,GREEN,BLUE,YELLOW,PINK,PURPLE
};
static  int  FColor[] = {
    FWHITE,FRED,FGREEN,FBLUE,FYELLOW,FPINK,FPURPLE
};
static int getScore[] = {0,10,30,50,100};

static int GamePool[HEIGHT][WIDTH] = {{0}};


typedef struct BlockInfo{
	int      color;
	int      group;
	int      status;
}   BLOCK, *PBLOCK;

//typedef struct BlockInfo    Block;
//typedef struct BlockInfo*   PBlock;

typedef struct BlockManager{
	int     move_direction; //0-down 1-left 2-right 3-around
	int     shakeDirection; //0-down 1-left 2-right 3-around
	struct  BlockInfo    *pbi;
} BMANAGER, *PBMANAGER;

typedef struct TotleInfo {
	int     score;
	int     deadline[5];  //0-all
	int     blockCounter[8];
	time_t  cost_time;
}  TOTLE, *PTOTLE;

typedef struct GameManager {
	struct   TotleInfo  Ti;
	struct   BlockManager*  pbm;
	int      status; //0:running   1:pause    2: dead
	/* 游戏等级取值:  0 - 10
	   等级0 是最简单的，等级10是是神一般的存在了
        用户每获取500分等级提升一级   */
	int      level;
	struct   BlockInfo  nextBlock;
	int      current_x;
	int      current_y;
} GMANAGER, *PGMANAGER;

extern void startGame(const char* filepath);
extern void initGameManager(PGMANAGER pgm);
extern BOOL isPause();
extern void print_block(PBLOCK  pb);
extern void clear_block(PBLOCK  pb);
extern void setPosition(int x, int y);
extern void displayWindow(const char* filepath);

extern void turnAround(PBMANAGER  pbm);
extern void move_left(PBMANAGER  pbm);
extern void move_right(PBMANAGER  pbm);
extern void move_down(PBMANAGER  pbm);

extern void fallDown(PBMANAGER  pbm);
extern BOOL shakeCheck();
extern BOOL shakeCheckBlock();
extern  void showInfo();
extern void showAllcount();
extern void showNextblock();
extern void loadHistory(const char* filepath);
extern void saveData();
extern void showMenu();
extern void soundChoose(int Level);
extern void* openMusic2(char* soundname);
extern void* showTime(void* arg);
extern void GameOver();
extern void reStartgame();
extern void showGameovermenu();
extern void printf_mouse(int i);
extern void* showSuppermarry(void* arg);
#endif




