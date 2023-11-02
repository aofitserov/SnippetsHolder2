#include "MainWidget.h"
#include "ui_MainWidget.h"

#include <QDebug>
#include <QMenu>
#include <QWidgetAction>
#include <QEvent>
#include <QClipboard>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QStackedWidget>
#include <QTabBar>
#include <QLineEdit>

#include "SnippetsHolderDesc.h"
#include "SnippetsManager.h"
#include "TabEditorWatcher.h"
#include "MouseWatcher.h"

using SM = SnippetsManager;

// MainWidget implementation part
//-------------------------------------------------------------
MainWidget::MainWidget(QWidget *parent)
    : QWidget           (parent)
    , ui                (new Ui::MainWidget)
    , m_tabEditor       (nullptr)
    , m_tabEditorWatcher(nullptr)
    , m_tabWidget       (nullptr)
    , m_commentEditor   (nullptr)
    , m_mouseWatcher    (nullptr)
{
    ui->setupUi(this);

    m_tabWidget = ui->twMain;
    m_tabWidget->setMovable(true);

    QTabBar *tabBar = m_tabWidget->tabBar();
    tabBar->setTabsClosable(true);

    m_tabEditor = new QLineEdit(tabBar);
    m_tabEditor->hide();

    m_commentEditor = ui->leComment;
    m_mouseWatcher = new MouseWatcher(m_commentEditor);

    foreach (const SnippetsTabDesc &tabDesc, SnippetsManager::sStorage().tabs)
    {
        QWidget *wTab = new QWidget();
        wTab->setProperty  (SM::_ID_, tabDesc.id);
        wTab->setObjectName(tabDesc.id.toString());

        m_tabWidget->addTab(wTab, tabDesc.tabName);
        m_tabWidget->setCurrentIndex(tabDesc.index);
        m_tabWidget->tabBar()->setTabToolTip(tabDesc.index, tabDesc.tabName);

        SM::sAddControls(wTab);

        connectTopButtons(wTab, tabDesc.id);

        for (const SnippetDesc &snippetDesc : tabDesc.snippets)
            createSnippetWidgetOnTab(wTab, snippetDesc);

        SM::sGetCheckBox(wTab, SM::_CB_DISABLE_)->setChecked(tabDesc.disabled);
    }

    auto curTabDesc = SM::sGetTab(SM::sStorage().currentTabId());
    m_tabWidget->setCurrentIndex(curTabDesc ? curTabDesc->index : 0);

    connect(m_tabEditor,            &QLineEdit::returnPressed,     [this](){m_tabEditor->hide(); SM::sSaveStorage();});
    connect(ui->tbAddTab,           &QToolButton::released,        this,      &MainWidget::on_pbAddTabReleased   );
    connect(tabBar,                 &QTabBar::tabBarDoubleClicked, this,      &MainWidget::on_tabBarDoubleClicked);
    connect(tabBar,                 &QTabBar::tabCloseRequested,   this,      &MainWidget::on_pbRemoveTabReleased);
    connect(m_tabWidget,            &QTabWidget::currentChanged,   this,      &MainWidget::on_currentChanged     );

    connect(m_tabEditor,            &QLineEdit::textChanged,  [this](const QString& text){
        int index = m_tabEditor->property(SM::_INDEX_).toInt();
        m_tabWidget->tabBar()->setTabText(index, text);
        m_tabWidget->tabBar()->setTabToolTip(index, text);
        const QUuid id = m_tabWidget->widget(index)->property(SM::_ID_).toUuid();
        auto tabDesc = SM::sGetTab(id);
        if (tabDesc) {
            tabDesc.value().tabName = text;
            tabDesc.value().index = index;
            SM::sSetTab(tabDesc.value());
        }
    });

    connect(m_commentEditor,       &QLineEdit::textChanged, [this](const QString &text){
        const QUuid id = m_commentEditor->property(SM::_ID_).toUuid();
        auto optSnippetDesc = SM::sGetSnippet(id);
        if (optSnippetDesc) {
            SnippetDesc snippetDesc = optSnippetDesc.value();
            snippetDesc.setComment(text);
            SM::sSetSnippet(snippetDesc);
        }

        QWidget *wTab = m_tabWidget->currentWidget();
        QWidget *wSnippet = findSnippetWidget(wTab, id);
        if (wSnippet == nullptr)
            return;

        wSnippet->setProperty(SM::_COMMENT_, text);
    });

    connect(m_commentEditor,       &QLineEdit::returnPressed, [this](){
        m_commentEditor->setEnabled(false);
        QWidget *wTab = m_tabWidget->currentWidget();
        QWidgetList wSnippets = wTab->findChildren<QWidget *>(QString(SM::_W_SNIPPET_));
        const QUuid id = m_commentEditor->property(SM::_ID_).toUuid();
        QWidget *wSnippet = findSnippetWidget(wTab, id);
        if (wSnippet == nullptr)
            return;

        if (SM::sGetType(wSnippet) == SnippetDesc::LineSnippet)
            SM::sGetLineEdit(wSnippet)->setFocus();
        else
            SM::sGetTextEdit(wSnippet)->setFocus();
    });
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::showEvent(QShowEvent *)
{
    //Восстанавливаем размеры окна
    if (!SM::sStorage().geometry().isEmpty())
        setGeometry(SM::sStorage().geometry());

    //Цепляем LineEdit к табам
    m_tabEditorWatcher = new TabEditorWatcher(m_tabEditor);
    m_tabWidget->tabBar()->installEventFilter(m_tabEditorWatcher);
}

void MainWidget::hideEvent(QHideEvent *)
{
    //Сохраняем размеры окна
    SM::sStorage().setGeometry(geometry());
    SM::sSaveStorage();
}

void MainWidget::on_pbAddTabReleased()
{
    SnippetsTabDesc tabDesc;
    QWidget *wTab = new QWidget();

    m_tabWidget->addTab(wTab, QString());
    const int index = m_tabWidget->count()-1;
    m_tabWidget->setCurrentIndex(index);

    wTab->setObjectName(tabDesc.id.toString());
    wTab->setProperty(SM::_ID_, tabDesc.id);

    tabDesc.index = index;
    SM::sStorage().setCurrentTab(tabDesc.id);
    SM::sSetTab(tabDesc);

    connect(m_tabWidget->tabBar(), &QTabBar::tabMoved, [this](int from, int to){
        Q_UNUSED(from)
        const QUuid id = ui->twMain->currentWidget()->property(SM::_ID_).toUuid();
        auto optTabDesc = SM::sGetTab(id);
        if (optTabDesc) {
            SM::sStorage().setCurrentTab(id);
            optTabDesc.value().index = to;
            SM::sSetTab(optTabDesc.value());
        }
    });

    SM::sAddControls(wTab);
    connectTopButtons(wTab, tabDesc.id);
    wTab->layout()->setSpacing(0);
    on_tabBarDoubleClicked(index);
}

void MainWidget::on_pbRemoveTabReleased()
{
    auto button = QMessageBox::warning(this,
                                       "Warning",
                                       "You are sure to remove the tab and lose snippets?",
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (button == QMessageBox::No)
        return;

    const int index = m_tabWidget->currentIndex();
    QWidget *wTab = m_tabWidget->widget(index);
    const QUuid id = wTab->property(SM::_ID_).toUuid();
    auto tabDesc = SM::sGetTab(id);
    m_tabWidget->removeTab(index);
    wTab->deleteLater();

    if (tabDesc) {
        SM::sStorage().tabs.removeOne(tabDesc.value());
        const QUuid nextId = m_tabWidget->currentWidget()->property(SM::_ID_).toUuid();
        SM::sStorage().setCurrentTab(nextId);
        auto optTabDesc = SM::sGetTab(nextId);
        if (optTabDesc) {
            optTabDesc.value().index = m_tabWidget->currentIndex();
            SM::sSetTab(optTabDesc.value(), false);
        }
        SM::sSaveStorage();
    }

}

void MainWidget::on_tabBarDoubleClicked(int index)
{
    QTabBar *tabBar = m_tabWidget->tabBar();
    m_tabEditor->setGeometry(tabBar->tabRect(index));
    m_tabEditor->setProperty(SM::_INDEX_, index);
    m_tabEditor->setText(tabBar->tabText(index));
    m_tabEditor->show();
    m_tabEditor->setFocus();
}

void MainWidget::on_tbDelItemReleased()
{
    QToolButton *tbDel = qobject_cast<QToolButton *>(sender());
    if (tbDel == nullptr)
        return;

    QWidget    *wSnippet  = tbDel->parentWidget();
    const QUuid id = wSnippet->property(SM::_ID_).toUuid();

    QWidget    *wTab      = wSnippet->parentWidget();
    wTab->layout()->removeWidget(wSnippet);
    wSnippet->deleteLater();

    SM::sRemoveSnippet(id);
}

void MainWidget::on_tbCopyItemReleased()
{
    QToolButton *tbCopy = qobject_cast<QToolButton *>(sender());
    if (tbCopy == nullptr)
        return;

    const QString txt = SM::sText(tbCopy->parentWidget());
    if (txt.isEmpty())
        return;

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(txt.trimmed());
}

void MainWidget::on_currentChanged(int index)
{
    m_tabEditor->hide();
    m_tabEditor->setProperty(SM::_INDEX_, index);

    QWidget *wTab = m_tabWidget->widget(index);
    SM::sStorage().setCurrentTab(wTab->property(SM::_ID_).toUuid());
    auto tabDesc = SM::sGetTab(SM::sStorage().currentTabId());
    if (tabDesc) {
        tabDesc.value().index = index;
        SM::sSetTab(tabDesc.value());
    }
}

void MainWidget::connectDelAndCopy(QWidget *wSnippet)
{
    //привязываем его кнопеля к слотам
    connect(SM::sGetButton(wSnippet, SM::_TB_DEL_), &QToolButton::released,
            this, &MainWidget::on_tbDelItemReleased);
    connect(SM::sGetButton(wSnippet, SM::_TB_COPY_), &QToolButton::released,
            this, &MainWidget::on_tbCopyItemReleased);
}

template <typename T>
concept HasStdMenuFatoryMethod = requires (T t) {
    {t.createStandardContextMenu()} -> std::convertible_to<QMenu *>;
};

static void sCreateCustomMenu(HasStdMenuFatoryMethod auto *editor,
                              std::function<void ()> addCommentFunc,
                              std::function<void ()> copyCommentFunc)
{
    QMenu *menu = editor->createStandardContextMenu();
    menu->addSeparator();
    menu->addAction(QIcon::fromTheme("document-print-preview"),
                    QObject::tr("Add comment"),
                    QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_C),
                    addCommentFunc);
    menu->addAction(QObject::tr("Copy comment"),
                    copyCommentFunc);
    menu->exec(QCursor::pos());
    delete menu;
}

static void sSaveSnippetText(const QUuid &snippetDescID, const QString &text)
{
    auto optSnippetDesc = SnippetsManager::sGetSnippet(snippetDescID);
    if (optSnippetDesc) {
        optSnippetDesc.value().setText(text);
        SnippetsManager::sSetSnippet(optSnippetDesc.value());
    }
}

void MainWidget::connectEditors(QWidget *wSnippet)
{
    const QUuid &snippetDescID = wSnippet->property(SM::_ID_).toUuid();

    auto addCommentFunc = [this, snippetDescID]()
    {
        m_commentEditor->setEnabled(true);
        m_commentEditor->setFocus();
        m_commentEditor->setProperty(SM::_ID_, snippetDescID);
        auto optSnippetDesc = SnippetsManager::sGetSnippet(snippetDescID);
        if (optSnippetDesc)
            m_commentEditor->setText(optSnippetDesc.value().comment());
    };

    auto copyCommentFunc = [snippetDescID]()
    {
        auto optSnippetDesc = SnippetsManager::sGetSnippet(snippetDescID);
        if (optSnippetDesc)
            QApplication::clipboard()->setText(optSnippetDesc.value().comment());
    };

    if (SM::sIsLineEdit(wSnippet)) {
        QLineEdit *le = SM::sGetLineEdit(wSnippet);
        le->installEventFilter(m_mouseWatcher);

        connect(le, &QLineEdit::textChanged,
                [snippetDescID](const QString &text){
            sSaveSnippetText(snippetDescID, text);
        });
        connect(le, &QWidget::customContextMenuRequested,
                [le, addCommentFunc, copyCommentFunc](const QPoint &){
            sCreateCustomMenu(le, addCommentFunc, copyCommentFunc);
        });
    } else {
        QPlainTextEdit *ptE = SM::sGetTextEdit(wSnippet);
        ptE->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        ptE->installEventFilter(m_mouseWatcher);

        connect(ptE, &QPlainTextEdit::textChanged,
                         [snippetDescID, ptE](){
            ptE->adjustSize();
            sSaveSnippetText(snippetDescID, ptE->toPlainText());
        });
        connect(ptE, &QWidget::customContextMenuRequested,
                         [ptE, addCommentFunc, copyCommentFunc](const QPoint &){
            sCreateCustomMenu(ptE, addCommentFunc, copyCommentFunc);
        });
    }
}

void MainWidget::connectTopButtons(QWidget *wTab, const QUuid &tabDescID)
{
    QCheckBox *cbDisable = SM::sGetCheckBox(wTab, SM::_CB_DISABLE_);

    connect(cbDisable, &QCheckBox::toggled, [wTab, tabDescID](bool check){
        QWidgetList snippets = wTab->findChildren<QWidget *>(SM::_W_SNIPPET_);
        for (QWidget *wSnippet : snippets)
            SM::sSetEditorEnabled(wSnippet, !check);

        auto optTabDesc = SM::sGetTab(tabDescID);
        if (optTabDesc) {
            optTabDesc.value().disabled = check;
            SM::sSetTab(optTabDesc.value());
        }
    });

    QStringList tbAddNames {SM::_TB_ADD_, SM::_TB_ADD_T_};
    for (const QString& tbName : tbAddNames) {
        QToolButton *tbAdd = SM::sGetButton(wTab, tbName);
        SnippetDesc::Type type = SM::sGetType(qobject_cast<QWidget *>(tbAdd));

        connect(tbAdd, &QToolButton::released, [this, type, wTab, cbDisable](){
            createSnippetWidgetOnTab(wTab, SnippetDesc(type));

            //Скрываем tabEditor
            m_tabEditor->hide();
            //Делаем активными все сниппеты если они были отключены
            if (cbDisable->isChecked())
                cbDisable->toggle();
        });
    }
}

void MainWidget::createSnippetWidgetOnTab(QWidget *wTab, const SnippetDesc &snippetDesc)
{
    //Создаём сниппет виджет
    QWidget *wSnippet = SM::sCreateSnippet(wTab, snippetDesc);
    const QUuid tabDescID = wTab->property(SM::_ID_).toUuid();
    SM::sSetSnippet(tabDescID, snippetDesc);
    //привязываем его кнопеля к слотам
    connectDelAndCopy(wSnippet);
    //Коннектим контейнеры текста
    connectEditors(wSnippet);
}

QWidget *MainWidget::findSnippetWidget(QWidget *wTab, const QUuid &id)
{
    QWidgetList wSnippets = wTab->findChildren<QWidget *>(QString(SM::_W_SNIPPET_));
    auto it = std::find_if(std::begin(wSnippets), std::end(wSnippets),[id](QWidget *wSnippet){
            return wSnippet->property(SM::_ID_).toUuid() == id;
    });
    if (it == std::end(wSnippets))
        return nullptr;

    return *it;
}

