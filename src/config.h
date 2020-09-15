#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

#define USR_HOME_DIR getenv("HOME")
#define NAPPI_CONF_DIR (QString(USR_HOME_DIR)  + "/.config/nappi/")
#define NAPPI_CONF_FILE QString(NAPPI_CONF_DIR + "config.json")
#define NAPPI_CONF_VAR_HEARTBEAT 1000
#define NAPPI_CONF_VAR_PORT 65282
#define NAPPI_CONF_VAR_HOST "192.168.0.3"

class Config
{
public:
  Config();

  int heartbeat_timeout;
  int port;
  std::string host;

private:
  void load();
};

#endif // CONFIG_H
