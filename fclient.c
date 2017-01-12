#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

#define HandleError(msg) do{\
				perror(msg);\
				exit(EXIT_FAILURE);\
				}while(0)

int main(int argc, char *argv[])
{
	char buff[1024] = "Hello from client\n", username[20], buff1[1024];
	char message[1200] = "hi this is message from user1 = ";
	int sockfd;
	struct sockaddr_in addr;

	if (argc != 3)
	{
		fprintf(stderr, "usage: ./<client exe name> <ip address> <username>\n");
		exit(EXIT_FAILURE);
	}
	memset(buff, '0', sizeof(buff));
	memset(&addr, '0', sizeof(addr));

	strcpy(username, argv[2]);
	puts("Enter the message to be sent to client");
	fgets(buff1, 1000, stdin);
	sprintf(buff, "%s:%s %s", username, message, buff1);
	strcpy(message, buff);

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		HandleError("socket");

	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;

	if (inet_pton(AF_INET, argv[1], &(addr.sin_addr)) <= 0)
		HandleError("pton");

	if (connect(sockfd, (struct sockaddr *)&addr, (socklen_t)sizeof(addr)) < 0)
		HandleError("connect");

	if (send(sockfd, (void *)message, sizeof(message), 0) < 0)
		HandleError("send");

	if (recv(sockfd, (void *)buff, 1024, 0))
		fprintf(stderr, "Message: %s\n", buff);

	close(sockfd);

	return 0;
}
