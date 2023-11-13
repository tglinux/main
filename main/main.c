#include <stdio.h>
#include <stdlib.h>
int main(int argc ,char **argv)
{
	int i;
	printf("%s\n", argv[0]);
	printf("avg=%d\n", atoi(argv[1]));
	for (i = 0; i < argc; i++)
	{
		printf("%s\n", argv[i]);
	}
	return 0;

}