/***************************************************************************
 *   Copyright (C) 2016 by santiago González                               *
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

#include <math.h>   // fabs(x,y)

#include "e-mosfet.h"
#include "simulator.h"
#include <QDebug>


eMosfet::eMosfet( std::string id )
    : eResistor( id )
{
    m_Pchannel = false;
    
    m_resist    = high_imp;
    m_RDSon     = 1;
    m_threshold = 3;

    m_ePin.resize(3);
}
eMosfet::~eMosfet(){}

void eMosfet::initialize()
{
    m_converged = true;
    m_resist    = high_imp;
    m_lastAdmit = cero_doub;
    m_DScurrent = 0;
    m_convTh    = 1e-5;
    m_lastGateV = 0;
    
    m_kRDSon = m_RDSon*(10-m_threshold);
    m_Gth = m_threshold-m_threshold/4;
    
    if( (m_ePin[0]->isConnected()) 
      & (m_ePin[1]->isConnected())
      & (m_ePin[2]->isConnected()) ) 
    {
        m_ePin[0]->getEnode()->addToNoLinList(this);
        m_ePin[1]->getEnode()->addToNoLinList(this);
        m_ePin[2]->getEnode()->addToNoLinList(this);
    }
    eResistor::initialize();
}

void eMosfet::setVChanged()
{
    double Vgs;
    double Vds;
    double Vd = m_ePin[0]->getVolt();
    double Vs = m_ePin[1]->getVolt();
    double Vg = m_ePin[2]->getVolt();

    if( m_Pchannel )
    {
        Vgs = Vs-Vg;
        Vds = Vs-Vd;
    }
    else
    {
        Vgs = Vg-Vs;
        Vds = Vd-Vs;
    }

    double GateV  = Vgs - m_Gth;
    //double dGateV = fabs(m_lastGateV-GateV);
    double Admit  = GateV/m_kRDSon;

    /*qDebug() <<" ";
    qDebug() << QString::fromStdString(m_elmId)<<"STAGE "<<m_converged;
    qDebug() <<"m_lastGateV"<<m_lastGateV<<"  GateV"<<GateV ;
    qDebug() <<"m_lastAdmit"<<m_lastAdmit<<"  Admit"<<Admit;*/

    if( fabs(Admit-m_lastAdmit)<m_convTh )
    {
        qDebug() <<" ";
        qDebug() << QString::fromStdString(m_elmId)<<"STAGE "<<m_converged;
        qDebug() <<"CONVERGED:           ";
        if( m_converged ) return;
        Admit = m_lastAdmit;
        m_converged = true;
    }
    else if( m_converged )
    {
        m_cAdmit = Admit;
        //m_dAdmit = -1e-7;
        m_dAdmit = (Admit-m_lastAdmit)/10;
        //if( Admit>m_lastAdmit )m_dAdmit = 1e-7;

        Admit = m_lastAdmit + m_dAdmit;
        //qDebug() <<"Admit 0"<<Admit;

        m_converged = false;
    }
    else if( m_lastGateV != GateV )
    {
        if( fabs(Vs - m_lastVs)>1e-6 )
        {
            qDebug() <<" ";
            qDebug() << QString::fromStdString(m_elmId)<<"STAGE "<<m_converged;
            qDebug() <<"Vs m_lastVs"<<Vs <<m_lastVs;
            double nextVs = Vg-m_Gth;
            double kVs = nextVs/Vs;
            double kdVs = 1;
            if( fabs(Vs-m_lastVs)>1 ) kdVs = 1/(fabs(Vs-m_lastVs));
            Admit = m_lastAdmit*kVs*kdVs;
            qDebug() <<"m_lastAdmit"<<m_lastAdmit<<"Admit"<<Admit;
            m_converged = false;
        }
        else
        {
            double dCAdmit = m_cAdmit - Admit;
            Admit= m_lastAdmit+(Admit-m_lastAdmit)/((1+dCAdmit/m_dAdmit));
            //qDebug() <<"Admit 1"<<Admit;
            m_converged = true;
        }
    }
    
    if( Admit > high_imp  ) Admit = high_imp;
    if( Admit < cero_doub ) Admit = cero_doub;
    //qDebug() <<"Final Admit "<<Admit;
    if( Admit != m_lastAdmit )
    {
        m_resist = 1/Admit;
        eResistor::stamp();
        //qDebug() <<".....................stamp";
    }
    else m_converged = true;
    
    double DScurrent = 0;
    if( (GateV>0)&(Vds > GateV) )
    {
        double Vdg = Vds-GateV;
        DScurrent = Vdg/m_resist;
        DScurrent -= DScurrent / (1 + Vdg );
        if( DScurrent < 0 )  DScurrent = 0;
    }
    if( DScurrent != m_DScurrent)
    {
        m_DScurrent = DScurrent;
        m_ePin[0]->stampCurrent( DScurrent );
        m_ePin[1]->stampCurrent(-DScurrent );
    }
    //qDebug() << "m_resist"<<m_resist<< "     DScurrent"<<DScurrent;
    m_lastVs    = Vs;
    m_lastGateV = GateV;
    m_lastAdmit = Admit;
}

void eMosfet::setRDSon( double rdson )
{
    if( rdson < cero_doub ) rdson = cero_doub;
    if( rdson > 10 ) rdson = 10;
    m_RDSon = rdson;
}
