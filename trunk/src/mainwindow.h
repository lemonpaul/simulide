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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QDomDocument>

#include "QPropertyEditorWidget.h"
#include "componentselector.h"
#include "circuitwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();
        ~MainWindow();

        static MainWindow* self() { return m_pSelf; }

        QTabWidget*  sidepanel;
        QSplitter*   splitter5;
        QSplitter*   splitter3;
        QGridLayout* ramTabWidgetLayout;

    public slots:
        void powerCircOn();
        void powerCircOff();
        void reset();
        void pause();

        void openCirc();
        void newCircuit();
        bool saveCirc();
        bool saveCircAs();

    protected:
        void closeEvent(QCloseEvent* event);

    private slots:
        void about();
        //void circCloseTab(int);

        void powerCirc();
        void simuRateChanged( int rate );

    private:

 static MainWindow* m_pSelf;

        bool m_blocked;

        void createWidgets();
        void createActions();
        void createMenus();
        void createToolBars();
        void readSettings();
        void writeSettings();

        //void loadFile( QString &fileName );
        //void load( QString &fileName );
        //void loadCircuit( QString &fileName );
        //bool saveCircuit( QString &fileName );
        //void listToDom( QList<Component*>* complist, QDomDocument* doc);
        //void setCurrentFile(const QString &fileName);

        void applyStile();

        QString m_styleSheet;

        QString m_curCirc;
        QString m_lastCircDir;
        QStringList m_docList;

        CircuitWidget* m_circuit;
        QSpinBox*      simuRate;

        //QTabWidget * centralWidget;
        //QTabWidget * circWidget;
        QPropertyEditorWidget* itemprop;
        ComponentSelector*     components;

        QLineEdit m_findLabel;

        //QMenuBar* menubar;
        //QMenu* fileMenu;
        //QMenu* editMenu;
        //QMenu* toolsMenu;
        //QMenu* debugMenu;
        //QMenu* circMenu;
        //QMenu* helpMenu;

        //QToolBar* fileToolBar;
        //QToolBar* editToolBar;
        //QToolBar* toolsToolBar;
        //QToolBar* findToolBar;
        QToolBar* circToolBar;
        //QToolBar* generalToolBar;

        QAction* exitAct;
        QAction* aboutAct;
        QAction* aboutQtAct;

        QAction* stepAct;
        QAction* runAct;
        QAction* pauseAct;
        QAction* resetAct;
        QAction* stopAct;

        QAction* newCircAct;
        QAction* openCircAct;
        QAction* saveCircAct;
        QAction* saveCircAsAct;
        QAction* powerCircAct;
};

#endif
