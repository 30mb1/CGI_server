#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

int is_ip(char * IP) {
    char * endptr;
    long int a;
    if (a = strtol(IP, &endptr, 10)) {
        if (a >= 0 && a <= 255) {
            int i;
            for (i = 0 ; i < 3; i++) {
                ++endptr;
                if (!(a = strtol(endptr, &endptr, 10)) || !(a >= 0 && a <= 255)) {
                    return 0;
                }
            }
            return 1;
        } else {
            return 0;
        } 
    } else {
        return 0;
    }
    return 1;
}

void gettime() {
    time_t now;
    time(&now);
    char buff[256];
    strcpy(buff, ctime(&now));
    buff[strlen(buff)-1]='\0';
    printf("%s ", buff);
}

void handle_request(int client_socket, struct sockaddr_in * client_address)
{
    char *client_ip = inet_ntoa(client_address->sin_addr);
    int client_port = ntohs(client_address->sin_port);

    printf("New request from %s:%d\n", client_ip, client_port);

    const char* wow_page =
        "HTTP/1.0 200 OK\r\n"
        "Server: customCGI\r\n"
        "Content-type: text/html\r\n"
        "\r\n"
        "<html>\r\n"
        " <body>\r\n"
        "  <h1></h1>"
        "  <img src='https://pbs.twimg.com/profile_images/378800000822867536/3f5a00acf72df93528b6bb7cd0a4fd0c.jpeg'>\r\n"
        "  <h2>WOOOOOOW SUCH SERVER SO HTTP</h2>"
        " </body>\r\n"
        "</html>\r\n";

      write(client_socket, wow_page, strlen(wow_page));

      close(client_socket);
}

void start_server_on_port(char* host, int port)
{
    int server_socket, client_socket;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    char printf_message[1024];

    socklen_t client_address_size = sizeof(client_address);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0) { // Something went wrong
        gettime();
        printf("[ERROR]: Could not create socket \n");
        return;
    }
    gettime();
    printf("[OK]: Socket started successfully \n");

    if (!(is_ip(host))) {
    	gettime();
    	printf("[ERROR]: Invalid host \n");
    	return;
    }
    inet_aton(host, &server_address.sin_addr);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        gettime();
        printf("[ERROR]: Binding error \n");
        return;
    }
    gettime();
    printf("[OK]: Successfully binded \n");

    if (listen(server_socket, 10)) {
        gettime();
        printf("[ERROR]: Can't listen on %s:%d\n", host, port);
        return;
    }
    gettime();
    printf("[OK]: Listening on %s:%d\n", host, port);
    gettime();
    printf("[INFO]: Try to open 'http://%s:%d' in your browser\n", host, port);

    while (client_socket =  accept(server_socket, 
                            (struct sockaddr*) &client_address, 
                            &client_address_size)) 
    {
        
        if (client_socket < 0) {
            gettime();
            printf("[ERROR]: Accept failed \n");
        }
        handle_request(client_socket, &client_address);
    }
}

int main(int argc, char *argv[])
{
    gettime();
    printf("[INFO]: Usage: './server <host> <port>' \n");
    gettime();
    printf("[INFO]: web-server CGI \n");
    gettime();
    printf("[INFO]: by Aliev Magomed \n");
    if (argc != 3) {
    	gettime();
    	printf("[ERROR]: wrong arguments \n");
    	return 0;
    }

    start_server_on_port(argv[1], atoi(argv[2]));
    return 0;
}
