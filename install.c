#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>



int main(int argc, char *argv[]) {
	char host[16], path[PATH_MAX], cur_dir[PATH_MAX];
	int port;
	printf("This utility will help you to set up your server as a systemd service.\n");
	printf("Please, enter server host:\n");
	scanf("%s", host);
	//printf("%s\n", host);
	printf("Please, enter port:\n");
	scanf("%d", &port);
	//printf("%d\n", port);
	printf("Please, enter path to executable file:\n");
	scanf("%s", path);
	//printf("%s\n", path);

	getcwd(cur_dir, PATH_MAX);
	strcat(cur_dir, "/cgi.service");
	FILE * file = fopen(cur_dir, "w+");
	if (!file) {
		printf("cant creat cgi file\n");
	}
	getcwd(cur_dir, PATH_MAX);
	fprintf(file, "[Unit]\n"
				"Description=my cgi service.\n"
				"\n"
				"[Service]\n"
				"Type=forking\n"
				"ExecStart=%s %s %d %s\n"
				"ExecStop=%s stop %s\n"
				"\n"
				"[Install]\n"
				"WantedBy=multi-user.target\n",
				path, host, port, cur_dir,
				path, cur_dir);

	fclose(file);

	return 0;
}

