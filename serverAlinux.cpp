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
#define port 21795
#define portaws 24795

#pragma comment( lib, "ws2_32.lib" )

using namespace std;


int computerit(int *n, int sumofnumber, char *o){
	int outcome;
	if(o[2] == 'N'){
		outcome = n[0];
		for(int i = 0; i < sumofnumber; ++ i)
			outcome = min(outcome, n[i]);
	}
	else if(o[2] == 'X'){
		outcome = n[0];
		for(int i = 0; i < sumofnumber; ++ i)
			outcome = max(outcome, n[i]);
	}
	else if(o[2] == 'M'){
		outcome = 0;
		for(int i = 0; i < sumofnumber; ++ i)
			outcome += n[i];
	}
	else{
		outcome = 0;
		for(int i = 0; i < sumofnumber; ++ i)
			outcome += n[i] * n[i];
	}
	return outcome;
}

int main()
{

	printf("The Server A is up and running using UDP on port %d\n", port);

	while(1){

	int socksrv = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in addrsrv;
	addrsrv.sin_addr.s_addr = htonl(INADDR_ANY);
	addrsrv.sin_family = AF_INET;
	addrsrv.sin_port = htons(port);

	bind(socksrv,(sockaddr*)&addrsrv,sizeof(addrsrv));
	bzero(&addrsrv, sizeof(addrsrv));

	socklen_t len = sizeof(sockaddr_in);
	char recvBuf[50000];

	recvfrom(socksrv, recvBuf, sizeof(recvBuf), 0, (sockaddr*)&addrsrv, &len);

	// analysis the data
	int i = 0; // for recvBuf
	char operation[10];
	int o = 0; // for operation
	for(; recvBuf[i] != ','; ++ i)
		operation[o++] = recvBuf[i];
	operation[o] = '\0';
	i++;

	char temp[10];
	int sumnumber = 0;
	int t = 0; // for temp;
	for(; recvBuf[i] != ','; ++ i)
		temp[t++] = recvBuf[i];
	temp[t] = '\0';
	for(t = 0; temp[t] != '\0'; ++ t)
		sumnumber = sumnumber * 10 + (temp[t] - '0');
	i++;

	int number[20000];
	int nindex = 0; // for nindex;
	int startn = i, endn, nn = 0;
	for(; nn < sumnumber; ++ i){
		if(recvBuf[i] == ','){
			endn = i - 1;
			nn++;
			number[nindex] = 0;
			for(int k = startn; k <= endn; ++ k)
				number[nindex] = number[nindex] * 10 + (recvBuf[k] - '0');
			nindex++;
			startn = i + 1;
		}
	}

	printf("The Server A has received %d numbers\n", sumnumber);

	int outcome;
	string soutcome;
	char coutcome[20];
	outcome = computerit(number, sumnumber, operation);
	stringstream stream;
	stream << outcome;
	stream >> soutcome;
	soutcome += ",";
	strcpy(coutcome, soutcome.c_str());
	int ll = sizeof(struct sockaddr_in);

	printf("The Server A has successfully finished the reduction %s: %d\n", operation, outcome);

	//send to aws

	int sockaws = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in addraws;
	bzero(&addraws, sizeof(addraws));
	addraws.sin_addr.s_addr = inet_addr("127.0.0.1");
	addraws.sin_family = AF_INET;
	addraws.sin_port = htons(portaws);

	char ok[20];
	memset(ok, 0, sizeof(ok));
	recvfrom(socksrv, ok, sizeof(ok), 0, (sockaddr*)&addrsrv, &len);
	close(socksrv);


	sendto(sockaws, coutcome, strlen(coutcome), 0, (sockaddr*)&addraws, sizeof(sockaddr_in));
	close(sockaws);

	printf("The Server A has successfully finished sending the reduction value to AWS server\n");
}
	int yy;
	scanf("%d", &yy);


	return 0;
}
