#pragma once

#include <QObject>
#include <QUuid>

class MouseWatcher final : public QObject
{
public:
    MouseWatcher(QObject *oSW);
    bool eventFilter(QObject *o, QEvent *e);

private:
    QUuid m_currentEditorID;
};

