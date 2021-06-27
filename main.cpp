#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define M 15 // 故宫景点数量
#define INF 10000 //int最大值

/*数据类型*/
typedef struct
{
    int num; //景点编号
    char name[20]; //景点名称
    char introduction[300]; //景点介绍
} Scenic;

typedef struct
{
    Scenic spots[M];
    int edges[M][M];
    int nodes_num,edges_Num; //总结点数，总边数
} Graph;
/*数据类型 End*/


/**************************************************函数定义表*************************************************************************/
void get_graph(Graph *g);                        //创建地图
void get_edge(Graph *g);                         //创建地图权值
int  check(int ch);                              //判断景点编号是否输入错误
void search(Graph *g);                           //查看景点信息
void comment();                                  //游客评论
void view_comment();                             //游客查看评论
void short_path(Graph *g);                       //球所有景点之间的最短路径长度及其路径
void print_short_path(Graph *g);                 //打印最短路径
void print_all_path(Graph *g);                   //输出所有路径
void dfs_all_path(Graph *g, int start, int end); //利用DFS求全路径
void best_path(Graph *g);                        //最佳路径
void floyd_print(Graph *g, int start, int end);  ///*递归实现打印两点间的最短路径(不包括起始点)*/
void put_map();                                  //输出故宫轮廓图
int  put_menu();                                 //输出主菜单
void exit_system();                              //系统退出界面
int  main();                                     //主函数功能实现
/********************************************************************************************************************************/

int check(int ch)
{
    if (ch < 0 || ch >= 12)
    {
        printf("\n");
        printf("您的输入有误，请输入1~11之间的数字！\n");
        return 0;
    }
    return 1;
}

void get_graph(Graph *g)
{
    int ta, tb, tmp; //临时变量
    FILE *rf;
    rf = fopen("./map.txt", "r"); //文件读取
    if (!rf)
    {
        printf("ERROR!\n");
        return;
    } //如果文件读取失败，返回error！
    else if (rf)
    {
        fscanf(rf, "%d %d", &g->nodes_num, &g->edges_Num); //文件读取总结点数和总边数
        for (int i = 1; i <= g->nodes_num; i++)
        {
            //读取文件景点编号，景点名称，景点简介
            fscanf(rf, "%d %s %s", &g->spots[i].num, g->spots[i].name, g->spots[i].introduction);
        }
        //地图初始化：除了自己，其他点都是不可达
        for (int i = 0; i <= g->nodes_num; i++)
        {
            for (int j = 0; j <= g->nodes_num; j++)
            {
                if (i == j)
                    g->edges[i][j] = 0;
                else
                    g->edges[i][j] = INF;
            }
        }
        get_edge(g);
        fclose(rf); //文件关闭
    }
}
void get_edge(Graph *g)
{
    int ta, tb, tmp; //临时变量
    FILE *rf;
    rf = fopen("./edge.txt", "r"); //文件读取
    //得到边信息
    for (int i = 1; i <= g->edges_Num; i++)
    {
        fscanf(rf, "%d %d %d", &ta, &tb, &tmp);
        //printf("%d %d %d\n",ta,tb,tmp);
        g->edges[ta][tb] = g->edges[tb][ta] = tmp;
    }
    fclose(rf); //文件关闭
}
void search(Graph *g)
{
    int number; //编号
    while (true)
    {
        put_map();
        printf("请问您想查看哪个景点（请输入景点编号,输入0结束）:");
        scanf("%d", &number);
        system("cls"); //清空屏幕
        if (check(number))
        {
            if (number == 0)
                break;
            printf("\n景点编号:%d\n", g->spots[number].num);
            printf("\n景点名称:%s\n", g->spots[number].name);
            printf("\n景点介绍:%s\n", g->spots[number].introduction);
            printf("\n");
        }
    }
}

void comment()
{
    char com[250];
    char name[30];
    int flag; //判断是否匿名
    FILE *rf;
    rf = fopen("./comment.txt", "a");
    printf("               是否匿名(输入0匿名,输入1不匿名):");
    scanf("%d", &flag);
    while (flag != 1 && flag != 0)
    {
        printf("           输入错误，请重新输入:\n");
    }
    if (flag)
    {
        printf("           请输入您的姓名：");
        scanf("%s", name);
    }

    printf("               请输入你的留言:");
    scanf("%s", com);
    printf("\n");
    time_t t;
    struct tm *Time;
    t = time(NULL);
    Time = localtime(&t); //获取本地时间
    if (flag)
    {
        fprintf(rf, "    留言人:%s 留言时间:%s留言内容: %s\n", name, asctime(Time), com);
    }
    else
    {
        fprintf(rf, "    留言人:匿名 留言时间:%s留言内容: %s\n", asctime(Time), com);
    }

    printf("               感谢您的留言!\n");
    fclose(rf);
}
void view_comment()
{
    char check[1024];
    FILE *rf;
    rf = fopen("./comment.txt", "r");

    printf("                            以下为留言内容:\n");
    printf("\n");
    while (fgets(check, 1024, rf) != NULL)
    {
        printf("\t                      %s\n", check);
    }
    printf("\n");
    fclose(rf);
}

int path_stack[M]; //路径栈
int top;
int count;      //栈顶位置，路径数
int visited[M]; //判断是否已经经过
void dfs_all_path(Graph *g, int start, int end)
{
    int dis = 0;            //总距离
    path_stack[top] = start; //将起点入栈
    top++;
    visited[start] = 1;                     //表示该点已经访问过
    for (int i = 1; i <= g->nodes_num; i++) //遍历与这个结点相邻的结点
    {
        if (g->edges[start][i] > 0 && g->edges[start][i] != INF && !visited[i]) //如果与i不是自己并且这个点有可达路径并且未被访问过
        {
            if (i == end) //如果已经到达目的地
            {
                printf("             第%d条道路:", count++); //将该路径输出
                for (int j = 0; j < top; j++)
                {
                    printf("%s->", g->spots[path_stack[j]].name); //输出该景点
                    if (j < top - 1)
                        dis = dis + g->edges[path_stack[j]][path_stack[j + 1]];
                }
                dis = dis + g->edges[path_stack[top - 1]][end]; //计算最后一个边长1
                printf("%s\n", g->spots[end].name);
                printf("             总长度为: %dm\n\n", dis);
            }
            else //如果还未达到
            {
                dfs_all_path(g, i, end); //递归遍历
                top--;                  //出栈
                visited[i] = 0;         //释放该节点
            }
        }
    }
}

void print_all_path(Graph *g)
{
    int flag = 0;
    int start;                               //起点
    int end;                                 //终点
    memset(path_stack, 0, sizeof(path_stack)); //路径栈初始化
    memset(visited, 0, sizeof(visited));     //标记数组初始化
    top = 0;                                 //初始化栈顶
    count = 1;                               //初始化路径条数
    while (true)
    {
        printf("\n");
        printf("                请输入起点编号:");
        scanf("%d", &start);
        if (check(start))
        {
            flag = 1;
            if (1 == flag)
                break;
        }
    }
    flag = 0; //flag重新置0
    while (true)
    {
        printf("\n");
        printf("                请输入终点编号:");
        scanf("%d", &end);
        if (check(end))
        {
            flag = 1;
            if (1 == flag)
                break;
        }
    }
    printf("\n");
    dfs_all_path(g, start, end);
    printf("\n");
}

/*Floyd算法求两景点间的一条最短的路径*/
int distances[M][M]; //距离向量
int path[M][M];     //路径向量
void short_path(Graph *g)
{
    for (int i = 1; i <= g->nodes_num; i++) //初始化距离向量矩阵与路径向量矩阵
        for (int j = 1; j <= g->nodes_num; j++)
        {
            distances[i][j] = g->edges[i][j];
            if (i != j && distances[i][j] != INF)
                path[i][j] = i; //表示如果i和j相邻，i到j需要经过i
            else
                path[i][j] = -1; //否则用 -1代表当前两点不可达
        }

    for (int k = 1; k <= g->nodes_num; k++) //递推求解每两景点的最短路径
        for (int i = 1; i <= g->nodes_num; i++)
            for (int j = 1; j <= g->nodes_num; j++) //更新distances[i][j]的值
                if (distances[i][j] > (distances[i][k] + distances[k][j]))
                {
                    //如果发现引入k点可以使得路径更短
                    distances[i][j] = distances[i][k] + distances[k][j]; //更新最短路径长度
                    path[i][j] = k;                                   // 更新最短路径上的经结点
                }
}
/*递归实现打印两点间的最短路径(不包括起始点)*/
void floyd_print(Graph *g, int start, int end)
{
    //递归出口：如果两点相邻或者两点不可达，结束递归
    if (path[start][end] == -1 || path[start][end] == end || path[start][end] == start)
        return;
    else
    {
        floyd_print(g, start, path[start][end]);       //将中间点作为终点继续打印路径
        printf("%s->", g->spots[path[start][end]].name); //打印中间景点名字
        floyd_print(g, path[start][end], end);         //将中间点作为起点继续打印路径
    }
}

/*输出并打印两点间的最短路径*/
void print_short_path(Graph *g)
{
    int start, end; //起点编号，终点编号
    do
    {
        printf("         请输入起点编号：");
        scanf("%d", &start);
    }
    while (!check(start));
    do
    {
        printf("         请输入终点编号：");
        scanf("%d", &end);
    }
    while (!check(end));
    printf("             %s到%s的最短距离是：%dm\n", g->spots[start].name, g->spots[end].name, distances[start][end]);
    printf("             最佳游览路线：");
    printf("%s->", g->spots[start].name); //输出路径上的起点
    floyd_print(g, start, end);         //输出路径上的中间点
    printf("%s\n", g->spots[end].name);   //输出路径上的终点
}

/*多景点间求最佳路径*/
void best_path(Graph *g)
{
    int v_num[M] = {0}, count = 1; //记录用户输入的编号信息
    int dis = 0;                  //统计全程路径总长
    printf("             请输入你要游览的第%d个景点的编号（输入0结束输入）：", count);
    scanf("%d", &v_num[count]);
    while (v_num[count] != 0 && count <= 11)
    {
        while (!check(v_num[count]))
        {
            printf("     请输入你要游览的第%d个景点编号：", count);
            scanf("%d", &v_num[count]);
        }
        if (v_num[count] == 0)
            break;
        printf("         请输入你要游览的第%d个景点编号：", ++count);
        scanf("%d", &v_num[count]);
    }
    printf("             已为您挑选最佳访问路径：");
    for (int i = 1; v_num[i] > 0 && v_num[i + 1] > 0; i++)
    {
        //遍历所有输入的景点
        printf("%s->", g->spots[v_num[i]].name);  //输出路径上的起点
        floyd_print(g, v_num[i], v_num[i + 1]);  //利用Floyd算法得到这两点之间的最短路径
        dis += distances[v_num[i]][v_num[i + 1]]; //算出最短路长度
    }
    printf("%s\n\n", g->spots[v_num[count - 1]].name); //输出路径上的终点
    printf("             全程总长为：%dm\n\n", dis);
}
void put_map()
{
    printf("***************************************************************************************************************\n");
    printf("*                                『故宫博物院景点地图一览』                                                   *\n");
    printf("*                                                                                                             *\n");
    printf("*          ⑾◎御花园                                                      ⑽◎乾隆花园                       *\n");
    printf("*         //                                                                \\\\        \\\\                      *\n");
    printf("*  ⑸◎乾清门<=====>⑻◎养心殿                                               \\\\       ⑼◎永和宫              *\n");
    printf("*                     \\\\                                                      \\\\         //                   *\n");
    printf("*                    ⑺◎慈宁宫                                                 ◎⑹皇极殿                    *\n");
    printf("*                     //                                                            //                        *\n");
    printf("*                   ⑶◎中和殿                                                   ⑷◎九龙壁                   *\n");
    printf("*                          \\\\                                                   //                            *\n");
    printf("*                                \\\\                                     //                                    *\n");
    printf("*                                       \\\\                           //                                       *\n");
    printf("*                                           \\\\                   //                                           *\n");
    printf("*                                                ⑵◎太和门                                                   *\n");
    printf("*                                                  ||                                                         *\n");
    printf("*                                                  ||                                                         *\n");
    printf("*                                                  ||                                                         *\n");
    printf("*                                                  ||                                                         *\n");
    printf("*                                                  ⑴◎午门                                                   *\n");
    printf("***************************************************************************************************************\n");
}

int put_menu()
{
    int op;
    printf("\n");
    printf("                      *     *       ********     *            *            ********                        \n");
    printf("                      *     *       *            *            *           *        *                       \n");
    printf("                      *******       *******      *            *           *        *                       \n");
    printf("                      *     *       *            *            *           *        *                       \n");
    printf("                      *     *       ********     *******      *******      ********                        \n\n");
    printf("                                                                                                   \n");
    printf("                                                                                                   \n");
    printf("       = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =\n");
    printf("       = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =\n");
    printf("       = =                                                                                   = =\n");
    printf("       = =                         欢迎使用故宫博物院导航咨询系统                            = =\n");
    printf("       = =                                                                                   = =\n");
    printf("       = =                                   请选择功能:                                     = =\n");
    printf("       = =                                                                                   = =\n");
    printf("       = =             1.查看故宫地图                      5.游玩路径策划                    = =\n");
    printf("       = =                                                                                   = =\n");
    printf("       = =             2.查看景点信息                      6.我对故宫有话说                  = =\n");
    printf("       = =                                                                                   = =\n");
    printf("       = =             3.寻找两景点之间的最短路径          7.查看评论                        = =\n");
    printf("       = =                                                                                   = =\n");
    printf("       = =             4.寻找两景点之间所有路径            8.退出系统                        = =\n");
    printf("       = =                                                                                   = =\n");
    printf("       = =                                                                                   = =\n");
    printf("       = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =\n");
    printf("       = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =\n");
    printf("\n\n");
    printf("             请根据你的需求选择操作：");
    scanf("%d", &op);
    return op;
}

void putEdge(Graph *g)
{
    for (int i = 1; i <= g->nodes_num; i++)
    {
        for (int j = 1; j <= g->nodes_num; j++)
        {
            if (g->edges[i][j] == INF)
                printf("INF ");
            else
                printf("%d ", g->edges[i][j]);
        }
        printf("\n");
    }
}

void exit_system()
{
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("                                                                     \n");
    printf("                                               期待您的下一次使用！！！\n");
    exit(0);
}

int main()
{
    //system("color f5"); //改变颜色
    //put_map();
    int quit = 0;
    Graph g;
    get_graph(&g);  //从文件读取信息建立故宫地图
    short_path(&g); //通过Floyd求出distances表与path表
    while (!quit)
    {
        //系统退出条件满足判定
        switch (put_menu())
        {
        //打印主菜单
        case 1:
            system("cls");
            put_map();
            break; //故宫景点平面图查询
        case 2:
            system("cls");
            search(&g);
            break; //故宫景点信息查询
        case 3:
            system("cls");
            put_map();
            print_short_path(&g);
            break; //两景点间最短路径查询
        case 4:
            system("cls");
            put_map();
            print_all_path(&g);
            break; //两景点间所有路线查询
        case 5:
            system("cls");
            put_map();
            best_path(&g);
            break; //多景点间访问最优路线查询
        case 6:
            system("cls");
            comment();
            break; //对故宫的评价
        case 7:
            system("cls");
            view_comment();
            break; //查看评价
        case 8:
            system("cls");
            exit_system();
            break; //退出故宫导游咨询系统
        default:
            printf("             对不起！没有该选项对应的操作。\n\n");
        }
        system("pause");
        system("cls");
        /*清屏*/
    }
    return 0;
}
