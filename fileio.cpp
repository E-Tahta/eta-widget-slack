/*****************************************************************************
 *   Copyright (C) 2016 by Yunusemre Senturk                                 *
 *   <yunusemre.senturk@pardus.org.tr>                                       *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 2 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .          *
 *****************************************************************************/
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QString>
#include "fileio.h"

#include <QDebug>

FileIO::FileIO(QObject *parent) :
    QObject(parent)
{

    filepath = QDir::homePath() + "/.config/eta/slack";
    filename = "data.eta";
    fullpath = filepath + "/" + filename;
    QFileInfo checkFile(fullpath);

    d = new QDir(QDir::home());

    if(checkFile.exists() && checkFile.isFile()) {

        QFile file(fullpath);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Could not open data file";
        } else {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                l.append(line);
            }
        }

    } else {        
        d->mkpath(filepath);
        QFile file(fullpath);
    }
}

QStringList FileIO::readData()
{
    l.clear();
    QFileInfo checkFile(fullpath);
    if(checkFile.exists() && checkFile.isFile()) {
        d->mkpath(filepath);
        QFile file(fullpath);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Could not open data file while trying to read";
        } else {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                l.append(line);
            }

        }

    } else {
        qDebug() << "Data file does not exist or corrupted";
    }
    return l;
}

void FileIO::writeData(const QString &data)
{
    if(!QDir::isAbsolutePath(filepath)) {
      d->mkpath(filepath);
    }
    QFile file(fullpath);
    if (!file.open(QIODevice::Append)) {
        qDebug() << "Could not open data file while trying to write";
    } else {
        QTextStream out(&file);
        out << data << "\t\t" << getenv("USER") << "\n";
    }

}

#include "fileio.moc"
