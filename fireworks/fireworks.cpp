#include <graphics.h>
#include <conio.h>
#include <iostream>
using namespace std;
#include <cmath>
#include <time.h>
#include <stdio.h>
#include <mmsystem.h>
#pragma comment ( lib, "Winmm.lib" )

#define W 1200
#define H 1000

struct JET{
	int x,y,gx,gy,height;
	bool shoot;
	DWORD t1,t2,dt; 
	IMAGE img[2];
	byte m:1;
}jet[13];
struct FIREWORK{
	int x,y,cx,cy,r,maxr,width,height;
	int xy[240][240];
	bool show;
	bool draw;
	DWORD t1,t2,dt;
}firework[13];

void Init(int i);
void Load();
void Shoot();
void Chose(DWORD&);
void Show(DWORD* );
void Clear(DWORD* pMem);

int main(){
	srand(time(0));
	initgraph(W,H);
	setbkcolor(RGB(0,0,0)); 
	cleardevice();
	DWORD t1 = timeGetTime();
	DWORD* pMem = GetImageBuffer();
	
	for(int i=0;i<13;i++){
		Init(i);
	}
	Load();
	BeginBatchDraw();
	while (!_kbhit()){
		Sleep(10);
		Clear(pMem);
        Chose(t1);
	    Shoot();
	    Show(pMem);
	    FlushBatchDraw();   
   }
	
	closegraph();
	return 0;
} 

void Init(int i){
	int x[13] = { 120, 120, 110, 117, 110, 93,  102, 102, 110, 105, 100, 108, 110 };
    int y[13] = { 120, 120, 85,  118, 120, 103, 105, 110, 110, 120, 120, 104, 85 };
    jet[i].height=0;
    jet[i].t1 = timeGetTime();  
    jet[i].dt = rand() % 10;    
    jet[i].m = 0;               
    jet[i].shoot = false;       
    
	int r[13] = { 120, 120, 155, 123, 130, 147, 138, 138, 130, 135, 140, 132, 155 };
    firework[i].show=false;
    firework[i].dt=5;
    firework[i].t1=timeGetTime();
    firework[i].width=240;
    firework[i].height=240;
    firework[i].maxr=r[i];
    firework[i].r=0;
    firework[i].cx=x[i];
    firework[i].cy=y[i];
    
}

void Load(){
	IMAGE jm;
	loadimage(&jm,"./fire/shoot.jpg",200,50);
	for (int i=0;i<13;i++){
		SetWorkingImage(&jm);
		int n=rand()%5;
		getimage(&jet[i].img[0],n*20,0,20,50);
		getimage(&jet[i].img[1],(n+5)*20,0,20,50);
	}
	
	
	IMAGE fm,gm;
    loadimage(&fm,"./fire/flower.jpg",3120,240);
    for (int i=0;i<13;i++){
    	SetWorkingImage(&fm);
    	getimage(&gm,i*240,0,240,240);
    	SetWorkingImage(&gm);
    	for(int a=0;a<240;a++)
    	    for(int b=0;b<240;b++)
    	        firework[i].xy[a][b]=getpixel(a,b);
    	
	}
	SetWorkingImage();
}

void Chose(DWORD &t1){
	DWORD t2 = timeGetTime();
	if (t2-t1 > 100){
		int n =rand() % 22;
		if(n<13 && jet[n].shoot == false && firework[n].show == false){
			jet[n].x=rand()%W;
			jet[n].y=rand()%100+H-100;
			jet[n].gx = jet[n].x;
			jet[n].gy = rand()%326;
			jet[n].height = jet[n].y-jet[n].gy;
			jet[n].shoot=true;
			putimage(jet[n].x,jet[n].y,&jet[n].img[jet[n].m],SRCINVERT);
			
			char c1[50],c2[30],c3[30];
			sprintf(c1, "open ./fire/shoot.mp3 alias s%d", n);
			sprintf(c2, "play s%d", n);
			sprintf(c3, "close n%d", n);
			mciSendString(c3, 0, 0, 0);  
			mciSendString(c1, 0, 0, 0);  
			mciSendString(c2, 0, 0, 0);
		}
		t1=t2;
	}
	
}	

void Shoot(){
	for(int i=0;i<13;i++){
		jet[i].t2=timeGetTime();
		if(jet[i].t2-jet[i].t1>jet[i].dt && jet[i].shoot == true){
			putimage(jet[i].x,jet[i].y,&jet[i].img[jet[i].m],SRCINVERT);
			if (jet[i].y>jet[i].gy){
				jet[i].m++;
				jet[i].y-=5;
			} 
			putimage(jet[i].x,jet[i].y,&jet[i].img[jet[i].m],SRCINVERT);
			if((jet[i].y-jet[i].gy)*4<jet[i].height) jet[i].dt=rand()%4+10;
			if(jet[i].y<=jet[i].gy){
				char c1[50], c2[30], c3[30];
                                sprintf(c1, "open ./fire/bomb.wav alias n%d", i);  
                                sprintf(c2, "play n%d", i);                        
                                sprintf(c3, "close s%d", i);                       

                                mciSendString(c3, 0, 0, 0);  
                                mciSendString(c1, 0, 0, 0);  
                                mciSendString(c2, 0, 0, 0);  
                                putimage(jet[i].x, jet[i].y, &jet[i].img[jet[i].m], SRCINVERT);
				                firework[i].x=jet[i].gx+10;
				                firework[i].y=jet[i].gy;
				                firework[i].show=true;
				                jet[i].shoot=false;
				                
			}
			jet[i].t1=jet[i].t2;
			
		}
	}
}


void Show(DWORD* pMem){
	int drt[16] = { 5, 5, 5, 5, 5, 6, 25, 25, 25, 25, 55, 55, 55, 55, 55,55 };
	for(int i=0;i<13;i++){
		firework[i].t2=timeGetTime();
		if (firework[i].t2-firework[i].t1>firework[i].dt &&firework[i].show==true){
			if(firework[i].r<firework[i].maxr){
				firework[i].r++;
				firework[i].dt=drt[firework[i].r/10];
				firework[i].draw=true;
				
			}
			if(firework[i].r>=firework[i].maxr-1){
				firework[i].draw = false;
				Init(i);
				
			}
			firework[i].t1=firework[i].t2;
			
		}
		if (firework[i].draw){
			for (double s=0;s<6.28;s+=0.01){
				int x1= (int)(firework[i].cx+firework[i].r*cos(s));
				int y1= (int)(firework[i].cy-firework[i].r*sin(s));
				if (x1>0 && x1<firework[i].width &&y1>0 && y1<firework[i].height){
					int b =firework[i].xy[x1][y1]&0xff;
					int g = (firework[i].xy[x1][y1] >> 8) & 0xff;
                    int r = (firework[i].xy[x1][y1] >> 16);
                    int xx = (int)(firework[i].x + firework[i].r * cos(s));
                    int yy = (int)(firework[i].y - firework[i].r * sin(s));
                    if (r > 0x30 && g > 0x30 && b > 0x30 && xx > 0 && xx < W && yy > 0 && yy < H)
                        pMem[yy * W + xx] = BGR(firework[i].xy[x1][y1]);
				}
			}
		firework[i].draw = false;	
		}
	} 
}

void Clear(DWORD* pMem){
    for (int i = 0; i < 1000; i++){
        for (int j = 0; j < 3; j++){
            int px1 = rand() % W;  
            int py1 = rand() % H;      
            if (px1 < W - 1) {
                pMem[py1 * W + px1] = BLACK;
                pMem[py1 * W + px1 + 1] = BLACK;
            }
        }                                                                     
    }
}

