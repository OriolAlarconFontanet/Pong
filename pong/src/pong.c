#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>

#define PORT 8080
#define MAXBUFFER 65535

int main(int argc, char **argv) {

    (void) argc; // This is how an unused parameter warning is silenced.
    (void) argv;

    //Creacio d'un UDP socket
    int sock =socket(AF_INET, SOCK_DGRAM, 0);


    struct sockaddr_in serverAddr, clientAddr;
    uint8_t buffer[MAXBUFFER];
    socklen_t addrLen = sizeof(struct sockaddr);

  
    if ( sock == -1) {
        perror("Socket could not be created");
        exit(1);
    }

    // Adreça servidor i port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Vincular el socket a la adreça i al port
    if (bind(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind");
        exit(1);
    }

    // Esperar a que arribin els datagramas i respondre cada un d'ells
 
    for(int i= 0; i < 3; i++) {
        // Rebre un datagrama
        ssize_t Nbytesrebuts = recvfrom(sock, buffer, MAXBUFFER, 0, (struct sockaddr *)&clientAddr, &addrLen);
        if (Nbytesrebuts < 0) {
            perror("Recfrom no ha rebut res");
            exit(1);
        }
        buffer[Nbytesrebuts] = '\0';

       
        // Enviar un datagrama de resposta al remitent original utilitzant el mateix buffer
        ssize_t Nbytesenviats = sendto(sock, buffer, (size_t)Nbytesrebuts, 0, (struct sockaddr *)&clientAddr, addrLen);
if (Nbytesenviats == -(size_t)0) {
            perror("Sndto no ha evniat res");
            exit(1);
        }

      

        // Buidar el buffer per no tindre errors quan s'envien més d'un ping
        memset(buffer, 0, MAXBUFFER);
    }

    // Tancar el socket
    close(sock);

    return 0;
}
