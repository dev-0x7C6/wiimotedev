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

#include "datalogger.h"

DataLogger::DataLogger(QObject *parent) : QObject(parent)
{
    logging = false;
    verbose = 1;
}

DataLogger::~DataLogger()
{
     closeLog();
}

bool DataLogger::openLog(QString file)
{
    if (logging) closeLog();

    handle = new QFile(file);
    bool result;

    if (result = handle->open(QFile::ReadWrite | QFile::Text | QFile::Append))
    {
        stream.setDevice(handle);
        logging = true;
    }

    return result;
}

void DataLogger::closeLog()
{
    if (!logging) return;
    logging = false;

    stream.flush();
    handle->close();

    if (!handle) delete handle;
}

void DataLogger::addLine(QString line)
{
    if (logging) stream << QString("/[%1:%2]// " + line + "\n").arg(QString::number(QTime::currentTime().hour()),
                                                                    QString::number(QTime::currentTime().minute()));
    stream.flush();
}

