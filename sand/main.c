#include <stdio.h>
#include <string.h>

int main(int argc, char * argv)
{
	char * cstr = "Hello World!\n";
	int len = strlen(cstr);
	int i=0;
	for(i=0;i<len;i++)
		printf("%c",cstr[i]);
	printf("END\n");


	while(*cstr!=0)
	{
		printf("%c",*(cstr++));
	}
	printf("END\n");

}
