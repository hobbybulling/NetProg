#include <iostream>
#include <cstring>
#include <memory>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Использование: " << argv[0] << " <IP_сервера> <Порт>\n";
        return 1;
    }

    const char* ip_str = argv[1];
    int port = std::stoi(argv[2]);

    int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock < 0) {
        perror("Ошибка создания сокета");
        return 1;
    }

    std::unique_ptr<sockaddr_in> server_addr(new sockaddr_in);
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(port);
    inet_pton(AF_INET, ip_str, &server_addr->sin_addr);

    struct timeval tv {5, 0};
    setsockopt(udp_sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    const char* msg = "PING";
    ssize_t sent = sendto(udp_sock, msg, strlen(msg), 0, 
                          reinterpret_cast<sockaddr*>(server_addr.get()), 
                          sizeof(sockaddr_in));
    if (sent < 0) {
        perror("Ошибка отправки");
        close(udp_sock);
        return 1;
    }

    char recv_buf[512];
    socklen_t addr_len = sizeof(sockaddr_in);
    ssize_t bytes = recvfrom(udp_sock, recv_buf, sizeof(recv_buf) - 1, 0,
                             reinterpret_cast<sockaddr*>(server_addr.get()), 
                             &addr_len);

    if (bytes < 0) {
        perror("Ошибка приема (сервер недоступен)");
    } else {
        recv_buf[bytes] = '\0';
        std::cout << "Время с сервера: " << recv_buf;
    }

    close(udp_sock);
    return 0;
}
