/***************************************************************************
 *   Copyright (C) 2018 by santiago González                               *
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

#include "audio_out.h"
#include "simulator.h"
#include "connector.h"
#include "itemlibrary.h"
#include "pin.h"

Component* AudioOut::construct( QObject* parent, QString type, QString id )
{ return new AudioOut( parent, type, id ); }

LibraryItem* AudioOut::libraryItem()
{
    return new LibraryItem(
            tr( "Audio Out" ),
            tr( "Outputs" ),
            "audio_out.png",
            "AudioOut",
            AudioOut::construct);
}

AudioOut::AudioOut( QObject* parent, QString type, QString id )
        : Component( parent, type, id ),
          eResistor( id.toStdString() )
{
    m_area = QRect( -10, -12, 20, 16 );
    
    m_pin.resize( 2 );
    
    QString pinId = m_id;
    pinId.append(QString("-lPin"));
    QPoint pinPos = QPoint(-8-8,-8);
    m_pin[0] = new Pin( 180, pinPos, pinId, 0, this);
    m_pin[0]->setLabelText( "+" );
    m_pin[0]->setLabelColor( QColor( 0, 0, 0 ) );
    m_ePin[0] = m_pin[0];

    pinId = m_id;
    pinId.append(QString("-rPin"));
    pinPos = QPoint(-8-8,0);
    m_pin[1] = new Pin( 180, pinPos, pinId, 1, this);
    m_pin[1]->setLabelText( " -" );
    m_pin[1]->setLabelColor( QColor( 0, 0, 0 ) );
    m_ePin[1] = m_pin[1];

    //m_idLabel->setText( QString("") );
    m_idLabel->setPos(-12,-24);
    setLabelPos(-12,-24, 0);
    
    m_resist = 8;
    
    m_deviceinfo = QAudioDeviceInfo::defaultOutputDevice(); 

    m_format.setSampleRate( 40000 );  
    m_format.setChannelCount(1);
    m_format.setSampleSize(8);
    m_format.setCodec( "audio/pcm" );  
    m_format.setByteOrder( QAudioFormat::LittleEndian );  
    m_format.setSampleType( QAudioFormat::UnSignedInt );  
    
    if( !m_deviceinfo.isFormatSupported( m_format )) 
    {  
        qDebug() << "Default format not supported - trying to use nearest";  
        m_format = m_deviceinfo.nearestFormat( m_format );  
        
        qDebug() << m_format.sampleRate() << m_format.channelCount()<<m_format.sampleSize();
    }  
    m_audioOutput = new QAudioOutput( m_deviceinfo, m_format );   
    m_audioOutput->setBufferSize( 10000 );
}

AudioOut::~AudioOut(){
    //qDebug() << "AudioOut::~AudioOut deleting" << QString::fromStdString( m_elmId );
}

void AudioOut::initialize()
{
    if( m_ePin[0]->isConnected() && m_ePin[1]->isConnected() )
        Simulator::self()->addToSimuClockList( this );
    
    eResistor::initialize();
    
    m_counter = 0;
    
    m_auIObuffer = m_audioOutput->start();
}

void AudioOut::simuClockStep()
{
    m_counter++;
    if( m_counter == 25 ) 
    {
        m_counter = 0;
        
        double voltPN = m_ePin[0]->getVolt()-m_ePin[1]->getVolt();
        if( voltPN > 5 ) voltPN = 5;
 
        uint8_t outVal = voltPN*51;
        
        m_auIObuffer->write( (const char*)&outVal, 1 );
    }
}

void AudioOut::remove()
{
    Simulator::self()->remFromSimuClockList( this );
    
    if( m_ePin[0]->isConnected() ) (static_cast<Pin*>(m_ePin[0]))->connector()->remove();
    if( m_ePin[1]->isConnected() ) (static_cast<Pin*>(m_ePin[1]))->connector()->remove();
    Component::remove();
}

void AudioOut::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );

    //p->drawRect( -10.5, -12, 12, 24 );
    static const QPointF points[7] = {
        QPointF(-10,-12 ),
        QPointF(-10, 4 ),
        QPointF( 0, 4 ),
        QPointF( 10, 16 ),
        QPointF( 10, -24 ),
        QPointF( 0, -12 ),
        QPointF(-10, -12 )
        };

    p->drawPolygon(points, 7);
}

#include "moc_audio_out.cpp"
