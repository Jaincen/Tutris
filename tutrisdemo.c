//Filename: tutrisdemo.c

#include "tutrisdemo.h"

static BOOL block = True;
BOOL islock = True;
HANDLE  output;
static PGMANAGER  _pgm = NULL;
void errorMsg(const char* msg)
{
    setPosition(10, 10);
    SetConsoleTextAttribute(output, FOREGROUND_RED\
                                     |BACKGROUND_GREEN\
                                     |BACKGROUND_BLUE\
                                     |BACKGROUND_RED);
    printf("%s", msg);
    fflush(stdout);
}

void updateGamePool()
{
    int i = 0, j = 0;
    for(i = 0; i < HEIGHT; i++){
        for(j = 0; j < WIDTH; j++){
            setPosition(i+TOP_MARGIN, j+LEFT_MARGIN);
            //printf("%2d", GamePool[i][j]);
            if(GamePool[i][j] == 0){
                SetConsoleTextAttribute(output, 7);
                printf("  ");
            }else{
                SetConsoleTextAttribute(output, GamePool[i][j]);
                //printf("%2d", GamePool[i][j]);
                printf("□");
            }
        }
    }
}

void startGame(const char* filepath)
{
    //print  top line
    //print  left  margin
    int i = 0;
    SetConsoleTextAttribute(output, WHITE);
    for(i = 0; i < HEIGHT; i++){
        setPosition(i+TOP_MARGIN, LEFT_MARGIN - 1);
        printf("※");
    }

    //print  right margin
    for(i = 0; i < HEIGHT; i++){
        setPosition(i+TOP_MARGIN, LEFT_MARGIN+WIDTH);
        printf("※");
    }
//
//    //print bottom  margin
    for(i = 0; i < WIDTH+2; i++){
        setPosition(TOP_MARGIN+HEIGHT,LEFT_MARGIN-1+i);
        printf("※");
    }

//    FILE* fp = fopen(filepath, "r");
//    char buffer[1024];
//    while(!feof(fp)){
//        memset(buffer, 0, sizeof(1024));
//        fgets(buffer, 1024, fp);
//        printf("%s", buffer);
//    }

    memset(GamePool, 0, sizeof(GamePool));
}

//static  PGMANAGER  _pgm = NULL;
void initGameManager(PGMANAGER pgm)
{
    if(_pgm) return;
    _pgm = pgm;
    srand(time(NULL));
    memset(pgm, 0, sizeof(GMANAGER));
    pgm->nextBlock.color = MColor[rand()%7];
    pgm->nextBlock.group = rand() % GROUPS;
    pgm->nextBlock.status = rand() % STATUS;
    pgm->current_x = LEFT_MARGIN+4;
    pgm->current_y = TOP_MARGIN;
    //showNextblock();


    //pgm->Ti.blockCounter[pgm->nextBlock.group]++;

    memset(GamePool, 0, sizeof(GamePool));
}

BOOL isPause()
{
    if(!_pgm){
        errorMsg("GameManager not exist!");
        return False;
    }
    return  _pgm->status==1?True:False;
}

int  get_bit(int value, int i)
{
    return (value >> i) & 1;
}

void print_block(PBLOCK  pb)
{
    if(!pb){
        errorMsg("print_block args error!");
        return;
    }
    int  value = blockArray[pb->group][pb->status];
    int  blankrow = (value >> 24) & 0xFF;
    int  blankcol = (value >> 16) & 0xFF;
    int  i = 0, j = 0;
    for(i = 0; i < (4 - blankrow); i++){ //br = 1
        for(j = 0; j < (4 - blankcol); j++){ //bc = 2
            setPosition(_pgm->current_y + i, _pgm->current_x + j);
            int t = 15 - ((blankrow + i)*4 + (blankcol + j));
            if(get_bit(value, t) == 0){
            }else{
                SetConsoleTextAttribute(output, pb->color);
                printf("□");
            }
        }
    }
}

void clear_block(PBLOCK  pb)
{
    if(!pb){
        errorMsg("clear_block args error!");
        return;
    }
    int  value = blockArray[pb->group][pb->status];
    int  blankrow = (value >> 24) & 0xFF;
    int  blankcol = (value >> 16) & 0xFF;
    int  i = 0, j = 0;
    for(i = 0; i < (4 - blankrow); i++){ //br = 1
        for(j = 0; j < (4 - blankcol); j++){ //bc = 2
            setPosition(_pgm->current_y + i, _pgm->current_x + j);
            int t = 15 - ((blankrow + i)*4 + (blankcol + j));
            if(get_bit(value, t) == 0){
            }else{
                SetConsoleTextAttribute(output, 0);
                printf("  ");
            }
        }
    }
}

void setPosition(int row, int col)
{
    COORD  pos;
    pos.X = 2*col;
    pos.Y = row;
    SetConsoleCursorPosition(output, pos);
}

void displayWindow(const char* filepath)
{

}

void turnAround(PBMANAGER  pbm)
{
    if(!block) return ;
    block = False;
    pbm->move_direction =  D_UP;
    if(shakeCheck())
    {
        block = True;
        return;
    }

    clear_block(pbm->pbi);
    pbm->pbi->status = (pbm->pbi->status + 1) % STATUS;
    print_block(pbm->pbi);
    block = True;
}

void move_left(PBMANAGER  pbm)
{
    pbm->move_direction = D_LEFT;
    if(shakeCheck()){
        return;
    }
    clear_block(pbm->pbi);
    if(_pgm->current_x > LEFT_MARGIN)
        _pgm->current_x --;
    print_block(pbm->pbi);
}

void move_right(PBMANAGER  pbm)
{
    pbm->move_direction = D_RIGHT;
    if(shakeCheck()){
        return;
    }
    clear_block(pbm->pbi);
    int value = blockArray[_pgm->pbm->pbi->group][_pgm->pbm->pbi->status];
    int  cl = 4 - ((value >> 16) & 0xFF);
    if(_pgm->current_x+cl < (LEFT_MARGIN+WIDTH))
        _pgm->current_x ++;
    print_block(pbm->pbi);
}

void  AddBlockToGamePool(PBLOCK  pb)
{
    int i,j;
    int  value = blockArray[pb->group][pb->status];
    int  blankrow = (value >> 24) & 0xFF;
    int  blankcol = (value >> 16) & 0xFF;
    int cx = _pgm->current_x - LEFT_MARGIN;
    int cy = _pgm->current_y - TOP_MARGIN;
    for(i = 0; i < (4 - blankrow); i++){ //br = 1
        for(j = 0; j < (4 - blankcol); j++){ //bc = 2
            int t = 15 - ((blankrow + i)*4 + (blankcol + j));
            if(get_bit(value, t) == 0){
            }else{
                GamePool[cy+i][cx+j] =  pb->color;
            }
        }
    }
}

void  coverFullLine(int bottom)
{
    int i = 0;
    for(i = bottom; i > 0; i--){
        memcpy(GamePool[i], GamePool[i-1], sizeof(int)*WIDTH);
    }
    memset(GamePool[0], 0, sizeof(int)* WIDTH);
}

void  clearAllLine()
{
    BOOL flag = False;
    int i = 0, j = 0;
    int linenumber = 0;
   // BOOL  isFull = False;
    for(i = HEIGHT; i > 0; i--){
        for(j = 0; j < WIDTH; j++){
            if(GamePool[i][j] == 0){
                //isFull = False;
                break;
            }else{
                //
            }
        }
        if(j == WIDTH){
            linenumber ++;
            //printf("%d            ",linenumber);
            coverFullLine(i);
            i = HEIGHT;
            //SetConsoleTextAttribute(output, FOREGROUND_RED|FOREGROUND_GREEN);
            flag  = True;

        }
    }
    if(linenumber)_pgm->Ti.deadline[linenumber]++;
    if(flag)_pgm->Ti.deadline[0] += linenumber;
    _pgm->Ti.score = _pgm->Ti.score+  getScore[linenumber];
    _pgm->level = _pgm->Ti.score/100;
    soundChoose(linenumber);
//    setPosition(3,6);
//    printf("%d",_pgm->Ti.deadline[0]);

    showAllcount();


    }



void move_down(PBMANAGER  pbm)
{
    if(!block)return ;
    block = False;
    if(!pbm){
        errorMsg("blockManager error!");
        block = True;
        return;
    }

    pbm->move_direction = D_DOWN;
    if(shakeCheck() && _pgm->pbm->shakeDirection == D_DOWN){
        //1,record new block data to GamePool
        AddBlockToGamePool(_pgm->pbm->pbi);
        //2, clear full line
        clearAllLine();
        //3,update GamePool display
        updateGamePool();
        //4, make  a new block
        int g = rand() % GROUPS;
        int s = rand() % STATUS;
        int c = rand() % 7;
        _pgm->current_x = LEFT_MARGIN+4;
        _pgm->current_y = TOP_MARGIN;
        _pgm->pbm->move_direction = D_DOWN;
        _pgm->pbm->shakeDirection = -1;
        _pgm->pbm->pbi->color = _pgm->nextBlock.color;
        _pgm->pbm->pbi->group = _pgm->nextBlock.group;
        _pgm->pbm->pbi->status = _pgm->nextBlock.status;
        _pgm->nextBlock.color = MColor[c];
        _pgm->nextBlock.group = g;
        _pgm->nextBlock.status = s;
        _pgm->Ti.blockCounter[_pgm->pbm->pbi->group]++;
        showNextblock();
        if(shakeCheck()&&(_pgm->pbm->shakeDirection == D_DOWN))
        {
            GameOver();
            block = True;
            reStartgame();
            return ;
        }
        block = True;

    }
    //pbm.move_direction = 0;
    clear_block(pbm->pbi);
    _pgm->current_y ++;
    print_block(pbm->pbi);
    block = True;
}
void fallDown(PBMANAGER  pbm)
{
    if(!block) return ;
    block = False;
    pbm ->move_direction = D_DOWN;
    clear_block(pbm->pbi);
    while(!shakeCheck())
    {
        _pgm->current_y++;
    }
    print_block(pbm->pbi);
    block = True;
}

BOOL shakeCheck()
{
    int  value = blockArray[_pgm->pbm->pbi->group][_pgm->pbm->pbi->status];
    switch(_pgm->pbm->move_direction){
        case D_DOWN:
        {
            int h = 4 - ((value >> 24) & 0xFF);
            if((_pgm->current_y+h) >= (TOP_MARGIN+HEIGHT)
                || shakeCheckBlock()){
                _pgm->pbm->shakeDirection = D_DOWN;
                return True;
            }
        }
            break;
        case D_LEFT:
        {
            if(_pgm->current_x <= LEFT_MARGIN \
                || shakeCheckBlock()){
                _pgm->pbm->shakeDirection = D_LEFT;
                return True;
            }
        }
            break;
        case D_RIGHT:
        {
            int h = 4 - ((value >> 16) & 0xFF);
            if(_pgm->current_x+h >= LEFT_MARGIN+WIDTH \
                || shakeCheckBlock()){
                _pgm->pbm->shakeDirection = D_RIGHT;
                return True;
            }
        }
            break;
        case D_UP:
        {
            int h = 4 - (value >> 24);
            //int x = 4 - (value>>16);
            if(_pgm->current_x+h > LEFT_MARGIN+WIDTH \
                || shakeCheckBlock()){
                _pgm->pbm->shakeDirection = D_UP;
                return True;
            }
        }
            break;
        default:break;
    }
    return False;
}

BOOL  shakeCheckBlock()
{
    int  i = 0, j = 0;
    int  row = _pgm->current_y - TOP_MARGIN;
    int  col = _pgm->current_x - LEFT_MARGIN;
    switch(_pgm->pbm->move_direction){
        case D_DOWN:
        {
            row += 1;
        }
        break;
        case D_LEFT:
        {
            col -= 1;
        }
        break;
        case D_RIGHT:
        {
            col += 1;
        }
        break;
        default:break;
    }

    int  value = blockArray[_pgm->pbm->pbi->group][_pgm->pbm->pbi->status];
    int  blankrow = (value >> 24) & 0xFF;
    int  blankcol = (value >> 16) & 0xFF;
    for(i = 0; i < (4 - blankrow); i++){ //br = 1
        for(j = 0; j < (4 - blankcol); j++){ //bc = 2
            int t = 15 - ((blankrow + i)*4 + (blankcol + j));
            if(get_bit(value, t) == 0){
            }else{
                if(GamePool[row+i][col+j] == 0){
                }else{
                    return True;
                }
            }
        }
    }
    return False;
}


void showInfo()
{


    SetConsoleTextAttribute(output, FOREGROUND_RED|FOREGROUND_BLUE);
    setPosition(1,1);
    printf("■您的分数:");

    setPosition(3,1);
    printf("■消行总数:");

    setPosition(4,2);
    printf("消①:");

    setPosition(5,2);
    printf("消②:");

    setPosition(6,2);
    printf("消③:");

    setPosition(7,2);
    printf("消④:");

    setPosition(9,1);
    printf("■记录区:");

    setPosition(10,2);
    printf("历史最高分:");

    setPosition(10,8);
    //loadHistory("./data/loadhistory/score.bat");
    initVector();
    //printf("%s %d  %d",array[2].name,array[2].record,counter);
    showH();


    setPosition(12,2);
    printf("时间:");
    setPosition(12,8);
    showT();

//    setPosition(12,2);
//    printf("现在排行:");
    setPosition(14,1);
    printf("■方块型号:");
    setPosition(15,2);
    printf("□I:");
    setPosition(16,2);
    printf("□T:");
    setPosition(17,2);
    printf("□L:");
    setPosition(18,2);
    printf("□J:");
    setPosition(19,2);
    printf("□Z:");
    setPosition(20,2);
    printf("□S:");
    setPosition(21,2);
    printf("□0:");


    int i,j;
    for(i=0;i<8;i++)
    {
        setPosition(1+i,28);


        for(j=0;j<10;j++)
        {
            if(i == 0 || i==7)printf("◇");
            else if(j == 0)setPosition(1+i,28),printf("◇");
            else if(j == 9)setPosition(1+i,37),printf("◇");
        }
    }

    setPosition(10,28);
    printf("■操作小手册:");

    setPosition(12,30);
    printf("向左移动:← A 4 ");

    setPosition(13,30);
    printf("向右移动:→ D 6");

    setPosition(14,30);
    printf("向下移动:↓ S 2");

    setPosition(15,30);
    printf("顺时针旋转:↑ W 8");

    setPosition(16,30);
    printf("逆时针旋转:0");

    setPosition(17,30);
    printf("直接落地:空格");

    setPosition(18,30);
    printf("暂停游戏:回车");

    setPosition(19,28);
    printf("■By:01小队");

    setPosition(22,16);
    printf("Valentin - A Little Story");
    setPosition(15,30);
}
/*
void showInfo()
{


    SetConsoleTextAttribute(output, FOREGROUND_RED|FOREGROUND_BLUE);
    setPosition(1,1);
    printf("■Score:");

    setPosition(6,1);
    printf("■Total:");

    setPosition(8,2);
    printf("□One:");

    setPosition(9,2);
    printf("□Two:");

    setPosition(10,2);
    printf("□Three:");

    setPosition(11,2);
    printf("□Four:");

    setPosition(14,1);
    printf("■The Total Of Square:");

    setPosition(16,2);
    printf("□  :");
    setPosition(17,2);
    printf("□  :");
    setPosition(18,2);
    printf("□  :");
    setPosition(19,2);
    printf("□  :");
    setPosition(20,2);
    printf("□  :");
    setPosition(21,2);
    printf("□  :");
    setPosition(22,2);
    printf("□  :");



    int i,j;
    for(i=0;i<6;i++)
    {
        setPosition(1+i,28);
//        Position.X = 68;
//        Position.Y = 1+i;
//        SetConsoleCursorPosition(OutPut,Position);

        for(j=0;j<11;j++)
        {
            if(i == 0 || i==5)printf("◇");
            else if(j == 0)setPosition(1+i,28),printf("◇");
            else if(j == 5)setPosition(1+i,33),printf("◇");
            else if(j == 10)setPosition(1+i,38),printf("◇");
        }
    }

    setPosition(10,28);
    printf("■Control:");

    setPosition(12,30);
    printf("Left:  ← A 4 ");

    setPosition(13,30);
    printf("Right: → D 6");

    setPosition(14,30);
    printf("Down:  ↓ 2");

    setPosition(15,30);
    printf("C.W:   ↑ 8");

    setPosition(16,30);
    printf("C.A:      0");

    setPosition(22,16);
    printf("Music:July - My Soul");

    setPosition(18,28);
    printf("■By:zjc");




}
*/

void showAllcount()
{

    SetConsoleTextAttribute(output, FOREGROUND_RED|FOREGROUND_GREEN);

    setPosition(1,6);
    printf("%d",_pgm->Ti.score);
    setPosition(3,6);

    printf("%d",_pgm->Ti.deadline[0]);
    setPosition(4,6);
    printf("%d",_pgm->Ti.deadline[1]);
    setPosition(5,6);
    printf("%d",_pgm->Ti.deadline[2]);
    setPosition(6,6);
    printf("%d",_pgm->Ti.deadline[3]);
    setPosition(7,6);
    printf("%d",_pgm->Ti.deadline[4]);

//    setPosition(15,4);
//    printf("%d",_pgm->Ti.blockCounter[5]);//I
//    setPosition(16,4);
//    printf("%d",_pgm->Ti.blockCounter[0]);//T
//    setPosition(17,4);
//    printf("%d",_pgm->Ti.blockCounter[1]);//L
//    setPosition(18,4);
//    printf("%d",_pgm->Ti.blockCounter[2]);//J
//    setPosition(19,4);
//    printf("%d",_pgm->Ti.blockCounter[3]);//Z
//    setPosition(20,4);
//    printf("%d",_pgm->Ti.blockCounter[4]);//S
//    setPosition(21,4);
//    printf("%d",_pgm->Ti.blockCounter[6]);//O

}
void showNextblock()
{

    //setPosition(3,30);

    int  value = blockArray[_pgm->pbm->pbi->group][_pgm->pbm->pbi->status];
    int  blankrow = (value >> 24) & 0xFF;
    int  blankcol = (value >> 16) & 0xFF;
    int  i = 0, j = 0;
    for(i = 0; i < (4 - blankrow); i++){ //br = 1
        for(j = 0; j < (4 - blankcol); j++){ //bc = 2
            setPosition(4 + i, 32 + j);
            int t = 15 - ((blankrow + i)*4 + (blankcol + j));
            if(get_bit(value, t) == 0){
            }else{
                SetConsoleTextAttribute(output, 0);
                printf("  ");
            }
        }
    }

    value = blockArray[_pgm->nextBlock.group][_pgm->nextBlock.status];
    blankrow = (value >> 24) & 0xFF;
    blankcol = (value >> 16) & 0xFF;
    for(i = 0; i < (4 - blankrow); i++){ //br = 1
        for(j = 0; j < (4 - blankcol); j++){ //bc = 2
            setPosition(4 + i, 32 + j);
            int t = 15 - ((blankrow + i)*4 + (blankcol + j));
            if(get_bit(value, t) == 0){
            }else{
                SetConsoleTextAttribute(output, _pgm->nextBlock.color);
                printf("□");
            }
        }
    }
    //print_block(&_pgm->nextBlock);
    SetConsoleTextAttribute(output, FOREGROUND_RED|FOREGROUND_GREEN);
    setPosition(15,4);
    printf("%d",_pgm->Ti.blockCounter[5]);//I
    setPosition(16,4);
    printf("%d",_pgm->Ti.blockCounter[0]);//T
    setPosition(17,4);
    printf("%d",_pgm->Ti.blockCounter[1]);//L
    setPosition(18,4);
    printf("%d",_pgm->Ti.blockCounter[2]);//J
    setPosition(19,4);
    printf("%d",_pgm->Ti.blockCounter[3]);//Z
    setPosition(20,4);
    printf("%d",_pgm->Ti.blockCounter[4]);//S
    setPosition(21,4);
    printf("%d",_pgm->Ti.blockCounter[6]);//O


}
void loadHistory(const char* filepath)
{


    //setPosition(1,20);
    FILE* fp = fopen(filepath, "r+");
    char buffer[1024];
    while(!feof(fp)){

            memset(buffer, 0, sizeof(1024));
            fgets(buffer, 1024, fp);
            printf("%s", buffer);

        }


}
void saveData(const char* filepath)
{
    int t;
    FILE* fp = fopen(filepath, "r+");
    //char buffer[1024];
    fscanf(fp,"%d",&t);
    if(t>_pgm->Ti.score)fprintf(fp,"%d",_pgm->Ti.score);

}
void gotoxy(int x,int y)
    {
    COORD pos;
    pos.X=x;
    pos.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
    }
void printf_mouse(int i)
{
    gotoxy(40+i,8+i);
	printf("                    .----.");
	gotoxy(40+i,9+i);
    printf("                 _.'__    `. ");
    gotoxy(40+i,10+i);
    printf("             .--(#)(##)---/#\\");
    gotoxy(40+i,11+i);
    printf("           .' @          /###\\");
    gotoxy(40+i,12+i);
    printf("           :         ,   #####");
    gotoxy(40+i,13+i);
    printf("            `-..__.-' _.-\###/  ");
    gotoxy(40+i,14+i);
    printf("                  `;_:    `\"'");
    gotoxy(40+i,15+i);
    printf("                .'\"\"\"\"\"`. ");
    gotoxy(40+i,16+i);
    printf("               /,  COLIN ,\\");
    gotoxy(40+i,17+i);
    printf("              //  COOL!   \\\\");
    gotoxy(40+i,18+i);
    printf("              `-._______.-'");
    gotoxy(40+i,19+i);
    printf("              ___`. | .'___");
    gotoxy(40+i,20+i);
    printf("             (______|______)");

}
//void showGameover()
//{
//    //loadHistory("./data/loadhistory/gameovershow.bat");
//    printf_mouse(-5);
//}
void Menu()
{
    loadHistory("./data/loadhistory/menu2.bat");

}
void* openMusic2(char* soundname)
{

    mciSendString(soundname,NULL,0,NULL);
    //mciSendString(TEXT("mysong"),NULL,0,NULL);
    //Sleep(1000);
    //pthread_exit(0);


    return (void*)0;
}
void soundChoose(int Level)
{
    pthread_t  tm2;
    switch(Level)
    {
        case 1:
            {

                pthread_create(&tm2,NULL,openMusic2("play ./data/groundmusic/9414.wav"),NULL);
            }
            break;
        case 2:
            {

                pthread_create(&tm2,NULL,openMusic2("play ./data/groundmusic/GOOD.wav"),NULL);
            }
            break;
        case 3:
            {

                pthread_create(&tm2,NULL,openMusic2("play ./data/groundmusic/9586.wav"),NULL);
            }
            break;
        case 4:
            {
                pthread_create(&tm2,NULL,openMusic2("play ./data/groundmusic/6731.wav"),NULL);

            }
            break;
        default :break;
    }

}

void showMenu()
{

    system("cls");
    char c;
    SetConsoleTextAttribute(output, FOREGROUND_RED|FOREGROUND_BLUE);
    Menu();
    printf_mouse(-1);
    //system("pause");
    while(1)
    {
        c = getch();
        switch(c)
        {
            case '1':system("cls");Sleep(1500);
                return ;break;
            case '2':
                {
                    system("cls");
                    initVector();
                    display();
                    system("pause");
                    system("cls");
                    Menu();
                }
                break;
            case 'n':
            case 'N':
            case '3':system("cls");showGameovermenu();exit(0); break;
            case 'Y':
            case 'y':system("cls");return;break;


           // case '3':system("cls");showGameover();exit(0);break;

            default: break;



        }



    }



}
void showGameovermenu()
{
    loadHistory("./data/loadhistory/gameovershow.bat");
}

void GameOver()
{
    //savedata
    islock = False;
    SetConsoleTextAttribute(output, FOREGROUND_RED|FOREGROUND_GREEN);
    setPosition(5,18);
    UIData puid;
    puid.record = _pgm->Ti.score;
    finishtime = clock();
    puid.secd= (finishtime - starttime)/1000;
    printf("Game Over!");
    system("cls");
    printf("Please Key Your Game Name: \n");
    //getchar();
    gets(puid.name);
    initVector();
    addUIData(&puid);
    //system("cls");
    display();
    printf("Press Y to restart,Press N to quit ");

    system("pause");
    //initGameManager(_pgm);
    showMenu();
    islock = True;

    //show gameover menu


    //choose  conitinu  or not




}
void reStartgame()
{

    int s,g,c;
    srand(time(NULL));
    g = rand() % GROUPS;
    s = rand() % STATUS;
    c = rand() % 7;
    memset(GamePool, 0 , sizeof(GamePool));
    memset(&_pgm->Ti, 0 , sizeof(TOTLE));
   _pgm->level = 0;
   _pgm->status = 0;

    _pgm->nextBlock.color = MColor[rand()%7];
    _pgm->nextBlock.group = rand() % GROUPS;
    _pgm->nextBlock.status = rand() % STATUS;
    _pgm->pbm->pbi->color =  MColor[c];
    _pgm->pbm->pbi->group = g;
    _pgm->pbm->pbi->status = s;
    _pgm->nextBlock.color = MColor[c];
    _pgm->nextBlock.group = g;
    _pgm->nextBlock.status = s;
    _pgm->current_x = LEFT_MARGIN+4;
    _pgm->current_y = TOP_MARGIN;
    _pgm->Ti.blockCounter[_pgm->pbm->pbi->group]++;
    //memset()
    //block = True;
    print_block(_pgm->pbm->pbi);
    showNextblock();
    showInfo();
    showAllcount();
    startGame("zzz");


}







