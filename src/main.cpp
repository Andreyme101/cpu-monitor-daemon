#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <csignal>
#include <cstdlib>
#include <sstream>

bool running = true;


void handle_signal(int signal) {
    running = false;
}

float get_cpu_usage() {
    static long prev_idle = 0, prev_total = 0;

    std::ifstream stat_file("/proc/stat");
    std::string line;
    std::getline(stat_file, line);

    std::istringstream iss(line);
    std::string cpu;
    long user, nice, system, idle, iowait, irq, softirq, steal;

    iss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    long idle_time = idle + iowait;
    long total_time = user + nice + system + idle_time + irq + softirq + steal;

    long diff_idle = idle_time - prev_idle;
    long diff_total = total_time - prev_total;

    prev_idle = idle_time;
    prev_total = total_time;

    return 100.0f * (1.0f - static_cast<float>(diff_idle) / diff_total);
}

int main() {
    signal(SIGINT, handle_signal);

    std::ofstream log("/var/log/cpu_monitor.log", std::ios::app);
    if (!log.is_open()) {
        std::cerr << "Не удалось открыть лог-файл. Запусти с sudo!" << std::endl;
        return 1;
    }

    log << "== CPU Monitor запущен ==" << std::endl;

    while (running) {
        float cpu_usage = get_cpu_usage();
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        log << std::ctime(&now) << "CPU Usage: " << cpu_usage << "%" << std::endl;
        log.flush();
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    log << "== CPU Monitor остановлен ==" << std::endl;
    return 0;
}

