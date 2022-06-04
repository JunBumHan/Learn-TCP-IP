#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handling(char * message);

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len;

    if(argc != 3)
    {
        printf("Usage : %s <IP> <PORT>\n",argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    /*
        PF_INET : IPv4 인터넷 프로토콜 체화
        SOCK_STREAM : 연결지향형 소켓
    */
   if(sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr)); //serv_addr 구조체 초기화
    serv_addr.sin_family =  AF_INET; // IPv4 주소 체계
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // inet_addr 함수를 사용함으로써 네트워크 바이트 순서로 바꾸어 주는 동시에 문자열을 정수로 바꾸어준다. IP 할당
    serv_addr.sin_port = htons(atoi(argv[2])); // PORT 할당

    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) 
    /*
    int connect(int sock, struct sockaddr * servaddr, socklen_t addrlen);
    */
        error_handling("connect() error");

    str_len = read(sock, message, sizeof(message)-1); // 수신하기
    if(str_len == -1)
        error_handling("read() error");

    printf("서버측에서 보낸 문자입니다 : %s",message);
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}