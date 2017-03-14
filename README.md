#web-server с функциональностью CGI

# OVERVIEW #

Сервер умеет закрепляться на определенному хосту:порту и слушать входящие соединения. На заданном адресе выдается HTML-страничка, если запрос корректен, либо оишбка 404, если запрашиваемой страницы не существует. 

## УСТАНОВКА И ЗАПУСК ##

1. Собираем и устанавливаем сервер.


```
#!bash

mkdir MyCgiServer
cd MyCgiServer
cmake /path/to/cgi/project/
make
```


2. Оформляем сервер в виде системной службы для systemd.


```
#!bash

cd /path/to/cgi/project/
gcc install.c -o install
```


3. Запускаем скомпилированный файл с правами админа и следуем инструкциям.


```
#!bash

sudo ./install
```