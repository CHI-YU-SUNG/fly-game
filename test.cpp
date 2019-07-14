/* hw2_2.c */
#include <stdio.h>      /* 把stdio.h含括進來 */
#include <stdlib.h>     /* 把stdlib.h含括進來 */
int main()          /* 主函數main()開始 */
{
   int i=5;             /* 宣告整數變數i，並設值為5 */
   printf("%d+%d=%d\n",i,i,i+i);    /* 呼叫printf()函數，印出並計算i+i的值 */
   system("pause");     /* 呼叫dos裡的pause指令，用來暫停程式的執行 */
   return 0;            /* main()結束，傳回0，表示函數正常結束 */
}

/* output---
5+5=10
----------*/
