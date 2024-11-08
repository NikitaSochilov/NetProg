#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h> 

#define SERVER_PORT 13
#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " hostname" << std::endl;
    return 1;
  }

  // 1. Создаем UDP сокет
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
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

  // 4. Отправляем пустой запрос
  int bytes_sent = sendto(sockfd, "", 0, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  if (bytes_sent < 0) {
    std::cerr << "Error sending to socket: " << strerror(errno) << std::endl;
    return 1;
  }

  // 5. Получаем ответ
  char buffer[BUFFER_SIZE];
  socklen_t addr_len = sizeof(serv_addr);
  int bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&serv_addr, &addr_len);
  if (bytes_received < 0) {
    std::cerr << "Error receiving from socket: " << strerror(errno) << std::endl;
    return 1;
  }

  // 6. Обработка конца строки:
  //    - Ищем символ конца строки в полученных данных.
  //    - Если он не найден, добавляем его в конец строки.
  for (int i = 0; i < bytes_received; ++i) {
    if (buffer[i] == '\n') {
      buffer[i] = '\0'; // Заменяем символ конца строки на нулевой символ 
      break;            // Прекращаем цикл
    }
  }
  if (buffer[bytes_received - 1] != '\0') {
    buffer[bytes_received] = '\0'; // Добавляем нулевой символ в конец строки
  }

  // 7. Выводим текущее время
  std::cout << "Current time: " << buffer << std::endl;

  // 8. Закрываем сокет
  close(sockfd); 

  return 0;
}
