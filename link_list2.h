#include <stdio.h>
#include <stdlib.h>
struct node_b{
	//int **data=new *int[3];
	int x,y,state;
	//data[0]=&x,data[1]=&y,data[2]=&state; //get data position
    struct node_b *next; 
};	
class link_l{
	public:		
		node_b *top;
		node_b *tail;		
		int Length;
		~link_l(){
			free(top);
			free(tail);
		}
		link_l(){
			Length=0;
			top=(node_b*)malloc(sizeof(node_b));			
			top->next=NULL;
		}						
		void pop(){			
			if(top->next == NULL){    				
        		 
    		}else{
    			Length--;				 
    			node_b* fisrt_node_b = top->next;				
				top->next = fisrt_node_b->next; //top->second node_b
    			free(fisrt_node_b);						
			}
    		
		} 
		void push(int x1,int y1,int state1){//0~3			
			//int **tmp;
			//*tmp[0]=x1,*tmp[1]=y1,*tmp[2]=state;
			node_b *newnode_b;			
			newnode_b=(node_b*)malloc(sizeof(node_b));
			newnode_b->x=x1,newnode_b->y=y1,newnode_b->state=state1;
			newnode_b->next=NULL;
			
			if(top->next==NULL){
				top->next=newnode_b;//set top->newnode_b
				tail=newnode_b;
				Length=1;						
				return;
			}							
			tail->next=newnode_b;//link newer
			tail=newnode_b;//change tail to newer
			Length++;			
		}			
		
};
