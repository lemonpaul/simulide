/***************************************************************************
 *   Copyright (C) 2012 by santiago González                               *
 *   santigoro@gmail.com                                                   *
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
 *   along with this program; if not, see <http://www.gnu.org/licenses/>.  *
 *                                                                         *
 ***************************************************************************/

#ifndef AVRPROCESSOR_H
#define AVRPROCESSOR_H

#include <QtGui>
#include <QHash>

#include "baseprocessor.h"

// simavr includes
#include "sim_avr.h"
struct avr_t;

class AvrProcessor : public BaseProcessor
{
    Q_OBJECT
    public:
        AvrProcessor( QObject* parent=0 );
        ~AvrProcessor();

 static AvrProcessor* self() { return m_pSelf; }

        bool loadFirmware( QString file );
        void terminate();

        void step();

        void reset();

        avr_t* getCpu() { return m_avrProcessor; }

        int  getRamValue( QString name );

    public slots:
        void runAvr();

    private:
        QHash<QString, int> getRegsTable( QString lstFileName );

        //QThread          m_avrRunThread;

 static AvrProcessor *m_pSelf;

        //From simavr
        avr_t* m_avrProcessor;
};


#endif
