#pragma once

/*!
 * \class JsonFile
 * \brief JSON file saver/loader
 */

#include <QString>
class QSerializer;

class JsonFile
{
public:
    JsonFile(const QString &name, QSerializer *jsonHolder = nullptr);

    bool load();
    void save();

private:
    QString _name;
    QSerializer * _jsonHolder;
};

QString AppDataDirPath(const QString &subDir);

QString DataDirPath();

