#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h> 

#define SERVER_PORT 7
#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " hostname" << std::endl;
    return 1;
  }

  // 1. Создаем TCP сокет
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    std::cerr << "Error opening socket: " << strerror(errno) << std::endl;
    return 1;
  }

  // 2. Получаем информацию о хосте
  struct hostent* server = gethostbyname(argv[1]);
  if (server == NULL) {
    std::cerr << "Error: No such host" << std::endl;
    return 1;
  }

  // 3. Заполняем структуру sockaddr_in
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
  serv_addr.sin_port = htons(SERVER_PORT);

  // 4. Подключаемся к серверу
  if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    std::cerr << "Error connecting: " << strerror(errno) << std::endl;
    return 1;
  }

  // 5. Вводим сообщение
  std::cout << "Enter message: ";
  std::string message;
  std::getline(std::cin, message);

  // 6. Отправляем сообщение
  int bytes_sent = send(sockfd, message.c_str(), message.length(), 0);
  if (bytes_sent < 0) {
    std::cerr << "Error sending to socket: " << strerror(errno) << std::endl;
    return 1;
  }

  // 7. Получаем ответ
  char buffer[BUFFER_SIZE];
  int bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0);
  if (bytes_received < 0) {
    std::cerr << "Error receiving from socket: " << strerror(errno) << std::endl;
    return 1;
  }

  // 8. Выводим полученное сообщение
  std::cout << "Echo: " << buffer << std::endl;

  // 9. Закрываем сокет
  close(sockfd);

  return 0;
}
