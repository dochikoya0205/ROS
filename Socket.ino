#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// 사용자 함수
void error_handling(char *message) {
    //printf("%s\n", message);
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];           // 수신데이터 저장변수
    int str_len;                // 수신 확인

    if(argc != 3) {  // 매개변수의 개수를 확인
        printf("Usage : %s <IP> <Port>\n", argv[0]);
        exit(1);
    }

    //1.소켓의 생성
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        error_handling("socket() error"); // 매개변수를 출력하는 함수
    }
     
    // 2. 소켓 주소 할당 및 연결
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);  // IP number
    serv_addr.sin_port = htons(atoi(argv[2]));         // Port number

    
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("connect() error");
    }


    // 3. 데이터 전송
    str_len = read(sock, message, sizeof(message)-1);
    if (str_len == -1) {
        error_handling("read() error");
    }

    // 4. 수신된 메세지 출력
    printf("Message from server: %s \n", message);

    // 5. 소켓종료
    close(sock); 

    return 0;
}
