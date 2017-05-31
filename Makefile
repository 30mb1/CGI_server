all:
	gcc -o cgi main.c utility.c
	gcc -o install_service install.c
	./install_service
	rm install_service

install:
	mv cgi /usr/local/bin/
	mv cgi.service /etc/systemd/system/
	systemctl daemon-reload

clean:
	rm /usr/local/bin/cgi
	rm /etc/systemd/system/cgi.service
	systemctl daemon-reload
