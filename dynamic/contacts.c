#include "contacts.h"

/* 字符串过滤(去掉末尾换行符) */
static void stringFliter(char *str){
	if( *(str + strlen(str) - 1) == '\n' ){
		*(str + strlen(str) - 1) = 0x00;
	}
}

static void saveNodeData(T_CONT *pdata, FILE *fp){
	if( strcmp(pdata->name,"HEAD_NODE") ){
		if( fwrite(pdata, 1, sizeof(T_CONT), fp) < 0 ){
			ERROR_HANDLE("fwrite");
			return ;
		}
	}
}

/* 链表初始化 */
void *linkListInit(T_CONT **pphead){

	/* 分配一段动态内存(开辟头节点空间) */
	*pphead = (T_CONT *)malloc(sizeof(T_CONT));
	if( *pphead == NULL ){
		ERROR_HANDLE("malloc");
		return NULL;
	}
	FILE* fp = fopen(SAVE_DATA_PATH, "rb");
	strcpy((*pphead)->name,"HEAD_NODE");	//标记头节点
	(*pphead)->next = NULL;					//头节点不存储数据
	
	return *pphead;
}

/* 链表释放 */
void linkListDestory(T_CONT *phead){

	T_CONT *p1 = phead, *p2 = NULL;
	FILE *fp = fopen(SAVE_DATA_PATH,"wb");
	if( !fp ){
		ERROR_HANDLE("fopen");
		return ;
	}

	while( p1->next ){
		p2 = p1;
		p1 = p1->next;
		saveNodeData(p2,fp);
		free(p2);
	}
	saveNodeData(p1,fp);
	free(p1);
	fclose(fp);
}

/* 插入节点 */
void insertNode(T_CONT *phead){

	T_CONT        *p1 = phead, *p2 = NULL;	//指针p1指向表头,p2指向空
	T_CONT *new_node  = NULL;				//指向新节点

	system("clear");
	new_node = (T_CONT *)malloc(sizeof(T_CONT));	//开辟新节点存储新联系人信息
	if( new_node == NULL ){
		ERROR_HANDLE("malloc");
		return;
	}
	/* 编辑联系人信息 */
	printf("Input new contact name         : ");
	fgets(new_node->name,sizeof(new_node->name),stdin);
	stringFliter(new_node->name);
	printf("                  phone-number : ");
	fgets(new_node->phone_number,sizeof(new_node->phone_number),stdin);
	stringFliter(new_node->phone_number);

	while( p1->next ){
		p2 = p1;		//p2记录p1上一次指向的位置
		p1 = p1->next;	//p1指向下一个节点
		if( strcmp(p1->name,new_node->name) > 0 ){
			/* 插入新节点 */
			p2->next = new_node;
			new_node->next = p1;
			return ;
		}
	}
	p1->next = new_node;
	new_node->next = NULL;
}
//删除联系人
void deleteNode(T_CONT *phead){
	printf("%s - %d - %s\n",__FILE__,__LINE__,__func__);
	T_CONT* p1 = phead, * p2 = NULL;	//指针p1指向表头,p2指向空
	T_CONT* new_node = NULL;				//指向新节点
	system("clear");
	new_node = (T_CONT*)malloc(sizeof(T_CONT));	//开辟新节点存储新联系人信息
	if (new_node == NULL) {
		ERROR_HANDLE("malloc");
		return;
	}
	/* 输入要删除联系人信息 */
	printf("Input deleteNode contact name         : ");
	fgets(new_node->name, sizeof(new_node->name), stdin);
	stringFliter(new_node->name);
	while (p1->next) {
		p2 = p1;		//p2记录p1上一次指向的位置
		p1 = p1->next;	//p1指向下一个节点
		if (strstr(p1->name, new_node->name) == p1->name) {
			/* 删除节点 */
			p2->next = p1->next;
			free(p1);
			return;
		}
	}
	printf("null");
	getchar();getchar();
}

/* 遍历链表中的节点 */
void printNode(T_CONT *phead){
	T_CONT *p = phead;
	int number = 0;
	system("clear");
	printf("NAME           PHONE-NUMBER\n");
	printf("==============================\n");
	while( p->next != NULL ){
		p = p->next;
		printf("%-15s%-11s\n",p->name,p->phone_number);
		number++;
	}
	printf("==============================\n");
	printf("A total of %d contacts\n",number);
	getchar();
};

void changeNode(T_CONT *phead){
	printf("%s - %d - %s\n",__FILE__,__LINE__,__func__);
	getchar();getchar();
};

void searchNode(T_CONT *phead){
	printf("%s - %d - %s\n",__FILE__,__LINE__,__func__);
	T_CONT* p1 = phead;	//指针p1指向表头,p2指向空
	T_CONT* new_node = NULL;				//指向新节点
	system("clear");
	new_node = (T_CONT*)malloc(sizeof(T_CONT));	//开辟新节点存储新联系人信息
	if (new_node == NULL) {
		ERROR_HANDLE("malloc");
		return;
	}
	/* 输入要查找联系人信息 */
	printf("Input searchNode contact name         : ");
	fgets(new_node->name, sizeof(new_node->name), stdin);
	stringFliter(new_node->name);
	while (p1->next) {
		p1 = p1->next;	//p1指向下一个节点
		if (strstr(p1->name, new_node->name) == p1->name) {
			printf("%-15s%-11s\n", p1->name, p1->phone_number);
			getchar();
			return;
		}
	}
	printf("null");
	getchar();getchar();
};

