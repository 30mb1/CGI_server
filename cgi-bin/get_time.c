#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	time_t timer;
	time(&timer);

	struct tm timeinfo = *localtime(&timer);
	char cur_date[100];
	sprintf (cur_date, "The current date/time is: %s", asctime (&timeinfo) );


	FILE * file_stream = fdopen(atoi(argv[0]), "w");

	fprintf(file_stream, "HTTP/1.1 200 OK\r\n"
			"Server: CustomCGI\r\n"
			"Content-Type: text/html\r\n"
			"Content-Length: %ld\r\n"
			"\r\n"
			"%s",
			strlen(cur_date), cur_date);

	return 0;
}