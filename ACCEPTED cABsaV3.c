//CABSAv3


#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>

//ROUND
#define SKIP 0
#define SOL 1
#define PAR 2
#define TRI 3
#define FIV 5
#define STR 4
#define FLS 5
#define FLH 6
#define BOM 7
#define STF 8
#define RLF 9
#define DRG 10

#define COMPLETE 13

int deck[52];
int playerWithTigaTahu=-1;
int existInFull=0;
int existInTris=0;
int existInPair=0;
int existInBomb=0;
int existInHand=0;

int roundStarter=1;
int roundNow=0;
int lastId = 0;
int isPlayerPlaying[4];
struct SMALL_RECT {
    SHORT Left;
    SHORT Top;
    SHORT Right;
    SHORT Bottom;
};
void adjustWindowSize()
{
	HWND hWnd;
	SetConsoleTitle("cABsa - NAR21-2");
	hWnd = FindWindow(NULL, "cABsa - NAR21-2");
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD NewSBSize = GetLargestConsoleWindowSize(hOut);
	SMALL_RECT DisplayArea = {0, 0, 0, 0};
	
	SetConsoleScreenBufferSize(hOut, NewSBSize);
	DisplayArea.Right = NewSBSize.X - 1;
	DisplayArea.Bottom = NewSBSize.Y - 1;
	SetConsoleWindowInfo(hOut, TRUE, &DisplayArea);
	ShowWindow(hWnd, SW_MAXIMIZE);
	return;
}
struct Last{
	int weight;
	int cards[5];
	int type;
}last;
struct Played{
	int id;
	struct Played *next;
}*head,*tail;
struct Combo{
	int id;
	int code;
	int** cards; //arr of pointer
	struct Combo *left, *right;
};
struct Player{
	int cards[13];
	struct Combo* pair;
	struct Combo* tris;
	struct Combo* bom;
	struct Combo* full;
}players[4];
int checkWin(int arr[]){
	int i,c=0;
	for(i=0;i<13;i++) if(arr[i]==0) c++;
	if(c==13) return 1;
	else return 0;
}
int checkRemaining(int i){
	int j,cnt=0;
	for(j=0;j<13;j++)
		if(players[i].cards[j]!=0) cnt++;
	return cnt;
}
void clearLast(){
	int i;
	for(i=0;i<5;i++)
		last.cards[i]=0;
	last.weight=0;
	last.type=0;
}
void printLast(){
	printf("\n\t\t\t\tLAST CARD PLAYED : \n\t\t\t\t");
	int i;
	if(last.type==SOL){
		printf(" ------\n\t\t\t\t|");
		int x = last.weight;
		if(x!=0){	
			char sym;
			int y=x%4;
			int z = x/4;
			if(y==1){
				z++;
			} else if (y==2){
				z++;
			}else if (y==3){
				z++;
			}else if (y==0){
			}
			z+=2;
			if(z==10){
				sym='X';
			} else if(z==11){
				sym='J';
			} else if(z==12){
				sym='Q';
			} else if(z==13){
				sym='K';
			} else if(z==14){
				sym='A';
			}  else if(z==15){
				sym='2';
			} else{
				sym=z+48;
			}
			if(i==0)
					printf("   %c |",sym);
			else printf("    %c |",sym);
		}
		printf("\n\t\t\t\t|");
		if(x!=0){
			char sym;
			int y=x%4;
			if(y==1){
				sym='\4';
			} else if (y==2){
				sym='\5';
			}else if (y==3){
				sym='\3';
			}else if (y==0){
				sym='\6';
			}
			if(i==0)
					printf("   %c |",sym);
			else printf("    %c |",sym);
		}
		printf("\n\t\t\t\t ------\n");
		return;
	}
	for(i=0;i<5;i++)
		if(last.cards[i]!=0)
			printf(" ------");
	
	if(last.cards[0]!=0)
		printf("\n\t\t\t\t|");
	for(i=0;i<5;i++){
		int x = last.cards[i];
		if(x!=0){		
			char sym;
			int y=x%4;
			int z = x/4;
			if(y==1){
				z++;
			} else if (y==2){
				z++;
			}else if (y==3){
				z++;
			}else if (y==0){
			}
			z+=2;
			if(z==10){
				sym='X';
			} else if(z==11){
				sym='J';
			} else if(z==12){
				sym='Q';
			} else if(z==13){
				sym='K';
			} else if(z==14){
				sym='A';
			}  else if(z==15){
				sym='2';
			} else{
				sym=z+48;
			}
			if(i==0)
					printf("   %c |",sym);
			else printf("    %c |",sym);
		}
	}
	if(last.cards[0]!=0)
		printf("\n\t\t\t\t|");
	for(i=0;i<5;i++){
		int x = last.cards[i];
		if(x!=0){
			char sym;
			int y=x%4;
			if(y==1){
				sym='\4';
			} else if (y==2){
				sym='\5';
			}else if (y==3){
				sym='\3';
			}else if (y==0){
				sym='\6';
			}
			if(i==0)
					printf("   %c |",sym);
			else printf("    %c |",sym);
		}
	}
	
	printf("\n\t\t\t\t");
	for(i=0;i<5;i++)
		if(last.cards[i]!=0)
			printf(" ------");
	printf("\n");
}
void putLast(struct Combo* c, int w, int l,int t){
	int i;
	if(c!=NULL){
		for(i=0;i<l;i++){
			last.cards[i]=*(c->cards[i]);
		}
	}
	last.weight=w;
	last.type = t;
}
void resetStat(){
	int i;
	for(i=0;i<4;i++){
		isPlayerPlaying[i]=1;
	}
}
struct Played* newPlayed(int id){
	struct Played* cc = malloc(sizeof *cc);
	cc->id=id;
	cc->next=NULL;
	return cc;
}
void addPlayed(int id){
	struct Played* np = newPlayed(id);
	if(head==NULL){
		head=tail=np;
	}else{
		tail->next=np;
		tail=np;
	}
}
void printPlayed(){
	struct Played* p = head;
	while(p!=NULL){
		printf("-%d-\n",p->id);
		if(p->next==NULL) break;
		p=p->next;
	}
}
void removePlayed(){
	while(head!=NULL){
		struct Played* t=head;
			head=t->next;
			free(t);
			t=NULL;
		if(t->next!=NULL){
		}
	}
}
int findMinInPlayed(){
	int min=999;
	struct Played* p = head;
	while(p!=NULL){
		if(p->id<min&&p->id>lastId) min = p->id;
		if(p->next==NULL)break;
		p=p->next;
	}
	removePlayed();
	if(min==999) return -1;
	return min;
}
struct Combo* minValue(struct Combo* node){
    struct Combo* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}
struct Combo* deleteAll(struct Combo* c){
	if(c!=NULL){
		c->left = deleteAll(c->left);
		c->right = deleteAll(c->right);
		free(c);
	}
	return NULL;
}
struct Combo* createCombo(int code,int* cards[],int len){
	struct Combo* c = malloc(sizeof(struct Combo));
	c->code=code;
	c->left=c->right=NULL;
	int i;
	int x=0;
	c->cards=malloc(len* sizeof(int**));
	for(i=0;i<len;i++){
		c->cards[i]=cards[i];
		x+=*cards[i];
	}
	c->id=x+code;
	return c;
}
struct Combo* addCombo(struct Combo* c, int code, int* cards[],int len){
	if(c == NULL) return createCombo(code,cards,len);
	int cc = 0;
	int i;
	for(i=0;i<len;i++)
		cc+=**(cards+i);
	cc+=code;
	int ccds = (c->id);
	if(cc<ccds) c->left = addCombo(c->left,code,cards,len);
	if(cc>ccds) c->right = addCombo(c->right,code,cards,len);
	return c;
}
int findNum(int arr[],int key,int len){
	int i;
	for(i=0;i<len;i++)
		if(arr[i]==key) {
			return i;
		}
	return -1;
}
int findNumBetween(int arr[],int max,int min,int len){
	int i;
	for(i=0;i<len;i++)
		if(arr[i]>=min&&arr[i]<=max) 
			return i;
	return -1;
}
void findPairs(struct Player* p){
	int i,idx=0;
	int arrTemp[2];
	for(i=1;i<53;i+=4){
		int x =0;
		int j;
		int cnt=0;
		int* arrTemp[2];
		int idxI = findNum(p->cards,i,COMPLETE);
		int idxII = findNum(p->cards,i+1,COMPLETE);
		int idxIII = findNum(p->cards,i+2,COMPLETE);
		int idxIV = findNum(p->cards,i+3,COMPLETE);
		if(idxI!=-1&&idxII!=-1){
			arrTemp[0]=&p->cards[idxI];
			arrTemp[1]=&p->cards[idxII];
			p->pair=addCombo(p->pair,2,arrTemp,2);
		}
		if(idxI!=-1&&idxIII!=-1){
			arrTemp[0]=&p->cards[idxI];
			arrTemp[1]=&p->cards[idxIII];
			p->pair=addCombo(p->pair,2,arrTemp,2);
		}
		if(idxI!=-1&&idxIV!=-1){
			arrTemp[0]=&p->cards[idxI];
			arrTemp[1]=&p->cards[idxIV];
			p->pair=addCombo(p->pair,2,arrTemp,2);
		}
		if(idxIII!=-1&&idxII!=-1){
			arrTemp[0]=&p->cards[idxIII];
			arrTemp[1]=&p->cards[idxII];
			p->pair=addCombo(p->pair,2,arrTemp,2);
		}
		if(idxIV!=-1&&idxII!=-1){
			arrTemp[0]=&p->cards[idxIV];
			arrTemp[1]=&p->cards[idxII];
			p->pair=addCombo(p->pair,2,arrTemp,2);
		}
		if(idxIV!=-1&&idxIII!=-1){
			arrTemp[0]=&p->cards[idxIV];
			arrTemp[1]=&p->cards[idxIII];
			p->pair=addCombo(p->pair,2,arrTemp,2);
		}
	}
}
void findTris(struct Player* p){
int i,idx=0;
	int arrTemp[2];
	for(i=1;i<53;i+=4){
		int x =0;
		int j;
		int cnt=0;
		int* arrTemp[3];
		int idxI = findNum(p->cards,i,COMPLETE);
		int idxII = findNum(p->cards,i+1,COMPLETE);
		int idxIII = findNum(p->cards,i+2,COMPLETE);
		int idxIV = findNum(p->cards,i+3,COMPLETE);
		if(idxI!=-1&&idxII!=-1&&idxIII!=-1){
//			printf("asd");
			arrTemp[0]=&p->cards[idxI];
			arrTemp[1]=&p->cards[idxII];
			arrTemp[2]=&p->cards[idxIII];
			p->tris=addCombo(p->tris,3,arrTemp,3);
		}
		if(idxI!=-1&&idxII!=-1&&idxIV!=-1){
//			printf("asd2");
			arrTemp[0]=&p->cards[idxI];
			arrTemp[1]=&p->cards[idxII];
			arrTemp[2]=&p->cards[idxIV];
			p->tris=addCombo(p->tris,3,arrTemp,3);
		}
		if(idxIV!=-1&&idxII!=-1&&idxIII!=-1){
//			printf("asd3");
			arrTemp[0]=&p->cards[idxIV];
			arrTemp[1]=&p->cards[idxII];
			arrTemp[2]=&p->cards[idxIII];
			p->tris=addCombo(p->tris,3,arrTemp,3);
		}
		if(idxIV!=-1&&idxI!=-1&&idxIII!=-1){
//			printf("asd4");
			arrTemp[0]=&p->cards[idxI];
			arrTemp[1]=&p->cards[idxIV];
			arrTemp[2]=&p->cards[idxIII];
			p->tris=addCombo(p->tris,3,arrTemp,3);
		}
	}
}
void findBOM(struct Player* p){
int i,idx=0;
	int arrTemp[2];
	for(i=1;i<53;i+=4){
		int x =0;
		int j;
		int cnt=0;
		int* arrTemp[4];
		int idxI = findNum(p->cards,i,COMPLETE);
		int idxII = findNum(p->cards,i+1,COMPLETE);
		int idxIII = findNum(p->cards,i+2,COMPLETE);
		int idxIV = findNum(p->cards,i+3,COMPLETE);
		if(idxI!=-1&&idxII!=-1&&idxIII!=-1&&idxIV!=-1){
			arrTemp[0]=&p->cards[idxI];
			arrTemp[1]=&p->cards[idxII];
			arrTemp[2]=&p->cards[idxIII];
			arrTemp[3]=&p->cards[idxIV];
			p->bom=addCombo(p->bom,7,arrTemp,4);
		}
	}
}
void findRLF(struct Player* p){
	int i;
	for(i=29;i<33;i++){
		int* tempArr[5];
		int x = findNum(p->cards,i,COMPLETE);
		if(x==-1) continue;
		else{
			int j,cnt=0;
			tempArr[cnt++]=&p->cards[x];
			for(j=i+4;j<53;j+=4){
				int y =findNum(p->cards,j,COMPLETE);
				if(y!=-1) {
					tempArr[cnt++]=&p->cards[y];
				}
			}
			if(cnt==5){ //ada royal flush
				p->full=addCombo(p->full,9,tempArr,5);
			}
		}
	}
}
void findSTF(struct Player* p){
	int i;
	for(i=1;i<37;i++){
		int x =0;
		int j;
		int cnt=0;
		int* arrTemp[5];
		int idxI = findNum(p->cards,i,COMPLETE);
		int idxII = findNum(p->cards,i+4,COMPLETE);
		int idxIII = findNum(p->cards,i+8,COMPLETE);
		int idxIV = findNum(p->cards,i+12,COMPLETE);
		int idxV = findNum(p->cards,i+16,COMPLETE);
		
		if(idxI!=-1&&idxII!=-1&&idxIII!=-1&&idxIV!=-1&&idxV!=-1){
			arrTemp[0]=&p->cards[idxI];
			arrTemp[1]=&p->cards[idxII];
			arrTemp[2]=&p->cards[idxIII];
			arrTemp[3]=&p->cards[idxIV];
			arrTemp[4]=&p->cards[idxV];
			p->full=	addCombo(p->full,8,arrTemp,5);
		}
	}
}
void findSTR(struct Player* p){
	int i;
    for(i=1;i<37;i+=4){
    	
		int* arrTemp[5];
		int idxTemp[5];
		int asdasd;
		int idxI,idxII,idxIII,idxIV,idxV;
		for(asdasd=i;asdasd<i+4;asdasd++){
			idxI=findNum(p->cards,asdasd,COMPLETE);
			if(idxI!=-1) break;
		}
		
		if(idxI!=-1){
			int x = i%4==0? i/4 : (i/4)+1;
			int j;
			int y = x+1;
			x*=4;
			for(j=(y*4);j>x;j--){
//				printf("\nidxI tidak -1\nx: %d y:%d j:%d\n",x,y,j);
				idxII = findNum(p->cards,j,COMPLETE);
				if(idxII!=-1) break;
			}
			if(idxII!=-1){
				y++;
				x+=4;
				int z;
				for(z=(y*4);z>x;z--){
//					printf("\nidxII tidak -1\nx: %d y:%d z:%d\n",x,y,z);
					idxIII = findNum(p->cards,z,COMPLETE);
					if(idxIII!=-1) break;
				}
				if(idxIII!=-1){
					y++;
					x+=4;
					int asd;
					for(asd=(y*4);asd>x;asd--){
//						printf("idxIII tidak -1\nx: %d y:%d asd:%d\n",x,y,asd);
						idxIV = findNum(p->cards,asd,COMPLETE);
						if(idxIV!=-1) break;
					}
					if(idxIV!=-1){
						y++;
						x+=4;
						int asdd;
						for(asdd=(y*4);asdd>x;asdd--){
//							printf("idxIV tidak -1\nx: %d y:%d asdd:%d\n",x,y,asdd);
							idxV = findNum(p->cards,asdd,COMPLETE);
							if(idxV!=-1) break;
						}
						if(idxV!=-1){
							arrTemp[0]=&p->cards[idxI];
							arrTemp[1]=&p->cards[idxII];
							arrTemp[2]=&p->cards[idxIII];
							arrTemp[3]=&p->cards[idxIV];
							arrTemp[4]=&p->cards[idxV];
							p->full=	addCombo(p->full,4,arrTemp,5);
						}
					}
				}
			}
		}
	}
}
void findFLS(struct Player* p){
	int i;
	int x=0;
	int* tempArr[5];
	for(i=0;i<13;i++){
		int* xz=&p->cards[i];
//		printf("\nangka : %d\n",*xz);
		if(*xz!=0){
			int d =*xz%4;
	//		printf("d : %d\n",d);
			tempArr[0]=xz;
	//		printf("isi temparr[0] : %d\n",*tempArr[0]);
			x+=1;
			int j;
			for(j=i+1;j<13;j++){
				if(x<5&&j==12) x=0;
	//			printf("masokk2\n");
				int* xx = &p->cards[j];
				if(*xx!=0){
		//			printf("\nangka2 : %d \n",*xx);
					int dd = *(xx)%4;
		//			printf("dd : %d \n",dd);
					if(x==1){
						if(dd==d){
							tempArr[1]=xx;
							x++;
						}
					}
					else if (x==2){
						if(dd==d){
							tempArr[2]=xx;
							x++;
						}
					}
					else if (x==3){
						if(dd==d){
							tempArr[3]=xx;
							x++;
						}
					}
					else if (x==4){
						if(dd==d){
							tempArr[4]=xx;
							x++;
						}
					}
					else if (x==5){
						p->full=addCombo(p->full,5,tempArr,5);
						x=0;break;
					}
				}
			}
		}
	}
}
int height(struct Combo* node){
    if (node==NULL)
        return 0;
    else
    {
        /* compute the height of each subtree */
        int lheight = height(node->left);
        int rheight = height(node->right);
 
        /* use the larger one */
        if (lheight > rheight)
            return(lheight+1);
        else return(rheight+1);
    }
}
int findMax(int **arr,int len){
	int i;
	int x;
	x=**arr;
	for(i=1;i<len;i++){
		if(*arr[i]>x) x = *arr[i];
	}
	return x;
}
void checkPairBelongsToTris(struct Combo* pair, struct Combo* tris,struct Player* p){
	if(tris!=NULL){
		int i;
		int* arrTemp[5];
		checkPairBelongsToTris(pair,tris->left,p);
		int maxT = findMax(tris->cards,3);
		int x = maxT%4==0 ? (maxT/4) +2 : (maxT/4)+3;
		if(pair!=NULL){
			int maxP = findMax(pair->cards,2);
			int y = maxP%4==0 ? (maxP/4) +2 : (maxP/4)+3;
			if(y!=x){
				arrTemp[0]=tris->cards[0];
				arrTemp[1]=tris->cards[1];
				arrTemp[2]=tris->cards[2];
				arrTemp[3]=pair->cards[0];
				arrTemp[4]=pair->cards[1];
				p->full=addCombo(p->full,6,arrTemp,5);
			}
		}
		checkPairBelongsToTris(pair,tris->right,p);
	}
}
void checkTrisBelongsToPair(struct Combo* pair,struct Combo*tris,struct Player*p){
	if(pair!=NULL){
		checkTrisBelongsToPair(pair->left,tris,p);
		checkPairBelongsToTris(pair,tris,p);
		checkTrisBelongsToPair(pair->right,tris,p);
	}
}
void findFLH(struct Player* p){
	struct Combo* tempPair=p->pair;
	struct Combo* tempTris=p->tris;
	if(p->pair!=NULL&&p->tris!=NULL){
		checkTrisBelongsToPair(tempPair,tempTris,p);
	}
}
void analyzePlayer(){
	int i;
	for(i=0;i<4;i++){
		findPairs(&players[i]);
		findTris(&players[i]);
		findRLF(&players[i]);
		findSTF(&players[i]);
		findBOM(&players[i]);
		findFLH(&players[i]);
		findSTR(&players[i]);
		findFLS(&players[i]);
	}
}
void init(){
	int i =0;
	for(i=0;i<52;i++){
		deck[i]=i+1;
	}
	srand(time(NULL));
	i = 51;
	for(i;i>0;i--){
		int j =rand()%52;
		int x = deck[i];
		deck[i] = deck[j];
		deck[j] = x;
	}
	int cnt=0;
	for(i=0;i<4;i++){
		int j;
		for(j=0;j<13;j++){	
			if(deck[cnt]==1) playerWithTigaTahu=i;
			players[i].cards[j]=deck[cnt++];
		}
	}
	analyzePlayer();
}
void printBack(int x){
//	printf("X: %d",x);
	printf("\t\t\t\t\t\tPLAYER %d\n",x+1);
	printf("\t");
	int i,j;
	int cnt = 0;
	for(i=0;i<13;i++)
		if(players[x].cards[i]!=0) {
			printf(" ------");
			cnt++;
		}
	for(j=0;j<2;j++){
		if(cnt!=0) printf("\n\t|");
		for(i=0;i<13;i++){
			int y = players[x].cards[i];
			if(y!=0){
				if(i==0) printf("     |");
				else printf("      |");
			}
		}
	}
	printf("\n\t");
	for(i=0;i<13;i++)
		if(players[x].cards[i]!=0) printf(" ------");
}
void printSides(int x,int y,int a,int b){
	int i;
//	printf("\n\t");
	printf("\n\tPLAYER %d\t\t\t\t\t\t\t\t\tPLAYER %d\n\t",a,b);
	if(x>y){
		for(i=0;i<x;i++){
			if(y==0) {
				if(i==0) printf(" ______\t\t\t\t\t\t\t\t\t\t       \n\t");
				else printf("|______|\t\t\t\t\t\t\t\t\t");
			} else{
				if(i==0){
					printf(" ______\t\t\t\t\t\t\t\t\t\t ______\n\t");
				}
				printf("|______|\t\t\t\t\t\t\t\t\t");
				if(i<=y) printf("|______|\n\t");
				else printf("\n\t");
			}
		}
	} else if (x<y){
		for(i=0;i<y;i++){
			if(x==0){
				if(i==0) printf("          \t\t\t\t\t\t\t\t\t ______\n\t");
				else printf("           \t\t\t\t\t\t\t\t\t|______|\n\t");
			}
			else{
				if(i==0){
					printf("    ______\t\t\t\t\t\t\t\t\t ______\n\t");
				}
				if(i<=x) 
					printf("   |______|\t\t\t\t\t\t\t\t\t|______|\n\t");
				else printf("           \t\t\t\t\t\t\t\t\t|______|\n\t");		
			}
		}
	} else{
		for(i=0;i<x;i++){
			if(i==0) printf(" ______\t\t\t\t\t\t\t\t\t\t ______\n\t");
			printf("|______|\t\t\t\t\t\t\t\t\t|______|\n\t");
		}
	}
}
void printPlayersCards(int x){
	int rem[4]={0,0,0,0};
	int i;
	for(i=0;i<4;i++)
		if(i!=x) rem[i]=checkRemaining(i);
	
	int atas,kiri,kanan;
	atas = x<=1 ? x+2 : x-2;
	kiri = x==0 ? 3 : x-1;
	kanan = x==3 ? 0 : x+1;
	
	printBack(atas);
	printSides(rem[kiri],rem[kanan],kiri+1,kanan+1);
	
}
void printCard(struct Combo* root,int len){
	int i;
	for(i=0;i<len;i++) printf(" ------");
	printf("\n|");
	int x;
	
	for(i=0;i<len;i++){
		x =*(root->cards[i]);
		char sym;
		int y=x%4;
		int z =x/4;
		if(y==1){
			z++;
		} else if (y==2){
			z++;
		}else if (y==3){
			z++;
		}else if (y==0){
		}
		z+=2;
		if(z==10){
			sym='X';
		} else if(z==11){
			sym='J';
		} else if(z==12){
			sym='Q';
		} else if(z==13){
			sym='K';
		} else if(z==14){
			sym='A';
		}  else if(z==15){
			sym='2';
		} else{
			sym=z+48;
		}
		
		if(i==0)
			 printf("   %c |",sym);
		else printf("    %c |",sym);
	}
	printf("\n|");
	for(i=0;i<len;i++){
		x = *(root->cards[i]);
		if(x!=0){
			char sym;
			int y=x%4;
			if(y==1){
				sym='\4';
			} else if (y==2){
				sym='\5';
			}else if (y==3){
				sym='\3';
			}else if (y==0){
				sym='\6';
			}
			if(i==0)
				 printf("   %c |",sym);
			else printf("    %c |",sym);
		}
	}
	printf("\n");
	for(i=0;i<len;i++) printf(" ------");
	printf("\n");
}
void printComboInOrder(struct Combo* root,int len){
	if(root!=NULL){
		printComboInOrder(root->left,len);
		int c = root->code;
		if(c==9) printf("ROYAL FLUSH"); 
		else if (c==8)	printf("STRAIGHT FLUSH");
		else if (c==7) printf("BOMB");
		else if (c==6) printf("FULL HOUSE");
		else if (c==5) printf("FLUSH");
		else if (c==4) printf("STRAIGHT");
		else if (c==3) printf("THREE OF A KIND");
		else if (c==2) printf("TWO OF A KIND");
		printf(" %d\n",root->id);
//		printf("\n");
		int i =0;
		for(i=0;i<len;i++) printf(" ------");
		printf("\n|");
		int x;
		for(i=0;i<len;i++){
			x = *(root->cards[i]);
			char sym;
			int y=x%4;
			int z = x/4;
			if(y==1){
				z++;
			} else if (y==2){
				z++;
			}else if (y==3){
				z++;
			}else if (y==0){
			}
			z+=2;
			if(z==10){
				sym='X';
			} else if(z==11){
				sym='J';
			} else if(z==12){
				sym='Q';
			} else if(z==13){
				sym='K';
			} else if(z==14){
				sym='A';
			}  else if(z==15){
				sym='2';
			} else{
				sym=z+48;
			}
			
			if(i==0)
				 printf("   %c |",sym);
			else printf("    %c |",sym);
		}
		printf("\n|");
		for(i=0;i<len;i++){
			x = *(root->cards[i]);
			if(x!=0){
				char sym;
				int y=x%4;
				if(y==1){
					sym='\4';
				} else if (y==2){
					sym='\5';
				}else if (y==3){
					sym='\3';
				}else if (y==0){
					sym='\6';
				}
				if(i==0)
					 printf("   %c |",sym);
				else printf("    %c |",sym);
			}
		}
		printf("\n");
		for(i=0;i<len;i++) printf(" ------");
		printf("\n");
		printComboInOrder(root->right,len);
	}
}
int printGivenLevel(struct Combo* c,int lv,int len){
	if(c==NULL) return;
	if(lv==1){
		int i;
		return 0;
	} else if (lv>1){
		printGivenLevel(c->left,lv-1,len);
		printGivenLevel(c->right,lv-1,len);
	}
}
void printLevelOrder(struct Combo* c, int len){
	int h = height(c);
	int i;
	for(i=1;i<=h;i++)
		printf("%d ",printGivenLevel(c,i,len));
}
void checkInFull(struct Combo* full,int w){
	if(full!=NULL){
		//do stufs
		int i;
		for(i=0;i<5;i++){
//			printf("%d \n",*full->cards[i]);
			if(*full->cards[i]==w){
//				printf("ADA BOS DI FULL!\n");
				existInFull=full->id;
			}
		}
		checkInFull(full->left,w);
		checkInFull(full->right,w);	
	}
}
void checkInTris(struct Combo* full,int w){
	if(full!=NULL){
		//do stufs
		int i;
		for(i=0;i<3;i++){
//			printf("%d ",i);
			if(*full->cards[i]==w){
//				printf("\nADA BOS DI TRIS\n\n");
//				return 1;
				existInTris=full->id;
			}
		}
		checkInTris(full->left,w);
		checkInTris(full->right,w);	
	}else{
//		printf("TRIS NULL!");
	}
}
void checkInPair(struct Combo* full,int w){
	if(full!=NULL){
		//do stufs
		int i;
		for(i=0;i<2;i++){
			if(*full->cards[i]==w){
//				printf("ADA BOS DI PAIR!\n\n");
				existInPair=full->id;
			}
		}
		checkInPair(full->left,w);
		checkInPair(full->right,w);
	}
}
void checkInBomb(struct Combo* full,int w){
	if(full!=NULL){
		//do stufs
		int i;
		for(i=0;i<4;i++){
			if(*full->cards[i]==w){
				existInBomb=full->id;
			}
		}
		checkInBomb(full->left,w);
		checkInBomb(full->right,w);
	}
}
struct Combo* putCardByWeight(struct Combo* hand, int w,int len){ //w = id
//	printf("Putted card with weight %d\n",w);
	if(hand==NULL) return hand;
	
	if(w< hand->id){
//		printf("kiri\n");
		hand->left = putCardByWeight(hand->left,w,len);
	}
	else if(w>hand->id){
//		printf("kanan\n");
		hand->right = putCardByWeight(hand->right,w,len);
	}
	else {
		struct Combo* temp = NULL;
		int i=0;
//		printf("CODE : %d\nID : %d\n",hand->code,hand->id);
		if(hand->left==NULL){
//			printf("masuk 1\n");
//			printCard(hand,len);
			for(i = 0;i<len;i++){
//				printf("%d ",*hand->cards[i]);
				*hand->cards[i]=0;
			}
			puts("");
			temp = hand->right;
			free(hand);
			return temp;
		} else if(hand->right==NULL){
//			printf("masuk 2\n");
//			printCard(hand,len);
			for(i = 0;i<len;i++){
//				printf("%d ",*hand->cards[i]);
				*hand->cards[i]=0;
			}
//			puts("");
			temp = hand->left;
			free(hand);
			return temp;
		}
		
//		printf("masuk 3\n");
		temp = minValue(hand->right);
		hand->code=temp->code;
		hand->id = temp->id;
//		for(i = 0;i<len;i++){
//			*hand->cards[i]=0;
//		}
		for(i=0;i<len;i++){
			hand->cards[i]=temp->cards[i];
		}
		hand->right = putCardByWeight(hand->right,temp->id,len);
	}
	return hand;
}
int findMin(int min,int arr[]){
	int i;
	int x=-1;
	int val = 999;
	for(i=0;i<13;i++){
		if(arr[i]>min&&arr[i]<val){
//			printf("val %d\n",val);
			val=arr[i];
		}
	}
//	printf("min val %d\n",val);
	for(i=0;i<13;i++){
		if(arr[i]==val) x=i;
	}
//	printf("x : %d\n",x);
	return x;
}
struct Combo* findId(struct Combo*c, int id){
	if(c==NULL||c->id==id) return c;
	if(c->id<id) return findId(c->right,id);
	return findId(c->left,id);
}
int firstMove(struct Player* p){
	int min;
	int x;
	lastId=0;
	if(p->full!=NULL){
		traverse(p->full);
		x = findMinInPlayed();
//		printf("ada full %d\n",x);

		clearLast();
		putLast(findId(p->full,x),x,5,FIV);
		p->full=putCardByWeight(p->full,x,5);
		roundNow=5;
		
	}else if (p->tris!=NULL){
		traverse(p->tris);
		x = findMinInPlayed();
//		printf("ada tris %d\n",x);
		clearLast();
		putLast(findId(p->tris,x),x,3,TRI);
		p->tris=putCardByWeight(p->tris,x,3);
		roundNow=3;
		
	}else if (p->pair!=NULL){
		traverse(p->pair);
		x = findMinInPlayed();
//		printf("ada pair %d\n",x);

		clearLast();
		putLast(findId(p->pair,x),x,2,PAR);
		p->pair=putCardByWeight(p->pair,x,2);
		roundNow=2;
	} else{
		min = findMin(0,p->cards);
//		printf("ada solo di idx %d \n",min);
		x = p->cards[min];
//		printCard(x,1);
		p->cards[min]=0;
//		printf("buang solo");
		clearLast();
		putLast(NULL,x,1,SOL);
		roundNow=1;
	}
//	printLast();
	lastId=x;
	roundStarter=0;
	p->pair = deleteAll(p->pair);
	p->tris = deleteAll(p->tris);
	p->full= deleteAll(p->full);
	p->bom =  deleteAll(p->bom);
	analyzePlayer();
	resetStat();
//	printf("\n");
	printf("\nPRESS [ENTER] TO CONTINUE!\n");
	getchar();
	return 0;
}
void printPlayerCards(int idx){
	int i,cnt=0;
	printf("\n\n\t");
	for(i=0;i<13;i++){
		int x = players[idx].cards[i];
		if(x!=0){
			printf("   %d  ",x);
		}
	}
	printf("\n\t");
	for(i=0;i<13;i++){
		int x = players[idx].cards[i];
		if(x!=0){
			printf(" ------");
			cnt++;
		}
	}
	if(cnt!=0)	printf("\n\t|");
	for(i=0;i<13;i++){
		int x = players[idx].cards[i];
		if(x!=0){
			char sym;
			int y=x%4;
			int z = x/4;
			if(y==1){
				z++;
			} else if (y==2){
				z++;
			}else if (y==3){
				z++;
			}else if (y==0){
			}
			z+=2;
			if(z==10){
				sym='X';
			} else if(z==11){
				sym='J';
			} else if(z==12){
				sym='Q';
			} else if(z==13){
				sym='K';
			} else if(z==14){
				sym='A';
			}  else if(z==15){
				sym='2';
			} else{
				sym=z+48;
			}
			
			if(i==0)
				 printf("   %c |",sym);
			else printf("    %c |",sym);
		}
	}
	if(cnt!=0) printf("\n\t|");
	for(i=0;i<13;i++){
		int x = players[idx].cards[i];
		if(x!=0){
			char sym;
			int y=x%4;
			if(y==1){
				sym='\4';
			} else if (y==2){
				sym='\5';
			}else if (y==3){
				sym='\3';
			}else if (y==0){
				sym='\6';
			}
			if(i==0)
				 printf("   %c |",sym);
			else printf("    %c |",sym);
		}
	}
	if(cnt!=0) printf("\n\t");
	for(i=0;i<13;i++)
		if(players[idx].cards[i]!=0) printf(" ------");
	printf("\n\n");
	printf("\t\t\t\t\t\tPLAYER %d (YOU)\n",idx+1);
}
void doSomething(struct Combo* c, int level){
	int i;
	if(c==NULL) return;
	if(level==1) addPlayed(c->id);
	else if(level>1) {
		doSomething(c->left,level-1);
		doSomething(c->right,level-1);
	}
}
int traverse(struct Combo* c){
	int i;
	int h=height(c);
	int x = 0;
	for(i=1;i<=h;i++)
		doSomething(c,i);
	return x;
}
int addNum(int curr){
	if(curr+1>3) return 0;
	else return curr+1;
}
int playerMove(struct Player* p){
	int onGoing = 1;
	int i,f,b,t,pp,inp=0; 
	char arr[4]="";
	int cnt=0;
	while(onGoing){
		while(inp<=0){
			if(p->full!=NULL) {
//				printf("ada full\n");
				f=1;
				cnt++;
			} else{
				f=0;
			}
			if(p->tris!=NULL) {
//				printf("ada tris\n");
				t=1;
				cnt++;
			}else{
				t=0;
			}
			if(p->pair!=NULL) {
//				printf("ada pair\n");
				pp=1;
				cnt++;
			}else{
				pp=0;
			}
//			cnt++;
			printf("You can play a round of : \n");
			for(i=0;i<cnt+1;i++){
				if(f==1){
					arr[i]='f';
					printf("%d. Full Combo\n",i+1);
					f=0;
					continue;
				}
				if(t==1){
					arr[i]='t';
					printf("%d. Three of a kind\n",i+1);
					t=0;
					continue;
				}
				if(pp==1){
					arr[i]='p';
					printf("%d. Two of a kind\n",i+1);
					pp=0;
					continue;
				}
				if(i==cnt) {
					arr[i]='s';
					printf("%d. One of a kind\n",i+1);
				}
			}
			printf(" >> ");
			scanf("%d",&inp);
			getchar();
			if(inp<=0||inp>cnt+1) inp = 0;
		}
		inp--;
		struct Combo** temp=NULL;
		int tempLen=-1;
		if(arr[inp]=='f'){
			tempLen=5;
			roundNow=5;
			temp = &p->full;
		}
		else if (arr[inp]=='t'){
			tempLen=3;
			roundNow=3;
			temp = &p->tris;
		} else if (arr[inp]=='p'){
			tempLen=2;
			roundNow=2;
			temp = &p->pair;
		} else if (arr[inp]=='s'){
			tempLen=1;
			roundNow=1;
			temp = NULL;
		}
		if(tempLen>1&&temp!=NULL){
			printf("Possible combinations : \n");
			printComboInOrder(*temp,tempLen);
		}
		int invalid=1;
		while(invalid){
			printf("Enter card id : ");
			int id;
			scanf("%d",&id); getchar();
			if(temp!=NULL){
				if(findId(*temp,id)!=NULL){	
					clearLast();
					putLast(findId(*temp,id),id,tempLen,roundNow);
					*temp = putCardByWeight(*temp,id,tempLen);
					invalid=0;
					onGoing=0;
					lastId=id;
				}else printf("INVALID ID !\n");
			}else{
				int xxx = findNum(p->cards,id,COMPLETE);
				if(xxx>=0){
					clearLast();
					invalid=0;				
					onGoing=0;
					putLast(NULL,id,SOL,SOL);
					p->cards[xxx]=0;
					lastId=id;
				}else printf("INVALID ID !\n");
			}
		}
	}
//	printLast();
	roundStarter=0;
	if(p->pair!=NULL)
		p->pair = deleteAll(p->pair);
	if(p->tris!=NULL)
		p->tris = deleteAll(p->tris);
	if(p->full!=NULL)
		p->full= deleteAll(p->full);
	if(p->bom!=NULL)
		p->bom =  deleteAll(p->bom);
	analyzePlayer();
	resetStat();
	getchar();
	return 0;
}
int playerMoveInRound(struct Player* p){
	int i,temp,tempLen;
	struct Combo**temps=NULL;
	if(roundNow==1){
		tempLen=1;
		if(findMin(lastId,p->cards)<0) isPlayerPlaying[playerWithTigaTahu]=0;
	} else {
		if (roundNow==2){
			if(p->pair!=NULL) {
				temps = &p->pair;
				tempLen=2;
			} else isPlayerPlaying[playerWithTigaTahu]=0;
		} else if (roundNow==3){
			if(p->tris!=NULL){
				temps = &p->tris;
				tempLen=3;
			}else isPlayerPlaying[playerWithTigaTahu]=0;
		} else if (roundNow==5){
			if(p->full!=NULL){
				temps=&p->full;
				tempLen=5;
			}else isPlayerPlaying[playerWithTigaTahu]=0;
		}
		
	}
	int invalid=1;
	if(temps!=NULL){
		traverse(*temps);
		int xxx = findMinInPlayed();
		if(xxx<lastId) {
			isPlayerPlaying[playerWithTigaTahu]=0;
		}else{
			printComboInOrder(*temps,tempLen);
		}
	}
	if(isPlayerPlaying[playerWithTigaTahu]!=0){
		while(invalid){
			printf("Enter card id : ");
			int id;
			scanf("%d",&id); getchar();
			if(id>=lastId){
				if(temps!=NULL){
					if(findId(*temps,id)!=NULL){
						clearLast();	
						putLast(findId(*temps,id),id,tempLen,roundNow);
						*temps = putCardByWeight(*temps,id,tempLen);
						invalid=0;
						lastId=id;
						isPlayerPlaying[playerWithTigaTahu]=1;
					}else{
						printf("INVALID ID !\n");
					}
				}else{
					int xxx = findNum(p->cards,id,COMPLETE);
					if(xxx>=0){
						clearLast();
						putLast(NULL,p->cards[xxx],SOL,SOL);
						invalid=0;
						p->cards[xxx]=0;
						lastId=id;
						isPlayerPlaying[playerWithTigaTahu]=1;
					}else printf("INVALID ID !\n");
				}
			}else{
				printf("You can not choose card lower than the last!\n");
			}
		}
//		printLast();
	}else{
		printf("You cannot move! [Enter] to continue \n");
//		getchar();
	}
	if(p->pair!=NULL)
		p->pair = deleteAll(p->pair);
	if(p->tris!=NULL)
		p->tris = deleteAll(p->tris);
	if(p->full!=NULL)
		p->full= deleteAll(p->full);
	if(p->bom!=NULL)
		p->bom =  deleteAll(p->bom);
	analyzePlayer();
	getchar();
}
int enemyMoveInRound(struct Player* p){
	int i;
	int temp,tempLen;
	struct Combo**temps=NULL;
	if(roundNow==1){
		int x = findMin(lastId,p->cards);
		temp = p->cards[x];
		if(temp<=0) isPlayerPlaying[playerWithTigaTahu]=0;
		else{
			existInFull=0;
			checkInFull(p->full,temp);
			if(existInFull!=0) {
				isPlayerPlaying[playerWithTigaTahu]=0;
			}
			else{
				clearLast();
				putLast(NULL,p->cards[x],SOL,SOL);
				lastId=p->cards[x];
				p->cards[x]=0;
			}
		}
	}
	else if (roundNow==2){
		if(p->pair!=NULL) {
			temps = &p->pair;
			tempLen=2;
		} else isPlayerPlaying[playerWithTigaTahu]=0;
	} else if (roundNow==3){
		if(p->tris!=NULL){
			temps = &p->tris;
			tempLen=3;
		}else isPlayerPlaying[playerWithTigaTahu]=0;
	} else if (roundNow==5){
		if(p->full!=NULL){
			temps=&p->full;
			tempLen=5;
		}else isPlayerPlaying[playerWithTigaTahu]=0;
	}
	if(isPlayerPlaying[playerWithTigaTahu]==0){
		printf("Player %d skip this round!\n",playerWithTigaTahu+1);
	}
	else if(temps!=NULL){
		traverse(*temps);
		int x = findMinInPlayed();
		if(x>lastId){
			clearLast();
			putLast(findId(*temps,x),x,tempLen,roundNow);
			*temps=putCardByWeight(*temps,x,tempLen);
			lastId=x;
		}
		else{
			isPlayerPlaying[playerWithTigaTahu]=0;
			printf("Player %d skip this round!\n",playerWithTigaTahu+1);
		}
	}else{
	}
	if(p->pair!=NULL)
		p->pair = deleteAll(p->pair);
	if(p->tris!=NULL)
		p->tris = deleteAll(p->tris);
	if(p->full!=NULL)
		p->full= deleteAll(p->full);
	if(p->bom!=NULL)
		p->bom =  deleteAll(p->bom);
	analyzePlayer();
	printf("\nPRESS [ENTER] TO CONTINUE!\n");
	getchar();
}
int checkCurrentPlayers(){
	int i=0;
	int cnt=0;
	for(i=0;i<4;i++){
		if(isPlayerPlaying[i]==0)cnt++;
		if(cnt==3) return 1;
	}
	return 0;
}
void endLogo(){
	int i;
//	char logo[20][100];
//	strcpy(logo[0],"                               +_+µ              -");
//	strcpy(logo[1],"                           ,_¦¦¦¦+++¦+,      _@+");
//	strcpy(logo[2],"                       ,_¦¦¦¦¦¦¦+¦¦+¦¦¦¦¦__¦¯n");
//	strcpy(logo[3],"                    +@¦¦¦¦¦¦+¦¦¦¦¦¦¦¦¦¦¦¦¯¦¦¦-");
//	strcpy(logo[4],"                +@¦¦¦¦¦¦+¦¦¦¦¦+¦¦¦_¦¦¦¯¦¦¦¦¦¦¦¦¦d-");
//	strcpy(logo[5],"                ¦¦¦¦+¦¦¦¦¦¦¦¦¦¦¦¦¦¦¯¦f¦¦¦¦¦¦¦++¦¦e");
//	strcpy(logo[6],"                ¦¦¦¦¦f¦¦¦¦¦¦¦¦¦¦¯¦¦¦¦¦¦¦¦+++''¦¦¦e");
//	strcpy(logo[7],"                ¦¦¦¦¦¦¦¦¦¦¦¦¦¯¦¦f¦¦Q--+¦¦¦¬'''¦¦¦e");
//	strcpy(logo[8],"                ¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦++¦¦_¦¦¦¦¦¬'''¦¦¦e");
//	strcpy(logo[9],"                ¦¦¦¦¦¦¦¦¦¦¦¦++¦¦+¦¦¦¦¦¦¦¦¦n.''¦¦¦e");
//	strcpy(logo[10],"                ¦¦¦¦¦¦¦¦¦¦¦;;;;¦¦¦¦¦¯¦++¯¦¦-~.¦¦¦[");
//	strcpy(logo[11],"                ¦+¦¦¦¦¦¦¦+¦+¦¦¦¦¦¯¦¦¦+f¦++'n¦¦¦¦¦[");
//	strcpy(logo[12],"                ¦¦¦+++¦¦¦¦f¦¦¦¯¦¦¦f¦-++.''''''^n¦[");
//	strcpy(logo[13],"                 '+¦¦¦¦¦_¦¦¯¦¦f+¦++^''''''");
//	strcpy(logo[14],"                     ]_¦¯=++++¦¦¦¬'''");
//	strcpy(logo[15],"                   ,¦¯+` '''''++'");
//	strcpy(logo[16],"                 _¯+          '''");
//	for(i=0;i<17;i++){
//		Sleep(500);
//		printf("%s\n",logo[i]);
//	}
	char motto[11][11];
	strcpy(motto[0],"Through");
	strcpy(motto[1],"Hardwork");
	strcpy(motto[2],"and");
	strcpy(motto[3],"Dedication");
	strcpy(motto[4],"We");
	strcpy(motto[5],"Hold");
	strcpy(motto[6],"Our");
	strcpy(motto[7],"Future");
	strcpy(motto[8],"in");
	strcpy(motto[9],"Our");
	strcpy(motto[10],"Hands");
	printf("\n\t");
	for(i=0;i<11;i++){
		Sleep(500);
		printf("%s ",motto[i]);
	}
	getchar();
}
int main(){
//	adjustWindowSize();
	int playerIDX=-1;
	while(playerIDX<1||playerIDX>4){
		printf("\n\t\tPick your cards! [1-4] :");
		scanf("%d",&playerIDX);
		getchar();
	}
	playerIDX--;
	init();
	int firstPlayer=playerWithTigaTahu;
	system("cls");
	printf("\n\t\tPlayer with 3 diamond go first!\n\t\tThat's player %d\n\n",playerWithTigaTahu+1);
	printf("\t\t[Enter] to continue!\n\n");	
	getchar();
	while(1){
		system("cls");
		printf("\n\n\t\t\t\t\t\tPlayer %d's turn\n\n",playerWithTigaTahu+1);
		printPlayersCards(playerIDX);
		printLast();
		printPlayerCards(playerIDX);
		if(playerIDX==playerWithTigaTahu){
			if(roundStarter)
				playerMove(&players[playerWithTigaTahu]);
			else if(isPlayerPlaying[playerWithTigaTahu]==1){
				playerMoveInRound(&players[playerWithTigaTahu]);
			}else{
				printf("YOU ARE NOT PLAYING!\n"); getchar();
			}
		} else{ //			COMPUTER MOVE
			if(roundStarter)
				firstMove(&players[playerWithTigaTahu]);
			else if(isPlayerPlaying[playerWithTigaTahu]==1){
				enemyMoveInRound(&players[playerWithTigaTahu]);
			}else{
				printf("PLAYER %d IS NOT PLAYING!\n",playerWithTigaTahu+1); getchar();
			}
		}
		if(checkWin(players[playerWithTigaTahu].cards)){
//			printf("MENANG BROOOOOOOO\n");
			isPlayerPlaying[playerWithTigaTahu]=0; 
			break;
		}
		if(checkCurrentPlayers()){
//			system("cls");
//			printf("NEW ROUND!");
			int xxx = findNum(isPlayerPlaying,1,4);
			playerWithTigaTahu=xxx;
			roundStarter=1;
		}else{
			playerWithTigaTahu=addNum(playerWithTigaTahu);
		}
	}
	system("cls");
	printPlayersCards(playerIDX);
	printLast();
	printPlayerCards(playerIDX);
	printf("\nPRESS [ENTER] TO CONTINUE!\n"); getchar();
	system("cls");
	char won[10][50];
	char playerWon[50];
	sprintf(playerWon,"             ((|  {{%d}}  |))",playerWithTigaTahu+1);
	
	strcpy(won[0],"              .-=========-.");
	strcpy(won[1],"              \\'-=======-'/");
	strcpy(won[2],"              _|   .=.   |_");
	strcpy(won[3],playerWon);
	strcpy(won[4],"              \\|   /|\\   |/");
	strcpy(won[5],"               \\__ '`' __/");
	strcpy(won[6],"                 _`) (`_");
	strcpy(won[7],"               _/_______\\_");
	strcpy(won[8],"              /___________\\");
	
	int i;
	for(i=0;i<9;i++){
		Sleep(500);
		printf("%s\n",won[i]);
	}
	
	printf("\t     CONGRATULATION!\n\t      PLAYER %d WON!",playerWithTigaTahu+1);
	printf("\n\n\tPRESS [ENTER] TO CONTINUE!\n"); getchar();
	
	endLogo();
	return 0;
}
