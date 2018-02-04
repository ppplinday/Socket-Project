#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
#include <arpa/inet.h>
#include <sys/socket.h>

int sum, sumor, sumand, qsum;
char sendtoor[5000], sendtoand[5000];
int quenenofnumber[500];
char thefinaloutcome[5000];
char recvBuffer[5000] = {0};
char recvBuffer1[5000] = {0};

void takeinfo(char *buf){
    sum = sumor = sumand = qsum = 0;
    int i, ori, andi;
    i = ori = andi = 0;
    for(; buf[i] != ',' && i < strlen(buf); ++ i){
        sum = sum * 10 + buf[i] - '0';
    }
    i++;
    for(; buf[i] != ',' && i < strlen(buf); ++ i){
        sumor = sumor * 10 + buf[i] - '0';
        sendtoor[ori++] = buf[i];
    }
    i++;
    for(; buf[i] != ',' && i < strlen(buf); ++ i){
        sumand = sumand * 10 + buf[i] - '0';
        sendtoand[andi++] = buf[i];
    }
    i++;
    sendtoor[ori++] = ',';
    sendtoand[andi++] = ',';
    
    for(int j = 1; j <= sum; ++ j){
        if(buf[i] == 'o'){
            quenenofnumber[qsum++] = 1;
            i = i + 3;
            while(buf[i] != ','){
                sendtoor[ori++] = buf[i++];
            }
            sendtoor[ori++] = ',';
            i++;
            while(buf[i] != ','){
                sendtoor[ori++] = buf[i++];
            }
            sendtoor[ori++] = ',';
            i++;
        }
        else{
            quenenofnumber[qsum++] = 2;
            i = i + 4;
            while(buf[i] != ','){
                sendtoand[andi++] = buf[i++];
            }
            sendtoand[andi++] = ',';
            i++;
            while(buf[i] != ','){
                sendtoand[andi++] = buf[i++];
            }
            sendtoand[andi++] = ',';
            i++;
        }
    }
    //printf("or = %s\n and = %s\n", sendtoor, sendtoand);
}

void maketheoutcome(char *sor, char *sand){
    int sori, sandi, finali;
    sori = sandi = finali = 0;
    for(int i = 0; i < sum; ++ i){
        if(quenenofnumber[i] == 1){
            while(sor[sori] != '=')
                sori++;
            sori = sori + 2;
            for(; sor[sori]!= '\n'; ++ sori)
                thefinaloutcome[finali++] = sor[sori];
            thefinaloutcome[finali++] = '\n';
            sori++;
        }
        else{
            while(sand[sandi] != '=')
                sandi++;
            sandi = sandi + 2;
            for(; sand[sandi] != '\n'; ++ sandi)
                thefinaloutcome[finali++] = sand[sandi];
            thefinaloutcome[finali++] = '\n';
            sandi++;
        }
    }
}

int main(){
    printf("The edge server is up and running.\n");
    
    while(1){
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(23350);
        int tmp = 1;
        setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
    int r = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        while(r == -1){
            r = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        }
    int t = listen(serv_sock, 20);
    
    while(1){
    memset(recvBuffer, 0, sizeof(recvBuffer));
    memset(recvBuffer1, 0, sizeof(recvBuffer1));
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    
    char recvBuf[5000];
    memset(recvBuf, 0, sizeof(recvBuf));
    
    struct sockaddr_in addrClient;
    socklen_t len=sizeof(sockaddr);
    ssize_t re = recvfrom(clnt_sock,recvBuf,sizeof(recvBuf),0, (sockaddr*)&addrClient, &len);
    memset(sendtoor, 0, sizeof(sendtoor));
    memset(sendtoand, 0, sizeof(sendtoand));
    takeinfo(recvBuf);
    printf("%s\n", recvBuf);
    printf("The edge server has received %d lines from the client using TCP over port 23350\n", sum);
    
        
    //UDP begin
    //recvBuffer from or; recvBuffer1 from and
    int sockor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct sockaddr_in addror;
    memset(&addror, 0, sizeof(addror));
    addror.sin_family = AF_INET;
    addror.sin_addr.s_addr = inet_addr("127.0.0.1");
    addror.sin_port = htons(21350);
    
    sendto(sockor, sendtoor, strlen(sendtoor), 0, (struct sockaddr*)&addror, sizeof(addror));
    printf("The edge server has successfully sent %d lines to Backend-Server OR.\n", sumor);
    
    int sockand = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct sockaddr_in addrand;
    memset(&addrand, 0, sizeof(addrand));
    addrand.sin_family = AF_INET;
    addrand.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrand.sin_port = htons(22350);
    
    sendto(sockand, sendtoand, strlen(sendtoand), 0, (struct sockaddr*)&addrand, sizeof(addrand));
    printf("The edge server has successfully sent %d lines to Backend-Server AND.\n", sumand);
    
    struct sockaddr_in fromAddror;
    
    unsigned int fromLen = sizeof(fromAddror);
    
    recvfrom(sockor, recvBuffer, 5000, 0, (struct sockaddr*)&fromAddror, &fromLen);
    //printf("recive:\n%s\n", recvBuffer);
    
    struct sockaddr_in fromAddrand;
    
    unsigned int fromLen1 = sizeof(fromAddrand);
    
    recvfrom(sockand, recvBuffer1, 5000, 0, (struct sockaddr*)&fromAddrand, &fromLen1);
    //printf("recive:\n%s\n", recvBuffer1);
    
    printf("The edge server start receiving the computation results from Backend-Server OR and Backend-Server AND using UDP port 24350.\n");
    printf("The computation results are:\n%s%s", recvBuffer, recvBuffer1);

    //UDP end
    
    memset(thefinaloutcome, 0, sizeof(thefinaloutcome));
    maketheoutcome(recvBuffer, recvBuffer1);
    
    printf("The edge server has successfully finished receiving all computation results from the Backend-Server OR and Backend-Server AND.\n");
    //printf("outcome = \n%s\n", thefinaloutcome);
    
    //char str[] = "Hello World!";
    write(clnt_sock, thefinaloutcome, sizeof(thefinaloutcome));
    printf("The edge server has successfully finished sending all computation results to the client.\n");
    
    close(clnt_sock);
    close(serv_sock);
        break;

    }
    }
        
    return 0;
}

