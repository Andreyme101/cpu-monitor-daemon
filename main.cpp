#include <iostream>
#include <fstream>
#include <csignal>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>
#include <fcntl.h>

void daemonize() {
    pid_t pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);

    if (pid > 0)
        exit(EXIT_SUCCESS);  // Родитель выходит

    if (setsid() < 0)
        exit(EXIT_FAILURE);  // Становимся лидером сессии

    signal(SIGHUP, SIG_IGN);  // Игнорируем сигнал терминала

    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);

    if (pid > 0)
        exit(EXIT_SUCCESS);  // Первый дочерний выходит

    umask(0);                // Сбрасываем маску прав
    chdir("/");              // Меняем рабочую директорию

    // Закрываем стандартные дескрипторы
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int main() {
    daemonize();  // Уходим в фоновый режим

    openlog("cpu_monitor", LOG_PID, LOG_DAEMON);
    syslog(LOG_INFO, "CPU Monitor Daemon started");

    while (true) {
        syslog(LOG_INFO, "Still alive...");

        sleep(5);  // Засыпаем на 5 секунд
    }

    closelog();
    return 0;
}

