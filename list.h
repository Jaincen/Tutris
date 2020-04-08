#ifndef __LIST_H__
#define __LIST_H__

#include <time.h>
#include <stdio.h>

#define  True   1
#define  False  0

typedef  int  BOOL;
static int  counter = 0;
typedef struct UserInfo {
    char   name[30];
    time_t costTime;
    int    secd;
    int    record;
    char   date[100];
}  UIData;
static UIData  array[5];
extern void initVector();
extern BOOL addUIData(const UIData* d);
extern void clear();
extern void updateUIData(const UIData* d);
extern int  size();
extern void writeRecord();
extern void display();
extern void showH();
extern void mysort();

#endif





