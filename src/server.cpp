#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <signal.h>
#include <sys/time.h>

using namespace std;

#define BUFFER_SIZE 1024
#define MAX_IDLE_TIME 10 // 10 segundos de inactividad antes de reiniciar la conexión

bool is_idle = false;

void close_connection(int signal) {
    is_idle = true;
}

void timer_handler(int signal) {
    is_idle = true;
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;

    cout << "\n\tFile Extractor\n";
    cout << "\nProvide the port number\n";
    cout << "\nServer Port: ";
    cin >> portno;

    socklen_t clilen;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    // Crear un socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // Establecer la dirección del servidor
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Vincular el socket a la dirección del servidor
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    // Escuchar conexiones entrantes
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // Configurar el temporizador
    struct sigaction sa;
    struct itimerval timer;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timer_handler;
    sigaction(SIGALRM, &sa, NULL);

    timer.it_value.tv_sec = MAX_IDLE_TIME;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    while (true) {
        // Configurar el temporizador
        setitimer(ITIMER_REAL, &timer, NULL);

        // Aceptar una conexión entrante
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            continue;
        }

        // Reiniciar el temporizador
        timer.it_value.tv_sec = MAX_IDLE_TIME;
        timer.it_value.tv_usec = 0;

        // Leer los datos del archivo
        FILE *fp = fopen("leak.txt", "wb");
        if (fp == NULL) {
            perror("ERROR opening file");
            close(newsockfd);
            continue;
        }
        bzero(buffer, BUFFER_SIZE);
        while ((n = recv(newsockfd, buffer, BUFFER_SIZE, 0)) > 0) {
            fwrite(buffer,sizeof(char),n,fp);
            memset(buffer, 0, BUFFER_SIZE);
        }
        fclose(fp);
        cout << "\nFile received successfully\n";
        close(newsockfd);

        if (is_idle) {
            is_idle = false;
        }
        }

    // Cerrar el socket
    close(sockfd);

    return 0;

}
