/*
server
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handling(char * message);

int main(int argc, char* argv[])
{
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size; // typedef unsigned int socklen_t (POSIX)

    char message[] = "server : Hell World!";

    if(argc != 2)
    {
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    /*
        int socket(int domain, int type, int protocol)
        반환 값
        실패 : -1
        성공 : -1 이외의 소켓 디스크립터
        --------------------------
        PF_INET : IPv4 인터넷 프로토콜 체계
        SOCK_STREAM : 연결지향형 소켓
    */
   if(serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr)); //serv_addr 구조체 초기화 -> struct sockaddr의 바이트열을 같게 해주기 위함.
    serv_addr.sin_family = AF_INET; // Ipv4 주소 체계
    serv_addr.sin_addr.s_addr = htons(INADDR_ANY); // IP 할당
    /*
    htons : Host to network short
    빅엔디로 변환 시켜줌. ( 네트워크 바이트 순서 )

    INADDR_ANY : 호스트 IP주소 ( 사용가능한 랜카드의 IP주소 )
    */

    serv_addr.sin_port = htons(atoi(argv[1])); // Port 할당
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    /*
        int bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);

        serv_sock에 주소할당을 해줌.
    */

   if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");
    /*
        listen(int sock, int backlog);

        serv_sock (sock) : 문지기 역활
        5 (backlog): 연결 요청 대기 큐 
        이제 서버 소켓이라 할 수 있음.
    */
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
    /*
        int accept(int sock, struct sockaddr * addr, socklen_t * addrlen);

        accept() 함수는 자동적으로 소켓을 생성한 뒤 클라이언트와 송수신을 함.
        참고로 연결 요쳥 대기 큐가 계속 비어있는 상태라면, 클라이언트의 연결요청이 들어올 때까지 accpet함수는 반환하지 않음.
    */
    if(clnt_sock == -1)
        error_handling("accept() error");

    write(clnt_sock, message, sizeof(message)); // write함수를 사용해 클라이언트에게 데이터 전송.
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}