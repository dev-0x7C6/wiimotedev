/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2010  Bartłomiej Burdukiewicz                                    *
 * Contact: dev.strikeu@gmail.com                                                 *
 *                                                                                *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU Lesser General Public                     *
 * License as published by the Free Software Foundation; either                   *
 * version 2.1 of the License, or (at your option) any later version.             *
 *                                                                                *
 * This program is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU              *
 * Lesser General Public License for more details.                                *
 *                                                                                *
 * You should have received a copy of the GNU Lesser General Public               *
 * License along with this program; if not, see <http://www.gnu.org/licences/>.   *
 **********************************************************************************/

#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

#include <QWidget>
#include "headers/consts.h"
#include "src/interfaces/profilemanager.h"

namespace Ui {
    class ProfileWidget;
}

class ProfileWidget : public QWidget
{
  Q_OBJECT
  QString profileName;
  QString profileAuthor;
  QString profileVersion;
  QString profileEMail;
  DBusProfileManagerInterface *profile;

  quint32 fontSize;
  quint64 lastButtons;

public:
  explicit ProfileWidget(QString profileName, QString profileFile,
                         QString profileAuthor, QString profileVersion,
                         QString profileEMail, DBusProfileManagerInterface *profile, QWidget *parent = 0);
  ~ProfileWidget();
    QString profileFile;


protected:
  virtual void mouseMoveEvent (QMouseEvent * event);

public slots:
  void dbusWiimoteGeneralButtons(quint32 id, quint64 value);
  void selectProfile();

  void setActive();
  void setInactive();

signals:
  void loadProfile(ProfileWidget*);

private:
  Ui::ProfileWidget *ui;
};

#endif // PROFILEWIDGET_H
