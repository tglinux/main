#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {

	char   c;
	char   str[128] = {0};

	if (argc != 3) {
		printf("<usage> : %s [file_path] [string]\n", argv[0]);
		return -1;
	}

	FILE* fp = fopen(argv[1], "rb");

	if (fp == NULL) {
		perror("fopen failed");
		return -1;
	}

	//char* str = argv[2];
	int i = 0;
		//∂¡»°
	 while( (c = fgetc(fp)) != EOF ){
		 str[i] = c;		
		 printf("%c", str[i]);
		 i++;	
		}		 


	if (fclose(fp)) {
		printf("fclose failed.\n");
	}
	FILE* fp1 = fopen(argv[2], "wb");


	//fgets(str,sizeof(str),stdin);

	/* –¥»Î */
	for (int i = 0; i < strlen(str); i++) {
		fputc(str[i], fp1);
	}
	if (fclose(fp1)) {
		printf("fclose failed.\n");
	}

		//while(1){
		//	c = fgetc(fp);
		//	if( c == EOF ){
		//		break;
		//	}
		//	printf("%c",c);
		//}
	return 0;
}