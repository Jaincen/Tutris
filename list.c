
#include "list.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define  DSize()    sizeof(UIData)

//static UIData  array[5];



static FILE*   fp = NULL;

void  writeRecord()
{
    fp = fopen("./data/loadhistory/score.bat", "wb+");
    if(!fp) {
        printf("fopen error for write\n");
        return;
    }
    fwrite(&counter, sizeof(int), 1, fp);
    fwrite(array, DSize(), counter, fp);
    fclose(fp);
    fp = NULL;
}

void initVector()
{
    memset(array, 0, sizeof(array));
    counter = 0;
    fp = fopen("./data/loadhistory/score.bat", "rb+");
    if(!fp){
        printf("fopen error for read\n");
        return;
    }
    fread(&counter, sizeof(int), 1, fp);
    if(counter > 5 || counter < 0) counter = 0;
    fread(array, DSize(), counter, fp);
    fclose(fp);
    fp = NULL;
}

BOOL addUIData(const UIData* d)
{
    if(d == NULL) return False;
    if(counter == 5){
        updateUIData(d);
    }else{
        memcpy(&array[counter], d, sizeof(UIData));
        counter ++;
    }
    mysort();
    writeRecord();
    return True;
}

void clear()
{
    memset(array, 0, sizeof(array));
    counter = 0;
}

void backword(int index)
{
    int j = 4;
    for(; j > index; j--){
        memcpy(&array[j], &array[j-1], DSize());
    }
//    while(j > index)
//        memcpy(&array[j], &array[j - 1], DSize());
}

void updateUIData(const UIData* d)
{
      int i = 0;
//    int j = 1;
//    for(i = 0; i < 5; i++){
//        for(j = 0; j < 5; j++){
//            if(array[i].record > array[j].record){
//                UIData  tmp;
//                memcpy(&tmp, &array[i], DSize());
//                memcpy(&array[i], &array[j],DSize());
//                memcpy(&array[j], &tmp, DSize());
//            }
//        }
//    }
    mysort();
    if(d->record < array[counter-1].record) return;
    for(i = 0; i < 5; i++){
        if(d->record > array[i].record){
            break;
        }
    }
    backword(i);
    memcpy(&array[i], d, DSize());
}

void display()
{
    printf("------------------- User List:%d ------------------------------------\n", counter);
    int i;
    for(i = 0; i < 5; i++){
        printf("|Name:%15s|UseTime:%15d|Score:%15d|\n",
               array[i].name, array[i].secd,
               array[i].record);
    }
    printf("--------------------------------------------------------------------\n");
}
void showH()
{
    printf("%d",array[0].record);
}

void showT()
{
    printf("%d",array[0].secd);
}

int  size()
{
    return counter;
}

static BOOL isEmpty()
{
    if(counter == 0)
        return  True;
    else
        return False;
}
void mysort()
{

    int i = 0;
    int j = 1;
    for(i = 0; i < counter; i++){
        for(j = 0; j < counter; j++){
            if(array[i].record > array[j].record){
                UIData  tmp;
                memcpy(&tmp, &array[i], DSize());
                memcpy(&array[i], &array[j],DSize());
                memcpy(&array[j], &tmp, DSize());
            }
        }
    }

}





