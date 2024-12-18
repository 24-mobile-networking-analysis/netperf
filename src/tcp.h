#pragma once

#include "conn.h"
#include "fd.h"
#include "line.h"
#include <arpa/inet.h>
#include <linux/ip.h>
#include <netinet/in.h>
#include <memory>
#include <optional>
#include <utility>

class Tcp {
 public:
  static Fd Listen(int port, std::optional<Plan> const& plan = std::nullopt);

  static std::pair<Fd, sockaddr_in> Accept(Fd const& fd);

  static std::pair<Fd, sockaddr_in> Connect(
      char* ip, uint16_t port, std::optional<Plan> const& plan = std::nullopt);
  static std::pair<Fd, sockaddr_in> Connect(
      sockaddr_in address, std::optional<Plan> const& plan = std::nullopt);
};

class TcpConn : public Conn {
  Fd fd_;

 public:
  explicit TcpConn(Fd const& fd) { fd_ = fd; }
  int Send(char const* data, int size) override;
  int Receive(char data[], int size, int& skip_hint) override;
  int GetAdditionalBufferSize() override;
  ~TcpConn() override = default;

  static std::shared_ptr<TcpConn> CreateClientSide(Line const& line,
                                                   Plan const& plan);
  static std::shared_ptr<TcpConn> CreateServerSide(Plan const& plan);

  void Shutdown() override;
};