/***************************************************************************
 *   Copyright (C) 2012 by santiago González                               *
 *   santigoro@gmail.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
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

#ifndef MCUCOMPONENT_H
#define MCUCOMPONENT_H

#include <QtWidgets>

#include "chip.h"

class BaseProcessor;
class McuComponentPin;

class MAINMODULE_EXPORT McuComponent : public Chip
{
    Q_OBJECT
    Q_PROPERTY( QString  Program     READ program WRITE setProgram DESIGNABLE true  USER true )
    Q_PROPERTY( int      Mhz         READ freq    WRITE setFreq    DESIGNABLE true  USER true )
    Q_PROPERTY( bool     Ser_Port    READ serPort WRITE setSerPort )
    Q_PROPERTY( bool     Ser_Monitor READ serMon  WRITE setSerMon )

    public:

        McuComponent( QObject* parent, QString type, QString id );
        ~McuComponent();
        
 static McuComponent* self() { return m_pSelf; }

        QString program()   const      { return  m_symbolFile; }
        void setProgram( QString pro );
        
        QString device() { return m_device; }

        int  freq();
        virtual void setFreq( int freq );
        virtual void initChip();
        
        bool serPort();
        void setSerPort( bool set );
        
        bool serMon();
        void setSerMon( bool set );
        
        QList<McuComponentPin*> getPinList() { return m_pinList; }

        virtual void paint( QPainter* p, const QStyleOptionGraphicsItem* option, QWidget* widget );
  
    public slots:
        virtual void terminate();
        virtual void remove();
        virtual void reset();
        virtual void load( QString fileName );
        void slotLoad();
        void slotReload();
        void slotOpenTerm();
        void slotCloseTerm();
        void slotOpenSerial();
        void slotCloseSerial();
        
        void contextMenu( QGraphicsSceneContextMenuEvent* event, QMenu* menu );
        
    protected:
 static McuComponent* m_pSelf;
 static bool m_canCreate;
        
        virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

        virtual void addPin( QString id, QString type, QString label, int pos, int xpos, int ypos, int angle )=0;
        virtual void attachPins()=0;

        BaseProcessor* m_processor;

        int m_freq;             // Clock Frequency Mhz
        
        bool m_attached;
        bool m_serPort;
        bool m_serMon;

        QString m_device;       // Name of device
        QString m_symbolFile;   // firmware file loaded
        QString m_lastFirmDir;  // Last firmware folder used
        
        QList<McuComponentPin*> m_pinList;
};
#endif
