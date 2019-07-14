#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // for PlaySound()

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
	
	return img;
}

int main()
{
    srand(time(NULL));
    initwindow(900,900);
    char f1[100]="123.JPG";
    char f2[100]="123.bmp";
        
    //readimagefile(f1,0,0,200,100);
    //readimagefile(f2,50,50,250,150);
    //PlaySound("./Sound/123.wav", NULL, SND_FILENAME|SND_LOOP|SND_ASYNC);
    //s
    // method 1
	char f3[8][100]={".\\Backup\\15\\01.bmp",
                     ".\\Backup\\15\\02.bmp",
                     ".\\Backup\\15\\03.bmp",
                     ".\\Backup\\15\\04.bmp",
                     ".\\Backup\\15\\05.bmp",
                     ".\\Backup\\15\\06.bmp",
                     ".\\Backup\\15\\07.bmp",
                     ".\\Backup\\15\\08.bmp"};
    int flag=0,x_path=0;
    int cur_page=0;
	
	while (!kbhit())
    {	/*
        setvisualpage(cur_page);
        
        cur_page = -cur_page + 1;    
        setactivepage(cur_page);
        */
        
        cleardevice();
        readimagefile(f3[flag],x_path,0,200+x_path,200);
        flag=(++flag)%8;
        x_path=(x_path+10)%600;
        _sleep(50);
    }
    
    getch();
    
    // method 2
	char f6[100] = ".\\Backup\\Pictures\\28418550_184023668881955_1155926220777455616_n.jpg";
	int wa=864, ha=864;
	int pn=9, wn=3, hn=3;
	
	void **img = new void*[pn];
	int k=0, w=wa/wn, h=ha/hn;
		
	setbkcolor(WHITE);
	cleardevice();
    readimagefile( f6, 0, 0, wa-1, ha-1);
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
	
	k = 0;
	while (!kbhit())
    {			
        setvisualpage(cur_page);//  0 1
        
        cur_page = -cur_page + 1;    
        setactivepage(cur_page);//  1 0
        		
		x_path = (x_path-10+1100)%1100;
		
        setbkcolor(WHITE);
        cleardevice();
    	putimage(x_path-250, 100, img[k], COPY_PUT);
        k = (++k)%pn;
        _sleep(40);
    }	
    
    getch();
    
    void **pic = getimage(".\\Backup\\Pictures\\12504182_951789811536742_1723270911_n.jpg", 1152, 864, 12, 4, 3, WHITE);
	setbkcolor(WHITE);
    cleardevice();
	putimage(500, 500, pic[0], COPY_PUT);
	setvisualpage(cur_page);
    
    getch();
    closegraph();
    return 0;
}
