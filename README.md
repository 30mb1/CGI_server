#web-server с функциональностью CGI

Сервер умеет закрепляться на определенному хосту:порту и слушать входящие соединения. На заданном адресе выдается простенькая HTML-страничка. 

Работа с сокетами реализована через встроенную библиотеку sys/socket.h. 
Реализована мультипроцессорность с помощью функции fork.