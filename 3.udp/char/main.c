//#include <stdio.h>
//#include <string.h>
// char * StringToHex(char* str);	
// char strhex[100] = {0};
//int main(){
//
//	printf("%s\n", StringToHex("AAAA"));
//
//	return 0;
//}
// char * StringToHex(char* str)
//{
//	while (*str) {        //结束符判断
//			sprintf(strhex, "%s%2x", strhex, *(unsigned int*)str);
//			str++; //指向下一个字符
//	
//	}
//	return strhex;
//
//}

#include <stdio.h>
char hexStr[100] = {0}; // 初始化为空字符串
char * stringToHex(const char* str) {
    while (*str) {
        sprintf(hexStr, "%s%02X ", hexStr, (unsigned char)*str);
        str++;
    }
    return hexStr;
}

int main() {
    char str[100] = "";
    scanf("%s", str);
    printf("%s\n", stringToHex(str));
    return 0;
}


