#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int is_ip(char * IP) {

    char localhost[] = "localhost";
    char ip_local[] = "127.0.0.1";

    if (!strcmp(IP, localhost)) {
        IP = ip_local;
    }

    char * endptr;
    long int a;
    if ((a = strtol(IP, &endptr, 10))) {
        if (a >= 0 && a <= 255) {
            int i;
            for (i = 0 ; i < 3; i++) {
                ++endptr;
                char * tmp = endptr;
                if (*(endptr) == 48 && (*(endptr+1) == 46 || *(endptr+1) == 0) && strtol(endptr, &endptr, 10) == 0) continue;
                endptr = tmp;
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

char * parse_request(char *buffer) {
    char *end = strstr(buffer, "HTTP/1.1");
    int bytes = end - (buffer + 4);
    char *clean_str = (char *)malloc(bytes);
    memmove(clean_str, buffer + 4, bytes);
    *(clean_str + bytes - 1) = '\0';
    return clean_str;
}

void write_header(int client_socket, int status, long int file_size) {
    char headers[2048];
    if (status == 200) {
        sprintf(headers,"HTTP/1.1 200 OK\r\n"
                        "Server: CustomCGI\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: %ld\r\n"
                        "\r\n", 
                        file_size);
    } else {
        char error[] = "sorry, this file does not exist ;(";
        sprintf(headers,"HTTP/1.1 404 NOT FOUND\r\n"
                        "Server: CustomCGI\r\n"
                        "Content-Type: text/plain\r\n"
                        "Content-Length: %ld\r\n"
                        "\r\n"
                        "%s",
                        strlen(error), error);
    }
    write(client_socket, headers, strlen(headers));
}

void GET(int client_socket, char *url) {
    char file_name[2048]; 
    strcpy(file_name, "../htdocs");
    if (0 == strcmp("/", url) || 0 == strlen(url) || 0 == strcmp("/favicon.ico", url)) {
        strcat(file_name, "/index.html");
    } else {
        strcat(file_name, url);
    }
    //printf("%s\n", file_name);
    int file_id = open(file_name, O_RDONLY);
    if (-1 == file_id) {
        write_header(client_socket, 404, 0);
    } else {
        struct stat st;
        fstat(file_id, &st);
        //printf("%ld\n", st.st_size);
        write_header(client_socket, 200, st.st_size);
        char page[8192];
        int ret_in;
        while ((ret_in = read(file_id, page, 8192)) > 0) {
            int ret_out = 0;
            while (ret_out != ret_in) {
                ret_out += write(client_socket, page + ret_out, ret_in - ret_out);
            }
        }
        close(file_id);
    }

}

