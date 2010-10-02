/**********************************************************************************
 * QWiimotedev, wiimotedev-uinput qt4 front-end                                   *
 * Copyright (C) 2010  Bartlomiej Burdukiewicz                                    *
 * Contact: dev.strikeu@gmail.com                                                 *
 *                                                                                *
 * This library is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU Lesser General Public                     *
 * License as published by the Free Software Foundation; either                   *
 * version 2.1 of the License, or (at your option) any later version.             *
 *                                                                                *
 * This library is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU              *
 * Lesser General Public License for more details.                                *
 *                                                                                *
 * You should have received a copy of the GNU Lesser General Public               *
 * License along with this library; if not, write to the Free Software            *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QProcess>
#include <QMenu>
#include <QTimer>

#include "headers/consts.h"
#include "src/interfaces/customjobs.h"
#include "src/interfaces/deviceevents.h"
#include "src/interfaces/profilemanager.h"

namespace Ui {
    class MainWindow;
}

struct ProfileItem
{
    QString ProfileName;
    QString ProfileAuthor;
    QString ProfileEmail;
    QString ProfileVersion;
    QString ProfilePath;
};

class MainWindow :public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void nextProfile();
    void previousProfile();

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void wiimoteGeneralButtons(quint32 id, quint64 value);
    void slotMousePosition();

    void executeRequest(QStringList list);

private:
    bool windowAtMousePosition;

    QTimer checkMousePos;

    QRect calcGeometry();

private:
    QSystemTrayIcon *tray;
    QMenu *menu;
    QRect defaultGeometry;

    QMap< QString, quint64> devicebuttons;
    QList< struct ProfileItem> profileList;

    bool connectedWithService;

    quint64 buttons;
    quint64 lastButtons;
    quint32 index;

    Ui::MainWindow *ui;

    DBusCustomJobsInterface *customJobsInterface;
    DBusProfileManagerInterface *profileInterface;
    DBusDeviceEventsInterface *deviceInterface;

    void showProfile(struct ProfileItem item);
    void moveToCenter();

};

#endif // MAINWINDOW_H
