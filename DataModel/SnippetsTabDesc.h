#pragma once

#include "qserializer.h"
#include "SnippetDesc.h"

class SnippetsTabDesc : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    public:
        SnippetsTabDesc()
      : id      {QUuid::createUuid()}
      , index   {-1                 }
      , disabled{false              }
    {
    }
    QS_FIELD(QUuid  , id      )
    QS_FIELD(int    , index   )
    QS_FIELD(bool   , disabled)
    QS_FIELD(QString, tabName )
    QS_FIELD(QString, mComment)

    QS_COLLECTION_OBJECTS(QList, SnippetDesc, snippets)

};

inline bool operator == (const SnippetsTabDesc& tD1, const SnippetsTabDesc& tD2) {return tD1.id == tD2.id;}

