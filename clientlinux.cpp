#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
//#include <Winsock2.h>
#include <sstream>
#define bzero(a, b) memset(a, 0, b)
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#define port 25795



using namespace std;

int sum; // the number of nums.csv

string readthefile(){
	FILE* pfile;
	pfile = fopen("nums.csv", "r");
	char s[100];
	string ss = "";
	string temp;
	sum = 0;

	if(pfile == NULL){
		perror("Error opening file");
		exit(1);
	}
	else{
		while( fgets(s, 100, pfile) != NULL){
			ss += s;
			sum++;
		}
		for(int i = 0; i < ss.length(); ++ i)
			if(ss[i] == '\n')
				ss[i] = ',';

		stringstream stream;
		stream << sum;
		stream >> temp;

		ss = temp + "," + ss;
	}

		return ss;
}

int main(int argc, const char * argv[])
{

//	printf("Please input the reduction type:");
	string reductiontype;
	reductiontype = argv[1];
//	cin >> reductiontype;
	printf("The client is up and running.\n");
	printf("The client has sent the reduction type %s to AWS.\n", reductiontype.c_str());

	int s;
	struct sockaddr_in addr;
	s = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(port);

	if(connect(s, (struct sockaddr *)&addr, sizeof(addr)) == -1){
        perror("connect");
        exit(1);
	}


	string tosocket;
	stringstream stream;
	stream << s;
	stream >> tosocket;
	string sfile = readthefile();
	sfile = tosocket + "," + reductiontype + "," + sfile + '\0';
	char cfile[50000];
	strcpy(cfile, sfile.c_str());



	while(1){

		int f = send(s, cfile, sfile.length(), 0);  //we maybe change the sizeof of cfile

		if(f >= 0){
			printf("The client has sent %d numbers to AWS\n", sum);

			//recvform the aws

			char outcome[20];
			memset(outcome, 0, sizeof(outcome));
			while( recv(s,outcome,sizeof(outcome),0) > 0 ){s



			for(int k = 0; k < 20; ++ k){
				if(outcome[k] == ','){
					outcome[k] = '\0';
					break;
				}
			}

			printf("The client has received reduction SUM: %s\n", outcome);
			}
			}
		close(s);
		break;
		}




	return 0;
}
