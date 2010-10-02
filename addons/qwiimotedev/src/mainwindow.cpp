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


#include <QDesktopWidget>
#include <QSettings>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QtDebug>
#include <QPointer>
#include <QCursor>

#include "mainwindow.h"
#include "ui_mainwindow.h"

const QString wiimotedevDirectory = ".wiimotedev/";
const QString profileDirectory = "profiles/";

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    QStringList directoryList;

    directoryList << QDir::homePath() + QString::fromUtf8("/");
    directoryList << directoryList.at(0) + wiimotedevDirectory;
    directoryList << directoryList.at(1) + profileDirectory;

    devicebuttons.insert("wiimote.1", WIIMOTE_BTN_1);
    devicebuttons.insert("wiimote.2", WIIMOTE_BTN_2);
    devicebuttons.insert("wiimote.a", WIIMOTE_BTN_A);
    devicebuttons.insert("wiimote.b", WIIMOTE_BTN_B);
    devicebuttons.insert("wiimote.plus", WIIMOTE_BTN_PLUS);
    devicebuttons.insert("wiimote.minus", WIIMOTE_BTN_MINUS);
    devicebuttons.insert("wiimote.home", WIIMOTE_BTN_HOME);
    devicebuttons.insert("wiimote.up", WIIMOTE_BTN_UP);
    devicebuttons.insert("wiimote.down", WIIMOTE_BTN_DOWN);
    devicebuttons.insert("wiimote.left", WIIMOTE_BTN_LEFT);
    devicebuttons.insert("wiimote.right", WIIMOTE_BTN_RIGHT);
    devicebuttons.insert("wiimote.shift.up", WIIMOTE_BTN_SHIFT_UP);
    devicebuttons.insert("wiimote.shift.down", WIIMOTE_BTN_SHIFT_DOWN);
    devicebuttons.insert("wiimote.shift.left", WIIMOTE_BTN_SHIFT_LEFT);
    devicebuttons.insert("wiimote.shift.right", WIIMOTE_BTN_SHIFT_RIGHT);
    devicebuttons.insert("wiimote.shift.forward", WIIMOTE_BTN_SHIFT_FORWARD);
    devicebuttons.insert("wiimote.shift.backward", WIIMOTE_BTN_SHIFT_BACKWARD);
    devicebuttons.insert("wiimote.tilt.front", WIIMOTE_BTN_TILT_FRONT);
    devicebuttons.insert("wiimote.tilt.back", WIIMOTE_BTN_TILT_BACK);
    devicebuttons.insert("wiimote.tilt.left", WIIMOTE_BTN_TILT_LEFT);
    devicebuttons.insert("wiimote.tilt.right", WIIMOTE_BTN_TILT_RIGHT);
    devicebuttons.insert("nunchuk.z", NUNCHUK_BTN_Z);
    devicebuttons.insert("nunchuk.c", NUNCHUK_BTN_C);
    devicebuttons.insert("nunchuk.stick.up", NUNCHUK_BTN_STICK_UP);
    devicebuttons.insert("nunchuk.stick.down", NUNCHUK_BTN_STICK_DOWN);
    devicebuttons.insert("nunchuk.stick.left", NUNCHUK_BTN_STICK_LEFT);
    devicebuttons.insert("nunchuk.stick.right", NUNCHUK_BTN_STICK_RIGHT);
    devicebuttons.insert("nunchuk.shift.up", NUNCHUK_BTN_SHIFT_UP);
    devicebuttons.insert("nunchuk.shift.down", NUNCHUK_BTN_SHIFT_DOWN);
    devicebuttons.insert("nunchuk.shift.left", NUNCHUK_BTN_SHIFT_LEFT);
    devicebuttons.insert("nunchuk.shift.right", NUNCHUK_BTN_SHIFT_RIGHT);
    devicebuttons.insert("nunchuk.shift.forward", NUNCHUK_BTN_SHIFT_FORWARD);
    devicebuttons.insert("nunchuk.shift.backward", NUNCHUK_BTN_SHIFT_BACKWARD);
    devicebuttons.insert("nunchuk.tilt.front", NUNCHUK_BTN_TILT_FRONT);
    devicebuttons.insert("nunchuk.tilt.back", NUNCHUK_BTN_TILT_BACK);
    devicebuttons.insert("nunchuk.tilt.left", NUNCHUK_BTN_TILT_LEFT);
    devicebuttons.insert("nunchuk.tilt.right", NUNCHUK_BTN_TILT_RIGHT);
    devicebuttons.insert("classic.x", CLASSIC_BTN_X);
    devicebuttons.insert("classic.y", CLASSIC_BTN_Y);
    devicebuttons.insert("classic.a", CLASSIC_BTN_A);
    devicebuttons.insert("classic.b", CLASSIC_BTN_B);
    devicebuttons.insert("classic.l", CLASSIC_BTN_L);
    devicebuttons.insert("classic.r", CLASSIC_BTN_R);
    devicebuttons.insert("classic.zl", CLASSIC_BTN_ZL);
    devicebuttons.insert("classic.zr", CLASSIC_BTN_ZR);
    devicebuttons.insert("classic.minus", CLASSIC_BTN_MINUS);
    devicebuttons.insert("classic.plus", CLASSIC_BTN_PLUS);
    devicebuttons.insert("classic.home", CLASSIC_BTN_HOME);
    devicebuttons.insert("classic.up", CLASSIC_BTN_UP);
    devicebuttons.insert("classic.down", CLASSIC_BTN_DOWN);
    devicebuttons.insert("classic.left", CLASSIC_BTN_LEFT);
    devicebuttons.insert("classic.right", CLASSIC_BTN_RIGHT);
    devicebuttons.insert("classic.lstick.up", CLASSIC_BTN_LSTICK_UP);
    devicebuttons.insert("classic.lstick.down", CLASSIC_BTN_LSTICK_DOWN);
    devicebuttons.insert("classic.lstick.left", CLASSIC_BTN_LSTICK_LEFT);
    devicebuttons.insert("classic.lstick.right", CLASSIC_BTN_LSTICK_RIGHT);
    devicebuttons.insert("classic.rstick.up", CLASSIC_BTN_RSTICK_UP);
    devicebuttons.insert("classic.rstick.down", CLASSIC_BTN_RSTICK_DOWN);
    devicebuttons.insert("classic.rstick.left", CLASSIC_BTN_RSTICK_LEFT);
    devicebuttons.insert("classic.rstick.right", CLASSIC_BTN_RSTICK_RIGHT);

    QDir dir(QChar('/'));

    for (register qint8 i = 0; i < directoryList.count(); ++i)
        if (!dir.exists(directoryList.at(i)))
            dir.mkdir(directoryList.at(i));

    dir.setPath(directoryList.at(2));

    QFileInfoList list = dir.entryInfoList();

    struct ProfileItem item;
    for (register qint32 i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.isFile() && (fileInfo.suffix().toLower() == QString("ini")))
        {
            QSettings settings(fileInfo.absoluteFilePath(), QSettings::IniFormat);
            settings.beginGroup("profile");
            item.ProfileName = settings.value("name", QString("undefined")).toString();
            item.ProfileVersion = settings.value("version", QString("undefined")).toString();
            item.ProfileAuthor = settings.value("author", QString("undefined")).toString();
            item.ProfileEmail = settings.value("email", QString("undefined")).toString();
            item.ProfilePath = fileInfo.absoluteFilePath();
            profileList << item;
            settings.endGroup();
        }
    }

    windowAtMousePosition = true;

    ui->setupUi(this);
    ui->nextProfile->setIcon(QIcon("/usr/share/qwiimotedev/arrow-right.png"));
    ui->previousProfile->setIcon(QIcon("/usr/share/qwiimotedev/arrow-left.png"));

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setWindowOpacity(0.95);

    moveToCenter();

    index = 1;

    ui->previousProfile->setEnabled(false);
    if (profileList.count() == 1)
        ui->nextProfile->setEnabled(false);

    if (profileList.count() < 1){
        item.ProfileAuthor = "";
        item.ProfileEmail = "";
        item.ProfileVersion = "";
        item.ProfilePath = "";
        item.ProfileName = "";
        showProfile(item);
    } else
        showProfile(profileList.at(0));

    connect(ui->nextProfile, SIGNAL(clicked()), this, SLOT(nextProfile()));
    connect(ui->previousProfile, SIGNAL(clicked()), this, SLOT(previousProfile()));


    profileInterface = new DBusProfileManagerInterface("org.wiimotedev.uinput",
                                                       "/profileManager",
                                                       QDBusConnection::systemBus(), this);

    customJobsInterface = new DBusCustomJobsInterface("org.wiimotedev.uinput",
                                                      "/customJobs",
                                                      QDBusConnection::systemBus(), this);

    deviceInterface = new DBusDeviceEventsInterface("org.wiimotedev.daemon",
                                                    "/deviceEvents",
                                                    QDBusConnection::systemBus(), this);


    connect(deviceInterface, SIGNAL(dbusWiimoteGeneralButtons(quint32,quint64)), this, SLOT(wiimoteGeneralButtons(quint32,quint64)));
    connect(customJobsInterface, SIGNAL(executeRequest(QStringList)), this, SLOT(executeRequest(QStringList)));

    connectedWithService = false;

    startTimer(1000);


    connect(&checkMousePos, SIGNAL(timeout()), this, SLOT(slotMousePosition()));
    checkMousePos.start(100);

    menu = new QMenu(this);

    tray = new QSystemTrayIcon(QIcon("/usr/share/qwiimotedev/tray.png"));
    // tray->setContextMenu(menu);

    QMenu *m;
    m = menu->addMenu("Screen");
    m->addAction("At mouse position")->setCheckable(true);
    for (register int i = 0; i < QApplication::desktop()->screenCount(); ++i)
        m->addAction(QString("Screen %1").arg(i + 1))->setCheckable(true);
    tray->show();
}

void MainWindow::slotMousePosition()
{
    if (isVisible())
    {
        QRect windowPos = calcGeometry();
        if (windowPos != defaultGeometry)
            moveToCenter();
    }
}

QRect MainWindow::calcGeometry()
{
    int screenId = 0;

    for (register int i = QApplication::desktop()->screenCount(); i > 0; --i)
    {
        if (QApplication::desktop()->screenGeometry(i - 1).x() <= QCursor::pos().x())
        {
            screenId = i - 1;
            break;
        }
    }

    QRect desktopRect = QApplication::desktop()->screenGeometry(screenId);
    QRect windowPos;

    windowPos.setX(desktopRect.x() + ((desktopRect.width() / 2) - (geometry().width() / 2)));
    windowPos.setY(desktopRect.y() + ((desktopRect.height() / 2) - (geometry().height() / 2)));
    windowPos.setWidth(geometry().width());
    windowPos.setHeight(geometry().height());

    return windowPos;
}

void MainWindow::moveToCenter()
{
    setGeometry(calcGeometry());
    defaultGeometry = calcGeometry();
}

MainWindow::~MainWindow()
{
    tray->hide();
    delete tray;
    delete ui;
}

void MainWindow::executeRequest(QStringList list)
{
    if (!list.count())
        return;

    QString process = list.at(0);
    list.removeAt(0);

    QProcess::execute(process, list);
}

void MainWindow::wiimoteGeneralButtons(quint32 id, quint64 value)
{
    buttons = value & WIIMOTE_BUTTON_MASK;

    if ((id != 1) || (buttons == lastButtons)) return;
    lastButtons = buttons;

    if (value & WIIMOTE_BTN_HOME)
        if (isVisible())
      {
            hide();
          } else
            {
                moveToCenter();
                show();
            }

    if (isVisible())
    {
        if ((value & WIIMOTE_BTN_B) || (value & WIIMOTE_BTN_PLUS))
            ui->stackedWidget->setCurrentIndex(1); else
            ui->stackedWidget->setCurrentIndex(0);
        if ((value & WIIMOTE_BTN_LEFT) || (value & WIIMOTE_BTN_UP)) previousProfile();
        if ((value & WIIMOTE_BTN_RIGHT) || (value & WIIMOTE_BTN_DOWN)) nextProfile();
        if ((value & WIIMOTE_BTN_A) || (value & WIIMOTE_BTN_2))
        {
            profileInterface->loadProfile(profileList.at(index - 1).ProfilePath);
            setVisible(false);
        }
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if (connectedWithService != profileInterface->isValid())
    {
        if (profileInterface->isValid())
            tray->showMessage(QString::fromUtf8("D-Bus"), QString::fromUtf8("Connection with service established"), QSystemTrayIcon::Information, 5000); else
            tray->showMessage(QString::fromUtf8("D-Bus"), QString::fromUtf8("Disconnected from service"), QSystemTrayIcon::Information, 5000);
        connectedWithService = profileInterface->isValid();
    }
}

void MainWindow::nextProfile()
{
    if (index < profileList.count()) {
        index++;
        if (index == profileList.count())
            ui->nextProfile->setEnabled(false);
        if (index > 1)
            ui->previousProfile->setEnabled(true);
        showProfile(profileList.at(index - 1));
    }
}

void MainWindow::previousProfile()
{
    if (index > 1)
    {
        index--;
        if (index == 1)
            ui->previousProfile->setEnabled(false);
        if (index < profileList.count())
            ui->nextProfile->setEnabled(true);
        showProfile(profileList.at(index - 1));
    }
}

void MainWindow::showProfile(struct ProfileItem item)
{
    ui->profileName->setText(item.ProfileName);
    ui->profileVersion->setText(item.ProfileVersion);
    ui->profilePath->setText(item.ProfilePath);
    ui->profileEmail->setText(item.ProfileEmail);
    ui->profileAuthor->setText(item.ProfileAuthor);
    ui->largeProfileName->setText(item.ProfileName);
    this->repaint();
    if (geometry() != defaultGeometry) moveToCenter();
}

