/***************************************************************************
 *   Copyright (C) 2003-2006 by David Saxton                               *
 *   david@bluehaze.org                                                    *
 *                                                                         *
 *   Copyright (C) 2010 by santiago González                               *
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
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef ELOGICDEVICE_H
#define ELOGICDEVICE_H

#include "e-source.h"
#include "e-pin.h"

#define Low     0
#define Falling 1
#define Rising  2
#define High    3

class eLogicDevice : public eElement
{
    public:

        eLogicDevice( string id );
        ~eLogicDevice();

        int  numInps() const            { return m_numInputs; }
        virtual void setNumInps( int inputs );
                                            
        int  numOuts() const            { return m_numOutputs; }
        void setNumOuts( int outputs );

        double inputHighV() const          { return m_inputHighV; }
        void  setInputHighV( double volt ) { m_inputHighV = volt; }

        double inputLowV() const          { return m_inputLowV; }
        void  setInputLowV( double volt ) { m_inputLowV = volt; }

        double outHighV() const            { return m_outHighV; }
        void  setOutHighV( double volt );

        double outLowV() const             { return m_outLowV; }
        void  setOutLowV( double volt );

        double inputImp() const            { return m_inputImp; }
        void  setInputImp( double imp );

        double outImp() const            { return m_outImp; }
        void  setOutImp( double imp );

        void initEpins(){;}
        //ePin* getEpin( int pin );
        ePin* getEpin( QString pinName );

        int getClockState();
        bool outputEnabled();

        virtual void initialize();

        void createClockPin();
        void createOutEnablePin();

    protected:
        virtual void createPins( int inputs, int outputs );
        void createInputs( int inputs );
        void createOutputs( int outputs );
        void deleteInputs( int inputs );
        void deleteOutputs( int inputs );
        void setOut( int num, bool out );

        double m_inputHighV;
        double m_inputLowV;
        double m_outHighV;
        double m_outLowV;

        double m_inputImp;
        double m_outImp;

        int m_numInputs;
        int m_numOutputs;

        bool m_clock;
        bool m_outEnable;

        eSource* m_clockPin;
        eSource* m_outEnablePin;

        std::vector<eSource*> m_output;
        std::vector<eSource*> m_input;
        std::vector<bool>     m_inputState;
};

#endif


