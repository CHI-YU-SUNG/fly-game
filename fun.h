char f_name[8][100]={".\\resourse\\plane1.jpg",
                     	 ".\\resourse\\small_bullet01.jpg",
                     	 ".\\resourse\\outer-space.jpg",
                     	 ".\\resourse\\enemy.jpg",
                     	 ".\\resourse\\small_bullet_e.jpg",//4
                      	 ".\\Backup\\15\\06.bmp",
                     	 ".\\Backup\\15\\07.bmp",
                     	 ".\\Backup\\15\\08.bmp"};
void draw_bomb(int x,int y){
	setcolor(YELLOW);  //color of the circumference
  	int r=50;
  	setfillstyle(SOLID_FILL, RED);  
  	fillellipse(x-r/2,y-r/2, r, r);
}
void draw_bullet(plane *plane1,plane *enemy){//依照flying queue	
    		
		int fly_bullet_n=plane1->flying_q->Length;
		//if(fly_bullet_n){
			//printf("fly_bullets= %d \n",fly_bullet_n);				
		//}
		
	if(fly_bullet_n){
		
		node *cur;
			
		for(cur=plane1->flying_q->top->next	;cur!=NULL;)
		{
			bullet* cur_bullet=plane1->bb[cur->data];
			//printf("loop cur->data=%d\n",cur->data);
			if(!(cur_bullet->visible))
				plane1->set_b(cur->data);
				
						
			if((cur_bullet->x  + cur_bullet->vx) >860){
				cur_bullet->x=860;
				cur_bullet->visible=0;
			}	    		
	    	else if((cur_bullet->x  +  cur_bullet->vx)<0){
	    		cur_bullet->x=0;
	    		cur_bullet->visible=0;
			}
				
			else
				cur_bullet->x=(cur_bullet->x+cur_bullet->vx);
			
			if((cur_bullet->y  +  cur_bullet->vy) >800){
				cur_bullet->y=800;
				cur_bullet->visible=0;
			}	    		
	    	else if((cur_bullet->y  +  cur_bullet->vy)<-15){
	    		cur_bullet->y=0;
	    		cur_bullet->visible=0;
	    		/*printf("flying = \n");
	    		plane1->flying_q->show();
	    		printf("end flying \n");  		
	    		
	    		printf("magazine= \n");
				plane1->magazine_q->show();
				printf("end magazine \n");	*/   		
			}				
			else
				cur_bullet->y=(cur_bullet->y  +  cur_bullet->vy);
			///////boundary/////////	
			bullet* kb=cur_bullet;
			//////////collide/////////////
			for(int j=0;j<1;j++){
				int rangex=enemy->w,rangey=enemy->h;
				if(kb->x > enemy->x && kb->x < enemy->x+rangex &&
				   kb->y > enemy->y && kb->y < enemy->y+rangey ){
					kb->visible=0;
					//kb->boom=1;
					draw_bomb(kb->x,kb->y);
					enemy->armor=enemy->armor-kb->damage;//kb->damage;
					
					printf("flying = \n");
	    			plane1->flying_q->show();
	    			printf("end flying \n");  		
	    		
	    			printf("magazine= \n");
					plane1->magazine_q->show();
					printf("end magazine \n");	 
				}
			}
    		  			
			///////////////////////////////
			// printf("cur address=%x\n",cur);	
					
			if(kb->visible==1){
				  
    			//readimagefile(f_name[kb->file_count],kb->x		 ,kb->y
				//		        ,kb->x+kb->pic_w	,kb->y+kb->pic_h);//position wh(自position算起)
				putimage(kb->x,kb->y,plane1->img_b , COPY_PUT);
				cur=cur->next;
				//printf("cur next address=%x\n",cur);
			}else{//上面剛好被回收了 
				node *tmp =cur->next;//record who is after head,head will be recycled
				kb->vy=0;
				plane1->recycle_b(cur->data);//some problem
				cur=tmp;
				//printf("cur next address=%x\n",cur);	
				//if(cur==NULL)break;
			}		
    		
			
		}
	}
        
    
	//catch(const char* str){
	//	printf("錯誤: %s\n",str);
	//}
}
void draw_plane(plane *p1){	
    
    if((p1->x+p1->vx) >800)
    	p1->x=800;
    else if((p1->x+p1->vx)<0)
		p1->x=0;
	else
		p1->x=(p1->x+p1->vx);		
//	p1->x=(p1->x+p1->vx)%900;
	if((p1->y+p1->vy) >800)
    	p1->y=800;
    else if((p1->y+p1->vy)<p1->top_stop)
		p1->y=p1->top_stop;
	else
		p1->y=(p1->y+p1->vy);
		
     
    //readimagefile(f_name[p1->file_count],p1->x		 ,p1->y
						  // ,p1->x+p1->w,p1->y+p1->h);//position wh(自position算起)
        
    putimage(p1->x,p1->y,p1->img, COPY_PUT);
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
				p1->vy=p1->tmp_vy;
				p1->vy= p1->vy<0?p1->vy:p1->vy*-1;
				p1->vx=0;
				break;
			case 2://VK_DOWN
				p1->vy=p1->tmp_vy;
        		p1->vy= p1->vy>0?p1->vy:p1->vy*-1 ;
        		p1->vx=0;	
				break;
			case 4://VK_RIGHT
				p1->vx=p1->tmp_vx;        	
        		p1->vx= p1->vx>0?p1->vx:p1->vx*-1 ;
        		p1->vy=0;
        		break;
        	case 7://VK_LEFT
        		p1->vx=p1->tmp_vx;        	
        		p1->vx= p1->vx<0?p1->vx:p1->vx*-1 ;
        		p1->vy=0;
        		break;
        	case 5://up+right
        		p1->vy=p1->tmp_vy;
        		p1->vx=p1->tmp_vx;
        		p1->vy= p1->vy<0?p1->vy:p1->vy*-1;
        		p1->vx= p1->vx>0?p1->vx:p1->vx*-1 ;
        		break;
        	
		}		
		
        if(c[4]==0)p1->shoot=false;
        else{
        	p1->shoot=true;
			printf("shoot\n");
    		p1->b_timer++;
    		while(p1->b_timer>1){
    			p1->shoot_one();    			
    			p1->b_timer=0;
			}//control shoot rate
			//printf("shoot=%d \n",p1->shoot);        	
		}
        	
		
}
void draw_enemy(plane* enemy,plane* p){
	int a=rand()%100+1;
	if(a>0){
		enemy->shoot_one();
		
	}
	draw_bullet(enemy,p);
	draw_plane(enemy);
	
	if(enemy->x>700){
		enemy->vx=-enemy->vx;
		enemy->x=699;
	}
		
	if(enemy->x<100){
		enemy->vx=-enemy->vx;
		enemy->x=101;
	}
		
}
void ini_plane(plane* p){
	readimagefile( p->filename, 0, 0, p->w-1, p->h-1);
	int size = imagesize(0, 0, p->w-1, p->h-1);
	p->img = malloc(size);  //allocate space in memory
  	getimage(0, 0, p->w-1, p->h-1, p->img);
  	//////
  	readimagefile( p->bb[0]->filename, 0, 0, p->bb[0]->pic_w-1, p->bb[0]->pic_h-1);
	size = imagesize(0, 0, p->bb[0]->pic_w-1, p->bb[0]->pic_h-1);
	p->img_b = malloc(size);  //allocate space in memory
  	getimage(0, 0, p->bb[0]->pic_w-1, p->bb[0]->pic_h-1, p->img_b);
  	
}
void draw_blood(int x,int y,int blood){//blood0~100
	int rec[8];
  	int h=10,w=100;
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
  	float load;
  	if(blood)
  		load=960/blood;
  		
  	if(load)
  		blood=w/load;
  	
  	rec[2]=x+blood;
  	rec[4]=x+blood;
  	setfillstyle(SOLID_FILL, RED);  
  	fillpoly(4,rec);
}
int draw_frame(plane *p,plane *e){
	setcolor(RED);  //color of the circumference
  	//color of the circle
  		
  	draw_blood(500,50,p->armor);
  	//set the font name, orientation, and size
  	settextstyle(SIMPLEX_FONT, HORIZ_DIR, 2);
  	//set alignment
  	settextjustify(CENTER_TEXT, CENTER_TEXT);
  	//print "0" @ (maxx / 4, maxy / 2)
  	outtextxy(500, 50, "your blood");
  	outtextxy(700, 50, p->get_blood());
  	outtextxy(0, 50, "enemy blood");
  	outtextxy(200, 50, e->get_blood());
  	if(p->armor<=0 ) {
  		settextstyle(SIMPLEX_FONT, HORIZ_DIR, 20);
  		outtextxy(400, 500, "game over");
  		return 0;
	  }  		
  	else if(e->armor<=0){
  		settextstyle(SIMPLEX_FONT, HORIZ_DIR, 20);
  		outtextxy(400, 500, "you win");
  		return 2;
	  }  		
  	else
  		return 1;
}
