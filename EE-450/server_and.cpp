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

char outcome[5000];
int sum;

std::string cale(std::string a, std::string b){
    int na, nb, longn;
    na = a.length();
    nb = b.length();
    if(na > nb){
        longn = na;
        for(int i = 1; i <= na - nb; ++ i)
            b = '0' + b;
    }
    else{
        longn = nb;
        for(int i = 1; i <= nb - na; ++ i)
            a = '0' + a;
    }
    std::string c, cc;
    for(int i = 0; i < longn; ++ i){
        if(a[i] == '1' && b[i] == '1')
            c = c + '1';
        else
            c = c + '0';
    }
    //std::cout << na << " " << a << " " << b << " " << c << std::endl;
    int n = 0;
    while(c[n] == '0' && n < longn) n++;
    if(n == longn){
        cc = '0';
    }
    else{
        for(; n < longn; ++ n)
            cc = cc + c[n];
    }
    return cc;
}

void makestring(char *buf){
    sum = 0;
    int i = 0;
    std::string outc = "";
    for(; buf[i] != ',' && i < strlen(buf); ++ i){
        sum = sum * 10 + buf[i] - '0';
    }
    i++;
    for(int j = 1; j <= sum; ++ j){
        std::string a, b;
        a = "";
        b = "";
        while(buf[i] != ','){
            a = a + buf[i];
            i++;
        }
        i++;
        while(buf[i] != ','){
            b = b + buf[i];
            i++;
        }
        i++;
        std::string c = cale(a, b);
        outc = outc + a + " " + "and " + b + " = " + c + "\n";
    }
    for(int i = 0; i < outc.length(); ++ i)
        outcome[i] = outc[i];
}

int main()
{
    printf("The Server AND is up and running using UDP on port 22350.\n");
    
    while(1){
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if (sock < 0)
    {
        printf("Create socket faild\r\n");
        exit(1);
    }
    
    struct sockaddr_in fromAddr;
    memset(&fromAddr, 0, sizeof(fromAddr));
    fromAddr.sin_family = AF_INET;
    fromAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    fromAddr.sin_port = htons(22350);
    
    if (bind(sock, (struct sockaddr*)&fromAddr, sizeof(fromAddr)) < 0)
    {
        printf("Bind socket faild!\r\n");
        close(sock);
        exit(1);
    }
    
    struct sockaddr_in toAddr;
    char recvBuffer[128] = {0};
    int recvLen = -1;
    unsigned int addrLen = sizeof(toAddr);
    
    if ((recvLen = recvfrom(sock, recvBuffer, 128, 0, (struct sockaddr*)&toAddr, &addrLen)) < 0)
    {
        printf("Receive message faild!\r\n");
        close(sock);
        exit(1);
    }
    
    /*if (recvLen > 0)
     {
     printf("Recive message:%s\r\n", recvBuffer);
     }*/
    printf("The Server AND start receiving lines from the edge server for AND computation. The computation results are:\n");
    memset(outcome, 0, sizeof(outcome));
    makestring(recvBuffer);
    printf("%s", outcome);
    
    //char recvinfo[] = "back!";
    if (sendto(sock, outcome, sizeof(outcome), 0, (struct sockaddr*)&toAddr, sizeof(toAddr)) != sizeof(outcome))
    {
        printf("sendto faild\r\n");
        close(sock);
        exit(1);
    }
    printf("The Server AND has successfully received %d lines from the edge server and finished all AND computations.\n", sum);
    printf("The Server AND has successfully finished sending all computation results to the edge server\n");
    
    close(sock);
    }
    return 0;
}








