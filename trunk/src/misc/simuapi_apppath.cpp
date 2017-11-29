#include <qapplication.h>
#include <QStandardPaths>
#include "simuapi_apppath.h"

SIMUAPI_AppPath* SIMUAPI_AppPath::m_pSelf = 0l;

SIMUAPI_AppPath *SIMUAPI_AppPath::self()
{
    if (!m_pSelf) {
        m_pSelf = new SIMUAPI_AppPath();
    }
    return m_pSelf;
}

SIMUAPI_AppPath::SIMUAPI_AppPath()
    : m_RODataFolder(qApp->applicationDirPath()),
      m_RWDataFolder(QStandardPaths::writableLocation(QStandardPaths::DataLocation))
{
    m_RODataFolder.cd( "../share/simulide/data" );
    m_RWDataFolder.cd( "data" );
}

QDir SIMUAPI_AppPath::RWDataFolder() const
{
    return m_RWDataFolder;
}

void SIMUAPI_AppPath::setRWDataFolder(const QDir &RWDataFolder)
{
    m_RWDataFolder = RWDataFolder;
}

QDir SIMUAPI_AppPath::RODataFolder() const
{
    return m_RODataFolder;
}

void SIMUAPI_AppPath::setRODataFolder(const QDir &RODataFolder)
{
    m_RODataFolder = RODataFolder;
}

QString SIMUAPI_AppPath::availableDataFilePath(QString fileRelPath)
{
    QString filePath = m_RWDataFolder.absoluteFilePath(fileRelPath);
    if (QFile::exists(filePath))
        return filePath;
    filePath = m_RODataFolder.absoluteFilePath(fileRelPath);
    if (QFile::exists(filePath))
        return filePath;
    return "";
}

QString SIMUAPI_AppPath::availableDataDirPath(QString dirRelPath)
{
    if (m_RWDataFolder.exists(dirRelPath))
        return m_RWDataFolder.absoluteFilePath(dirRelPath);
    if (m_RODataFolder.exists(dirRelPath))
        return m_RODataFolder.absoluteFilePath(dirRelPath);
    return "";
}



