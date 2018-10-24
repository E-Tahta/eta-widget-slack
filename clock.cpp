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
#include "clock.h"
#include <QDateTime>
#include <QTimer>

#define TIMER_INTERVAL 1000
#define MINIMUM_LECTURE_TIME_SECS 30

Clock::Clock(QObject *parent) :
    QObject(parent),
    count(0),
    isOk(false)
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeOutSlot()));
}

QString Clock::getCurrentTime() const
{
    return QDateTime::currentDateTime().toString("hh:mm:ss");
}

QString Clock::getCurrentDate() const
{
    return QDateTime::currentDateTime().toString("dd/MM/yyyy");
}

void Clock::startCounting()
{
    count = 0;
    isOk = false;
    timer->start(TIMER_INTERVAL);
}

bool Clock::isOkeyToSave() const
{
    return isOk;
}

void Clock::timeOutSlot()
{
    count ++;
    if ( count == MINIMUM_LECTURE_TIME_SECS ) {
        isOk = true;
        count = 0;
    } else {
        timer->start(TIMER_INTERVAL);
    }
}


#include "clock.moc"
