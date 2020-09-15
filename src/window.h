#ifndef WIDGET_H
#define WIDGET_H

#include "config.h"

#include <QWidget>
#include <QMenu>
#include <QSystemTrayIcon>


class Window : public QWidget
{
  Q_OBJECT

public:
  Window(Config* config, QWidget *parent = nullptr);
  ~Window();

private:
  void onMessage();
  inline void refreshTrayIcon(const bool& online);

  QMenu *tray_icon_menu;
  QAction *wake_action;
  QAction *show_action;
  QAction *quit_action;
  QSystemTrayIcon *tray_icon;


};
#endif // WIDGET_H
