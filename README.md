#web-server с функциональностью CGI

Сервер умеет закрепляться на определенному хосту:порту и соугать входящие соединения. На заданном адресе выдается простенькая HTML-страничка. Работа с сокетами реализована через встроенную библиотеку sys/socket.h.

В будущем будет добавлена работа с несколькими подключениями.