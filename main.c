#include "utility.h"


void handle_request(int client_socket, struct sockaddr_in * client_address, char* htdocs) {
    char *client_ip = inet_ntoa(client_address->sin_addr);
    int client_port = ntohs(client_address->sin_port);

    printf("[INFO]: New request from %s:%d\n", client_ip, client_port);

    char recv_buffer[4096];
    read(client_socket, recv_buffer, sizeof(recv_buffer));
    //printf("%s\n", recv_buffer);
    char *query_str = parse_request(recv_buffer);
    GET(client_socket, query_str, htdocs);
    close(client_socket);
}

void start_server_on_port(char* host, int port, char* htdocs)
{

    int server_socket, client_socket;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;


    if (!(is_ip(host))) {
    	gettime();
    	printf("[ERROR]: Invalid host \n");
    	exit(0);
    }

    socklen_t client_address_size = sizeof(client_address);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0) { // Something went wrong
        gettime();
        printf("[ERROR]: Could not create socket \n");
        exit(0);
    }
    gettime();
    printf("[OK]: Socket started successfully \n");


    inet_aton(host, &server_address.sin_addr);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        gettime();
        printf("[ERROR]: Binding error \n");
        exit(0);
    }
    gettime();
    printf("[OK]: Successfully binded \n");

    if (listen(server_socket, 10)) {
        gettime();
        printf("[ERROR]: Can't listen on %s:%d\n", host, port);
        exit(0);
    }
    gettime();
    printf("[OK]: Listening on %s:%d\n", host, port);
    gettime();
    printf("[INFO]: Try to open 'http://%s:%d' in your browser\n", host, port);

    while ((client_socket =  accept(server_socket, 
                            (struct sockaddr*) &client_address, 
                            &client_address_size)))
    {
        if (client_socket < 0) {
            gettime();
            printf("[ERROR]: Accept failed \n");
        }
        pid_t pid = fork();
        if (0 == pid) {
        	handle_request(client_socket, &client_address, htdocs);
        	exit(0);
        } else {
        	close(client_socket);
        }
    }
}

int main(int argc, char *argv[]) {

    if (strcmp(argv[1], "stop") == 0) {
        char path_to_pidfile[PATH_MAX];         //getting path to file with the PID of
        strcpy(path_to_pidfile, argv[2]);       //running server
        strcat(path_to_pidfile, "/cgi.pid");
        FILE * file = fopen(path_to_pidfile, "r+");
        //printf("%s\n", path_to_pidfile);
        if (!file) {
            gettime();
            printf("[ERROR]: Can't open PID file. Server is offline or some error is occured.\n");
            exit(0);
        } else {
            pid_t pid;
            fscanf(file, "%d", &pid);
            if (kill(pid, SIGTERM) == -1) {
                gettime();
                printf("[ERROR]: Can't terminate server. It is offline or some error is occured.\n");
                fclose(file);
                exit(0);
            }
            gettime();
            printf("[OK]: Server correctly stopped.\n");
            fclose(file);
            remove(path_to_pidfile);
        }
        exit(0);
    }

    //gettime();
    //printf("[INFO]: Usage: './server <host> <port>' \n");
    gettime();
    printf("[INFO]: web-server CGI \n");
    gettime();
    printf("[INFO]: by Aliev Magomed \n");
    //for (int i = 0; i < argc; i++) {
    //    printf("%s\n", argv[i]);
    //}
    
    if ((daemon(0, 1)) == -1) {
        printf("[ERROR]: Fork failed, something went wrong ;C\n");
        exit(0);
    }

    //OK, fork succsessful. Now creating PID file.

    char path_to_pidfile[PATH_MAX];         //getting path to file with the PID of
    strcpy(path_to_pidfile, argv[3]);       //running server
    strcat(path_to_pidfile, "/cgi.pid");


    FILE * file = fopen(path_to_pidfile, "a+");

    int tmp;
    if (fscanf(file, "%d", &tmp) != EOF) { //if PID file is not empty, it means server is running at the moment.
        gettime();
        printf("[ERROR]: Server is already running.\n");
        fclose(file);
        exit(0);
    }

    fprintf(file, "%d", getpid());
    fclose(file);

    char *htdocs = argv[3];
    strcat(htdocs, "/htdocs");
    start_server_on_port(argv[1], atoi(argv[2]), htdocs);
    return 0;
}
