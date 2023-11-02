#pragma once

#include "qserializer.h"
#include "SnippetsTabDesc.h"
#include "SettingsDesc.h"
#include "SingletonSharedPtrHolder.h"

class SnippetsHolderDesc : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
public:
    Q_DISABLE_COPY(SnippetsHolderDesc)
    using S = SingletonSharedPtrHolder<SnippetsHolderDesc>;

    SnippetsHolderDesc() {}
    QS_FIELD(QUuid, current_tab_id)
    QS_COLLECTION_OBJECTS(QList, SnippetsTabDesc, tabs)
    QS_OBJECT(SettingsDesc, settings)

public:
    void setCurrentTab(const QUuid &id) {current_tab_id = id;}
    QUuid currentTabId() const          {return current_tab_id;}

    void setGeometry(const QRect &r) {settings.winSettings.coord = r;}
    QRect geometry() const           {return settings.winSettings.coord.rect();}
};
