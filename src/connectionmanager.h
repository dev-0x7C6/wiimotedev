/***************************************************************************
 *   Copyright (C) 2008-2010 by Bartlomiej Burdukiewicz                    *
 *   dev.strikeu@gmail.com                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QThread>
#include <QList>
#include <QTimer>
#include <QTime>


#include "wiimoteconnection.h"
#include "profilemanager.h"


class ConnectionManager : public QThread
{
    Q_OBJECT

private:
    QMap< QString, quint16> wiiremoteSequence;
    QList< void*> objectList;

    bdaddr_t bdaddr_any;
    bool terminateReq;

    WiimoteConnection *connectionObject;
    ProfileManager *managerObject;

public:
    ConnectionManager(ProfileManager *manager = 0);
   ~ConnectionManager();

protected:
    void run();

private slots:
    void registerConnection(void *object);
    void unregisterConnection(void *object);

signals:
    void sendIRMesg(struct cwiid_ir_mesg ir);

};

#endif // CONNECTIONMANAGER_H
