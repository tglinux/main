#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define ERR_MSG(msg)                              \
    do {                                          \
        fprintf(stderr, "line : %d\n", __LINE__); \
        perror(msg);                              \
    } while (0)

#define IP "192.168.0.60"
#define PORT 69

void menu(void); // ��ʾTFTP�ļ��ͻ��˹��ܲ˵�
int Download_TFTP(int cfd, struct sockaddr_in sin); // �����ļ�
int Upload_TFTP(int cfd, struct sockaddr_in sin); // �ϴ��ļ�
int upload(int sfd, struct sockaddr_in sin);

int main(int argc, char const* argv[])
{
    int choice; // ����ѡ��
    // 1.������ʽ�׽���
    int cfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (cfd < 0) {
        ERR_MSG("socket error");
        return -1;
    }

    // ����������Ϣ�ṹ��
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    sin.sin_addr.s_addr = inet_addr(IP);

    while (1) {
        menu();
        scanf("%d", &choice);
        getchar();
        switch (choice) {
        case 1:
            Download_TFTP(cfd, sin);
            break;
        case 2:
            Upload_TFTP(cfd, sin);
            break;
        case 3:
            return -1;
        default:
            printf("\n�����ʽ����!!!\n���ڹ���1 2 3��ѡ��!!!\n\n");
            sleep(1);
            break;
        }
    }

    return 0;
}

void menu(void)
{
    printf("\n**************************TFTP�ļ��ͻ���**************************\n");
    printf("***************************���ܲ˵�Ŀ¼***************************\n");
    printf("*\t\t\t1.�����ļ�\t\t\t\t *\n");
    printf("*\t\t\t2.�ϴ��ļ�\t\t\t\t *\n");
    printf("*\t\t\t3.�˳�ϵͳ\t\t\t\t *\n");
    printf("******************************************************************\n");
    printf("��ѡ����Ҫִ�еĲ�����");
}

int Download_TFTP(int cfd, struct sockaddr_in sin)
{
    ssize_t res; // �洢����ֵ
    ssize_t Data_Num; // �������ݰ���ʵ�ʵĴ�С
    char ACK1, ACK2; // ��������������ݰ��п�ı��
    int fd; // ���ص��ļ�������
    unsigned short opcode; // �洢������
    int size; // ����Э����Ĵ�С
    char buf[516] = { 0 }; // �洢TFTP��5��Э���
    char filename[20] = { 0 }; // �洢�����ļ�������

    // ���Ͷ�����
    printf("���������ص��ļ����ƣ�");
    fgets(filename, sizeof(filename), stdin);
    filename[strlen(filename) - 1] = '\0';

    // ��װ�����������ݰ�
    size = sprintf(buf, "%c%c%s%c%s%c", 0x00, 0x01, filename, 0, "octet", 0);
    if (sendto(cfd, buf, size, 0, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        ERR_MSG("sendto error");
        return -1;
    }

    // ���ļ�
    if ((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
        ERR_MSG("open error");
        return -1;
    }

    // ��ȡ����������ʱ�˿�
    struct sockaddr_in tem_sin;
    socklen_t addrlen = sizeof(tem_sin);

    while (1) {
        // ���buf
        bzero(buf, sizeof(buf));

        // ���շ����������İ�
        if ((res = recvfrom(cfd, buf, sizeof(buf), 0, (struct sockaddr*)&tem_sin, &addrlen)) < 0) {
            ERR_MSG("recvfrom error");
            break;
        }

        // ȡ�������룬�жϷ����������İ�������
        opcode = ntohs(*(short*)buf);
        if (opcode == 3) // ���ݰ�
        {
            // д������
            Data_Num = res - 4; // ���ݵ�ʵ�ʴ�С = �������ݰ��Ĵ�С - 4(������ + ����)
            if (write(fd, buf + 4, Data_Num) < 0) {
                ERR_MSG("write error");
                return -1;
            }

            // ��ACK�������͸�������
            ACK1 = buf[2];
            ACK2 = buf[3];
            bzero(buf, sizeof(buf));
            size = sprintf(buf, "%c%c%c%c", 0x00, 0x04, ACK1, ACK2);
            res = sendto(cfd, buf, size, 0, (struct sockaddr*)&tem_sin, sizeof(tem_sin));
            if (res < 0) {
                ERR_MSG("sendto error");
                return -1;
            }

            // ���ݵĴ�С < 512,˵�������Ѿ����꣬���سɹ�
            if (Data_Num < 512) {
                printf("���سɹ������������\n");
                break;
            }
        }
        else if (opcode == 5) // �����
        {
            printf("[%d : %s]\n", ntohs(*(short*)(buf + 2)), buf + 4);
            break;
        }
    }
    return 0;
}

int Upload_TFTP(int cfd, struct sockaddr_in sin)
{
    ssize_t res; // ����ֵ
    int fd; // ���ص��ļ�������
    unsigned short opcode; // ������
    unsigned short block_num; // ����
    char buf[516] = { 0 }; // �洢TFTP��5��Э���
    char filename[20] = { 0 }; // �洢�����ļ�������

    // �����ϴ�����
    printf("��������Ҫ�ϴ��ļ������ƣ�");
    fgets(filename, sizeof(filename), stdin);
    filename[strlen(filename) - 1] = '\0';

    // ��װ�����������ݰ�
    int size = sprintf(buf, "%c%c%s%c%s%c", 0x00, 0x02, filename, 0, "octet", 0);
    if (sendto(cfd, buf, size, 0, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        ERR_MSG("sendto error");
        return -1;
    }

    // ��ȡ����������ʱ�˿�
    struct sockaddr_in tem_sin;
    socklen_t addrlen = sizeof(tem_sin);

    // ���շ����������İ�
    bzero(buf, sizeof(buf));
    if ((res = recvfrom(cfd, buf, sizeof(buf), 0, (struct sockaddr*)&tem_sin, &addrlen)) < 0) {
        ERR_MSG("recvfrom error");
    }
    opcode = ntohs(*(short*)buf); // ȡ��������
    block_num = ntohs(*(unsigned short*)(buf + 2)); // ȡ������

    // �жϷ�������ACK���Ƿ�Ϸ�
    if (opcode != 4 || block_num != 0) {
        return -1;
    }
    printf("������ACK�״�Ӧ��ɹ�,��ʼ�������ݡ���\n");

    // ���ļ�
    if ((fd = open(filename, O_RDONLY)) < 0) {
        ERR_MSG("open error");
        return -1;
    }

    while (1) {
        // ���buf
        bzero(buf, sizeof(buf));

        // ��װ�������ݰ�
        block_num++;
        char* ptr = buf;
        unsigned short* p_make = (unsigned short*)ptr;
        *p_make = htons(3); // ������
        unsigned short* p_block = p_make + 1;
        *p_block = htons(block_num); // ����
        res = read(fd, buf + 4, 512);
        if (res == 0) {
            printf("�ϴ��ɹ������������\n");
            break;
        }
        else if (res == -1) {
            ERR_MSG("read error");
            return -1;
        }

        // ���������������ݰ�
        res = sendto(cfd, buf, res + 4, 0, (struct sockaddr*)&tem_sin, sizeof(tem_sin));
        if (res < 0) {
            ERR_MSG("sendto error");
            return -1;
        }

        // ���ղ���������������İ�
        bzero(buf, sizeof(buf));
        if ((res = recvfrom(cfd, buf, sizeof(buf), 0, (struct sockaddr*)&tem_sin, &addrlen)) < 0) {
            ERR_MSG("recvfrom error");
            return -1;
        }

        // ȡ�������������İ��Ĳ�����
        opcode = ntohs(*(short*)buf);
        if (opcode == 4) {
            if (block_num != ntohs(*(unsigned short*)(buf + 2))) {
                break;
            }
        }
        else if (opcode == 5) // �����
        {
            printf("[%d : %s]\n", ntohs(*(short*)(buf + 2)), buf + 4);
            break;
        }
    }
    return 0;
}