/*
 * Copyright (c) 2017, Sohrab Monfared
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {

  struct sockaddr_in server_addr, client_addr;

  unsigned int server_socket = 0;
  unsigned int accepted_socket = 0;
  unsigned int client_len = 0;

  char *response = "HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\n";

  if (argc < 3) {
    fprintf(stderr,"usage: %s <IP> <PORT>\n", argv[0]);
    return 1;
  }

  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (server_socket < 0) {
    perror("socket");
    return 2;
  }

  bzero((char *) &server_addr, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);
  server_addr.sin_port = htons(atoi(argv[2]));

  if (bind(server_socket, (struct sockaddr *) &server_addr,
      sizeof(server_addr)) < 0)
  {
    perror("bind");
    return 3;
  }

  if (listen(server_socket,5) < 0){
    perror("listen");
    return 4;
  }

  client_len = sizeof(client_addr);

  while (1){

    accepted_socket = accept(server_socket, (struct sockaddr *)&client_addr,
                             &client_len);

    if (accepted_socket < 0){
      perror("accept");
      break;
    }

    if (write(accepted_socket, response, 45) < 0) {
      perror("write");
    }

    if (close(accepted_socket) < 0){
      perror("close");
    }

  }

  return 0; 
}

