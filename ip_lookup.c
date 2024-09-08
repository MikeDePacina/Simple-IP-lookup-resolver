#include "portability_config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef AI_ALL
#define AI_ALL 0x0100
#endif

/*
Program that will take in hostname or IP address in cli then well use getaddrinfo
to print info about it
*/
int main(int argc, char *argv[]){

    if(argc < 2){
        printf("Usage:\tprogram.exe hostname\n");
        printf("Example:\t./ip_lookup youtube.com\n");
        exit(0);
    }

    #if defined(_WIN32)
        WSADATA d;
        if (WSAStartup(MAKEWORD(2, 2), &d)) {
            fprintf(stderr, "Failed to initialize.\n");
            return 1;
        }
    #endif

    printf("Resolving hostname '%s'\n", argv[1]);
    struct addrinfo hints;

    memset(&hints, 0, sizeof(hints));
    //we want all IP addresses (IPv4 and IPv6)
    hints.ai_flags = AI_ALL;
    //will hold the address(es) we can use with getnameinfo() to convert to text
    struct addrinfo *peer_address;

    //2nd argument is 0 cos we don't care about port number or service type
    /*
    OS performs DNS query checks its own cache first. If cli argument
    is already an IP address then it will just simply fill that in peer_address
    */
    if (getaddrinfo(argv[1], 0, &hints, &peer_address)) {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Remote address is:\n");
    struct addrinfo *address = peer_address;
    do {
        char address_buffer[100];
        getnameinfo(address->ai_addr, address->ai_addrlen,
                address_buffer, sizeof(address_buffer),
                0, 0,
                NI_NUMERICHOST);
        printf("\t%s\n", address_buffer);
    } while ((address = address->ai_next));

    freeaddrinfo(peer_address);

    #if defined(_WIN32)
        WSACleanup();
    #endif

    return 0;
}