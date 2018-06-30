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

#ifndef BASEDEBUGGER_H
#define BASEDEBUGGER_H

#include <QtGui>
#include "outpaneltext.h"
#include "mcucomponent.h"

class BaseDebugger : public QObject    // Base Class for all debuggers
{
    Q_OBJECT
    Q_PROPERTY( QString  Compiler_Path   READ compilerPath    WRITE setCompilerPath   DESIGNABLE true USER true )
    public:
        BaseDebugger( QObject* parent=0, OutPanelText* outPane=0, QString filePath=0 );
        ~BaseDebugger();
        //QString getFileName() { return m_symbolFile; }  // Returns symbol file
        
        QString compilerPath();
        virtual void setCompilerPath( QString path );

        virtual bool loadFirmware();
        virtual void upload();
        virtual int  step();        // Run 1 step,returns actual source line number
        virtual int  stepOver();    // Run until next source file
        virtual void stop();

        virtual void getProcName();
        virtual void setProcType( int type ){ m_processorType = type; }
        
        virtual int compile()=0;
        
        virtual int getValidLine( int line );       // Returns next asm line with code
        virtual int getProgramStart(){return 1;}

        virtual void getCompilerPath();
        
        virtual void readSettings();
        
    public slots:
        void ProcRead();

    protected:
    
        OutPanelText*  m_outPane;

        //QHash<QString, int> regsTable;              // Map register names to Ram adress

 static bool m_loadStatus;                          // Is symbol file loaded?
        bool m_running;                             // Is processor running?

        int m_processorType;
        
        QString m_device;
        QString m_appPath;
        QString m_firmware;
        QString m_fileDir;
        QString m_fileName;
        QString m_fileExt;
        QString m_compilerPath;
        QString m_compSetting;
        
        QHash<QString, QString> m_typesList;
        QHash<QString, QString> m_varList;
        
        QProcess m_compProcess;
};

#endif

