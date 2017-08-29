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


using namespace std;

int numberofand;
int numberofor;
int sum;

string readthefile(){
    FILE* pfile;
    pfile = fopen("job.txt", "r");
    char s[100];
    string ss = "";
    string temp, temp1, temp2;
    numberofand = numberofor = sum = 0;
    
    if(pfile == NULL){
        perror("Error opening file");
        exit(1);
    }
    else{
        while( fgets(s, 100, pfile) != NULL){
            ss += s;
        }
        for(int i = 0; i < ss.length(); ++ i)
            if(ss[i] == '\n')
                ss[i] = ',';
    }
    for(int i = 0; i < ss.length(); ++ i){
        if(ss[i] == 'a') numberofand++, sum++;
        if(ss[i] == 'o') numberofor++, sum++;
    }
    //cout << sum << " " << numberofor << " " << numberofand << endl;
    stringstream stream;
    stream << numberofand;
    stream >> temp;
    
    ss = temp + "," + ss;
    
    stringstream stream1;
    stream1 << numberofor;
    stream1 >> temp1;
    
    ss = temp1 + "," + ss;
    
    stringstream stream2;
    stream2 << sum;
    stream2 >> temp2;
    
    ss = temp2 + "," + ss;
    
    return ss;
}

int main(){
    printf("The client is up and running.\n");
    
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    int value = -1;
    //setsockopt(sock, SOL_SOCKET, SO_NOSIGPIPE, &value, sizeof(value));
    
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(23350);
    int t = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    
    string sfile = readthefile();
    sfile = sfile + '\0';
    char cfile[5000];
    for(int i = 0; i < sfile.length(); ++ i) cfile[i] = sfile[i];
    
    send(sock, cfile, sfile.length(), 0);
    
    printf("The client has successfully finished sending %d lines to the edge server.\n", sum);
    
    char buffer[10000];
    read(sock, buffer, sizeof(buffer)-1);
    
    printf("The client has successfully finished receiving all computation results from the edge server.\n");
    
    printf("The final computation results are:\n%s", buffer);
    
    close(sock);
   
    //string s1 = readthefile();
    //cout << s1 << endl;
    
    return 0;
}
