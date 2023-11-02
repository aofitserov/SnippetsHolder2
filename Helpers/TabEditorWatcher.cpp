#include <QMouseEvent>
#include <QLineEdit>
#include <QTabBar>

#include "SnippetsManager.h"
#include "TabEditorWatcher.h"

// LineEdit for Tab processing
//-------------------------------------------------------------
TabEditorWatcher::TabEditorWatcher(QObject *oLe)
    : QObject(oLe)
{
}

bool TabEditorWatcher::eventFilter(QObject *o, QEvent *e)
{
    if (!o->isWidgetType())
        return false;

    QTabBar *tabBar = qobject_cast<QTabBar *>(o);
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(parent());
    QVariant vIndex = lineEdit->property(SnippetsManager::_INDEX_);

    if (tabBar != nullptr && e->type() == QEvent::Resize)
    {
        lineEdit->resize(tabBar->tabRect(vIndex.toInt()).size());
        QResizeEvent* re = static_cast<QResizeEvent*>(e);
        if (re->size().width() < re->oldSize().width())
                return false;

        lineEdit->show();
    }

    if (tabBar != nullptr && e->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* me = static_cast<QMouseEvent*>(e);
        int clickedTabIndex = tabBar->tabAt(me->pos());
        if (vIndex.toInt() != clickedTabIndex)
                   lineEdit->hide();
    }

    if (tabBar != nullptr && e->type() == QEvent::FocusOut)
    {
        lineEdit->hide();
    }
    return false;
}
