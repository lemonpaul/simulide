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

 #ifndef NO_PIC
 
#ifndef PICCOMPONENTPIN_H
#define PICCOMPONENTPIN_H

#include <qstring.h>
#include <stdint.h>

#include "mcucomponentpin.h"

#include "gpsim/stimuli.h"
#include "gpsim/ioports.h"
#include "gpsim/pic-processor.h"
#include "gpsim/stimuli.h"

class PICComponentPin : public McuComponentPin, public stimulus
{
    Q_OBJECT
        public:
        PICComponentPin( McuComponent *mcu, QString id, QString type, QString label, int pos, int xpos, int ypos, int angle );
        ~PICComponentPin();

        void attach( pic_processor *PicProcessor );
        
        void setVChanged();
        
        void resetOutput();
        
        virtual void set_nodeVoltage( double v );
        virtual double get_Vth();

    protected:
        char m_port;
        int  m_pinN;
        //int  m_channel;
        int  m_pos;

        //const QString m_esp;
        
        pic_processor *m_PicProcessor;
        Stimulus_Node *m_pStimulusNode;
        IOPIN         *m_pIOPIN;
};

#endif
#endif
