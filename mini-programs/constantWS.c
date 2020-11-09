#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>

#define size 102400

struct tab_page{
        unsigned long  test[512];
};

int main(void){
        unsigned long i = 0;
        void *tab_data[size];
        int r;
        struct tab_page *temp;

	FILE *file;
	char tab_ad[25]; 
	
	file = fopen("/home/stella/linux/vtf-uio/app_add","a+");
        for(i = 0 ;i < size ; i++){
		memset(tab_ad,0,sizeof(tab_ad));
                tab_data[i] = (struct tab_page *) malloc(4096);
		sprintf(tab_ad,"\n%p",tab_data[i]);
		fwrite(tab_ad, sizeof(tab_ad), 1, file);
        }
	fclose(file);

         for( ; ; ){
                r = i++%size;
                //printf("%d\n",r);
                temp = tab_data[r];
                temp->test[r%512] = r;
        }
}
