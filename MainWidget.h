#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class QLineEdit;
class TabEditorWatcher;
class QTabWidget;
class SnippetDesc;
class MouseWatcher;

class MainWidget : public QWidget
{
    Q_OBJECT

public:

    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

protected:
    void showEvent(QShowEvent *) override;

    void hideEvent(QHideEvent *) override;

private slots:
    void on_pbAddTabReleased();

    void on_pbRemoveTabReleased();

    void on_tabBarDoubleClicked(int index);

    void on_tbDelItemReleased();

    void on_tbCopyItemReleased();

    void on_currentChanged(int index);

private:
    void connectDelAndCopy(QWidget *wSnippet);

    void connectEditors(QWidget *wSnippet);

    void connectTopButtons(QWidget *wTab, const QUuid &tabDescID);

    void createSnippetWidgetOnTab(QWidget *wTab, const SnippetDesc &snippetDesc);

    QWidget * findSnippetWidget(QWidget *wTab, const QUuid &id);

    Ui::MainWidget *ui;

    QLineEdit        *m_tabEditor;
    TabEditorWatcher *m_tabEditorWatcher;
    QTabWidget       *m_tabWidget;
    QLineEdit        *m_commentEditor;
    MouseWatcher     *m_mouseWatcher;
};

