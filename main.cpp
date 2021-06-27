#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define M 15 // �ʹ���������
#define INF 10000 //int���ֵ

/*��������*/
typedef struct
{
    int num; //������
    char name[20]; //��������
    char introduction[300]; //�������
} Scenic;

typedef struct
{
    Scenic spots[M];
    int edges[M][M];
    int nodes_num,edges_Num; //�ܽ�������ܱ���
} Graph;
/*�������� End*/


/**************************************************���������*************************************************************************/
void get_graph(Graph *g);                        //������ͼ
void get_edge(Graph *g);                         //������ͼȨֵ
int  check(int ch);                              //�жϾ������Ƿ��������
void search(Graph *g);                           //�鿴������Ϣ
void comment();                                  //�ο�����
void view_comment();                             //�οͲ鿴����
void short_path(Graph *g);                       //�����о���֮������·�����ȼ���·��
void print_short_path(Graph *g);                 //��ӡ���·��
void print_all_path(Graph *g);                   //�������·��
void dfs_all_path(Graph *g, int start, int end); //����DFS��ȫ·��
void best_path(Graph *g);                        //���·��
void floyd_print(Graph *g, int start, int end);  ///*�ݹ�ʵ�ִ�ӡ���������·��(��������ʼ��)*/
void put_map();                                  //����ʹ�����ͼ
int  put_menu();                                 //������˵�
void exit_system();                              //ϵͳ�˳�����
int  main();                                     //����������ʵ��
/********************************************************************************************************************************/

int check(int ch)
{
    if (ch < 0 || ch >= 12)
    {
        printf("\n");
        printf("������������������1~11֮������֣�\n");
        return 0;
    }
    return 1;
}

void get_graph(Graph *g)
{
    int ta, tb, tmp; //��ʱ����
    FILE *rf;
    rf = fopen("./map.txt", "r"); //�ļ���ȡ
    if (!rf)
    {
        printf("ERROR!\n");
        return;
    } //����ļ���ȡʧ�ܣ�����error��
    else if (rf)
    {
        fscanf(rf, "%d %d", &g->nodes_num, &g->edges_Num); //�ļ���ȡ�ܽ�������ܱ���
        for (int i = 1; i <= g->nodes_num; i++)
        {
            //��ȡ�ļ������ţ��������ƣ�������
            fscanf(rf, "%d %s %s", &g->spots[i].num, g->spots[i].name, g->spots[i].introduction);
        }
        //��ͼ��ʼ���������Լ��������㶼�ǲ��ɴ�
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
        fclose(rf); //�ļ��ر�
    }
}
void get_edge(Graph *g)
{
    int ta, tb, tmp; //��ʱ����
    FILE *rf;
    rf = fopen("./edge.txt", "r"); //�ļ���ȡ
    //�õ�����Ϣ
    for (int i = 1; i <= g->edges_Num; i++)
    {
        fscanf(rf, "%d %d %d", &ta, &tb, &tmp);
        //printf("%d %d %d\n",ta,tb,tmp);
        g->edges[ta][tb] = g->edges[tb][ta] = tmp;
    }
    fclose(rf); //�ļ��ر�
}
void search(Graph *g)
{
    int number; //���
    while (true)
    {
        put_map();
        printf("��������鿴�ĸ����㣨�����뾰����,����0������:");
        scanf("%d", &number);
        system("cls"); //�����Ļ
        if (check(number))
        {
            if (number == 0)
                break;
            printf("\n������:%d\n", g->spots[number].num);
            printf("\n��������:%s\n", g->spots[number].name);
            printf("\n�������:%s\n", g->spots[number].introduction);
            printf("\n");
        }
    }
}

void comment()
{
    char com[250];
    char name[30];
    int flag; //�ж��Ƿ�����
    FILE *rf;
    rf = fopen("./comment.txt", "a");
    printf("               �Ƿ�����(����0����,����1������):");
    scanf("%d", &flag);
    while (flag != 1 && flag != 0)
    {
        printf("           �����������������:\n");
    }
    if (flag)
    {
        printf("           ����������������");
        scanf("%s", name);
    }

    printf("               �������������:");
    scanf("%s", com);
    printf("\n");
    time_t t;
    struct tm *Time;
    t = time(NULL);
    Time = localtime(&t); //��ȡ����ʱ��
    if (flag)
    {
        fprintf(rf, "    ������:%s ����ʱ��:%s��������: %s\n", name, asctime(Time), com);
    }
    else
    {
        fprintf(rf, "    ������:���� ����ʱ��:%s��������: %s\n", asctime(Time), com);
    }

    printf("               ��л��������!\n");
    fclose(rf);
}
void view_comment()
{
    char check[1024];
    FILE *rf;
    rf = fopen("./comment.txt", "r");

    printf("                            ����Ϊ��������:\n");
    printf("\n");
    while (fgets(check, 1024, rf) != NULL)
    {
        printf("\t                      %s\n", check);
    }
    printf("\n");
    fclose(rf);
}

int path_stack[M]; //·��ջ
int top;
int count;      //ջ��λ�ã�·����
int visited[M]; //�ж��Ƿ��Ѿ�����
void dfs_all_path(Graph *g, int start, int end)
{
    int dis = 0;            //�ܾ���
    path_stack[top] = start; //�������ջ
    top++;
    visited[start] = 1;                     //��ʾ�õ��Ѿ����ʹ�
    for (int i = 1; i <= g->nodes_num; i++) //���������������ڵĽ��
    {
        if (g->edges[start][i] > 0 && g->edges[start][i] != INF && !visited[i]) //�����i�����Լ�����������пɴ�·������δ�����ʹ�
        {
            if (i == end) //����Ѿ�����Ŀ�ĵ�
            {
                printf("             ��%d����·:", count++); //����·�����
                for (int j = 0; j < top; j++)
                {
                    printf("%s->", g->spots[path_stack[j]].name); //����þ���
                    if (j < top - 1)
                        dis = dis + g->edges[path_stack[j]][path_stack[j + 1]];
                }
                dis = dis + g->edges[path_stack[top - 1]][end]; //�������һ���߳�1
                printf("%s\n", g->spots[end].name);
                printf("             �ܳ���Ϊ: %dm\n\n", dis);
            }
            else //�����δ�ﵽ
            {
                dfs_all_path(g, i, end); //�ݹ����
                top--;                  //��ջ
                visited[i] = 0;         //�ͷŸýڵ�
            }
        }
    }
}

void print_all_path(Graph *g)
{
    int flag = 0;
    int start;                               //���
    int end;                                 //�յ�
    memset(path_stack, 0, sizeof(path_stack)); //·��ջ��ʼ��
    memset(visited, 0, sizeof(visited));     //��������ʼ��
    top = 0;                                 //��ʼ��ջ��
    count = 1;                               //��ʼ��·������
    while (true)
    {
        printf("\n");
        printf("                �����������:");
        scanf("%d", &start);
        if (check(start))
        {
            flag = 1;
            if (1 == flag)
                break;
        }
    }
    flag = 0; //flag������0
    while (true)
    {
        printf("\n");
        printf("                �������յ���:");
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

/*Floyd�㷨����������һ����̵�·��*/
int distances[M][M]; //��������
int path[M][M];     //·������
void short_path(Graph *g)
{
    for (int i = 1; i <= g->nodes_num; i++) //��ʼ����������������·����������
        for (int j = 1; j <= g->nodes_num; j++)
        {
            distances[i][j] = g->edges[i][j];
            if (i != j && distances[i][j] != INF)
                path[i][j] = i; //��ʾ���i��j���ڣ�i��j��Ҫ����i
            else
                path[i][j] = -1; //������ -1����ǰ���㲻�ɴ�
        }

    for (int k = 1; k <= g->nodes_num; k++) //�������ÿ����������·��
        for (int i = 1; i <= g->nodes_num; i++)
            for (int j = 1; j <= g->nodes_num; j++) //����distances[i][j]��ֵ
                if (distances[i][j] > (distances[i][k] + distances[k][j]))
                {
                    //�����������k�����ʹ��·������
                    distances[i][j] = distances[i][k] + distances[k][j]; //�������·������
                    path[i][j] = k;                                   // �������·���ϵľ����
                }
}
/*�ݹ�ʵ�ִ�ӡ���������·��(��������ʼ��)*/
void floyd_print(Graph *g, int start, int end)
{
    //�ݹ���ڣ�����������ڻ������㲻�ɴ�����ݹ�
    if (path[start][end] == -1 || path[start][end] == end || path[start][end] == start)
        return;
    else
    {
        floyd_print(g, start, path[start][end]);       //���м����Ϊ�յ������ӡ·��
        printf("%s->", g->spots[path[start][end]].name); //��ӡ�м侰������
        floyd_print(g, path[start][end], end);         //���м����Ϊ��������ӡ·��
    }
}

/*�������ӡ���������·��*/
void print_short_path(Graph *g)
{
    int start, end; //����ţ��յ���
    do
    {
        printf("         ����������ţ�");
        scanf("%d", &start);
    }
    while (!check(start));
    do
    {
        printf("         �������յ��ţ�");
        scanf("%d", &end);
    }
    while (!check(end));
    printf("             %s��%s����̾����ǣ�%dm\n", g->spots[start].name, g->spots[end].name, distances[start][end]);
    printf("             �������·�ߣ�");
    printf("%s->", g->spots[start].name); //���·���ϵ����
    floyd_print(g, start, end);         //���·���ϵ��м��
    printf("%s\n", g->spots[end].name);   //���·���ϵ��յ�
}

/*�ྰ��������·��*/
void best_path(Graph *g)
{
    int v_num[M] = {0}, count = 1; //��¼�û�����ı����Ϣ
    int dis = 0;                  //ͳ��ȫ��·���ܳ�
    printf("             ��������Ҫ�����ĵ�%d������ı�ţ�����0�������룩��", count);
    scanf("%d", &v_num[count]);
    while (v_num[count] != 0 && count <= 11)
    {
        while (!check(v_num[count]))
        {
            printf("     ��������Ҫ�����ĵ�%d�������ţ�", count);
            scanf("%d", &v_num[count]);
        }
        if (v_num[count] == 0)
            break;
        printf("         ��������Ҫ�����ĵ�%d�������ţ�", ++count);
        scanf("%d", &v_num[count]);
    }
    printf("             ��Ϊ����ѡ��ѷ���·����");
    for (int i = 1; v_num[i] > 0 && v_num[i + 1] > 0; i++)
    {
        //������������ľ���
        printf("%s->", g->spots[v_num[i]].name);  //���·���ϵ����
        floyd_print(g, v_num[i], v_num[i + 1]);  //����Floyd�㷨�õ�������֮������·��
        dis += distances[v_num[i]][v_num[i + 1]]; //������·����
    }
    printf("%s\n\n", g->spots[v_num[count - 1]].name); //���·���ϵ��յ�
    printf("             ȫ���ܳ�Ϊ��%dm\n\n", dis);
}
void put_map()
{
    printf("***************************************************************************************************************\n");
    printf("*                                ���ʹ�����Ժ�����ͼһ����                                                   *\n");
    printf("*                                                                                                             *\n");
    printf("*          �ϡ�����԰                                                      �Ρ�Ǭ¡��԰                       *\n");
    printf("*         //                                                                \\\\        \\\\                      *\n");
    printf("*  �ɡ�Ǭ����<=====>�̡����ĵ�                                               \\\\       �͡����͹�              *\n");
    printf("*                     \\\\                                                      \\\\         //                   *\n");
    printf("*                    �ˡ������                                                 ��ʻʼ���                    *\n");
    printf("*                     //                                                            //                        *\n");
    printf("*                   �ǡ��к͵�                                                   �ȡ������                   *\n");
    printf("*                          \\\\                                                   //                            *\n");
    printf("*                                \\\\                                     //                                    *\n");
    printf("*                                       \\\\                           //                                       *\n");
    printf("*                                           \\\\                   //                                           *\n");
    printf("*                                                �ơ�̫����                                                   *\n");
    printf("*                                                  ||                                                         *\n");
    printf("*                                                  ||                                                         *\n");
    printf("*                                                  ||                                                         *\n");
    printf("*                                                  ||                                                         *\n");
    printf("*                                                  �š�����                                                   *\n");
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
    printf("       = =                         ��ӭʹ�ùʹ�����Ժ������ѯϵͳ                            = =\n");
    printf("       = =                                                                                   = =\n");
    printf("       = =                                   ��ѡ����:                                     = =\n");
    printf("       = =                                                                                   = =\n");
    printf("       = =             1.�鿴�ʹ���ͼ                      5.����·���߻�                    = =\n");
    printf("       = =                                                                                   = =\n");
    printf("       = =             2.�鿴������Ϣ                      6.�ҶԹʹ��л�˵                  = =\n");
    printf("       = =                                                                                   = =\n");
    printf("       = =             3.Ѱ��������֮������·��          7.�鿴����                        = =\n");
    printf("       = =                                                                                   = =\n");
    printf("       = =             4.Ѱ��������֮������·��            8.�˳�ϵͳ                        = =\n");
    printf("       = =                                                                                   = =\n");
    printf("       = =                                                                                   = =\n");
    printf("       = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =\n");
    printf("       = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =\n");
    printf("\n\n");
    printf("             ������������ѡ�������");
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
    printf("                                               �ڴ�������һ��ʹ�ã�����\n");
    exit(0);
}

int main()
{
    //system("color f5"); //�ı���ɫ
    //put_map();
    int quit = 0;
    Graph g;
    get_graph(&g);  //���ļ���ȡ��Ϣ�����ʹ���ͼ
    short_path(&g); //ͨ��Floyd���distances����path��
    while (!quit)
    {
        //ϵͳ�˳����������ж�
        switch (put_menu())
        {
        //��ӡ���˵�
        case 1:
            system("cls");
            put_map();
            break; //�ʹ�����ƽ��ͼ��ѯ
        case 2:
            system("cls");
            search(&g);
            break; //�ʹ�������Ϣ��ѯ
        case 3:
            system("cls");
            put_map();
            print_short_path(&g);
            break; //����������·����ѯ
        case 4:
            system("cls");
            put_map();
            print_all_path(&g);
            break; //�����������·�߲�ѯ
        case 5:
            system("cls");
            put_map();
            best_path(&g);
            break; //�ྰ����������·�߲�ѯ
        case 6:
            system("cls");
            comment();
            break; //�Թʹ�������
        case 7:
            system("cls");
            view_comment();
            break; //�鿴����
        case 8:
            system("cls");
            exit_system();
            break; //�˳��ʹ�������ѯϵͳ
        default:
            printf("             �Բ���û�и�ѡ���Ӧ�Ĳ�����\n\n");
        }
        system("pause");
        system("cls");
        /*����*/
    }
    return 0;
}
