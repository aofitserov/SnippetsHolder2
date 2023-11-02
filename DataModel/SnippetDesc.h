#pragma once

#include "qserializer.h"

class SnippetDesc : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    public:
    enum Type
    {
        LineSnippet,
        TextSnippet,
    };
    Q_ENUM(Type)

    SnippetDesc()
        : id {QUuid::createUuid()}
    {

    }
    SnippetDesc(const QString &text, const QString &comment, Type type)
        : mText(text)
        , mComment(comment)
        , mType(type)
        , id {QUuid::createUuid()}
    {

    }

    SnippetDesc(Type type)
        : mText(QString())
        , mComment(QString())
        , mType(type)
        , id {QUuid::createUuid()}
    {

    }

    QS_FIELD(QString, mText   )
    QS_FIELD(QString, mComment)
    QS_FIELD(Type   , mType   )
    QS_FIELD(QUuid  , id      )

public:
    void    setText   (const QString &text) {mText = text;}
    QString text      (void)          const {return mText;}

    void setComment   (const QString &text) {mComment = text;}
    QString comment   ()              const {return mComment;}

    void setType(Type t) {mType = t;}
    Type type() const {return mType;}

    void setID(const QUuid &uuid) {id = uuid;}
    QUuid ID() const {return id;}

};

inline bool operator == (const SnippetDesc& tD1, const SnippetDesc& tD2) {return tD1.id == tD2.id;}
