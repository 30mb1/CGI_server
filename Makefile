.SILENT:
install:
	gcc -o cgi main.c utility.c
	gcc -o install install.c
	sudo install ./cgi /usr/local/bin/
	sudo ./install
	rm install
	rm cgi
	sudo systemctl daemon-reload