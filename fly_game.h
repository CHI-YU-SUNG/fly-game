#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

//#define debug 
void** getimage(char *filename, int big_width, int big_height,int wn, int hn);
struct point{
	int x,y;
};
struct node{
	int data; 
    struct node *next; 
};	
struct background{
	point pos;
	void *pic;
};
class goods{
	public:
		void *img_good=new void*;
		int x,y,vx,vy;
		char *function;		
		goods(int x1,int y1,char *s){
			function=new char[10];
			for(int i=0;i<10;i++){
				function[i]=s[i];
			}
			x=x1;
			y=y1;
		}
};
class queue{
	public:		
		node *top;
		node *tail;
		char *q_name;
		int _top;
		int Length;
		~queue(){
			free(top);
			free(tail);
		}
		queue(){
			Length=_top=0;
			top=(node*)malloc(sizeof(node));
			//tail=(node*)malloc(sizeof(node));
			//top->next=tail->next=NULL;
			top->next=NULL;
		}
		void change_top(node* bk){//k ,top->next->data
						
			node *pre=top;
			while(pre->next!=bk){
				pre=pre->next;
			}
			if(tail==bk){////////////important
				tail=pre;//if you change delete tail but not update tail 
			}					
			pre->next=pre->next->next;	///////////////////////		
			
			bk->next=top->next;//new_node->first			
			top->next=bk;//top->new_node->first		
			
		}
		int get_top(){
			return top->next->data;
		} 				
		int pop(){			
			if(top->next == NULL){     
				//tail=NULL;///////////////////////////////VERY IMPORTANT
				//printf("no elements i can pop!\n") ;
        		return -1; 
    		}else{
    			Length--;				 
    			node* fisrt_node = top->next;
				
				int tmp=fisrt_node->data;//top will be kill so memory first    		 
    			top->next = fisrt_node->next; //top->second node
    			free(fisrt_node);  			
				return tmp;	    			
				#ifdef debug
					printf("ele %d has been pop from %s\n",top->next->data,q_name);
					show();	
				#endif
				
			}
    		
		} 
		void push(int push_in){
			if(push_in==-1)return;//not allow
			node *newnode;			
			newnode=(node*)malloc(sizeof(node));
			newnode->data=push_in;
			newnode->next=NULL;
			
			if(top->next==NULL){
				top->next=newnode;//set top->newnode
				tail=newnode;
				Length=1;
				
				//printf("q %s top push\n",q_name);
				return;
			}			
						
			tail->next=newnode;//link newer
			tail=newnode;//change tail to newer
			Length++;
			#ifdef debug
				printf("ele %d has been pushed into %s\n",push_in,q_name);
				show();
			#endif
			
		}			
		void show(){
			node *cur=top->next;
			int count=0;
			if(cur==NULL)printf("empty now!\n");
			else{
				while(cur!=NULL){								
					printf("q %s %d= %d\n",q_name,count,cur->data);
					count++;
					if(count>50){
						printf("queue_print error \n");
						break;
					}				
						cur=cur->next;
				}
			}
		} 
};
class bullet{
	public:
		
		///void *img=new void*;
		char *filename=new char[60];
		int vy_o;		
		int x,y,vx,vy;
		int pic_h,pic_w;
		int precise=10;
		int damage;
		int visible;
		/*int collidesWith(plane *p1){
			if((p1->x-b_x<precise) && (p1->y-b_y<precise))
				return 1;
			else 
				return 0;
		}*/		
		bullet(){
			x=200,y=500;
			vx=0,vy_o=vy=-30;
			pic_h=23;//23
			pic_w=9;//9
			visible=0;
			//boom=0;//judge boom!
			damage=100;			
		}	
};
class plane{
	public:	
		void **img_b;		
		//void *imgg=new void*[2];
		void **img;
				
		//char *filename=new char[60];
		
		int x,y,w,h;
		int vx,tmp_vx,vy,tmp_vy;
		int full_armor,armor,bullet_n;
		int b_timer;
		int under_attack;
		int shoot;
		int top_stop;
		bullet *bb[15];
		queue * magazine_q;		
    	queue * flying_q;
    	char nn[2][10]={"magazine","fly"};
	public:
		plane(char* file_n,int w1,int h1){
			//////////////
			w=w1;
			h=h1;
			img=getimage(file_n,w*2, h,2,1);
			
			//////////////
			top_stop=200;
			x=0;
			y=500;
			tmp_vx=tmp_vy=30;
			vx=vy=0;
			
			full_armor=4500;
			armor=full_armor;
			bullet_n=15;
			b_timer=0;
			under_attack=0;
			
			magazine_q=new queue();
			flying_q=new queue();
			magazine_q->q_name=nn[0];
			flying_q->q_name=nn[1];
			
			//load bullet
			for(int i=0;i<bullet_n;i++){
				bb[i]=new bullet();
				magazine_q->push(i);
			}
		}
		void shoot_one(int x_one=0,int y_one=0,int vx_one=0){
			int b_pop=magazine_q->pop();
			if(b_pop!=-1){
				bb[b_pop]->x=x+w/2-bb[b_pop]->pic_w/2+x_one;
				bb[b_pop]->y=y+y_one;
				bb[b_pop]->vx=vx_one;
				flying_q->push(b_pop);
			}
							
				#ifdef debug
					printf("shooot: %d has been pushed into fly_q\n",tmp);
					printf("no enough bullets\n");
				#endif
				//return 1;				
			
				//return 0;//fail to shoot			
		}
		void recycle_all(){
			
		}
		node* recycle_b(node* k)
		{				
			//printf("recycle bullet %d\n",k);
			node *k_next=k->next;
			
			if(k!=flying_q->top->next){//if not at top	
				flying_q->change_top(k);
				#ifdef debug
					printf("aftre swap------------------\n");
					flying_q->show();
				#endif					
			}			
				//still recycle
				int tmp=flying_q->pop();			
				if(tmp==-1){
					printf("recycle error\n");
				}else{
					#ifdef debug
						printf("recycle bullet %d\n",tmp);
					#endif	
					magazine_q->push(tmp);//collide something or fly out range					
				}	
			return k_next;	
								
		}
		void set_b(int k){			
			bb[k]->visible=1;					
			//bb[k]->vy=bb[k]->vy_o;
			//printf("   set b   %d\n",k);
		}
		char* get_blood(){
			char b[6]={0};
			for(int j=0;j<6;j++)b[j]='0';
			b[5]='\0';
			int i=4,k=armor;
			while(k){
				b[i]='0'+k%10;
				k=k/10;
				i--;
			}
			
			return b;
		}
};

void** getimage(char *filename, int big_width, int big_height,int wn, int hn)// function overloading
{	
	int all_pic_number=wn*hn;
	void **img = new void*[all_pic_number];
	int k=0, w=big_width/wn, h=big_height/hn;		
	//setbkcolor(bkcolor);
	//cleardevice();
    readimagefile( filename, 0, 0, big_width-1, big_height-1);
	int size = imagesize(0, 0, w-1, h-1);//LITTLE PIC w h
	for (int i=0; i<hn; i++) {
		for (int j=0; j<wn; j++) {			
  			img[k] = malloc(size);  //allocate space in memory
  			getimage(w*j, h*i, w*(j+1)-1, h*(i+1)-1, img[k]);//read img to memory at img[k]
  			k++;
  			if (k==all_pic_number)
				break;  				
		}
		if (k==all_pic_number)
			break;  				
	}
	
	return img;
}

