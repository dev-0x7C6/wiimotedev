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

#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <QObject>
#include <QFile>
#include <QTime>
#include <QTextStream>

const QString errorLogPrefix("error: ");
const QString warningLogPrefix("warning: ");
const QString hintLogPrefix("hint: ");
const QString infoLogPrefix("info: ");

class DataLogger : public QObject
{
    Q_OBJECT

private:
    QString fileName;
    QFile *handle;
    QTextStream stream;

    bool logging;
    quint8 verbose;

public:
    DataLogger(QObject *parent = 0);
   ~DataLogger();

    bool isLogging() { return logging; }

    bool openLog(QString file);
    void closeLog();

    void setVerboseLevel(quint8 level) { verbose = level; };
    inline quint8 getVerboseLevel() { return verbose; }

    void addLine(QString line);

    void addErrorLine(QString line) { if (verbose > 0) addLine(errorLogPrefix + line); }
    void addWarningLine(QString line) { if (verbose > 1) addLine(warningLogPrefix + line); }
    void addInfoLine(QString line) { if (verbose > 2) addLine(infoLogPrefix + line); }
    void addHintLine(QString line) { if (verbose > 3) addLine(hintLogPrefix + line); }
    void addVerboseLine(QString line, quint8 level) { if (verbose > level) addLine(line); }
};

#endif // DATALOGGER_H
