#include "GreedySnake.h"
bool is_record = false;//此次是否被记录，false表示还没记录
int Init_len = 4;//初始长度
int screen_height = 30;//游戏屏幕高
int screen_width = 50;//游戏屏幕宽
int menu_height = 20;//菜单高
int menu_width = 40;//菜单宽
int about_height = 20;//关于界面高
int about_width = 40;//关于界面宽
int help_height = 20;//帮助界面高
int help_width = 40;//帮助界面宽
int record_height = 20;//记录界面高
int record_width = 40;//记录界面宽
int max_food_nums = 2;//最大食物数量
int foods_cnt[4] = {0,0,0,0};//统计各个种类食物的个数
int food_kind = 4;//食物种类
int score = 0;//得分
int goal_score = 20;//目标得分
int max_score = 0;//历史最大得分
int level = 1;//当前关卡
int max_level = 3;//最大关卡
int d_wall_length = 5;//动态墙长度
PIX snake_char = 'S';//蛇的字符
PIX wall_char = 'X';//墙的字符
KEYSTATUS keysArray[256];//按键状态数组
int keysArray_size = 256;
DIR Init_dir = {0,1};//初始方向
DIR snake_dir = {-1,0};//蛇的方向
Point Start_pos = {10,10};//初始位置
Point screen_left_up = {10,10};//游戏屏幕左上角坐标
Point menu_left_up = {10,5};//菜单左上角
Point about_left_up = {10,5};//关于界面左上角
Point help_left_up = {10,5};//帮助界面左上角
Point record_left_up = {10,5};//记录界面左上角
Point start_game_begin_p = {10,8};
Point start_game_end_p = {19,8};
Point help_begin_p = {13,10};
Point help_end_p = {16,10};
Point about_begin_p = {12,12};
Point about_end_p = {16,12};
Point record_begin_p = {12,14};
Point record_end_p = {17,14};
Point game_quit_begin_p = {65,10};
Point game_quit_end_p = {68,10};
Point help_up_p = {1,5};
Point help_down_p = {1,7};
Point help_left_p = {1,9};
Point help_right_p = {1,11};
Point help_advice_p = {1,13};
Point help_quit_begin_p = {1,15};
Point help_quit_end_p = {4,15};
Point about_designer1_p = {8,7};
Point about_designer2_p = {10,9};
Point about_university_p = {1,11};
Point about_major_p = {8,13};
Point about_quit_begin_p = {18,15};
Point about_quit_end_p = {21,15};
Point record_title_begin_p = {3,1};
Point record_quit_begin_p = {9,19};
Point record_quit_end_p = {12,19};
Point d_wall_begin_p[] = {{10,5},{10,25},{25,11}};
Point d_wall_end_p[] = {{40,5},{40,25},{25,21}};
LIST_NODE* snake_head = nullptr;//头
LIST_NODE* snake_tail = nullptr;//尾
GAME_IMAGE* front_buffer = nullptr;//前缓冲区
GAME_IMAGE* back_buffer = nullptr;//后缓冲区
GAME_IMAGE* food_image = nullptr;//食物图层
GAME_IMAGE* solid_collision_image = nullptr;//静态碰撞图层
GAME_IMAGE* score_image = nullptr;//得分
GAME_IMAGE* goal_image = nullptr;//目标
GAME_IMAGE* menu_image = nullptr;//菜单
GAME_IMAGE* game_quit_image = nullptr;//游戏返回菜单
GAME_IMAGE* blank_image = nullptr;//空白图片,用于清空
GAME_IMAGE* help_image = nullptr;//帮助界面图层
GAME_IMAGE* about_image = nullptr;//关于界面图层
GAME_IMAGE* level_image = nullptr;//关卡图片
GAME_IMAGE* record_image = nullptr;//记录界面图片
Array* food_array = nullptr;//食物数组
Array* collision_array = nullptr;//碰撞网格
Food foods[] = {{'A',1,10},{'B',2,20},{'C',5,50},{'D',-1,10}};
char start_game[] = "Start Game";
char help[] = "Help";
char about[] = "About";
char c_record[] = "Record";
char quit[] = "Quit";
char designer1[] = "Designer 1: HuangYiXiang";
char designer2[] = "Designer 2: XiaLeTian";
char university[] = "University: Hangzhou Dianzi University";
char major[] = "Major in Computer Science";
char up_is_w[] = "Up : W.";
char down_is_s[] = "Down : S.";
char left_is_a[] = "Left : A.";
char right_is_d[] = "Right : D.";
char advice[] = "For best results, press and hold.";
char c_score[] = "Score:";
char goal[] = "Goal:";
char c_level[] = "Level:";
char c_record_title[] = "level     score";
char path[] = "D:/snake_data.txt";
String* str_start_game = nullptr;
String* str_help = nullptr;
String* str_about = nullptr;
String* str_quit = nullptr;
String* str_designer1 = nullptr;
String* str_designer2 = nullptr;
String* str_university = nullptr;
String* str_major = nullptr; 
String* str_up_is_w = nullptr;
String* str_down_is_s = nullptr;
String* str_left_is_a = nullptr;
String* str_right_is_d = nullptr;
String* str_advice = nullptr;
String* str_score = nullptr;
String* str_goal = nullptr;
String* str_level = nullptr;
String* str_record = nullptr;
String* str_record_title = nullptr;
Dynamic_wall* dynamic_wall[3] = {nullptr};
int main(){
    srand(time(NULL));
    HideCursor();
    BanKeyboardInput();
    str_start_game = (String*)malloc(sizeof(String));
    str_help = (String*)malloc(sizeof(String));
    str_about = (String*)malloc(sizeof(String));
    str_quit = (String*)malloc(sizeof(String));
    str_designer1 = (String*)malloc(sizeof(String));
    str_designer2 = (String*)malloc(sizeof(String));
    str_university = (String*)malloc(sizeof(String));
    str_major = (String*)malloc(sizeof(String));
    str_up_is_w = (String*)malloc(sizeof(String));
    str_down_is_s = (String*)malloc(sizeof(String));
    str_left_is_a = (String*)malloc(sizeof(String));
    str_right_is_d = (String*)malloc(sizeof(String));
    str_advice = (String*)malloc(sizeof(String));
    str_score = (String*)malloc(sizeof(String));
    str_goal = (String*)malloc(sizeof(String));
    str_level = (String*)malloc(sizeof(String));
    str_record = (String*)malloc(sizeof(String));
    str_record_title = (String*)malloc(sizeof(String));
    Init_String_use_Pchar(str_start_game,start_game);
    Init_String_use_Pchar(str_help,help);
    Init_String_use_Pchar(str_about,about);
    Init_String_use_Pchar(str_quit,quit);
    Init_String_use_Pchar(str_designer1,designer1);
    Init_String_use_Pchar(str_designer2,designer2);
    Init_String_use_Pchar(str_university,university);
    Init_String_use_Pchar(str_major,major);
    Init_String_use_Pchar(str_up_is_w,up_is_w);
    Init_String_use_Pchar(str_down_is_s,down_is_s);
    Init_String_use_Pchar(str_left_is_a,left_is_a);
    Init_String_use_Pchar(str_right_is_d,right_is_d);
    Init_String_use_Pchar(str_advice,advice);
    Init_String_use_Pchar(str_score,c_score);
    Init_String_use_Pchar(str_goal,goal);
    Init_String_use_Pchar(str_level,c_level);
    Init_String_use_Pchar(str_record,c_record);
    Init_String_use_Pchar(str_record_title,c_record_title);
    food_array = (Array*)malloc(sizeof(Array));
    collision_array = (Array*)malloc(sizeof(Array));
    Init_Array(food_array,screen_height,screen_width);
    Init_Array(collision_array,screen_height,screen_width);
    front_buffer = (GAME_IMAGE*)malloc(sizeof(GAME_IMAGE));
    back_buffer = (GAME_IMAGE*)malloc(sizeof(GAME_IMAGE));
    food_image = (GAME_IMAGE*)malloc(sizeof(GAME_IMAGE));
    solid_collision_image = (GAME_IMAGE*)malloc(sizeof(GAME_IMAGE));
    score_image = (GAME_IMAGE*)malloc(sizeof(GAME_IMAGE));
    menu_image = (GAME_IMAGE*)malloc(sizeof(GAME_IMAGE));
    game_quit_image = (GAME_IMAGE*)malloc(sizeof(GAME_IMAGE));
    blank_image = (GAME_IMAGE*)malloc(sizeof(GAME_IMAGE));
    about_image = (GAME_IMAGE*)malloc(sizeof(GAME_IMAGE));
    help_image = (GAME_IMAGE*)malloc(sizeof(GAME_IMAGE));
    goal_image = (GAME_IMAGE*)malloc(sizeof(GAME_IMAGE));
    level_image = (GAME_IMAGE*)malloc(sizeof(GAME_IMAGE));
    record_image = (GAME_IMAGE*)malloc(sizeof(GAME_IMAGE));
    Init_GAME_IMAGE(front_buffer,screen_height,screen_width);
    Init_GAME_IMAGE(back_buffer,screen_height,screen_width);
    Init_GAME_IMAGE(food_image,screen_height,screen_width);
    Init_GAME_IMAGE(solid_collision_image,screen_height,screen_width);
    Init_GAME_IMAGE(score_image,1,10);
    Init_GAME_IMAGE(menu_image,menu_height,menu_width);
    Init_GAME_IMAGE(game_quit_image,1,4);
    Init_GAME_IMAGE(blank_image,60,70);
    Init_GAME_IMAGE(about_image,about_height,about_width);
    Init_GAME_IMAGE(help_image,help_height,help_width);
    Init_GAME_IMAGE(goal_image,1,10);
    Init_GAME_IMAGE(level_image,1,10);
    Init_GAME_IMAGE(record_image,record_height,record_width);
    Insert_String_to_IMAGE(str_score,score_image,{0,0});
    Insert_String_to_IMAGE(str_start_game,menu_image,start_game_begin_p);
    Insert_String_to_IMAGE(str_help,menu_image,help_begin_p);
    Insert_String_to_IMAGE(str_about,menu_image,about_begin_p);
    Insert_String_to_IMAGE(str_record,menu_image,record_begin_p);
    Insert_String_to_IMAGE(str_quit,game_quit_image,{0,0});
    Insert_String_to_IMAGE(str_designer1,about_image,about_designer1_p);
    Insert_String_to_IMAGE(str_designer2,about_image,about_designer2_p);
    Insert_String_to_IMAGE(str_university,about_image,about_university_p);
    Insert_String_to_IMAGE(str_major,about_image,about_major_p);
    Insert_String_to_IMAGE(str_quit,about_image,about_quit_begin_p);
    Insert_String_to_IMAGE(str_up_is_w,help_image,help_up_p);
    Insert_String_to_IMAGE(str_down_is_s,help_image,help_down_p);
    Insert_String_to_IMAGE(str_left_is_a,help_image,help_left_p);
    Insert_String_to_IMAGE(str_right_is_d,help_image,help_right_p);
    Insert_String_to_IMAGE(str_advice,help_image,help_advice_p);
    Insert_String_to_IMAGE(str_quit,help_image,help_quit_begin_p);
    Insert_String_to_IMAGE(str_goal,goal_image,{0,0});
    Insert_String_to_IMAGE(str_level,level_image,{0,0});
    dynamic_wall[0] = (Dynamic_wall*)malloc(sizeof(Dynamic_wall));
    dynamic_wall[1] = (Dynamic_wall*)malloc(sizeof(Dynamic_wall));
    dynamic_wall[2] = (Dynamic_wall*)malloc(sizeof(Dynamic_wall));
    LIST_NODE* node = snake_head;
    while(node != nullptr){//更新后缓冲区
        Insert_PIX(back_buffer,snake_char,node->snake_block.pos);
        node = node->next;
    }
    bool click_start_game = false;
    bool click_help = false;
    bool click_about = false;
    bool click_record = false;
    Point begin_p = {1,1};//不含边界的起止位置
    Point end_p = {screen_width - 2,screen_height - 2};
    while(true){//菜单循环
        Show_GAME_IMAGE(menu_left_up,menu_image);//显示菜单
        Point click_p;
        if(UpdateMouse(&click_p)){
            click_start_game = In_range(start_game_begin_p,start_game_end_p,click_p,menu_left_up);
            click_help = In_range(help_begin_p,help_end_p,click_p,menu_left_up);
            click_about = In_range(about_begin_p,about_end_p,click_p,menu_left_up);
            click_record = In_range(record_begin_p,record_end_p,click_p,menu_left_up);
        }
        if(click_start_game || click_help || click_about || click_record){//打开任意界面都清除菜单
            Show_GAME_IMAGE({0,0},blank_image);
        }
        if(click_start_game){//初始化游戏
            snake_head = (LIST_NODE*)malloc(sizeof(LIST_NODE));
            snake_head->snake_block.pos = Start_pos;
            snake_tail = snake_head;
            for(int i=1;i<Init_len;i++){//添加蛇的初始身体
                LIST_NODE* node = (LIST_NODE*)malloc(sizeof(LIST_NODE));
                node->snake_block.pos = snake_tail->snake_block.pos;
                node->snake_block.pos.x++;
                node->next = node->pre = nullptr;
                Insert_LIST_NODE_T(&snake_head,&snake_tail,node);
            }
            for(int i=0;i<food_kind;i++){//清空计数
                foods_cnt[i] = 0;
            }
            Init_Dynamic_wall(dynamic_wall[0],d_wall_length,d_wall_begin_p[0],d_wall_end_p[0]);
            Fill_Array(food_array,-10000);//边界处预先标记，防止食物生成在边界
            Fill_Array_range(food_array,begin_p,end_p,0);
            Fill_Array(collision_array,1);//边界处标1
            Fill_Array_range(collision_array,begin_p,end_p,0);
            Fill_IMAGE_As_Array(solid_collision_image,collision_array,wall_char);
            Clear_IMAGE(food_image);
            Clear_IMAGE(front_buffer);
            Clear_IMAGE(back_buffer);
            snake_dir = Init_dir;
            goal_score = 20;
            level = 1;
            score = 0;
            max_score = 0;
            is_record = false;
        }
        if(click_record){//初始化记录
            Clear_IMAGE(record_image);
            Insert_String_to_IMAGE(str_record_title,record_image,record_title_begin_p);
            Fill_IMAGE_use_txt(path,record_image,10,{5,5});
            Insert_String_to_IMAGE(str_quit,record_image,record_quit_begin_p);
        }
        while(click_start_game){//游戏循环
            /*交换前后缓冲区*/
            GAME_IMAGE* tmp = front_buffer;
            front_buffer = back_buffer;
            back_buffer = tmp;
            Show_GAME_IMAGE(screen_left_up,front_buffer);
            UpdateKeys(keysArray,keysArray_size);
            if(IsKeyPressed(keysArray,'W') == true && snake_dir.dir_x != DOWN.dir_x && snake_dir.dir_y != DOWN.dir_y){
                snake_dir = UP;
            }else if(IsKeyPressed(keysArray,'S') == true && snake_dir.dir_x != UP.dir_x && snake_dir.dir_y != UP.dir_y){
                snake_dir = DOWN;
            }else if(IsKeyPressed(keysArray,'A') == true && snake_dir.dir_x != RIGHT.dir_x && snake_dir.dir_y != RIGHT.dir_y){
                snake_dir = LEFT;
            }else if(IsKeyPressed(keysArray,'D') == true && snake_dir.dir_x != LEFT.dir_x && snake_dir.dir_y != LEFT.dir_y){
                snake_dir = RIGHT;
            }
            Snake_move(&snake_head,&snake_tail,snake_dir);
            for(int i=0;i<level;i++){
                Wall_move(dynamic_wall[i],collision_array);
            }
            score -= Collision_check(&snake_head,&snake_tail,collision_array);
            int choich_food = rand() % food_kind;//选择加的食物种类
            if(foods_cnt[choich_food] < max_food_nums){//还能加食物
                foods_cnt[choich_food] += AddFood(food_image,food_array,foods[choich_food]);
            }
            EatData eat_res = EatFood(snake_head,food_array,food_image);
            score += eat_res.add_len;//加分
            max_score = max(max_score,score);
            if(eat_res.add_len != 0){//真的吃到食物了
                foods_cnt[eat_res.ch - 'A']--;
            }
            Update_Snake_Length(eat_res.add_len,&snake_head,&snake_tail);
            score -= EatSelf(snake_head,&snake_tail);
            Clear_IMAGE(back_buffer);//清空后缓冲区
            LIST_NODE* node = snake_head;
            Insert_IMAGE(food_image,back_buffer,0,0);//添加食物图层
            Insert_IMAGE(solid_collision_image,back_buffer,0,0);//添加碰撞图层
            while(node != nullptr){//添加蛇图层
                Insert_PIX(back_buffer,snake_char,node->snake_block.pos);
                node = node->next;
            }
            for(int i=0;i<level;i++){
                node = dynamic_wall[i]->wall_head;
                while(node != nullptr){
                    Insert_PIX(back_buffer,wall_char,node->snake_block.pos);
                    node = node->next;
                }
            }
            Clear_IMAGE(score_image);//更新分数图片
            String word_score = TransNumToString(score);
            Insert_String_to_IMAGE(str_score,score_image,{0,0});
            Insert_String_to_IMAGE(&word_score,score_image,{6,0});
            Show_GAME_IMAGE({50,8},score_image);
            Clear_IMAGE(goal_image);//更新目标分数图片
            String word_goal = TransNumToString(goal_score);
            Insert_String_to_IMAGE(str_goal,goal_image,{0,0});
            Insert_String_to_IMAGE(&word_goal,goal_image,{5,0});
            Show_GAME_IMAGE({30,8},goal_image);
            Clear_IMAGE(level_image);//更新关卡图片
            String word_level = TransNumToString(level);
            Insert_String_to_IMAGE(str_level,level_image,{0,0});
            Insert_String_to_IMAGE(&word_level,level_image,{6,0});
            Show_GAME_IMAGE({10,8},level_image);
            Show_GAME_IMAGE(game_quit_begin_p,game_quit_image);
            Point click_p;
            if(score >= goal_score && level < max_level){//下一关
                Init_Dynamic_wall(dynamic_wall[level],d_wall_length,d_wall_begin_p[level],d_wall_end_p[level]);
                goal_score *= 2;
                level++;
                Clear_IMAGE(food_image);
                Clear_IMAGE(front_buffer);
                Clear_IMAGE(back_buffer);
                Fill_Array(food_array,-10000);//边界处预先标记，防止食物生成在边界
                Fill_Array_range(food_array,begin_p,end_p,0);
                Delete_LIST_NODE_All(&snake_head,&snake_tail);
                snake_head = (LIST_NODE*)malloc(sizeof(LIST_NODE));
                snake_head->snake_block.pos = Start_pos;
                snake_tail = snake_head;
                for(int i=1;i<Init_len;i++){//添加蛇的初始身体
                    LIST_NODE* node = (LIST_NODE*)malloc(sizeof(LIST_NODE));
                    node->snake_block.pos = snake_tail->snake_block.pos;
                    node->snake_block.pos.x++;
                    node->next = node->pre = nullptr;
                    Insert_LIST_NODE_T(&snake_head,&snake_tail,node);
                }
                for(int i=0;i<food_kind;i++){//清空计数
                    foods_cnt[i] = 0;
                }
                snake_dir = Init_dir;
                Show_GAME_IMAGE({0,0},blank_image);
            }
            if(UpdateMouse(&click_p)){
                if(In_range(game_quit_begin_p,game_quit_end_p,click_p,{0,0})){
                    Show_GAME_IMAGE({0,0},blank_image);
                    click_start_game = false;
                    break;
                }
            }
            if(snake_head == nullptr && is_record == false){//死了没记录
                is_record = true;
                Insert_record(path,{level,max_score});
            }
            Sleep(40);
        }
        while(click_help){//帮助界面循环
            Show_GAME_IMAGE(help_left_up,help_image);
            Point click_p;
            if(UpdateMouse(&click_p)){
                if(In_range(help_quit_begin_p,help_quit_end_p,click_p,help_left_up)){
                    Show_GAME_IMAGE({0,0},blank_image);
                    click_help = false;
                    break;
                }
            }           
        }
        while(click_about){//关于界面循环
            Show_GAME_IMAGE(about_left_up,about_image);
            Point click_p;
            if(UpdateMouse(&click_p)){
                if(In_range(about_quit_begin_p,about_quit_end_p,click_p,about_left_up)){
                    Show_GAME_IMAGE({0,0},blank_image);
                    click_about = false;
                    break;
                }
            }
        }
        while(click_record){//记录界面循环
            Show_GAME_IMAGE(record_left_up,record_image);
            Point click_p;
            if(UpdateMouse(&click_p)){
                if(In_range(record_quit_begin_p,record_quit_end_p,click_p,record_left_up)){
                    Show_GAME_IMAGE({0,0},blank_image);
                    click_record = false;
                    break;
                }
            }
        }
        Sleep(40);
    }
    return 0;
}