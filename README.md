# web-server с функциональностью CGI #

CGI - это стандарт интерфейса, используемый для связи внешней программы для связи с сервером. Программу, которая работает по такому интерфейсу совместно с веб-сервером, принято называть шлюзом, хотя многие предпочитают названия «скрипт» или «CGI-программа». По сути позволяет использовать консоль ввода и вывода для взаимодействия с клиентом.

Сам интерфейс разработан таким образом, чтобы можно было использовать любой язык программирования, который может работать со стандартными устройствами ввода-вывода. Такими возможностями обладают даже скрипты для встроенных командных интерпретаторов операционных систем, поэтому в простых случаях могут использоваться даже командные скрипты.

CGI является одним из наиболее распространённых средств создания динамических сайтов. Этот стандарт используют такие популярные веб-сервера, как например, [Nginx](https://ru.wikipedia.org/wiki/Nginx) или [Apache](https://ru.wikipedia.org/wiki/Apache_HTTP_Server)


## OVERVIEW ##

Сервер умеет закрепляться на определенному хосту:порту и слушать входящие соединения. На заданном адресе выдается HTML-страничка, если запрос корректен, либо страница с ошибкой 404, если запрашиваемой страницы не существует. Также реализован cgi-скрипт показывающий время при нажатии на кнопку на главной странице.

## INSTALLING AND RUNNING SERVER ##

 Устанавливаем с помощью make. Установочный скрипт скомпилирует и переместит исполняемый файл в папку /usr/local/bin, затем создаст .service файл и поместит его в /etc/systemd/system/:


```

cd cgi/
make install
```

Сервер теперь можно запускать так же, как и системные службы(работает только на Ubuntu,
т.к помещает юнит-файл в папку /etc/systemd/system/, которая не является установочной на 
 других системах). По дефолту сервер запустится на 127.0.0.1:1235. Чтобы выставить свои настройки, нужно изменить config файл:


```

service cgi start
service cgi stop
```

## HOW IT WORKS ##

При запуске программа создает PID файл, в который записывает ID текущего процесса, чтобы в дальнейшем можно было послать на него сигнал для завершения работы. Если во время старта сервера возникла ошибка, файл не создается.


```
#!c

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
Далее с помощью набора функций для работы с сокетами настраиваем наш сервер.
Если все прошло без ошибок, начинаем принимать входящие соединения. Для каждого принятого соединения создаем отдельный процесс, родительский процесс при этом продолжает слушать.


```
#!c

pid_t pid = fork();
if (0 == pid) { //if it is child process - interact connection
    handle_request(client_socket, &client_address, htdocs);
    exit(0);
} else { //otherwise close socket and continue listening
    close(client_socket);
}
```

Дочерний процесс считывает запрос, обрабатывает его и выдает страницу ответ: если файл был найден, то выдается соответствующая страничка, в противном случае пользователь получит страницу с ошибкой 404. Вся статика хранится в папке htdocs сервера.


```
#!c

read(client_socket, recv_buffer, sizeof(recv_buffer));
char *query_str = parse_request(recv_buffer);
...
GET(client_socket, query_str, maindir);
```

При нажатии на главной странице на кнопку "get current time" выполняется скрипт, возвращающий текущую дату/время:


```
#!c

if (strcmp(query_str + strlen(query_str) - 4, "cgi?") == 0) {
        ...
        execve(main_dir, arg, empty2); //execute cgi-script
        close(client_socket);
}

```

### Отчет по проделанной работе ###

На данный момент реализовано:

1. Веб-сервер выдает статические страницы и обслуживает одновременно несколько подключений.
2. Сервер оформлен в виде службы для systemd.
3. Сервер корректно обрабатывает ошибочные запросы.
4. Сервер умеет работать со сторонними программами и выдавать результат их работы(на стадии разработки).

Coming soon:

1. Поддержка POST запросов.