# web-server с функциональностью CGI #

## OVERVIEW ##

Сервер умеет закрепляться на определенному хосту:порту и слушать входящие соединения. На заданном адресе выдается HTML-страничка, если запрос корректен, либо ошибка 404, если запрашиваемой страницы не существует. 

## УСТАНОВКА И ЗАПУСК ##

 Установка сервера:


```

cd cgi/
make install
```

Сервер теперь можно запускать также, как и системные службы(работает только на Ubuntu):


```

service cgi start
service cgi stop
```

## How it works ##

При запуске программа создает PID файл, в который записывает ID текущего процесса, чтобы в дальнейшем можно было послать на него сигнал для завершения работы.


```с++

//creating PID file
FILE * file = fopen(path_to_pidfile, "a+");
fprintf(file, "%d", getpid());
fclose(file);

//killing running server
FILE * file = fopen(path_to_pidfile, "r+");
fscanf(file, "%d", &pid);
kill(pid, SIGTERM);
fclose(file);
remove(path_to_pidfile);
```
Далее с помощью набор функций для работы с сокетами настраиваем наш сервер.
Если все прошло без ошибок, начинаем принимать входящие соединения. Для каждого принятого соединения создаем отдельный процесс, родительский процесс при этом продолжает слушать.


```с++

pid_t pid = fork();
if (0 == pid) {
    handle_request(client_socket, &client_address, htdocs);
    exit(0);
} else {
    close(client_socket);
}
```

Дочерний процесс считывает запрос, обрабатывает его и выдает страницу ответ: если файл был найден, то выдается соответствующая страничка, в противном случае пользователь получит ошибку 404.


```с++

read(client_socket, recv_buffer, sizeof(recv_buffer));
char *query_str = parse_request(recv_buffer);
GET(client_socket, query_str, htdocs);
```

### Отчет по проделанной работе ###

На данный момент реализовано:

1. Веб-сервер выдает статические страницы и обслуживает одновременно несколько подключений.
2. Сервер оформлен в виде службы для systemd.
3. Сервер корректно обрабатывает ошибочные запросы.

Coming soon:

1. Работа со сторонними программами и выдача результата их работы.
2. Поддержка POST запросов.
