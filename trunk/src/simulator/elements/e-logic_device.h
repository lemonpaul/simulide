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


class eLogicDevice : public eElement
{
   /* Q_OBJECT
    Q_PROPERTY( double Input_High_V READ inputHighV WRITE setInputHighV DESIGNABLE true USER true )
    Q_PROPERTY( double Input_Imped  READ inputImp   WRITE setInputImp   DESIGNABLE true USER true )
    Q_PROPERTY( double Out_High_V   READ outHighV   WRITE setOutHighV   DESIGNABLE true USER true )
    Q_PROPERTY( double Out_Low_V    READ outLowV    WRITE setOutLowV    DESIGNABLE true USER true )
    Q_PROPERTY( double Out_Imped    READ outImp     WRITE setOutImp     DESIGNABLE true USER true )*/
    //Q_PROPERTY( double Num_Inputs   READ num_Inps   WRITE setNum_Inps   DESIGNABLE true USER true )

    public:

        eLogicDevice( string id );
        ~eLogicDevice();

        int  numImps() const            { return m_numInputs; }
        void setNumImps( int inputs );
                                            
        int  numOuts() const            { return m_numOutputs; }
        void setNumOuts( int outputs );

        double inputHighV() const          { return m_inputHighV; }
        void  setInputHighV( double volt ) { m_inputHighV = volt; }

        double inputLowV() const          { return m_inputHighV; }
        void  setInputLowV( double volt ) { m_inputLowV = volt; }

        double outHighV() const            { return m_outHighV; }
        void  setOutHighV( double volt );

        double outLowV() const             { return m_outLowV; }
        void  setOutLowV( double volt );

        double inputImp() const            { return m_inputImp; }
        void  setInputImp( double imp );//    { m_inputImp = imp; }

        double outImp() const            { return m_outImp; }
        void  setOutImp( double imp );

        void initEpins(){;}
        ePin* getEpin( int pin );


    protected:
        virtual void createPins( int inputs, int outputs );
        void createInputs( int inputs );
        void createOutputs( int outputs );
        void deleteInputs( int inputs );
        void deleteOutputs( int inputs );

        double m_inputHighV;
        double m_inputLowV;
        double m_outHighV;
        double m_outLowV;

        double m_inputImp;
        double m_outImp;

        int m_numInputs;
        int m_numOutputs;

        std::vector<eSource*> m_output;
        std::vector<eSource*> m_input;
        std::vector<bool>     m_inputState;
        //std::vector<bool>     m_outValue;
};

#endif


