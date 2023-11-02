#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include <QApplication>
#include <QDir>

#include "qserializer.h"
#include "JsonFile.h"

JsonFile::JsonFile(const QString &name, QSerializer *jsonHolder)
    : _name      (name      )
    , _jsonHolder(jsonHolder)
{

}

bool JsonFile::load()
{
    if (_jsonHolder == nullptr || _name.isEmpty())
        return false;

    QFile file(_name);
    if (!file.exists())
        return false;

    if (!file.open(QIODevice::Text | QIODevice::ReadOnly))
        return false;

    QJsonDocument document = QJsonDocument::fromJson(file.readAll());

    file.close();

    if(!document.isObject())
        return false;

    _jsonHolder->fromJson(document.object());

    return true;
}

void JsonFile::save()
{
    if (_jsonHolder == nullptr || _name.isEmpty())
        return;

    QJsonObject json = _jsonHolder->toJson();

    QJsonDocument doc(json);

    QFile file(_name);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
        return;

    file.write(doc.toJson());

    file.close();
}

//----------------------------------------------------------------------------------------
static QString getSubDir(const QString &dirName)
{
    const QString AppDirPath = QApplication::applicationDirPath();
    const QString DirPath = dirName.isEmpty() ? QString("%1%2"    ).arg(AppDirPath).arg(QDir::separator())
                                              : QString("%1%2%3%2").arg(AppDirPath).arg(QDir::separator()).arg(dirName);
    QDir dir;
    if (!dir.exists(DirPath))
        dir.mkpath(DirPath);

    return DirPath;
}

//----------------------------------------------------------------------------------------
QString AppDataDirPath(const QString &subDir)
{
    return getSubDir(subDir);
}

//----------------------------------------------------------------------------------------
QString DataDirPath()
{
    return AppDataDirPath("data");
}
