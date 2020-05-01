/*
epidemic
made by: wangyxhaha
-lgraphics64 -lgdi32 -limm32 -lmsimg32 -lole32 -loleaut32 -lwinmm -luuid
*/
#include <bits/stdc++.h>
#include <graphics.h>
using namespace std;

struct block{
    int type;//0 null,1 human,2 zombie
    int v;
}world_map[500][500];

struct xy{
    int x,y;
};

int color[3]={0x696969,0x32CD32,0xB22222};
int dx[]={-1,0,1,0},
    dy[]={0,1,0,-1};
int human_at_first,danger;
double careful,ex;

void map_init(int seed){
    srand(seed);
    human_at_first=0;
    for (int i=0;i<500;i++){
        for (int j=0;j<500;j++){
            if (rand()%10==9) world_map[i][j].type=1,world_map[i][j].v=rand()%1000,human_at_first++;
            else world_map[i][j].type=0,world_map[i][j].v=0;
        }
    }
    int zx=rand()%500,zy=rand()%500;
    world_map[zx][zy].type=2,world_map[zx][zy].v=1000;
    return;
}

void draw_map(){
    color_t* buff = getbuffer(NULL);
    for (int i=0;i<500;i++){
        for (int j=0;j<500;j++){
            buff[i+j*500]=color[world_map[i][j].type];
        }
    }
}

void move(){
    int human_now=0;
    for (int i=0;i<500;i++){
        for (int j=0;j<500;j++){
            if (world_map[i][j].type==1) human_now++;
        }
    }
    careful=1.0-((double)(human_now)/(double)(human_at_first));
    danger=1000*careful;
//    danger=1000;
    for (int i=0;i<500;i++){ //human
        for (int j=0;j<500;j++){
            if (world_map[i][j].type==1){
                world_map[i][j].v+=10;
                if (world_map[i][j].v>1000){
                    bool m=0;
                    for (int k=0;k<4;k++){
                        int t1=i+dx[k],t2=j+dy[k];
                        if (t1>=0 && t1<500 && t2>=0 && t2<500){
                            if (world_map[t1][t2].type==0){
                                m=1;
                                break;
                            }
                        }
                    }
                    if (m){
                        while (1){
                            int go=rand()%4;
                            int t1=i+dx[go],t2=j+dy[go];
                            if (t1>=0 && t1<500 && t2>=0 && t2<500){
                                if (world_map[t1][t2].type==0){
                                    world_map[t1][t2].type=1;
                                    world_map[t1][t2].v=world_map[i][j].v-1000;
                                    world_map[i][j].v=1000;
                                    break;
                                }
                            }
                        }
                    }
                    else world_map[i][j].v=1000;
                    continue;
                }
                if (world_map[i][j].v>danger) continue;
                bool alone=0;
                for (int k=0;k<4;k++){
                    int t1=i+dx[k],t2=j+dy[k];
                    if (t1>=0 && t1<500 && t2>=0 && t2<500){
                        if (world_map[t1][t2].type==1){
                            alone=1;
                            if (world_map[i][j].v+world_map[t1][t2].v<1000){
                                world_map[t1][t2].v+=world_map[i][j].v;
                                world_map[i][j].type=0;
                            }
                            break;
                        }
                    }
                }
                if (!alone){
                    bool m=0;
                    for (int k=0;k<4;k++){
                        int t1=i+dx[k],t2=j+dy[k];
                        if (t1>=0 && t1<500 && t2>=0 && t2<500){
                            if (world_map[t1][t2].type==0){
                                m=1;
                                break;
                            }
                        }
                    }
                    if (m){
                        while (1){
                            int go=rand()%4;
                            int t1=i+dx[go],t2=j+dy[go];
                            if (t1>=0 && t1<500 && t2>=0 && t2<500){
                                if (world_map[t1][t2].type==0){
                                    world_map[t1][t2]=world_map[i][j];
                                    world_map[i][j].type=0;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    for (int i=0;i<500;i++){ //zombie
        for (int j=0;j<500;j++){
            if (world_map[i][j].type==2){
                if (world_map[i][j].v>1000){
                    bool m=0;
                    for (int k=0;k<4;k++){
                        int t1=i+dx[k],t2=j+dy[k];
                        if (t1>=0 && t1<500 && t2>=0 && t2<500){
                            if (world_map[t1][t2].type==0){
                                m=1;
                                break;
                            }
                        }
                    }
                    if (m){
                        while (1){
                            int go=rand()%4;
                            int t1=i+dx[go],t2=j+dy[go];
                            if (t1>=0 && t1<500 && t2>=0 && t2<500){
                                if (world_map[t1][t2].type==0){
                                    world_map[t1][t2].type=2;
                                    world_map[t1][t2].v=world_map[i][j].v-1000;
                                    world_map[i][j].v=1000;
                                    break;
                                }
                            }
                        }
                    }
                    else world_map[i][j].v=1000;
                }
                bool alone=0;
                for (int k=0;k<4;k++){
                    int t1=i+dx[k],t2=j+dy[k];
                    if (t1>=0 && t1<500 && t2>=0 && t2<500){
                        if (world_map[t1][t2].type==1){
                            alone=1;
                            if (world_map[t1][t2].v*(careful+1+ex)<world_map[i][j].v+min(world_map[i][j].v,world_map[t1][t2].v)*0.9){
                                world_map[t1][t2].type=2;
                                world_map[t1][t2].v=world_map[i][j].v+min(world_map[i][j].v,world_map[t1][t2].v)*0.9-world_map[t1][t2].v*(careful+1+ex);
                                world_map[i][j].type=0;
                            }
                            else if (world_map[t1][t2].v*(careful+1+ex)>world_map[i][j].v+min(world_map[i][j].v,world_map[t1][t2].v)*0.9){
                                world_map[i][j].type=0;
                                world_map[t1][t2].v=world_map[t1][t2].v*(careful+1+ex)-world_map[i][j].v+min(world_map[i][j].v,world_map[t1][t2].v)*0.9;
                            }
                            else{
                                world_map[i][j].type=0;
                                world_map[t1][t2].type=0;
                            }
                            break;
                        }
                    }
                }
                if (!alone){
                    bool m=0;
                    for (int k=0;k<4;k++){
                        int t1=i+dx[k],t2=j+dy[k];
                        if (t1>=0 && t1<500 && t2>=0 && t2<500){
                            if (world_map[t1][t2].type==0){
                                m=1;
                                break;
                            }
                        }
                    }
                    if (m){
                        while (1){
                            int go=rand()%4;
                            int t1=i+dx[go],t2=j+dy[go];
                            if (t1>=0 && t1<500 && t2>=0 && t2<500){
                                if (world_map[t1][t2].type==0){
                                    world_map[t1][t2]=world_map[i][j];
                                    world_map[i][j].type=0;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void main_loop(){
    for (;is_run();delay_fps(5)){
        draw_map();
        move();
        ex+=0.001;
//        cout << "flush" << endl;
	}
}

int main(){
    int seed;
    cout << "seed:";
    cin >> seed;
    map_init(seed);
    cout << 1 << endl;
    initgraph(500,500,0);
    main_loop();
    closegraph();
    return 0;
}
