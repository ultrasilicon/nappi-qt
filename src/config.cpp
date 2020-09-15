#include "config.h"

#include <QDir>
#include <QFile>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


using namespace boost::property_tree;

Config::Config()
{
  load();
}

void Config::load()
{
  QDir dir;
  if(!dir.mkpath(NAPPI_CONF_DIR)) {
    printf("Erro: failed to create directory: %s\n", NAPPI_CONF_DIR.toStdString().c_str());
    exit(1);
  }
  if(!dir.exists(NAPPI_CONF_FILE)) {
    printf("Info: config file not found, creating an example: %s\n", NAPPI_CONF_FILE.toStdString().c_str());
    ptree root;
    root.put("heartbeat_timeout", NAPPI_CONF_VAR_HEARTBEAT);
    root.put("port", NAPPI_CONF_VAR_PORT);
    root.put("host", NAPPI_CONF_VAR_HOST);
    write_json(NAPPI_CONF_FILE.toStdString(), root);
  } else {
    ptree root;
    read_json(NAPPI_CONF_FILE.toStdString(), root);
    this->heartbeat_timeout = root.get<int>("heartbeat_timeout");
    this->port = root.get<int>("port");
    this->host = root.get<std::string>("host");
    printf("{host: %s, port: %d, heatbeat_timeout: %d}\n", host.c_str(), port, heartbeat_timeout);
  }
}
