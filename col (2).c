#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define MAX_VERTEX_NUM 510
#define maxstep 100000000
#define jump 10
int vconflict[510]; //0号表示个数，剩余为冲突结点
int vconflict2[510][3]; //第一位为改结点在conflict中的位置，第二位为禁忌颜色，第三位为可再次改变的步数
int conflictcolour[510][510];   //结点*颜色
int colourtable[MAX_VERTEX_NUM];
//int arcnum[MAX_VERTEX_NUM];

typedef int status;
int times=0;


typedef struct VNode {  //头结点
	int date;   //顶点信息
	int n;
	int colour;
	int arc[MAX_VERTEX_NUM];
}VNode, AdjList[MAX_VERTEX_NUM];

typedef struct { //邻接表
	AdjList vertices;
	int vexnum, arcnum; //图的当前定点数和弧数
}ALGraph;

int main(void) {
	//ALGraph *graph = (ALGraph *)malloc(sizeof(ALGraph));
	int vexnum, arcnum;
	AdjList vertices;
	char name[30];
	int colour, i, step = 0, retu;
	times=0;
	printf("请输入文件名称： ");
	scanf("%s", name);
	getchar();

	printf("请输入颜色数量： ");

	scanf("%d", &colour);
	getchar();
	caculate(&vexnum, &arcnum, vertices, colour,name);
	return 0;
}

status Read(int* vexnum, int*  arcnum,  AdjList vertices, char FileName[]) {  //文件读取
	FILE *in;
	int i = 0, flag, n, m;
	char ch;

	if ((in = fopen(FileName, "r")) == NULL) {
		printf("\n    该文件不存在！\n");
		return ERROR;
	}
	fscanf(in, "%d %d\n", &(*vexnum), &(*arcnum));
	for (i = 1; i <= (*vexnum); i++) {
		(vertices[i]).n = 0;
	}
	// printf("+");
	for (i = 0; i<(*arcnum); i++) {
		fscanf(in, "%c %d %d\n", &ch, &m, &n);
		(vertices[m].n)++;
		(vertices[n].n)++;
		vertices[m].arc[vertices[m].n - 1] = n;
		vertices[n].arc[vertices[n].n - 1] = m;
	}
	fclose(in);
	return OK;
}

void Colouring(int* vexnum, int*  arcnum,  AdjList vertices, int colour) {//colour种颜色

	int i = 0, j = 0, k = 0;
	//    ArcNode *p=NULL;
	srand((unsigned int)time(0));
	for (i = 1; i <= (*vexnum); i++) {

		//vertices[i].colour = rand() % colour + 1;
		colourtable[i]=rand() % colour + 1;
	}//所有点着色
	return;
}

void vconflicttable(int*vexnum, int*  arcnum,  AdjList vertices) {
	vconflict[0] = 0;
	VNode p;
	int i = 0, pcolour = 0, j = 0, confnum = 0, pnum = 0, flag = 0, col;
	int qnum;
	for (i = 0; i<MAX_VERTEX_NUM; i++) {
		vconflict2[i][0] = 0;
		vconflict2[i][2] = 0;
		vconflict[i]=0;
	}//初始化禁忌表，所有结点都不在禁忌表中
	for (i = 1; i <= (*vexnum); i++) {
		for (j = 0; j <= MAX_VERTEX_NUM; j++)
			conflictcolour[i][j] = 0;//初始化该点对应冲突表
									 //flag=0;
		p = vertices[i];
		for (j = 0; j<p.n; j++) {
            qnum=p.arc[j];
			pcolour = colourtable[qnum];
			//pnum=p.arc[j][0];
			++conflictcolour[i][pcolour];//冲突表统计颜色
		}
		col =colourtable[i];
		if (conflictcolour[i][col] != 0) {
			vconflict[0]++;
			confnum = vconflict[0];
			vconflict[confnum] = i;
			vconflict2[i][0] = confnum;//禁忌表中位置
			vconflict2[i][1] = col;//禁忌颜色
			vconflict2[i][2]=0;
		}
	}//所有点
}

int changecolour(int *step, int *vexnum, int * arcnum,  AdjList vertices, int colour) {
	int k = 0;
	int oldconf = 0, newconf = 0;
		int best = 0, pbest = 0;//best表示冲突结点数，pbest表示原本冲突节点数
		int vnum = 0, i = 0, col = 0, num = 0;
		int bestchoice[MAX_VERTEX_NUM];//存放供选择的冲突结点
		int n = 0, j = 0;//n表示该结点边数,k标致是否陷入局部最优解
		int conf;//可供选择的结点个数
		int newcolnum = 0;	//新选择的颜色在最佳选择数组中位置
		int newcol;//新选择的颜色
				   //int conflict, confnum, location, conflict2, flag = 0;
		int confnum;//冲突表中结点个数
		int location;//冲突结点在冲突表中的位置
		int conflictnum = vconflict[0];
		int pvconf0, vconf0;
		int colconf=0;
		int lastnum=0;//冲突表中最后一位需要移动的结点
	while (vconflict[0] > 0 && (*step) < maxstep) {
		(*step)++;
        conflictnum = vconflict[0];
		num = rand() % conflictnum + 1;	//选取要改变的结点在冲突表中的位置
		vnum = vconflict[num];	//要改变的结点

    //    printf("\n*%d %d ", vconflict[0], vnum);

		VNode p = vertices[vnum];
		col = colourtable[vnum];//原本颜色
				pbest = 0;	//原本颜色的冲突结点个数
        for (j = 0; j < n; j++) {
				int qnum = vertices[vnum].arc[j];
				oldconf = 0;
				oldconf = conflictcolour[qnum][col];
				int qcol = colourtable[qnum];
				if (oldconf ==1 && qcol == col) {//原本冲突现在不冲突
                    pbest++;	//原本颜色的冲突结点个数
				}
			}
		best = pbest;
		clear(bestchoice);
		bestchoice[0]=0;
		i=(col+1)%colour;
		colconf=0;
        n = p.n;
        newcol=i;
        if (conflictcolour[vnum][i] == 0){//原本冲突现在不冲突
            colconf--;
        }
        for (j = 0; j < n; j++) {
            int qnum = p.arc[j];
            oldconf = 0, newconf = 0;
            newconf =conflictcolour[qnum][newcol];
            oldconf = conflictcolour[qnum][col];
            int qcol = colourtable[qnum];
            if (newconf == 0 && qcol == newcol) {//原本不冲突现在冲突
                colconf++;
            }
            if (oldconf == 1 && qcol == col) {//原本冲突现在不冲突
                colconf--;
            }
        }
	best = colconf;
	clear(&bestchoice);
	bestchoice[0] = 1;
	bestchoice[1] = i;
	for (i = 1; i <= colour; i++) {
            if(i!=col){

                colconf = pbest;
                n = p.n;
                newcol=i;
                if (conflictcolour[vnum][i] == 0){//原本冲突现在不冲突
                    colconf--;
                }
                for (j = 0; j < n; j++) {
                    int qnum = p.arc[j];
                    oldconf = 0, newconf = 0;
                    newconf =conflictcolour[qnum][newcol];
                    oldconf = conflictcolour[qnum][col];
                    int qcol = colourtable[qnum];
                    if (newconf == 0 && qcol == newcol) {//原本不冲突现在冲突
                        colconf++;
                    }
                    if (oldconf == 1 && qcol == col) {//原本冲突现在不冲突
                        colconf--;
                    }
                }
                if (colconf == best) {
                    bestchoice[0]++;
                    conf = bestchoice[0];
                    bestchoice[conf] = i;
                }
                else if (colconf < best) {
                    best = colconf;
                    clear(&bestchoice);
                    bestchoice[0] = 1;
                    bestchoice[1] = i;
                }
            }

		}

		newcolnum = 0;
		pvconf0 = vconflict[0];
			while (newcolnum == 0) {
				newcolnum = rand() % bestchoice[0] + 1;	//选取要改变的结点的位置
				newcol = bestchoice[newcolnum];//新颜色
				if (newcol == vconflict2[vnum][1] && vconflict2[vnum][2] > *step) {
					//新选择结点禁忌
					//if (bestchoice[0] == 1) {//禁忌结点为最优选择
					//	break;//解禁
					//}
					if (bestchoice[0] != 1) {
						newcolnum = 0;//重新进入循环选择颜色
					}
				}
                    if(bestchoice[0]!=1&&newcol==col){
                        newcolnum = 0;
                    }
			}
			colourtable[vnum]=newcol;
    //    printf("col: %d new :%d %d", col, newcol, graph->vertices[vnum].colour);
			vconflict2[vnum][2] = (*step) + 10;
			vconflict2[vnum][1] = newcol;
			if (conflictcolour[vnum][newcol] == 0) {//不再冲突
                    confnum = vconflict[0];
					location = vconflict2[vnum][0];
					vconflict2[vnum][0] = -1;
					vconflict[location] = vconflict[confnum];//最后一个结点前移，原本是最后一个结点也不干扰
					lastnum=vconflict[location];
					if(lastnum!=vnum)
                        vconflict2[lastnum][0]=location;
					vconflict[0]--;


			}
			//p = graph->vertices[num];
			n = p.n;
			colourtable[vnum] = newcol;
			for (j = 0; j < n; j++) {
				int qnum = p.arc[j];
				oldconf = 0, newconf = 0;
				newconf = conflictcolour[qnum][newcol];
				conflictcolour[qnum][newcol]++;
				oldconf = conflictcolour[qnum][col];
				conflictcolour[qnum][col]--;
				int qcol = colourtable[qnum];
				if (newconf == 0 && qcol == newcol) {//原本不冲突现在冲突
					vconflict[0]++;
					confnum = vconflict[0];
					vconflict[confnum] = qnum;
					vconflict2[qnum][0] = confnum;
				}
				if (oldconf == 1 && qcol == col) {//原本冲突现在不冲突
					confnum = vconflict[0];
					location = vconflict2[qnum][0];
					vconflict2[qnum][0] = -1;
					vconflict[location] = vconflict[confnum];//最后一个结点前移，原本是最后一个结点也不干扰
                    lastnum=vconflict[location];
                    if(lastnum!=qnum)
                        vconflict2[lastnum][0]=location;
					vconflict[0]--;
				}
			}
		vconf0 = vconflict[0];
		if (vconf0 == pvconf0) {
			k++;
	//		printf("k: %d", k);
			if (k >= (vconflict[0]*jump) ){//陷入局部最优解而无法求出最优解
							//printf(" + ");
				vnum = rand() % (*vexnum) + 1;
//				printf("\n%d\n", vnum);
				p = vertices[vnum];
				col = colourtable[vnum];//原本颜色
				newcol = rand() % colour + 1;
				n = p.n;
				p.colour = newcol;
				colourtable[vnum]=newcol;
				vconflict2[vnum][2] = (*step) + 10;
				vconflict2[vnum][1] = newcol;
				if (conflictcolour[vnum][newcol] != 0 && conflictcolour[vnum][col] == 0) {//修改的结点原本不冲突现在冲突
					confnum = ++vconflict[0];
					//location = vconflict2[vnum][0];
					vconflict2[vnum][0] = confnum;
					vconflict[confnum] = vnum;//最后一个结点前移，原本是最后一个结点也不干扰
				}
				for (j = 0; j < n; j++) {
					int qnum = p.arc[j];
					int oldconf = 0, newconf = 0;
					newconf = ++conflictcolour[qnum][newcol];
					oldconf = --conflictcolour[qnum][col];
					int qcol = colourtable[qnum];
					if (newconf == 1 && qcol == newcol) {//原本不冲突现在冲突
						vconflict[0]++;
						confnum = vconflict[0];
						vconflict[confnum] = qnum;
						vconflict2[qnum][0] = confnum;
					}
					if (oldconf == 0 && qcol == col) {//原本冲突现在不冲突
						confnum = vconflict[0];
						location = vconflict2[qnum][0];
						vconflict2[qnum][0] = 0;
						vconflict[location] = vconflict[confnum];//最后一个结点前移，原本是最后一个结点也不干扰
						vconflict[0]--;
					}
				}
				k = 0;
			}

		}
		else if (pvconf0<vconf0) {
			k = 0;
		}

	}
	if (vconflict[0] == 0 && (*step) <= maxstep) {
		return 1;
	}
	return 0;
}


void clear(int bestchoice[]) {
	int i = 0;
	for (i = 0; i < MAX_VERTEX_NUM; i++) {
		bestchoice[i] = 0;
	}
}
void caculate(int* vexnum, int* arcnum,  AdjList vertices, int colour, char name[]) {
	int i = 0;
	Read(vexnum, arcnum, vertices, name);
	printf("\n\nColour: %d:", colour);
	Colouring(vexnum, arcnum, vertices, colour);
	printf("*");
	vconflicttable(vexnum, arcnum, vertices);
	printf("\n%d", vconflict[0]);
	for (i = 1; i <= (*vexnum); i++) {
		printf("*%d %d ", i, colourtable[i]);
	}
	int step = 0;
	//printf("coloring:");
	int num = 1, retu = 0;
	retu = changecolour(&step, vexnum, arcnum, vertices, colour);
	if (retu == 0) {
            printf("FALL\n");
            printf("STEP: %d\n", step);
            printf( "%d", vconflict[0]);
            for(i=1; i<=vconflict[0];i++){
                printf("+%d+", vconflict[i]);
            }

            		for (i = 1; i <= (*vexnum); i++) {
			printf("%d ", colourtable[i]);
		}
            return;

	}
	else {
		printf("\nSUCCESS\n");
		printf("STEP: %d\n", step);
		for (i = 1; i <= (*vexnum); i++) {
			printf("%d ", colourtable[i]);
		}
		caculate(vexnum, arcnum, vertices, colour - 1, name);
	}
}



