#include <time.h>
#include <string.h>
#include <stdio.h>


int is_ip(char * IP) {
    char * endptr;
    long int a;
    if (a = strtol(IP, &endptr, 10)) {
        if (a >= 0 && a <= 255) {
            int i;
            for (i = 0 ; i < 3; i++) {
                ++endptr;
                char * tmp = endptr;
                if (*(endptr) == 48 && *(endptr+1) == 46 && strtol(endptr, &endptr, 10) == 0) continue;
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