#include<stdio.h>
int main(){
	int/*A Multi Line Comment */i=1; printf( " This is block b1\n" );
	if(i > 0 /* comment betwen declaration*/){
		printf( " Multiline comment between declaration works\n" );
	}
return 0;
}