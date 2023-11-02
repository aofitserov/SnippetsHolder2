#pragma once

#include <QObject>

class TabEditorWatcher final : public QObject
{
public:
    TabEditorWatcher(QObject *oLe);
    bool eventFilter(QObject *o, QEvent *e);

};
