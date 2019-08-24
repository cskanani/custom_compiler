#include<stdio.h>
int main(){
	int i = 5; printf( " This is simple loop using self calling in DECISION\n" );
	d1:
	if(i >= 0){
		printf( " i is : %d\n", i); i -= 1;
		goto d1;
	}
return 0;
}