#include <QMouseEvent>
#include <QLineEdit>
#include <QTabBar>
#include <QToolTip>

#include "SnippetsManager.h"
#include "MouseWatcher.h"

using SM = SnippetsManager;

MouseWatcher::MouseWatcher(QObject *oSW)
    : QObject(oSW)
    , m_currentEditorID()
{
}

bool MouseWatcher::eventFilter(QObject *o, QEvent *e)
{
    if (!o->isWidgetType())
        return false;

    QLineEdit *leComment = qobject_cast<QLineEdit *>(parent());
    if (leComment->isEnabled())
        return false;

    QMouseEvent *me = static_cast<QMouseEvent *>(e);
    QWidget *editor = qobject_cast<QWidget *>(o);
    QWidget *wSnippet = editor == nullptr ? nullptr : editor->parentWidget();

    if (wSnippet == nullptr || wSnippet->objectName() != QString(SM::_W_SNIPPET_))
        return false;

    if (wSnippet->property(SM::_ID_).toUuid() == m_currentEditorID)
        return false;

    if (me->type() == QEvent::MouseMove) {
        leComment->setProperty(SM::_ID_, wSnippet->property(SM::_ID_).toUuid());
        const QString comment = wSnippet->property(SM::_COMMENT_).toString();
        leComment->setText(comment);
        QToolTip::showText(me->globalPosition().toPoint(), comment);
        m_currentEditorID = wSnippet->property(SM::_ID_).toUuid();
    } else {
        leComment->setProperty(SM::_ID_, QUuid());
        leComment->setText(QString());
    }

    return false;
}
