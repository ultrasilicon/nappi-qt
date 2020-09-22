#include "service.h"
#include "version.h"

#include <boost/property_tree/json_parser.hpp>
#include <sstream>

using namespace Agio;
using namespace boost::property_tree;

Service::Service(Config* config, QObject* parent)
  : QThread(parent)
  , loop(Loop::defaultLoop())
  , udp_socket(new UdpSocket(loop))
  , server_address(HostAddress("10.10.10.21", this->port))
  , config(config)
{
}

void Service::run()
{
  fflush(stdout);

  udp_socket->bind("0.0.0.0", port);
  udp_socket->start();
  on(&udp_socket->onReadyRead, this, &Service::readMsg);

  Timer statusCheckTimer(loop);
  statusCheckTimer.start(1000);
  on(&statusCheckTimer.onTimedOut, this, &Service::checkPiOnlineState);

  Timer wakeTimer(loop);
  wakeTimer.start(300);
  on(&wakeTimer.onTimedOut, this, &Service::sendWake);

  loop->run();
}

void Service::checkPiOnlineState(Timer*)
{
  using namespace std::chrono;
  int64_t now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
  PiState newState = now - offline_timestamp > config->heartbeat_timeout
             ? PiState::Offline : PiState::Online;
  if(newState != pi_state) {
      printf("Info: pi went %s\n", newState == PiState::Online ? "online" : "offline");
      pi_state = newState;
      emit piStateChanged(pi_state);
    }
}

void Service::sendWake(Timer*)
{
  const char* wakeMsg = "{\"type\": \"wake\", \"version\": \"0.0.1\"}";
  udp_socket->write(config->host.c_str(),
                    config->port,
                    wakeMsg);
}

void Service::readMsg(std::string& data, HostAddress& addr)
{
  if(addr.ipString() != config->host)
    return;
  std::stringstream ss;
  ss << data;
  ptree msg;
  try {
    read_json(ss, msg);
  } catch (...) {
    // injection vulnerability here in printf %s.
    printf("Info: bad packet: %s\n", data.c_str());
  }

  if(msg.get<std::string>("version") != VERSION_STR)
    return;
  if(msg.get<std::string>("type") == "heartbeat") {
    using namespace std::chrono;
    offline_timestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
  }
}
