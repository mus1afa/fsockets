#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BACKLOG 10
#define HandleError(mesg) do{ \
				perror(mesg); \
				exit(EXIT_FAILURE); \
				}while(0)

			
int main(int argc, char *argv[])
{
	int sockfd, opt = 1, newfd = 0, last_fd = 1; 
	struct sockaddr_in addr;
	char buff[1024], curr_user[20], curr_msg[1000];
	int addrlen = sizeof(addr);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == 0)
		HandleError("socket");

	memset(&addr, '0', sizeof(addr));
	memset(buff, '0', sizeof(buff));

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (void *)&opt, (socklen_t)sizeof(opt)))
		HandleError("setsockopt");
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		HandleError("bind");

	if (listen(sockfd, BACKLOG) < 0)
		HandleError("listen");

	while (1)
	{
		if ((newfd = accept(sockfd, (struct sockaddr *)&addr, &addrlen)) < 0)
			HandleError("accept");
		printf("newfd = %d\n", newfd);
	
		if (recv(newfd, (void *)buff, 1024, 0))
			fprintf(stderr, "Message: %s\n", buff);

	        strcpy(curr_user, strtok(buff, ":"));
	        strcpy(curr_msg, strtok(NULL, ":"));
	        sprintf(buff, "hello %s", curr_user); 

		if (send(newfd, (void *)buff, sizeof(buff), 0) < 0)
			HandleError("send");

		close(newfd);
	}
	close(sockfd);

	return (0);
}
