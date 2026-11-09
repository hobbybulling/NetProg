#include <iostream>
#include <string>
#include <memory>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Формат: " << argv[0] << " <IP> <Порт>\n";
        return 1;
    }

    std::string host_ip = argv[1];
    int dest_port = std::stoi(argv[2]);

    int tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_sock == -1) {
        perror("Не удалось создать сокет");
        return 1;
    }

    std::unique_ptr<sockaddr_in> dest_addr(new sockaddr_in);
    dest_addr->sin_family = AF_INET;
    dest_addr->sin_port = htons(dest_port);
    inet_pton(AF_INET, host_ip.c_str(), &dest_addr->sin_addr);

    if (connect(tcp_sock, reinterpret_cast<sockaddr*>(dest_addr.get()), sizeof(sockaddr_in)) < 0) {
        perror("Ошибка подключения");
        close(tcp_sock);
        return 1;
    }

    std::cout << "Успешно подключились к " << host_ip << ":" << dest_port << "\n";
    std::cout << "Введите 'exit' для выхода.\n";

    std::string user_input;
    char response[1024];

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, user_input);

        if (user_input == "exit") {
            break;
        }
        if (user_input.empty()) {
            continue;
        }

        ssize_t bytes_sent = send(tcp_sock, user_input.c_str(), user_input.size(), 0);
        if (bytes_sent <= 0) {
            std::cerr << "Ошибка отправки.\n";
            break;
        }

        memset(response, 0, sizeof(response));
        ssize_t bytes_read = recv(tcp_sock, response, sizeof(response) - 1, 0);

        if (bytes_read > 0) {
            std::cout << "Эхо: " << response << "\n";
        } else if (bytes_read == 0) {
            std::cout << "Сервер закрыл соединение.\n";
            break;
        } else {
            perror("Ошибка чтения");
            break;
        }
    }

    close(tcp_sock);
    return 0;
}
