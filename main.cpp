#include <graphics.h>
#include <conio.h>
#include <time.h>
#include "fly_game.h"
#include "link_list2.h"
#include <windows.h>
#include<stdio.h>
#include<stdlib.h> 
char bf_name[3][60]={
	".\\resourse\\b1.jpg",
	".\\resourse\\bi1.jpg",
	".\\resourse\\b_fire.BMP",
};
char f_name[10][100]={".\\resourse\\player_1.jpg",                     	 
                    	 ".\\resourse\\outer-space.jpg",
                     	 ".\\resourse\\enemy-1.jpg",                     	 
                      	 ".\\resourse\\green_p1.jpg",//3
                     	 ".\\resourse\\plane1b.jpg",//4
                     	 ".\\resourse\\BIGBOOM.BMP",
						  "star.jpg",
						  "enemy.jpg"};
//window set
point window_size;   
//window set  

//BOOM
void **boom_p;
//BOOM
//int enemy_delay=25;
//int enemy1_delay=25;
int b_move=0;
int bonus=0;
int speed=1;
link_l *blow_q=new link_l();
/////////////////////////////////////////////////

void** getimage(char *filename, int wa, int ha, int pn, int wn, int hn, int bkcolor)
{	
	void **img = new void*[pn];
	int k=0, w=wa/wn, h=ha/hn;
		
	setbkcolor(bkcolor);
	cleardevice();
    readimagefile( filename, 0, 0, wa-1, ha-1);
	int size = imagesize(0, 0, w-1, h-1);
	for (int i=0; i<hn; i++) {
		for (int j=0; j<wn; j++) {			
  			img[k] = malloc(size);  //allocate space in memory
  			getimage(w*j, h*i, w*(j+1)-1, h*(i+1)-1, img[k]);
  			k++;
  			if (k==pn)
  				break;
		}
		if (k==pn)
  				break;
	}
	
	return img;   //回傳二維指標 
}

void set_enemy(class plane *enemy)
{
	enemy->top_stop=30;//control how up plane can go
    enemy->armor=enemy->full_armor=200;
    enemy->y=70;
    enemy->x=350;
    enemy->top_stop=70;    
    enemy->vx=80;
    enemy->vy=1;
    enemy->tmp_vy=5;
    //enemy->full_armor=600;
    enemy->img_b=getimage(bf_name[1],enemy->bb[0]->pic_w*2,enemy->bb[0]->pic_h,2,1); 
    for(int i=0;i<enemy->bullet_n;i++){
    	enemy->bb[i]->vy_o=enemy->bb[i]->vy=-enemy->bb[i]->vy;      	
	//enemy->bb[i]->damage=100;    	
	}
} 
void draw_bonus(void **bomb,void **star,int bx[],int by[],int n,int bomb_x[],int bomb_y[])
{
	
	
	
	int i;
	for(i=0;i<n;i++)
	{
		bomb_x[i]+=2;
		bomb_y[i]-=2;
		if(bomb_x[i]>=700)
			bomb_x[i]=rand()%500+50;
		if(bomb_y[i]<=0)
			bomb_y[i]=rand()%500+50;	
		bx[i]--;
		by[i]--;
		putimage(bx[i]-15,by[i]-15,star[0], AND_PUT);
    	putimage(bx[i]-15,by[i]-15,star[1], OR_PUT);
    	putimage(bomb_x[i]-15,bomb_y[i]-15,bomb[0], AND_PUT);
    	putimage(bomb_x[i]-15,bomb_y[i]-15,bomb[1], OR_PUT);
	}
}
void reset_bonus(int *bx,int *by)
{
	*bx=800;
	*by=800;
}

void draw_bomb(link_l *q){
	node_b *cur=q->top->next;
	while(cur!=NULL){
		putimage(cur->x,cur->y,boom_p[cur->state+5],OR_PUT);
		putimage(cur->x,cur->y,boom_p[cur->state],AND_PUT);			
		cur->state++;
		if(cur->state>4){//end of animation
			q->pop();
		}
		cur=cur->next;
	}  	
}

void speed_up(plane *p1)
{
	if(p1->tmp_vx<90)
		p1->tmp_vx+=10;
	if(p1->tmp_vy<90)
		p1->tmp_vy+=10;	
}
void speed_down(plane *p1)
{
	if(p1->tmp_vx>=30)
		p1->tmp_vx=20;
	if(p1->tmp_vy>=30)
		p1->tmp_vy=20;	
}
void draw_bullet(plane *plane1,plane *enemy){//依照flying queue	
    		
		int fly_bullet_n=plane1->flying_q->Length;
		#ifdef debug
		if(fly_bullet_n)
		{	
			//printf("fly_bullets= %d \n",fly_bullet_n);				
		}
		#endif		
	if(fly_bullet_n){
		
		node *cur;
		int left_bound=window_size.x;
		int down_bound=window_size.y;
		//node* died_bullet[plane1->bullet_n]={0};		
		for(cur=plane1->flying_q->top->next	;cur!=NULL;)
		{
			bullet* cur_bullet=plane1->bb[cur->data];
			//printf("loop cur->data=%d\n",cur->data);
			if(!(cur_bullet->visible))
				plane1->set_b(cur->data);
				
						
			if((cur_bullet->x  + cur_bullet->vx) >left_bound){
				cur_bullet->x=left_bound;
				cur_bullet->visible=0;
			}	    		
	    	else if((cur_bullet->x  +  cur_bullet->vx)<0){
	    		cur_bullet->x=0;
	    		cur_bullet->visible=0;
			}
				
			else
				cur_bullet->x=(cur_bullet->x+cur_bullet->vx);
			
			if((cur_bullet->y  +  cur_bullet->vy) >down_bound){
				cur_bullet->y=down_bound;
				cur_bullet->visible=0;
			}	    		
	    	else if((cur_bullet->y  +  cur_bullet->vy)<-15){
	    		cur_bullet->y=0;
	    		cur_bullet->visible=0;	    		 		
			}				
			else
				cur_bullet->y=(cur_bullet->y  +  cur_bullet->vy);
			///////boundary/////////	
			
			//////////collide/////////////
			for(int j=0;j<1;j++){
				int rangex=enemy->w,rangey=enemy->h;
				if(cur_bullet->x > enemy->x && cur_bullet->x < enemy->x+rangex &&
				   cur_bullet->y > enemy->y && cur_bullet->y < enemy->y+rangey ){
					cur_bullet->visible=0;
					//cur_bullet->boom=1;					
					blow_q->push(cur_bullet->x,cur_bullet->y,0);
					enemy->armor=enemy->armor-cur_bullet->damage;//cur_bullet->damage;					
					//plane1->flying_q->show();	    			
					//plane1->magazine_q->show();					
				}
			}    		  			
			
			// printf("cur address=%x\n",cur);	
					
			if(cur_bullet->visible==1){				  
    					   //     ,cur_bullet->x+cur_bullet->pic_w	,cur_bullet->y+cur_bullet->pic_h);//position wh(自position算起)
							
				
				putimage(cur_bullet->x,cur_bullet->y,plane1->img_b[0] , AND_PUT);
				putimage(cur_bullet->x,cur_bullet->y,plane1->img_b[1] , OR_PUT);
								
				cur=cur->next;
				//printf("cur next address=%x\n",cur);
			}else{				
				node *nn=cur->next;
				cur=plane1->recycle_b(cur);//some problem
				cur=nn;
				//printf("cur next address=%x\n",cur);	
				//if(cur==NULL)break;
			}		
    		
			
		}
		//puts("end print bullet");
	}
    
}
void draw_plane(plane *p1){	
    
    if((p1->x+p1->vx) >window_size.x-p1->w)
    	p1->x=window_size.x-p1->w;
    else if((p1->x+p1->vx)<0)
		p1->x=0;
	else
		p1->x=(p1->x+p1->vx);		
//	p1->x=(p1->x+p1->vx)%900;
	if((p1->y+p1->vy) >window_size.y-p1->h)
    	p1->y=window_size.y-p1->h;
    else if((p1->y+p1->vy)<p1->top_stop)
		p1->y=p1->top_stop;
	else
		p1->y=(p1->y+p1->vy);        
    putimage(p1->x,p1->y,p1->img[0], AND_PUT);
    putimage(p1->x,p1->y,p1->img[1], OR_PUT);
	//COPY_PUT, XOR_PUT, OR_PUT, AND_PUT, NOT_PUT
    //puts("end draw plane");
}

void key_control(plane *p1){
	char c[5]={0};
	p1->shoot=false;
	enum{
		up,down,right,left
	};
	int acm=0;
		c[0]=GetAsyncKeyState(VK_UP)==0?0:1;
        c[1]=GetAsyncKeyState(VK_DOWN)==0?0:2;
        c[2]=GetAsyncKeyState(VK_RIGHT)==0?0:4;
        c[3]=GetAsyncKeyState(VK_LEFT)==0?0:7;
        c[4]=GetAsyncKeyState(VK_SPACE)==0?0:50;
		for(int i=0;i<4;i++)
			acm=acm+c[i];
        
        switch (acm)
        {
			case 1://VK_UP
				p1->vy=-p1->tmp_vy;
				//p1->vy= p1->vy<0?p1->vy:p1->vy*-1;
				p1->vx=0;
				break;
			case 2://VK_DOWN
				p1->vy=p1->tmp_vy;
        		//p1->vy= p1->vy>0?p1->vy:p1->vy*-1 ;
        		p1->vx=0;	
				break;
			case 4://VK_RIGHT
				p1->vx=p1->tmp_vx;        	
        		//p1->vx= p1->vx>0?p1->vx:p1->vx*-1 ;
        		p1->vy=0;
        		break;
        	case 7://VK_LEFT
        		p1->vx=-p1->tmp_vx;        	
        		//p1->vx= p1->vx<0?p1->vx:p1->vx*-1 ;
        		p1->vy=0;
        		break;
        	case 5://up+right
        		p1->vy=p1->tmp_vy;
        		p1->vx=p1->tmp_vx;
        		p1->vy= p1->vy<0?p1->vy:p1->vy*-1;
        		p1->vx= p1->vx>0?p1->vx:p1->vx*-1 ;
        		break;
        	
		}		
		if(c[0]==0 && c[1]==0 && c[2]==0 && c[3]==0){//no press ways
			p1->vy=0;
			p1->vx=0;	
			//puts("noxxxx");
		}
        if(c[4]==0)p1->shoot=false;
        else{
        	p1->shoot=true;
			//printf("shoot\n");
    		p1->b_timer++;
    		while(p1->b_timer>2){
    			p1->shoot_one(5,0,0);
    			p1->shoot_one(-5,0,0);
    				//printf("i fired\n");
					
    			p1->b_timer=0;
			}//control shoot rate
			//printf("shoot=%d \n",p1->shoot);        	
		}
        	
		
}

void draw_enemy(plane* enemy,plane* p,int level){
		
	if(enemy->x+enemy->w>window_size.x){
		enemy->vx=-enemy->vx;
		enemy->x=window_size.x-enemy->w;
	}
		
	if(enemy->x<10){
		enemy->vx=-enemy->vx;
		enemy->x=11;
	}
	
	if(level>=2){//track player's plane
		int a=rand()%100+1;	
		
		//enemy->tmp_vx=5*level;
		//enemy->vx=enemy->tmp_vx;
		
		int attack_density=rand()%5;
		enemy->b_timer++;
		if(a>50 && enemy->b_timer>attack_density && abs(p->x-enemy->x)<200){
			enemy->b_timer=0;			
			enemy->shoot_one();//printf("enemy fired!\n");
			enemy->shoot_one(10,0,5);
			enemy->shoot_one(-10,0,-5);			
			//enemy->shoot_one(10,0,10);
			//enemy->shoot_one(-10,0,-10);	
		}		
			int error=p->x-enemy->x;
			//enemy->vx=enemy->tmp_vx;
			int k;
			if(error<300 || error >-300){//out of detect
				//if(level<10)
				//k=10-level;
				if(error<20 && error>-20){
					enemy->vx=error>0?5:-5;	
				}else
					enemy->vx=(500/error);	
			}else{
				enemy->vx=0;
			}
					
		
		
	}else{
		int a=rand()%100+1;	
		enemy->b_timer++;
		if(a>60 && enemy->b_timer>5){
			enemy->b_timer=0;
			enemy->shoot_one();//printf("enemy fired!\n");			
				
		}	
	}
	draw_bullet(enemy,p);	
	draw_plane(enemy);
}
/*void ini_plane(plane* p){
	/*readimagefile( p->filename, 0, 0, p->w-1, p->h-1);//read plane picture  into memory
	int size = imagesize(0, 0, p->w-1, p->h-1);
	p->img = malloc(size);  
  	getimage(0, 0, p->w-1, p->h-1, p->img);  	
  	
	//read plane pic
  	p->img=getimage(p->filename,p->w*2, p->h,2,1);	    	
  	//read bullet  	
  	p->img_b=getimage(p->bb[0]->filename,p->bb[0]->pic_w*2,p->bb[0]->pic_h,2,1);  	
}*/
void draw_blood(int x,int y,int full,int blood){//blood0~100
	int rec[8];
  	
  		int h=10,w=100;
  		double bd=(double)blood,ratio=(double)w/(double)full;
		rec[0]=x;
	  	rec[1]=y;  	
	  	rec[2]=x+w;
	  	rec[3]=y;  	
	  	rec[4]=x+w;
	  	rec[5]=y+h;  	
	  	rec[6]=x;
	  	rec[7]=y+h;	
	  	
	  	setcolor(BLACK);
	  	drawpoly(4,rec);
	  	
		bd*=ratio;
	  	
	  	rec[2]=x+(int)bd;
	  	rec[4]=x+(int)bd;
	  	setfillstyle(SOLID_FILL, RED);  
	  	fillpoly(4,rec);
	
}
int draw_frame(plane *p,plane *e,int level){
	setcolor(RED);  
  	//set the font name, orientation, and size
  	settextstyle(SIMPLEX_FONT, HORIZ_DIR, 2);  	
  	settextjustify(CENTER_TEXT, CENTER_TEXT);
  	//print "0" @ (maxx / 4, maxy / 2)
  	int half=window_size.x/2;
  	outtextxy(half, 50, "your blood");
  	outtextxy(half+200, 50, p->get_blood());
  	
  //	outtextxy(0, 50, "enemy blood");
  //	outtextxy(200, 50, e->get_blood());
  	
  	char l[30];
  	itoa(level,l,10);
  	outtextxy(10, 20, l);
  	
  	draw_blood(half+200,55,p->full_armor,p->armor);
  	//draw_blood(200,55,e->full_armor,e->armor);
  	if(p->armor<=0 ) {
  		setcolor(WHITE);		
  		settextstyle(SIMPLEX_FONT, HORIZ_DIR, 20);
  		outtextxy(window_size.x/2-10,window_size.y/2, "game over");
  		return 0;
	  }  		
  	else if(e->armor<=0){
  		//setcolor(WHITE);
  		//settextstyle(SIMPLEX_FONT, HORIZ_DIR, 20);
  		//outtextxy(window_size.x/2-10,window_size.y/2, "you win");
  		return 2;
	  }  		
  	else
  		return 1;
}
void draw_background(background &bp,background &bp1){
	//bp.pos.y=0;
	char b[50];
	char s[50];
	
	bp.pos.y-=2;
	bp1.pos.y=bp.pos.y+700;
	if(bp.pos.y<=-700)
		bp.pos.y=0;


	putimage(bp.pos.x, bp.pos.y ,bp.pic, COPY_PUT);
	putimage(bp1.pos.x, bp1.pos.y ,bp1.pic, COPY_PUT);
	setcolor(GREEN);
	if(bonus<0)
		bonus=0;
	if(speed<0)
		speed=0;	
	itoa(bonus,b,10);
	itoa(speed,s,10);
	outtextxy(500,20,"bonus: ");
	outtextxy(550,20,b);
	outtextxy(100,20,"speed level: ");
	if(speed>=8)
		outtextxy(220,20,"max!!!");
	else if(speed<=2)
		outtextxy(220,20,"min!!!");
	else
		outtextxy(200,20,s);
	
}
void reset(plane *p1,plane*p2,int &enemy_delay){
	p1->full_armor=200;
	p1->armor=p1->full_armor;	
	p1->y=-300;

//	p2->armor=p2->full_armor;
	enemy_delay=25;
	//enemy1_delay=25;
	//_sleep(1000);
}

void reset2(plane *e1,plane *e2,plane *p1)
{
	e1->full_armor=400;
	e1->armor=e1->full_armor;	
	e1->y=-300;
	
	e2->full_armor=200;
	e2->armor=e2->full_armor;	
	e2->y=-300;

//	p2->armor=p2->full_armor;
	//enemy_delay=25;
	//_sleep(1000);
}
void draw_line(class plane *plane1,class plane *enemy)

{
	int i,x,y,n=0;
	//setlinestyle(SOLID_LINE, USERBIT_LINE,25);
	if(bonus>=1)
	{
		if(GetAsyncKeyState(0x5A))
		{
			line(plane1->x+50,plane1->y,enemy->x+30,enemy->y);
			putpixel(350,350,YELLOW);
			if((plane1->x)<enemy->x)
			
				arc((plane1->x+enemy->x)/2,(plane1->y+enemy->y)/2,90,270,abs((plane1->y-enemy->y))*0.5);
			else
				 arc((plane1->x+enemy->x)/2,(plane1->y+enemy->y)/2,270,90,abs((plane1->y-enemy->y))*0.5);
			/*
			if(abs((plane1->x)-(enemy->x))<10)
				line(plane1->x+50,plane1->y,enemy->x+30,enemy->y);
			else if((plane1->x)>(enemy->x))
				arc(((plane1->x+100)+(enemy->x+100))/2,(plane1->y+enemy->y)/2,270,90,abs(plane1->y-enemy->y)-200);
			else if((plane1->x)<(enemy->x))
				arc(((plane1->x+100)+(enemy->x+100))/2,(plane1->y+enemy->y)/2,90,270,abs(plane1->y-enemy->y)-200);	
			*/
			
			for(i=1;i<=1000;i++)
			{
				n=i/1000;
				x=(1-n)*(1-n)*plane1->x+2*n*(1-n)*abs((enemy->x+plane1->x)/2)+n*n*enemy->x;
				y=(1-n)*(1-n)*plane1->y+2*n*(1-n)*(abs((enemy->y+plane1->y)/2)+200)+n*n*enemy->y;
				//putpixel(x,y,YELLOW);
				//putpixel(i,200,YELLOW);
			}
			
		}
	}
	
}

void background1(int &cur_page)
{
	int flag=1,entergame=0;
	//char s1[10];


	void **picfirst1 = getimage("first1.jpg", 700, 650, 1, 1, 1, BLACK);
	setbkcolor(BLACK);
  
	putimage(0, 0, picfirst1[0], COPY_PUT);

	
	do
	{
		
			while(!kbhit())
			{
				setvisualpage(cur_page);//  0 1
        
        		cur_page = -cur_page + 1;    
        		setactivepage(cur_page);
				//void **picfirst1 = getimage("first1.jpg", 1000, 650, 1, 1, 1, BLACK);
				void **picfirst2 = getimage("first2.jpg", 700, 650, 1, 1, 1, BLACK);
				void **picfirst3 = getimage("first3.jpg", 700, 650, 1, 1, 1, BLACK);
					setbkcolor(WHITE);
				   	
				if(flag==1)   	
					putimage(0, 0, picfirst1[0], COPY_PUT);
				else if(flag==2)
					putimage(0, 0, picfirst2[0], COPY_PUT);
				else if(flag==3)
					putimage(0, 0, picfirst3[0], COPY_PUT);
					
			}
		
		//listchoose=getche();
		
			getch();
			if(GetAsyncKeyState(VK_UP)!=0)
			{
					
				if(flag==2)
				{
					setvisualpage(cur_page);//  0 1
        
        			cur_page = -cur_page + 1;    
        			setactivepage(cur_page);
					void **picfirst1 = getimage("first1.jpg", 700, 650, 1, 1, 1, BLACK);
					setbkcolor(WHITE);
					putimage(0, 0, picfirst1[0], COPY_PUT);
					flag=1;	
				}
				
				else if(flag==3)
				{
					setvisualpage(cur_page);//  0 1
        
        			cur_page = -cur_page + 1;    
        			setactivepage(cur_page);
					void **picfirst2 = getimage("first2.jpg", 700, 650, 1, 1, 1, BLACK);
					setbkcolor(BLACK);
					putimage(0, 0, picfirst2[0], COPY_PUT);
					setvisualpage(cur_page);
					flag=2;
				}	
				else ;
			}
			if(GetAsyncKeyState(VK_DOWN)!=0)
			{
			
				if(flag==1)
				{
					setvisualpage(cur_page);//  0 1
        			
        			cur_page = -cur_page + 1;    
        			setactivepage(cur_page);
					void **picfirst2 = getimage("first2.jpg", 700, 650, 1, 1, 1, BLACK);
					setbkcolor(BLACK);
					putimage(0, 0, picfirst2[0], COPY_PUT);
					setvisualpage(cur_page);
					flag=2;
				}
				else if(flag==2)
				{
					setvisualpage(cur_page);//  0 1
        
        			cur_page = -cur_page + 1;    
        			setactivepage(cur_page);
					void **picfirst3 = getimage("first3.jpg", 700, 650, 1, 1, 1, BLACK);
					setbkcolor(BLACK);
					putimage(0, 0, picfirst3[0], COPY_PUT);
					setvisualpage(cur_page);
					flag=3;
				}
			}
			
		if(GetAsyncKeyState(0X0D)!=0)
		{	
			entergame=1;
		}
	}while(!entergame==1);
	
    void **picspace = getimage("space.jpg", 700, 650, 1, 1, 1, WHITE);
	setbkcolor(BLACK);
    //cleardevice();
	putimage(0, 0, picspace[0], COPY_PUT);
	settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 10);
	setcolor(RED);
	outtextxy(50, 350, "Start Game !!!");
	setvisualpage(cur_page);
	delay(1000);
}
int main()
{
	int enemy_delay=25;
	int enemy1_delay=25;
	int enemy2_delay=25;
	int bx[2]={800,800},by[2]={800,800};//invisible
	int j;
	int bomb_x[2]={100,150},bomb_y[2]={650,650};
    srand(time(NULL));    
	window_size.x=window_size.y=700; 
	initwindow(window_size.x,window_size.y,"test 1"); 
	void **star=getimage("star.jpg",100,100,2,1);
	void **bomb=getimage("bomb.jpg",100,100,2,1);
	int cur_page=1;
	background1(cur_page);
    plane *plane1=new plane(f_name[0],100,100);
    plane1->y=500;////////////////////////////////////////
    plane1->vx=80;
    plane1->tmp_vx=20;
    plane1->tmp_vy=20;
    plane1 ->top_stop=200;
    plane1->armor=plane1->full_armor=95000;
    //plane1->bb[0]->filename=bf_name[0];
	plane1->img_b=getimage(bf_name[2],plane1->bb[0]->pic_w*2,plane1->bb[0]->pic_h,2,1); 	
	//ini_plane(plane1);
	//printf("p1 file name=%s\n",plane1->filename);
    ////////// enemy seeting
    plane *enemy=new plane(f_name[3],70,70);//3 5    
    enemy->top_stop=50;//control how up plane can go
    enemy->armor=enemy->full_armor=200;
    enemy->y=70;
    enemy->x=350;
    enemy->top_stop=70;    
    enemy->vx=80;
    enemy->vy=0;
    enemy->tmp_vy=5;
    //enemy->full_armor=600;
    enemy->img_b=getimage(bf_name[1],enemy->bb[0]->pic_w*2,enemy->bb[0]->pic_h,2,1); 
    for(int i=0;i<enemy->bullet_n;i++){
    	enemy->bb[i]->vy_o=enemy->bb[i]->vy=-enemy->bb[i]->vy;      	
		//enemy->bb[i]->damage=100;    	
	}
	plane *enemy1=new plane(f_name[2],70,70);
	set_enemy(enemy1);
	plane *enemy2=new plane(f_name[7],70,70);
	set_enemy(enemy2);	
    //ini_plane(enemy);	
    ////////////////enemy setting
    
    ////////back setting
   	background back_pic;
   	background bk1;
	readimagefile( f_name[1], 0, 0, 3394, 2056);//read plane picture  into memory	
	back_pic.pic = malloc(imagesize(0, 0, 3394, 2056));
	readimagefile( f_name[1], 0, 0, 3394, 2056);
	bk1.pic = malloc(imagesize(0, 0, 3394, 2056)); 
  	getimage(0, 0, 3394, 2056, back_pic.pic);
	getimage(0, 0, 3394, 2056, bk1.pic); 	
	back_pic.pos.x=0,back_pic.pos.y=0;
	bk1.pos.x=0,bk1.pos.y=0;
	//boom_p=getimage(f_name[7], 100,50,4,2);       
    boom_p=getimage(f_name[5], 500,200,5,2);
	int game_state=1; 
    int level=2;
    int key_z;
	while(game_state!=-1)//
    {	
			cleardevice();
			//setbkcolor(WHITE);			
			draw_background(back_pic,bk1);
								     
			key_control(plane1); 			       
    		if(enemy_delay==0){
    			draw_enemy(enemy,plane1,level);
			
			}else{
				enemy_delay--;
				if(enemy_delay==1){
					enemy->y=70;
					enemy->x=rand()%window_size.x+1	;
				}
			}
			if(enemy1_delay==0){
    			draw_enemy(enemy1,plane1,level);
			
			}else{
				enemy1_delay--;
				if(enemy1_delay==1){
					enemy1->y=70;
					enemy1->x=rand()%window_size.x+1	;
				}
			}
			
			if(enemy2_delay==0){
    			draw_enemy(enemy2,plane1,level);
			
			}else{
				enemy2_delay--;
				if(enemy2_delay==1){
					enemy2->y=70;
					enemy2->x=rand()%window_size.x+1	;
				}
			}							   	
			if(GetAsyncKeyState(0x48)&&(bonus>=2))
			{
				plane1->armor+=500;
				bonus-=2;
			}
			
			if(enemy1->y>=500)
				enemy1->y=50;
			if(enemy2->y>=500)
				enemy2->y=50;	

			draw_plane(plane1);    		
			draw_bullet(plane1,enemy);
			draw_bullet(plane1,enemy1);
			draw_bullet(plane1,enemy2);				
    		draw_bomb(blow_q);
    		draw_bonus(bomb,star,bx,by,2,bomb_x,bomb_y);
    		//draw_line(plane1,enemy);
			game_state=draw_frame(plane1,enemy,level);//difficulty=1
			int defeat=draw_frame(plane1,enemy1,level);
			int defeat2=draw_frame(plane1,enemy2,level);
				
			for(j=0;j<2;j++)
			{
				if(((plane1->x-bx[j])*(plane1->x-bx[j])+(plane1->y-by[j])*(plane1->y-by[j]))<=2000)
				{
					//printf("get!!!\n");
					bonus++;
					reset_bonus(&bx[j],&by[j]);
					speed_up(plane1);
					speed++;
				}
				else
					;//printf("not get!!\n");
				
				if(((plane1->x-bomb_x[j])*(plane1->x-bomb_x[j])+(plane1->y-bomb_y[j])*(plane1->y-bomb_y[j]))<=2000)
				{
					plane1->armor-=300;
					reset_bonus(&bomb_x[j],&bomb_y[j]);
					speed--;
					speed_down(plane1);
					bonus--;	
				}	
				
			}

					
					
        	//_sleep(1);		
		    //buffer
        	setvisualpage(cur_page);//  0 1     
			cur_page= -cur_page + 1;   
    		setactivepage(cur_page);//  1 0 
    		
			while(game_state!=1||defeat!=1||defeat2!=1)
			{//end of game
    			
    			if(GetAsyncKeyState(0x51)&&game_state==0){//0x51 Q
    				game_state=-1;//go out
    				
    				break;
				}else if(game_state==2&&defeat==2){//G					
					bx[0]=500;
					by[0]=500;
					bx[1]=600;
					by[1]=600;
					level++;
					bonus++;
					reset2(enemy,enemy1,plane1);
					//reset(enemy1,plane1);
					//setbkcolor(WHITE);
					break;
				}
				else if(game_state==2)
				{
					bx[0]=500;
					by[0]=500;

					level++;
					//bonus++;
					reset(enemy,plane1,enemy_delay);
					//draw_bonus()
					break;
				}
				else if(defeat==2) 
				{

					bx[1]=600;
					by[1]=600;
					level++;
					//bonus++;
					reset(enemy1,plane1,enemy1_delay);
					break;
					
				}
				else if(defeat2==2)
				{
					bx[2]=650;
					by[2]=650;
					level++;
					//bonus++;
					reset(enemy2,plane1,enemy2_delay);
					break;
				}
    			
			}

    }   	
    closegraph();
    return 0;
}


