#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define PIX char
#ifndef GREEDY_SNAKE_H
struct Point{//点
    int x;
    int y;
};
bool In_range(Point begin_p,Point end_p,Point p,Point offset_p){//检测点是否在范围内
    int begin_x = begin_p.x;
    int begin_y = begin_p.y;
    int end_x = end_p.x;
    int end_y = end_p.y;
    int x = p.x - offset_p.x;
    int y = p.y - offset_p.y;
    return x >= begin_x && x <= end_x
    && y >= begin_y && y <= end_y;
}
struct String{
    int size;
    char* begin;
};
void Init_String_use_Pchar(String* str,char word[]){
    int size = 0;
    while(word[size] != '\0'){
        size++;
    }
    str->size = size;
    str->begin = (char*)malloc(size * sizeof(char));
    for(int i=0;i<size;i++){
        *(str->begin + i) = word[i];
    }
    return;
}
String TransNumToString(int number){
    String str;
    if(number < 0){
        str.size = 0;
    }else if(number == 0){
        str.size = 1;
        str.begin = (char*)malloc(sizeof(char));
        *str.begin = '0';
    }else{
        char word[100];
        int size = 0;
        while(number){
            int x = number % 10;
            word[size++] = '0' + x;
            number /= 10;
        }
        str.size = size;
        str.begin = (char*)malloc(size * sizeof(char));
        for(int i=0;i<size;i++){
            *(str.begin + i) = word[size - 1 - i];//反一下
        }
    }
    return str;
}
struct Array{//数组
    int size;
    int row;
    int col;
    int* begin;
};
void Init_Array(Array* array,int row,int col){
    int size = row * col;
    array->row = row;
    array->col = col;
    array->size = size;
    array->begin = (int*)malloc(size * sizeof(int));
    return;
}
void Fill_Array(Array* arr,int fill_val){//全部填充
    int size = arr->size;
    for(int i=0;i<size;i++){
        *(arr->begin + i) = fill_val;
    }
    return;
}
void Fill_Array_range(Array* arr,Point begin_p,Point end_p,int fill_val){//范围填充
    int begin_x = begin_p.x;
    int begin_y = begin_p.y;
    int end_x = end_p.x;
    int end_y = end_p.y;
    int width = arr->col;
    for(int y=begin_y;y<=end_y;y++){
        for(int x=begin_x;x<=end_x;x++){
            int index = y * width + x;
            *(arr->begin + index) = fill_val;
        }
    }
    return;
}
void Set_Array_value(Array* arr,Point pos,int val){//改数组单点值
    int index = pos.y * arr->col + pos.x;
    *(arr->begin + index) = val;
    return;
}
/********************************************** */
struct GAME_IMAGE{//一维数组模拟二维
    int height;
    int width;
    PIX* leftUp;
};
void Init_GAME_IMAGE(GAME_IMAGE* img,int height,int width){//初始化图片
    img->height = height;
    img->width = width;
    int size = height * width;
    img->leftUp = (PIX*)malloc(size * sizeof(PIX));
    for(int i=0;i<size;i++){//初始化空格
        *(img->leftUp + i) = ' ';
    }
    return;
}
/*********************************************** */
struct Food{//食物
    char ch;//显示的字符
    int add;//增益
    int chance;//生成几率,1 / chance
};

typedef struct{//方向
    int dir_x;
    int dir_y;
}DIR;
DIR UP = {0,-1};
DIR DOWN = {0,1};
DIR LEFT = {-1,0};
DIR RIGHT = {1,0};
struct KEYSTATUS{
    bool pre;
    bool cur;
} ;//按键情况

typedef struct{
    Point pos;
}SNAKE_BLOCK;//蛇的单体方块

struct LIST_NODE{
    SNAKE_BLOCK snake_block;
    LIST_NODE* pre;
    LIST_NODE* next;
};//链表节点

bool Insert_LIST_NODE_H(LIST_NODE** list_head,LIST_NODE** list_tail,LIST_NODE* list_node){//头插
    if(list_node == nullptr){//空节点
        return false;
    }else if(*list_head == nullptr){
        *list_head = list_node;
        *list_tail = list_node;
    }else{
        list_node->next = *list_head;
        (*list_head)->pre = list_node;
        *list_head = list_node;
    }
    return true;
}
bool Insert_LIST_NODE_T(LIST_NODE** list_head,LIST_NODE** list_tail,LIST_NODE* list_node){//尾插
    if(list_node == nullptr){
        return false;
    }else if(*list_tail == nullptr){
        *list_head = list_node;
        *list_tail = list_node;
    }else{
        list_node->pre = *list_tail;
        (*list_tail)->next = list_node;
        *list_tail = list_node;
    }
    return true;
}
bool Delete_LIST_NODE_H(LIST_NODE** list_head,LIST_NODE** list_tail){//头删
    if(*list_head == nullptr){//空节点
        return false;
    }else if((*list_head)->next == nullptr){//只剩头节点
        free((void*)(*list_head));
        *list_head = nullptr;
        *list_tail = nullptr;
    }else{//剩下多个节点
        *list_head = (*list_head)->next;
        ((*list_head)->pre)->next = nullptr;
        free((void*)((*list_head)->pre)); 
        (*list_head)->pre = nullptr;
    }
    return true;
}
bool Delete_LIST_NODE_T(LIST_NODE** list_head,LIST_NODE** list_tail){//尾删
    if(*list_tail == nullptr){//空节点
        return false;
    }else if((*list_tail)->pre == nullptr){//只剩尾节点
        free((void*)(*list_tail));
        *list_head = nullptr;
        *list_tail = nullptr;
    }else{//剩多个节点
        *list_tail = (*list_tail)->pre;
        ((*list_tail)->next)->pre = nullptr;
        free((void*)((*list_tail)->next));
        (*list_tail)->next = nullptr;
    }
    return true;
}

void Delete_LIST_NODE_All(LIST_NODE** list_head,LIST_NODE** list_tail){//全删
    while(*list_head != nullptr){
        Delete_LIST_NODE_T(list_head,list_tail);
    }
    return;
}
/************************************************* */
int min(int a,int b){
    return a <= b ? a : b;
}

int max(int a,int b){
    return a >= b ? a : b;
}

void HideCursor(){//隐藏光标
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor;
    GetConsoleCursorInfo(handle,&cursor);
    cursor.bVisible = false;
    SetConsoleCursorInfo(handle,&cursor);
    return;
}
void Goto(int x,int y){//废案,不用了
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {(short)x,(short)y};
    SetConsoleCursorPosition(handle,pos);
    return;
}
void Goto(Point pos){
    Goto(pos.x,pos.y);
    return;
}
void BanKeyboardInput(){//禁止键盘输入
    HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(handle,(ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS));
    return;
}

void WriteChar(int x,int y,char ch){//光标位置写字符
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD isWrite;
    COORD pos = {(short)x,(short)y};
    WriteConsoleOutputCharacterA(handle,&ch,1,pos,&isWrite);
    return;
}
void WriteChar(Point pos,char ch){
    WriteChar(pos.x,pos.y,ch);
    return;
}
void UpdateKeys(KEYSTATUS keysArray[],int arr_size){
    int size = min(256,arr_size);
    for(int i=0;i<size;i++){
        keysArray[i].pre = keysArray[i].cur;//覆盖
        keysArray[i].cur = GetAsyncKeyState(i) & 0x8000;//更新
    }
    return;
}
bool UpdateMouse(Point* click_pos){//鼠标左键检测
    HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD record;
    DWORD read;
    DWORD event;
    GetNumberOfConsoleInputEvents(handle,&event);
    if(event == 0){//无事件
        return false;
    }
    bool res = false;
    while(event--){   
        ReadConsoleInput(handle,&record,1,&read);
        if(record.EventType == MOUSE_EVENT){//是鼠标事件
            MOUSE_EVENT_RECORD mouse = record.Event.MouseEvent;
            if(mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED){//左键被点击
                int x = mouse.dwMousePosition.X;
                int y = mouse.dwMousePosition.Y;
                *click_pos = {x,y};
                res |= true;
            }
        }
    }
    return res;
}
bool IsKeyPressed(KEYSTATUS keysArray[],unsigned char ch){//单次点击检测
    return keysArray[ch].cur;
}

bool IsKeyDown(KEYSTATUS keysArray[],unsigned char ch){//持续点击检测
    return keysArray[ch].cur & keysArray[ch].pre;
}

bool CopyGAME_IMAGE(GAME_IMAGE* img_from,GAME_IMAGE* img_to){//图像拷贝
    int from_height = img_from->height;
    int from_width = img_from->width;
    int to_height = img_to->height;
    int to_width = img_to->width;
    if(from_height != to_height || from_width != to_width){//尺寸不同
        return false;
    }
    int size = from_height * from_width;
    for(int i=0;i<size;i++){
        *(img_to->leftUp + i) = *(img_from->leftUp + i);
    }
    return true;
}

void Show_GAME_IMAGE(Point start_pos,GAME_IMAGE* img){//显示图片
    int height = img->height;
    int width = img->width;
    for(int add_y = 0;add_y < height;add_y++){
        for(int add_x = 0;add_x < width;add_x++){
            int index = add_y * width + add_x;
            int nx = start_pos.x + add_x;
            int ny = start_pos.y + add_y;
            PIX ch = *(img->leftUp + index);
            WriteChar(nx,ny,ch);
        }
    }
    return;
}

void Show_GAME_IMAGE(int start_x,int start_y,GAME_IMAGE* img){
    Point start_pos = {start_x,start_y};
    Show_GAME_IMAGE(start_pos,img);
    return;
}

void Insert_PIX(GAME_IMAGE* img,PIX pix,int x,int y){//插入像素
    int index = y * img->width + x;
    *(img->leftUp + index) = pix;
    return; 
}

void Insert_PIX(GAME_IMAGE* img,PIX pix,Point p){//插入点
    Insert_PIX(img,pix,p.x,p.y);
    return;
}

void Insert_IMAGE(GAME_IMAGE* img_from,GAME_IMAGE* img_to,int begin_x,int begin_y){//插入图片
    int from_height = img_from->height;
    int from_width = img_from->width;
    for(int add_y = 0;add_y < from_height;add_y++){
        for(int add_x = 0;add_x < from_width;add_x++){
            int nx = add_x + begin_x;
            int ny = add_y + begin_y;
            int index = add_y * from_width + add_x;
            PIX pix = *(img_from->leftUp + index);
            if(pix != ' '){
                Insert_PIX(img_to,pix,nx,ny);
            }

        }
    }
    return;
}

void Insert_IMAGE(GAME_IMAGE* img_from,GAME_IMAGE* img_to,Point begin_p){
    Insert_IMAGE(img_from,img_to,begin_p.x,begin_p.y);
    return;
}

void Fill_IMAGE_As_Array(GAME_IMAGE* img,Array* arr,PIX fill_ch){//根据数组填充图片
    if(img->height != arr->row || img->width != arr->col){//格式不同
        return;
    }
    int width = img->width;
    int height = img->height;
    int index;
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            index = y * width + x;
            if(*(arr->begin + index) != 0){
                *(img->leftUp + index) = fill_ch;
            }
        }
    }
    return;
}

void Insert_String_to_IMAGE(String* str,GAME_IMAGE* img,Point begin_p){
    int size = str->size;
    int begin_x = begin_p.x;
    int begin_y = begin_p.y;
    int width = img->width;
    int index;
    for(int i=0;i<size;i++){
        index = begin_y * width + begin_x + i;
        *(img->leftUp + index) = *(str->begin + i);
    }
    return;
}

void Clear_IMAGE(GAME_IMAGE* img){
    int size = img->width * img->height;
    for(int i=0;i<size;i++){
        *(img->leftUp + i) = ' ';
    }
    return;
}

void Move_head_to_tail(LIST_NODE** list_head,LIST_NODE** list_tail,DIR move_dir){//头到尾
    int dir_x = move_dir.dir_x;
    int dir_y = move_dir.dir_y;
    int tail_x = (*list_tail)->snake_block.pos.x;
    int tail_y = (*list_tail)->snake_block.pos.y;
    int nx = tail_x + dir_x;
    int ny = tail_y + dir_y;
    LIST_NODE* ntail = (LIST_NODE*)malloc(sizeof(LIST_NODE));
    ntail->snake_block.pos = {nx,ny};
    ntail->next = nullptr;
    ntail->pre = nullptr;
    Delete_LIST_NODE_H(list_head,list_tail);
    Insert_LIST_NODE_T(list_head,list_tail,ntail);
    return;
}

void Move_tail_to_head(LIST_NODE** list_head,LIST_NODE** list_tail,DIR move_dir){//尾到头
    int dir_x = move_dir.dir_x;
    int dir_y = move_dir.dir_y;
    int head_x = (*list_head)->snake_block.pos.x;
    int head_y = (*list_head)->snake_block.pos.y;
    int nx = head_x + dir_x;
    int ny = head_y + dir_y;
    LIST_NODE* nhead = (LIST_NODE*)malloc(sizeof(LIST_NODE));
    nhead->snake_block.pos = {nx,ny};
    nhead->next = nullptr;
    nhead->pre = nullptr;
    Delete_LIST_NODE_T(list_head,list_tail);
    Insert_LIST_NODE_H(list_head,list_tail,nhead);
    return;
}

void Snake_move(LIST_NODE** list_head,LIST_NODE** list_tail,DIR move_dir){//蛇移动
    if(*list_head == nullptr || *list_tail == nullptr){
        return;
    }
    Move_tail_to_head(list_head,list_tail,move_dir);
    return;
}

int AddFood(GAME_IMAGE* food_img,Array* food_arr,Food food){//添加食物
    int height = food_img->height;
    int width = food_img->width;
    char ch = food.ch;
    int add = food.add;
    int x = rand() % food.chance;
    if(x != 0){//不添加食物
        return 0;
    }
    while(true){
        int x = rand() % width;
        int y = rand() % height;
        int index = y * width + x;
        if(*(food_arr->begin + index) == 0){//找到空位
            *(food_arr->begin + index) = add;
            *(food_img->leftUp + index) = ch;
            break;
        }
    }
    return 1;
}
struct EatData{//用于返回吃的食物类型及增加的长度
    char ch;
    int add_len;
};
EatData EatFood(LIST_NODE* snake_head,Array* food_array,GAME_IMAGE* food_img){//吃食物
    EatData eat = {' ',0};
    if(snake_head == nullptr){
        return eat;
    }
    int x = snake_head->snake_block.pos.x;
    int y = snake_head->snake_block.pos.y;
    int index = y * food_array->col + x;
    if(*(food_array->begin + index) != 0){
        eat.add_len += *(food_array->begin + index);
        eat.ch = *(food_img->leftUp + index);
        *(food_array->begin + index) = 0;
        *(food_img->leftUp + index) = ' ';
    }
    return eat;
}

int EatSelf(LIST_NODE* list_head,LIST_NODE** list_tail){//吃自己
    if(list_head == nullptr){//空节点
        return 0;
    }
    Point head_p = list_head->snake_block.pos;
    list_head = list_head->next;
    LIST_NODE* node = nullptr;
    while(list_head != nullptr){
        int x = list_head->snake_block.pos.x;
        int y = list_head->snake_block.pos.y;
        if(x == head_p.x && y == head_p.y){//截断点
            node = list_head;
            break;
        }
        list_head = list_head->next;
    }
    int lose_score = 0;
    if(node != nullptr){
        *list_tail = node->pre;
        while(node != nullptr){
            LIST_NODE* next = node->next;
            node->pre->next = nullptr;
            node->pre = nullptr;
            free((void*)node);
            node = next;
            lose_score++;
        }
    }
    return lose_score;
}

bool Update_Snake_Length(int add_len,LIST_NODE** list_head,LIST_NODE** list_tail){//更新蛇的长度
    /*return true表示更新后，长度为正,蛇还活着,false 表示蛇死亡*/
    if(add_len == 0){//不处理
        return true;
    }else if(add_len < 0){
        add_len = -add_len;
        while(add_len && *list_tail != nullptr){
            Delete_LIST_NODE_T(list_head,list_tail);
            add_len--;
        }
        return *list_tail == nullptr ? false : true;
    }else{
        Point pos = (*list_tail)->snake_block.pos;
        while(add_len){
            LIST_NODE* node = (LIST_NODE*)malloc(sizeof(LIST_NODE));
            node->next = nullptr;
            node->pre = nullptr;
            node->snake_block.pos = pos;
            Insert_LIST_NODE_T(list_head,list_tail,node);
            add_len--;
        }
        return true;
    }
}

struct Dynamic_wall{
    LIST_NODE* wall_head;
    LIST_NODE* wall_tail;
    Point begin_p;
    Point end_p;
    DIR dir;
    bool reverse;//true为尾到头
};

void Init_Dynamic_wall(Dynamic_wall* wall,int wall_lenth,Point begin_p,Point end_p){
    if(begin_p.x != end_p.x && begin_p.y != end_p.y){//不是直线
        return;
    }
    DIR dir;
    if(begin_p.x == end_p.x){//y方向移动
        dir.dir_x = 0;
        dir.dir_y = begin_p.y < end_p.y ? 1 : -1;
    }else{//x方向移动
        dir.dir_y = 0;
        dir.dir_x = begin_p.x < end_p.x ? 1 : -1;
    }
    LIST_NODE* node = (LIST_NODE*)malloc(sizeof(LIST_NODE));
    node->next = nullptr;
    node->pre = nullptr;
    node->snake_block.pos = begin_p;
    wall->wall_head = node;
    wall->wall_tail = node;
    for(int i=1;i<wall_lenth;i++){//插入剩余节点
        node = (LIST_NODE*)malloc(sizeof(LIST_NODE));
        node->next = nullptr;
        node->pre = nullptr;
        node->snake_block.pos = wall->wall_tail->snake_block.pos;
        node->snake_block.pos.x += dir.dir_x;
        node->snake_block.pos.y += dir.dir_y;
        Insert_LIST_NODE_H(&wall->wall_head,&wall->wall_tail,node);
    }
    wall->begin_p = begin_p;
    wall->end_p = end_p;
    wall->dir = dir;
    wall->reverse = true;
    return;
}

void Wall_move(Dynamic_wall* wall,Array* collision_array){//墙移动
    Point head_p = wall->wall_head->snake_block.pos;
    Point begin_p = wall->begin_p;
    Point end_p = wall->end_p;
    if(head_p.x == begin_p.x && head_p.y == begin_p.y
    || head_p.x == end_p.x && head_p.y == end_p.y){//到边界处了
        wall->dir.dir_x *= -1;//方向取反
        wall->dir.dir_y *= -1;
        wall->reverse = !wall->reverse;//头尾移动逻辑交换
    }
    if(wall->reverse == true){//尾到头
        Set_Array_value(collision_array,wall->wall_tail->snake_block.pos,0);//尾置0
        Move_tail_to_head(&wall->wall_head,&wall->wall_tail,wall->dir);//移动
        Set_Array_value(collision_array,wall->wall_head->snake_block.pos,1);//头置1
    }else{//头到尾
        Set_Array_value(collision_array,wall->wall_head->snake_block.pos,0);
        Move_head_to_tail(&wall->wall_head,&wall->wall_tail,wall->dir);
        Set_Array_value(collision_array,wall->wall_tail->snake_block.pos,1);
    }
    return;
}

int Collision_check(LIST_NODE** list_head,LIST_NODE** list_tail,Array* collision_grid){//碰撞检测
    int len = 0;
    LIST_NODE* node = *list_head;
    while(node != nullptr){
        int index = node->snake_block.pos.y * collision_grid->col + node->snake_block.pos.x;
        if(*(collision_grid->begin + index) == 1){
            break;
        }
        node = node->next;
    }
    if(node == nullptr){//没撞到
        return len;
    }
    int x = node->snake_block.pos.x;
    int y = node->snake_block.pos.y;
    while((*list_tail)->snake_block.pos.x != x || (*list_tail)->snake_block.pos.y != y){//删到node,不包括node
        Delete_LIST_NODE_T(list_head,list_tail);
        len++;
    }
    Delete_LIST_NODE_T(list_head,list_tail);//单独删除node,避免node是头节点时访问node->pre出错
    len++;
    return len;
}

struct Record_data{//记录数据
    int level;
    int score;
};

void Insert_record(char path[],Record_data data){
    FILE* file;
    file = fopen(path,"a");
    if(file == nullptr){
        return;
    }
    fprintf(file,"%d %d\n",data.level,data.score);
    fclose(file);
    return;
}

void Fill_IMAGE_use_txt(char path[],GAME_IMAGE* img,int max_size,Point begin_p){
    int cnt = 0;
    Record_data data;
    FILE* file;
    file = fopen(path,"r");
    if(file == nullptr){
        return;
    }
    int rdata[3][1000] = {0};
    while(fscanf(file,"%d %d\n",&data.level,&data.score) != EOF){
        rdata[data.level - 1][data.score % 1000]++;
    }
    int i_score = 999;
    for(int i=2;i>=0;i--){
        if(cnt >= max_size){
            break;
        }
        for(int j=999;j>=0;j--){
            if(cnt >= max_size){
                break;
            }
            while(cnt < max_size && rdata[i][j]){
                String str_level = TransNumToString(i + 1);
                String str_score = TransNumToString(j);
                Insert_String_to_IMAGE(&str_level,img,begin_p);
                Insert_String_to_IMAGE(&str_score,img,{begin_p.x + 10,begin_p.y});
                begin_p.y++;
                cnt++;
                rdata[i][j]--;
            }
        }
    }
    fclose(file);
    return;
}
#define GREEDY_SNAKE_H
#endif