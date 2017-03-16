	#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>



int main(int argc, char *argv[]) {
	char dir[PATH_MAX];
	strcpy(dir, "/etc/systemd/system/cgi.service");
	FILE * file = fopen(dir, "w+");
	if (!file) {
		printf("Can't create .service file.\n");
	}
	getcwd(dir, PATH_MAX);
	fprintf(file, "[Unit]\n"
				"Description=my cgi service.\n"
				"\n"
				"[Service]\n"
				"Type=forking\n"
				"EnvironmentFile=%s/config\n"
				"ExecStart=/usr/local/bin/cgi $ARG1 $ARG2 %s\n"
				"ExecStop=/usr/local/bin/cgi stop %s\n"
				"\n"
				"[Install]\n"
				"WantedBy=multi-user.target\n",
				dir, dir, dir);
	fclose(file);
	printf("Server installed successully.\n"
		   "To change default configurations(host, port) see config file at %s/config.\n", dir);
	return 0;
}

