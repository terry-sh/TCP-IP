/**
 * run test, for example:
 * 
 * ```bash
 * ./server.out 5143
 * ```
 * the number of port should not be too small.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define CODE_FAILURE -1
#define CODE_SUCCESS 0

// [Lib function declaration]
// int socket(int domain, int type, int protocol);
// int bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);
// int listen(int sockfd, int backlog);
// int accept(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);


void error_handling(char *message);

int main(int argc, char *argv[]) {
  int serv_sock;
  int clnt_sock;

  struct sockaddr_in serv_addr;
  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_size;

  char message[] = "Hello, World!";

  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    exit(1);
  }

  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  if (serv_sock == CODE_FAILURE) {
    error_handling("socket() error");
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));

  int bind_status = bind(
    serv_sock,
    (struct sockaddr *) &serv_addr,
    sizeof(serv_addr)
  );
  printf("bind status: %d\n", bind_status);
  if (bind_status == -1) {
    error_handling("bind() error");
  }

  if (listen(serv_sock, 5) == CODE_FAILURE) {
    error_handling("listen() error");
  }

  clnt_addr_size = sizeof(clnt_addr);
  // infinite loop of server
  while (1) {
    clnt_sock = accept(
      serv_sock,
      (struct sockaddr *) &clnt_addr,
      &clnt_addr_size
    );

    if (clnt_sock == CODE_FAILURE) {
      error_handling("accept() error");
    }
    write(clnt_sock, message, sizeof(message));
  }

  close(clnt_sock);
  close(serv_sock);

  return 0;
}

void error_handling(char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}