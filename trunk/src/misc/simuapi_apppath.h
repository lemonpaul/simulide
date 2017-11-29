#ifndef SIMUAPI_APPPATH_H
#define SIMUAPI_APPPATH_H

#include <qdir.h>

class MAINMODULE_EXPORT SIMUAPI_AppPath
{
    public:
        static SIMUAPI_AppPath* self();


        QDir RODataFolder() const;
        void setRODataFolder(const QDir &RODataFolder);

        QDir RWDataFolder() const;
        void setRWDataFolder(const QDir &RWDataFolder);

        QString availableDataFilePath(QString fileRelPath);
        QString availableDataDirPath(QString dirRelPath);

private:
        SIMUAPI_AppPath();
        static SIMUAPI_AppPath* m_pSelf;

        QDir m_RODataFolder;
        QDir m_RWDataFolder;
};

#endif // SIMUAPI_APPPATH_H
