#ifndef SERVICE_H
#define SERVICE_H

#include "config.h"

#include <libagio/udp_socket.h>
#include <libagio/timer.h>
#include <QThread>
#include <chrono>


class Service
    : public QThread
{
  Q_OBJECT

public:
  enum PiState {
    Online = 0,
    Offline,
  };

  Service(Config* config, QObject* parent = nullptr);

protected:
  void run();

private:
  void checkPiOnlineState(Agio::Timer*);
  void sendWake(Agio::Timer*);
  void readMsg(std::string& data, Agio::HostAddress& addr);

  Agio::Loop* loop;
  Agio::UdpSocket* udp_socket;

  Agio::HostAddress server_address;
  uint16_t port = 65282;

  PiState pi_state = Offline;
  int64_t offline_timestamp;
  Config* config;
};

#endif // SERVICE_H
