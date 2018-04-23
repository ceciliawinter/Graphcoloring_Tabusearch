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
int vconflict[510]; //0�ű�ʾ������ʣ��Ϊ��ͻ���
int vconflict2[510][3]; //��һλΪ�Ľ����conflict�е�λ�ã��ڶ�λΪ������ɫ������λΪ���ٴθı�Ĳ���
int conflictcolour[510][510];   //���*��ɫ
int colourtable[MAX_VERTEX_NUM];
//int arcnum[MAX_VERTEX_NUM];

typedef int status;
int times=0;


typedef struct VNode {  //ͷ���
	int date;   //������Ϣ
	int n;
	int colour;
	int arc[MAX_VERTEX_NUM];
}VNode, AdjList[MAX_VERTEX_NUM];

typedef struct { //�ڽӱ�
	AdjList vertices;
	int vexnum, arcnum; //ͼ�ĵ�ǰ�������ͻ���
}ALGraph;

int main(void) {
	//ALGraph *graph = (ALGraph *)malloc(sizeof(ALGraph));
	int vexnum, arcnum;
	AdjList vertices;
	char name[30];
	int colour, i, step = 0, retu;
	times=0;
	printf("�������ļ����ƣ� ");
	scanf("%s", name);
	getchar();

	printf("��������ɫ������ ");

	scanf("%d", &colour);
	getchar();
	caculate(&vexnum, &arcnum, vertices, colour,name);
	/*printf("color*");
	Colouring(graph, colour);
	vconflicttable(graph);
	printf("\n%d", vconflict[0]);
	for (i = 1; i <= graph->vexnum; i++) {
		printf("*%d %d ", i, graph->vertices[i].colour);
	}
	step = 0;
	//printf("coloring:");
	int num = 1;
	retu = changecolour(&step, graph, colour, num);

	if (retu = 0) printf("FALL\n\n");
	printf("STEP: %d\n", step);
	printf("\n%d", vconflict[0]);
	for (i = 1; i <= graph->vexnum; i++) {
		printf("*%d %d ", i, graph->vertices[i].colour);
	}
*/
	return 0;
}

status Read(int* vexnum, int*  arcnum,  AdjList vertices, char FileName[]) {  //�ļ���ȡ
	FILE *in;
	int i = 0, flag, n, m;
	char ch;

	if ((in = fopen(FileName, "r")) == NULL) {
		printf("\n    ���ļ������ڣ�\n");
		return ERROR;
	}
	//printf("*");
	fscanf(in, "%d %d\n", &(*vexnum), &(*arcnum));
	//  printf("%d %d", graph->arcnum, graph->vexnum);
	for (i = 1; i <= (*vexnum); i++) {
		// graph->vertices[i].date=i;
		(vertices[i]).n = 0;
		//vertices[i].colour = 0;
	}
	// printf("+");
	for (i = 0; i<(*arcnum); i++) {
		fscanf(in, "%c %d %d\n", &ch, &m, &n);
		(vertices[m].n)++;
		(vertices[n].n)++;
		//  printf("ook ");
		vertices[m].arc[vertices[m].n - 1] = n;
		//printf("+");
		//graph->vertices[m].arc[graph->vertices[m].n - 1][1] = 0;
		//  printf("1 ");
		vertices[n].arc[vertices[n].n - 1] = m;
		//graph->vertices[n].arc[graph->vertices[n].n - 1][1] = 0;
	}
	fclose(in);
	return OK;
}

void Colouring(int* vexnum, int*  arcnum,  AdjList vertices, int colour) {//colour����ɫ

	int i = 0, j = 0, k = 0;
	//    ArcNode *p=NULL;
	srand((unsigned int)time(0));
	for (i = 1; i <= (*vexnum); i++) {

		//vertices[i].colour = rand() % colour + 1;
		colourtable[i]=rand() % colour + 1;
	}//���е���ɫ
	 /*	for (i = 1; i <= graph->vexnum; i++) {
	 for (j = 0; j<graph->vertices[i].n; j++) {
	 k = graph->vertices[i].arc[j][0];
	 graph->vertices[i].arc[j][1] = graph->vertices[j].colour;
	 }
	 }*/
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
	}//��ʼ�����ɱ����н�㶼���ڽ��ɱ���
	for (i = 1; i <= (*vexnum); i++) {
		for (j = 0; j <= MAX_VERTEX_NUM; j++)
			conflictcolour[i][j] = 0;//��ʼ���õ��Ӧ��ͻ��
									 //flag=0;
		p = vertices[i];
		for (j = 0; j<p.n; j++) {
            qnum=p.arc[j];
			pcolour = colourtable[qnum];
			//pnum=p.arc[j][0];
			++conflictcolour[i][pcolour];//��ͻ��ͳ����ɫ
		}
		col =colourtable[i];
		if (conflictcolour[i][col] != 0) {
			vconflict[0]++;
			confnum = vconflict[0];
			vconflict[confnum] = i;
			vconflict2[i][0] = confnum;//���ɱ���λ��
			vconflict2[i][1] = col;//������ɫ
			vconflict2[i][2]=0;
		}
	}//���е�
}
//caculate(vexnum, arcnum, vertices, colour,name);

int changecolour(int *step, int *vexnum, int * arcnum,  AdjList vertices, int colour) {
	int k = 0;
	int oldconf = 0, newconf = 0;
		int best = 0, pbest = 0;//best��ʾ��ͻ�������pbest��ʾԭ����ͻ�ڵ���
		int vnum = 0, i = 0, col = 0, num = 0;
		int bestchoice[MAX_VERTEX_NUM];//��Ź�ѡ��ĳ�ͻ���
		int n = 0, j = 0;//n��ʾ�ý�����,k�����Ƿ�����ֲ����Ž�
		int conf;//�ɹ�ѡ��Ľ�����
		int newcolnum = 0;	//��ѡ�����ɫ�����ѡ��������λ��
		int newcol;//��ѡ�����ɫ
				   //int conflict, confnum, location, conflict2, flag = 0;
		int confnum;//��ͻ���н�����
		int location;//��ͻ����ڳ�ͻ���е�λ��
		int conflictnum = vconflict[0];
		int pvconf0, vconf0;
		int colconf=0;
		int lastnum=0;//��ͻ�������һλ��Ҫ�ƶ��Ľ��
	while (vconflict[0] > 0 && (*step) < maxstep) {
		(*step)++;
        conflictnum = vconflict[0];
		num = rand() % conflictnum + 1;	//ѡȡҪ�ı�Ľ���ڳ�ͻ���е�λ��
		vnum = vconflict[num];	//Ҫ�ı�Ľ��

    //    printf("\n*%d %d ", vconflict[0], vnum);

		VNode p = vertices[vnum];
		col = colourtable[vnum];//ԭ����ɫ
				pbest = 0;	//ԭ����ɫ�ĳ�ͻ������
        for (j = 0; j < n; j++) {
				int qnum = vertices[vnum].arc[j];
				oldconf = 0;
				oldconf = conflictcolour[qnum][col];
				int qcol = colourtable[qnum];
				if (oldconf ==1 && qcol == col) {//ԭ����ͻ���ڲ���ͻ
                    pbest++;	//ԭ����ɫ�ĳ�ͻ������
				}
			}

		best = pbest;
		clear(bestchoice);
		bestchoice[0]=0;
		i=(col+1)%colour;
		colconf=0;
        n = p.n;
        newcol=i;
        if (conflictcolour[vnum][i] == 0){//ԭ����ͻ���ڲ���ͻ
		//	        printf("\n   *QAQ   "   );
            colconf--;
        }
        for (j = 0; j < n; j++) {
            int qnum = p.arc[j];
            oldconf = 0, newconf = 0;
            newconf =conflictcolour[qnum][newcol];
            oldconf = conflictcolour[qnum][col];
            int qcol = colourtable[qnum];
            if (newconf == 0 && qcol == newcol) {//ԭ������ͻ���ڳ�ͻ
                //	    printf("   *QAQ+   "   );
                colconf++;
            }
            if (oldconf == 1 && qcol == col) {//ԭ����ͻ���ڲ���ͻ
                //	    printf("   *QAQ-   "   );
                colconf--;
            }
        }
                //printf("p %d n %d",pbest, colconf);
				best = colconf;
				clear(&bestchoice);
				bestchoice[0] = 1;
				bestchoice[1] = i;



		for (i = 1; i <= colour; i++) {
            if(i!=col){

                colconf = pbest;
                n = p.n;
                newcol=i;
                if (conflictcolour[vnum][i] == 0){//ԭ����ͻ���ڲ���ͻ
		//	        printf("\n   *QAQ   "   );
                    colconf--;
                }
                for (j = 0; j < n; j++) {
                    int qnum = p.arc[j];
                    oldconf = 0, newconf = 0;
                    newconf =conflictcolour[qnum][newcol];
                    oldconf = conflictcolour[qnum][col];
                    int qcol = colourtable[qnum];
                    if (newconf == 0 && qcol == newcol) {//ԭ������ͻ���ڳ�ͻ
                //	    printf("   *QAQ+   "   );
                        colconf++;
                    }
                    if (oldconf == 1 && qcol == col) {//ԭ����ͻ���ڲ���ͻ
                //	    printf("   *QAQ-   "   );
                        colconf--;
                    }
                }
                //printf("p %d n %d",pbest, colconf);
            //else colconf=pbest;
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
	//	printf("num: %d %d %d %d ", bestchoice[0], best-pbest, best, pbest);
	//	if (best <= pbest) {//�Ľ���Ч
			//pvconf0 = vconflict[0];
		//	printf("ok1");
			while (newcolnum == 0) {
				newcolnum = rand() % bestchoice[0] + 1;	//ѡȡҪ�ı�Ľ���λ��
				newcol = bestchoice[newcolnum];//����ɫ
				if (newcol == vconflict2[vnum][1] && vconflict2[vnum][2] > *step) {
					//��ѡ�������
					//if (bestchoice[0] == 1) {//���ɽ��Ϊ����ѡ��
					//	break;//���
					//}
					if (bestchoice[0] != 1) {
						newcolnum = 0;//���½���ѭ��ѡ����ɫ
					}
				}
                    if(bestchoice[0]!=1&&newcol==col){
                        newcolnum = 0;
                    }
		//		printf("ok2");
			}
		//	p=graph->vertices[vnum];
		//	p.colour=newcol;
		//	printf("+%d+", graph->vertices[vnum].colour);
			colourtable[vnum]=newcol;
			//			p=graph->vertices[vnum];

		//	printf("%d*", p.colour);
    //    printf("col: %d new :%d %d", col, newcol, graph->vertices[vnum].colour);
			vconflict2[vnum][2] = (*step) + 10;
			vconflict2[vnum][1] = newcol;
			if (conflictcolour[vnum][newcol] == 0) {//���ٳ�ͻ
                    confnum = vconflict[0];
					location = vconflict2[vnum][0];
					vconflict2[vnum][0] = -1;
					vconflict[location] = vconflict[confnum];//���һ�����ǰ�ƣ�ԭ�������һ�����Ҳ������
					lastnum=vconflict[location];
					if(lastnum!=vnum)
                        vconflict2[lastnum][0]=location;
					vconflict[0]--;


		//		printf("   QAQ %d  "  ,location );
			}
			//p = graph->vertices[num];
      //      printf("ok3");
			n = p.n;
			colourtable[vnum] = newcol;
			for (j = 0; j < n; j++) {
     //               printf("ok4");
				int qnum = p.arc[j];
				oldconf = 0, newconf = 0;
				newconf = conflictcolour[qnum][newcol];
				conflictcolour[qnum][newcol]++;
				oldconf = conflictcolour[qnum][col];
				conflictcolour[qnum][col]--;
				int qcol = colourtable[qnum];
       //     printf("ok5");
				if (newconf == 0 && qcol == newcol) {//ԭ������ͻ���ڳ�ͻ
					vconflict[0]++;
					confnum = vconflict[0];
					vconflict[confnum] = qnum;
					vconflict2[qnum][0] = confnum;
		//			printf("   QAQ+   %d" ,location  );
				}
				if (oldconf == 1 && qcol == col) {//ԭ����ͻ���ڲ���ͻ
					confnum = vconflict[0];
					location = vconflict2[qnum][0];
					vconflict2[qnum][0] = -1;
					vconflict[location] = vconflict[confnum];//���һ�����ǰ�ƣ�ԭ�������һ�����Ҳ������
                    lastnum=vconflict[location];
                    if(lastnum!=qnum)
                        vconflict2[lastnum][0]=location;
					vconflict[0]--;
		//			printf("   QAQ-  %d " ,location  );
				}
//printf("ok6");
			}

		//}//useful*/
		//printf("bestchoice: %d %d %d %d*", bestchoice[0], pbest, best, newcol);
	/*	for(j=1; j<=bestchoice[0]; j++){
            printf("%d ",bestchoice[j]);
		}*/
		vconf0 = vconflict[0];
		if (vconf0 == pvconf0) {
			k++;
	//		printf("k: %d", k);
			if (k >= (vconflict[0]*jump) ){//����ֲ����Ž���޷�������Ž�
							//printf(" + ");
				vnum = rand() % (*vexnum) + 1;
//				printf("\n%d\n", vnum);
				p = vertices[vnum];
				col = colourtable[vnum];//ԭ����ɫ
				newcol = rand() % colour + 1;
				n = p.n;
				p.colour = newcol;
				colourtable[vnum]=newcol;
				vconflict2[vnum][2] = (*step) + 10;
				vconflict2[vnum][1] = newcol;
				if (conflictcolour[vnum][newcol] != 0 && conflictcolour[vnum][col] == 0) {//�޸ĵĽ��ԭ������ͻ���ڳ�ͻ
					confnum = ++vconflict[0];
					//location = vconflict2[vnum][0];
					vconflict2[vnum][0] = confnum;
					vconflict[confnum] = vnum;//���һ�����ǰ�ƣ�ԭ�������һ�����Ҳ������
				}
				for (j = 0; j < n; j++) {
					int qnum = p.arc[j];
					int oldconf = 0, newconf = 0;
					newconf = ++conflictcolour[qnum][newcol];
					oldconf = --conflictcolour[qnum][col];
					int qcol = colourtable[qnum];
					if (newconf == 1 && qcol == newcol) {//ԭ������ͻ���ڳ�ͻ
						vconflict[0]++;
						confnum = vconflict[0];
						vconflict[confnum] = qnum;
						vconflict2[qnum][0] = confnum;
					}
					if (oldconf == 0 && qcol == col) {//ԭ����ͻ���ڲ���ͻ
						confnum = vconflict[0];
						location = vconflict2[qnum][0];
						vconflict2[qnum][0] = 0;
						vconflict[location] = vconflict[confnum];//���һ�����ǰ�ƣ�ԭ�������һ�����Ҳ������
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




//caculate(vexnum, arcnum, vertices, colour,name);
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
        //times++;
        //if(times>10){
            printf("FALL\n");
            printf("STEP: %d\n", step);
            printf( "%d", vconflict[0]);
            for(i=1; i<=vconflict[0];i++){
                printf("+%d+", vconflict[i]);
            }

            		for (i = 1; i <= (*vexnum); i++) {
			printf("*%d %d ", i, colourtable[i]);
		}
            return;
       // }
        //caculate(graph, colour, name);

	}
	else {
		printf("\nSUCCESS\n");
		printf("STEP: %d\n", step);
		for (i = 1; i <= (*vexnum); i++) {
			printf("*%d %d ", i, colourtable[i]);
		}
		caculate(vexnum, arcnum, vertices, colour - 1, name);
	}



}



