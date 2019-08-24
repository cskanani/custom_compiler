#include<stdio.h>
int main(){
	int i=0, j=0; int a[5]; int max, secondmax; a[0] =3456; a[1]=32; a[2] =4542; a[3]=1; a[4]=56;
	mydec1:
	if(i < 5){
		if(a[i] > max){
			secondmax = max; max = a[i];
			myblock5:
			i++;
			goto mydec1;
		}
		else{
			if(a[i] > secondmax){
				secondmax = a[i];
				goto myblock5;
			}
			else{
				goto myblock5;
			}
		}
	}
	else{
		printf( " max = %d, secmax = %d\n", max, secondmax);
	}
return 0;
}