#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
//#include <Winsock2.h>
#include <netdb.h>
#include <sys/types.h>
#define bzero(a, b) memset(a, 0, b)
#include <sstream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#define porttcp 25795
#define portudp 24795
#define portsa 21795
#define portsb 22795
#define portsc 23795



using namespace std;

char recvBufa[50000], recvBufb[50000], recvBufc[50000];
char operationit[10];
char clientsocket[10];
int intsocket;
int allsum;

long long int changechartoint(char *num){
	long long int sum = 0;
	for(int i = 0; num[i] != '\0'; ++ i)
		sum = sum * 10 + (num[i] - '0');
	return sum;
}

void divideallnumber(char *num){
	int i = 0; // for num
	int ja, jb, jc, jo, js; // for char a, b, c, operation
	int sum;
	char temp[10];
	int t;  //for temp
	ja = jb = jc = jo = js = 0;
	for(;num[i] != ','; ++ i)
		clientsocket[js++] = num[i];
	clientsocket[js] = '\0';
	intsocket = 0;
	for(int k = 0; clientsocket[k] != '\0'; ++ k)
		intsocket = intsocket * 10 + (clientsocket[k] - '0');
	i++;

	for(; num[i] != ','; ++ i){
		recvBufa[ja++] = num[i];
		recvBufb[jb++] = num[i];
		recvBufc[jc++] = num[i];
		operationit[jo++] = num[i];
	}
	recvBufa[ja++] = ',';
	recvBufb[jb++] = ',';
	recvBufc[jc++] = ',';
	operationit[jo] = '\0';

	i++;
	t = 0;
	for(; num[i] != ','; ++ i)
		temp[t++] = num[i];
	temp[i] = '\0';
	sum = changechartoint(temp);
	allsum = sum;

	sum = sum / 3;

	memset(temp, 0, sizeof(temp));
	string sumtemp;
	stringstream stream;
	stream << sum;
	stream >> sumtemp;
	strcpy(temp, sumtemp.c_str());

	for(int k = 0; temp[k] != '\0'; ++ k){
		recvBufa[ja++] = temp[k];
		recvBufb[jb++] = temp[k];
		recvBufc[jc++] = temp[k];
	}

	recvBufa[ja++] = ',';
	recvBufb[jb++] = ',';
	recvBufc[jc++] = ',';

	i++;
	for(int sa = 0;; ++ i){
		recvBufa[ja++] = num[i];
		if(num[i] == ',')
			sa++;
		if(sa == sum)
			break;
	}

	i++;
	for(int sb = 0;; ++ i){
		recvBufb[jb++] = num[i];
		if(num[i] == ',')
			sb++;
		if(sb == sum)
			break;
	}

	i++;
	for(int sc = 0;; ++ i){
		recvBufc[jc++] = num[i];
		if(num[i] == ',')
			sc++;
		if(sc == sum)
			break;
	}
}
long long int min(long long int a, long long int b){
	if(a > b)
	return b;
	return a;
}

long long int max(long long int a, long long int b){
	if(a < b)
	return b;
	return a;
}

long long int calculate(long long int a, long long int b, long long int c){
	long long int outcome;
	if(operationit[2] == 'N'){
		outcome = a;
		outcome = min(outcome, b);
		outcome = min(outcome, c);
	}
	else if(operationit[2] == 'X'){
		outcome = a;
		outcome = max(outcome, b);
		outcome = max(outcome, c);
	}
	else
		outcome = a + b + c;
	return outcome;
}

int main()
{

	printf("The AWS is up and running.\n");

	while(1){
	int sockSrv = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	struct sockaddr_in addrSrv;
	bzero(&addrSrv,sizeof(addrSrv));
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(porttcp);
	//addrSrv.sin_addr.s_addr = htonl(INADDR_ANY);
	addrSrv.sin_addr.s_addr = inet_addr("127.0.0.1");

	int on=1;
    if((setsockopt(sockSrv,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

	if(bind(sockSrv,(struct sockaddr*)&addrSrv,sizeof(sockaddr)) < 0){
        perror("connect");x
        exit(1);
	}

	listen(sockSrv,20);


	while(1){
		struct sockaddr_in addrClient;
		socklen_t len=sizeof(sockaddr);
		int sockConn=accept(sockSrv,(struct sockaddr*)&addrClient,&len);
		int sclient = sockConn;

		char recvBuf[50000];
        memset(recvBuf, 0, sizeof(recvBuf));

		int re = recvfrom(sockConn,recvBuf,sizeof(recvBuf),0, (sockaddr*)&addrClient, &len);

		if(re >= 0){
			//udp connect to server a, b, c

			printf("The AWS has received %d numbers from the client using TCP over port %d\n", allsum, porttcp);
            divideallnumber(recvBuf);

			int socksa = socket(AF_INET, SOCK_DGRAM, 0);
			struct sockaddr_in addrsa;
			bzero(&addrsa,sizeof(addrsa));
			addrsa.sin_family = AF_INET;
			addrsa.sin_addr.s_addr = inet_addr("127.0.0.1");
			addrsa.sin_port = htons(portsa);
			sendto(socksa, recvBufa, strlen(recvBufa), 0, (struct sockaddr*)&addrsa, sizeof(sockaddr));
			printf("The AWS has sent %d numbers to Backend-Server A\n", allsum / 3);

			int socksb = socket(AF_INET, SOCK_DGRAM, 0);
			struct sockaddr_in addrsb;
			addrsb.sin_family = AF_INET;
			addrsb.sin_addr.s_addr = inet_addr("127.0.0.1");
			addrsb.sin_port = htons(portsb);
			sendto(socksb, recvBufb, strlen(recvBufb), 0, (struct sockaddr*)&addrsb, sizeof(sockaddr));
			printf("The AWS has sent %d numbers to Backend-Server B\n", allsum / 3);

			int socksc = socket(AF_INET, SOCK_DGRAM, 0);
			struct sockaddr_in addrsc;
			addrsc.sin_family = AF_INET;
			addrsc.sin_addr.s_addr = inet_addr("127.0.0.1");
			addrsc.sin_port = htons(portsc);
			sendto(socksc, recvBufc, strlen(recvBufc), 0, (struct sockaddr*)&addrsc, sizeof(sockaddr));
			printf("The AWS has sent %d numbers to Backend-Server C\n", allsum / 3);

			//get information from a, b, c
			//a

			int sockra = socket(AF_INET, SOCK_DGRAM, 0);
			struct sockaddr_in addrra;
			bzero(&addrra, sizeof(addrra));
			addrra.sin_addr.s_addr = htonl(INADDR_ANY);
			addrra.sin_family = AF_INET;
			addrra.sin_port = htons(portudp);

			bind(sockra,(struct sockaddr*)&addrra,sizeof(addrra));

			socklen_t lena = sizeof(sockaddr_in);
			char recva[20];
            memset(recva, 0, sizeof(recva));
			sendto(socksa, "ok", 2, 0, (struct sockaddr*)&addrsa, sizeof(sockaddr));//important!! To let serverA know it is time to send information to AWS

			recvfrom(sockra, recva, sizeof(recva), 0, (struct sockaddr*)&addrra, &lena);
			//sendto(sockra, "ok", 2, 0, (sockaddr*)&addrra, lena);
			close(sockra);
			close(socksa);
			for(int k = 0; k < 20; ++ k)
				if(recva[k] == ','){
					recva[k] = '\0';
					break;
				}
			printf("The AWS received reduction result of %s from Backend-Server A using UDP over port %d and it is %s\n", operationit, portudp, recva);

			//b
			int sockrb = socket(AF_INET, SOCK_DGRAM, 0);
			struct sockaddr_in addrrb;
			bzero(&addrrb, sizeof(addrrb));
			addrrb.sin_addr.s_addr = htonl(INADDR_ANY);
			addrrb.sin_family = AF_INET;
			addrrb.sin_port = htons(portudp);

			bind(sockrb,(struct sockaddr*)&addrrb,sizeof(addrrb));


			socklen_t lenb = sizeof(sockaddr_in);
			char recvb[20];
            memset(recvb, 0, sizeof(recvb));
			sendto(socksb, "ok", 2, 0, (struct sockaddr*)&addrsb, sizeof(sockaddr));

			recvfrom(sockrb, recvb, sizeof(recvb), 0, (struct sockaddr*)&addrrb, &lenb);
			//sendto(sockrb, "ok", 2, 0, (sockaddr*)&addrrb, lenb);
			close(sockrb);
			close(socksb);
			for(int k = 0; k < 20; ++ k)
				if(recvb[k] == ','){
					recvb[k] = '\0';
					break;
				}
			printf("The AWS received reduction result of %s from Backend-Server B using UDP over port %d and it is %s\n", operationit, portudp, recvb);

			//c
			int sockrc = socket(AF_INET, SOCK_DGRAM, 0);
			struct sockaddr_in addrrc;
			bzero(&addrrc, sizeof(addrrc));
			addrrc.sin_addr.s_addr = htonl(INADDR_ANY);
			addrrc.sin_family = AF_INET;
			addrrc.sin_port = htons(portudp);

			bind(sockrc,(struct sockaddr*)&addrrc,sizeof(addrrc));


			socklen_t lenc = sizeof(sockaddr_in);
			char recvc[20];
            memset(recvc, 0, sizeof(recvc));
			sendto(socksc, "ok", 2, 0, (struct sockaddr*)&addrsc, sizeof(sockaddr));

			recvfrom(sockrc, recvc, sizeof(recvc), 0, (struct sockaddr*)&addrrc, &lenc);
			//sendto(sockrc, "ok", 2, 0, (sockaddr*)&addrrc, lenc);
			close(sockrc);
			close(socksc);
			for(int k = 0; k < 20; ++ k)
				if(recvc[k] == ','){
					recvc[k] = '\0';
					break;
				}

			printf("The AWS received reduction result of %s from Backend-Server C using UDP over port %d and it is %s\n", operationit, portudp, recvc);

			long long int ioutcomea, ioutcomeb, ioutcomec;
			ioutcomea = ioutcomeb = ioutcomec = 0;
			for(int k = 0; recva[k] != '\0'; ++ k)
				ioutcomea = ioutcomea * 10 + (recva[k] - '0');
			for(int k = 0; recvb[k] != '\0'; ++ k)
				ioutcomeb = ioutcomeb * 10 + (recvb[k] - '0');
			for(int k = 0; recvc[k] != '\0'; ++ k)
				ioutcomec = ioutcomec * 10 + (recvc[k] - '0');

			long long int finaloutcome = calculate(ioutcomea, ioutcomeb, ioutcomec);
			printf("The AWS has successfully finished the reduction %s: %lld\n", operationit, finaloutcome);

			//tcp to client

			char toclient[20];
			memset(toclient, 0, sizeof(toclient));
			string sanswer;
			stringstream stream;
			stream << finaloutcome;
			stream >> sanswer;
			sanswer += ",";
			strcpy(toclient, sanswer.c_str());

			send(sockConn, toclient, sanswer.length(), 0);
			close(sockConn);
			close(sockSrv);
			printf("The AWS has successfully finished sending the reduction value to client.\n");

			break;
		}
	}
}


	return 0;
}
