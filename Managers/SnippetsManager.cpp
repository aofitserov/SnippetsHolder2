#include <QToolButton>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QCheckBox>
#include <QApplication>
#include <QStyleFactory>
#include <QMenu>

#include "JsonFile.h"
#include "SnippetsManager.h"


void SnippetsManager::sSetFusionPalette(QApplication *app)
{
    app->setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Normal, QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::Normal, QPalette::WindowText, Qt::gray);
    darkPalette.setColor(QPalette::Normal, QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::Normal, QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::Normal, QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::Normal, QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Normal, QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Normal, QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::Normal, QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Normal, QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Normal, QPalette::Link, QColor(42, 130, 218));

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    darkPalette.setColor(QPalette::Disabled, QPalette::Window, QColor(83,83,83));
    darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, Qt::darkGray);
    darkPalette.setColor(QPalette::Disabled, QPalette::Base, QColor(45,45,45));

    darkPalette.setColor(QPalette::Active, QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Inactive, QPalette::WindowText, Qt::lightGray);

    app->setPalette(darkPalette);

    app->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
}

QVBoxLayout *SnippetsManager::sAddControls(QWidget *tab)
{
    QVBoxLayout *vlMain = new QVBoxLayout(tab);
    vlMain->setSpacing(2);
    vlMain->setContentsMargins(1, 1, 1, 1);
    vlMain->setObjectName(QString::fromUtf8("vlMain"));
    QHBoxLayout *hbLayout = new QHBoxLayout();
    hbLayout->setSpacing(2);
    hbLayout->setObjectName(QString::fromUtf8("hbLayout"));
    QToolButton *tbAdd = new QToolButton(tab);
    tbAdd->setObjectName(QString::fromUtf8(_TB_ADD_));
    tbAdd->setProperty(_TYPE_, SnippetDesc::LineSnippet);
    tbAdd->setMinimumSize(QSize(25, 25));

    hbLayout->addWidget(tbAdd);

    QToolButton *tbAddT = new QToolButton(tab);
    tbAddT->setObjectName(QString::fromUtf8(_TB_ADD_T_));
    tbAddT->setProperty(_TYPE_, SnippetDesc::TextSnippet);
    tbAddT->setMinimumSize(QSize(25, 25));

    hbLayout->addWidget(tbAddT);

    QSpacerItem *horizontalSpacer = new QSpacerItem(98, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hbLayout->addItem(horizontalSpacer);

    QCheckBox *cbDisable = new QCheckBox(tab);
    cbDisable->setObjectName(QString::fromUtf8(_CB_DISABLE_));

    hbLayout->addWidget(cbDisable);


    vlMain->addLayout(hbLayout);
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 341, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vlMain->addItem(verticalSpacer);

    tbAdd->setText("+ Line");
    tbAddT->setText("+ Text");
    cbDisable->setText("Disable Items");

    return vlMain;
}

QWidget *SnippetsManager::sGenItem(QWidget *parent, SnippetDesc::Type type)
{
    QWidget *widget = new QWidget(parent);
    widget->setObjectName(_W_SNIPPET_);
    widget->setProperty(_TYPE_, QVariant::fromValue(type));
    widget->setMouseTracking(true);

    QToolButton *tbDel  = new QToolButton(widget);
    tbDel->setText("-");
    tbDel->setObjectName(_TB_DEL_);
    tbDel->setFixedSize(28, 28);

    QLineEdit   *leText = new QLineEdit  (widget);
    leText->setObjectName(_LE_TEXT_);
    leText->setVisible(type == SnippetDesc::Type::LineSnippet);
    leText->setContextMenuPolicy(Qt::CustomContextMenu);
    leText->setMouseTracking(true);

    QPlainTextEdit *pteText = new QPlainTextEdit(widget);
    pteText->setObjectName(_PTE_TEXT_);
    pteText->setVisible(type == SnippetDesc::Type::TextSnippet);
    pteText->setContextMenuPolicy(Qt::CustomContextMenu);
    pteText->setMouseTracking(true);

    QToolButton *tbCopy = new QToolButton(widget);
    tbCopy->setText("C");
    tbCopy->setObjectName(_TB_COPY_);
    tbCopy->setFixedSize(28, 28);

    QHBoxLayout *hbL = new QHBoxLayout();
    const QWidgetList wList {tbDel, leText, pteText, tbCopy};
    for (auto w : wList)
        hbL->addWidget(w);

    hbL->setSpacing(2);
    hbL->setContentsMargins(1, 1, 1, 1);
    widget->setLayout(hbL);

    QWidget *focusWidget = type == SnippetDesc::LineSnippet
            ? qobject_cast<QWidget*>(leText) : qobject_cast<QWidget*>(pteText);
    focusWidget->setFocus();

    return widget;
}

QToolButton *SnippetsManager::sGetButton(QWidget *parent, const QString &name)
{
    return parent->findChild<QToolButton *>(name);
}

QCheckBox *SnippetsManager::sGetCheckBox(QWidget *parent, const QString &name)
{

    return parent->findChild<QCheckBox *>(name);
}

QToolButton *SnippetsManager::sGetDelButton(QWidget *parent)
{
    return sGetButton(parent, _TB_DEL_);
}

QToolButton *SnippetsManager::sGetCopyButton(QWidget *parent)
{
    return sGetButton(parent, _TB_COPY_);
}

QLineEdit *SnippetsManager::sGetLineEdit(QWidget *parent)
{
    return parent->findChild<QLineEdit *>(_LE_TEXT_);
}

QPlainTextEdit *SnippetsManager::sGetTextEdit(QWidget *parent)
{
    return parent->findChild<QPlainTextEdit *>(_PTE_TEXT_);
}

SnippetDesc::Type SnippetsManager::sGetType(QWidget *w)
{
    return w->property(_TYPE_).value<SnippetDesc::Type>();
}

bool SnippetsManager::sIsLineEdit(QWidget *parent)
{
    return parent->property(_TYPE_).value<SnippetDesc::Type>() == SnippetDesc::Type::LineSnippet;
}

void SnippetsManager::sSetText(QWidget *parent, QStringView text)
{
    return sIsLineEdit(parent) ? sGetLineEdit(parent)->setText(text.toString())
                               : sGetTextEdit(parent)->setPlainText(text.toString());
}

void SnippetsManager::sSetEditorEnabled(QWidget *parent, bool enabled)
{
    const QRegularExpression re{"^(" + QString(_LE_TEXT_) + "|" + QString(_PTE_TEXT_) + ")$"};
    const QWidgetList children = parent->findChildren<QWidget*>(re);
    foreach (QWidget * editor , children)
        editor->setEnabled(enabled);
}


QString SnippetsManager::sText(QWidget *parent)
{
    return sIsLineEdit(parent) ? sGetLineEdit(parent)->text()
                               : sGetTextEdit(parent)->toPlainText();
}

QWidget * SnippetsManager::sCreateSnippet(QWidget *tabPage, SnippetDesc::Type type)
{
    QVBoxLayout *vl = static_cast<QVBoxLayout *>(tabPage->layout());
    QWidget *wSnippet = SnippetsManager::sGenItem(tabPage, type);
    const int index = vl->count() - 1;
    wSnippet->setAutoFillBackground(true);
    wSnippet->setBackgroundRole(index%2 ? QPalette::AlternateBase : QPalette::Base);
    vl->insertWidget(index, wSnippet);
    return wSnippet;
}

QWidget *SnippetsManager::sCreateSnippet(QWidget *tabPage, const SnippetDesc &snippetDesc)
{
    QWidget *wSnippet = sCreateSnippet(tabPage, snippetDesc.type());
    SnippetsManager::sSetText(wSnippet, snippetDesc.text());
    wSnippet->setProperty(_COMMENT_, snippetDesc.comment());
    wSnippet->setProperty(_ID_, snippetDesc.ID());
    return wSnippet;
}

std::optional<SnippetsTabDesc> SnippetsManager::sGetParentTab(const SnippetDesc &snippetDesc)
{
    const QList<SnippetsTabDesc> &tabs = sStorage().tabs;
    auto it = std::find_if(std::cbegin(tabs),
                           std::cend(tabs),
                           [snippetDesc](const SnippetsTabDesc &tab)
    {
        const QList<SnippetDesc> &snippets = tab.snippets;
        return std::find(std::cbegin(snippets),
                         std::cend  (snippets),
                         snippetDesc)
                != std::cend(snippets);
    });

    if (it != std::end(tabs))
        return *it;

    return std::nullopt;
}

std::optional<SnippetsTabDesc> SnippetsManager::sGetTab(const QUuid &id)
{
    const QList<SnippetsTabDesc> tabs = sStorage().tabs;
    auto it = std::find_if(std::cbegin(tabs),
                           std::cend(tabs),
                           [id](const SnippetsTabDesc &tab) {
                                return tab.id == id;
                           });
    if (it != std::end(tabs))
        return *it;

    return std::nullopt;
}

void SnippetsManager::sSetTab(const SnippetsTabDesc &tabDesc, bool save)
{
    for (int i = 0; i < sStorage().tabs.size(); ++i) {
        if (sStorage().tabs.at(i).id == tabDesc.id) {
            sStorage().tabs[i] = tabDesc;
            return;
        }
    }
    sStorage().tabs.append(tabDesc);

    if (save)
        sSaveStorage();
}

std::optional<SnippetDesc> SnippetsManager::sGetSnippet(const QUuid &id)
{

    foreach (const SnippetsTabDesc &tabDesc, sStorage().tabs) {
        const QList<SnippetDesc> &snippets = tabDesc.snippets;
        auto it = std::find_if(std::cbegin(snippets), std::cend(snippets),
                               [id](const SnippetDesc& snippetDesc){
            return snippetDesc.id == id;
        });

        if (it != std::cend(snippets))
            return *it;
    }

    return std::nullopt;
}

void SnippetsManager::sSetSnippet(const QUuid &tabID, const SnippetDesc &snippetDesc, bool save)
{
    auto optTabDesc = sGetTab(tabID);
    if (!optTabDesc)
        return;

    SnippetsTabDesc tabDesc = optTabDesc.value();
    QList<SnippetDesc> &snippets = tabDesc.snippets;
    auto it = std::find(std::begin(snippets), std::end(snippets), snippetDesc);
    if (it == std::end(snippets))
        snippets.append(snippetDesc);
    else *it = snippetDesc;

    sSetTab(tabDesc, false);

    if (save)
        sSaveStorage();
}

void SnippetsManager::sSetSnippet(const SnippetDesc &snippetDesc, bool save)
{
    auto optTabDesc = sGetParentTab(snippetDesc);
    if (!optTabDesc)
        return;

    sSetSnippet(optTabDesc.value().id, snippetDesc, false);

    if (save)
        sSaveStorage();
}

void SnippetsManager::sRemoveSnippet(const QUuid &id, bool save)
{
    auto optSnippetDesc = sGetSnippet(id);
    if (!optSnippetDesc)
        return;

    auto optTabDesc = sGetParentTab(optSnippetDesc.value());
    if (!optTabDesc)
        return;

    optTabDesc.value().snippets.removeOne(optSnippetDesc.value());

    sSetTab(optTabDesc.value(), false);

    if (save)
        sSaveStorage();
}

SnippetsHolderDesc &SnippetsManager::sStorage()
{
    if (SnippetsHolderDesc::S::data() == nullptr)
        SnippetsHolderDesc::S::setData(new SnippetsHolderDesc{});

    return *SnippetsHolderDesc::S::data();
}

void SnippetsManager::sSaveStorage()
{
    JsonFile(DataDirPath() + "data.json", &sStorage()).save();
}

void SnippetsManager::sLoadStorage()
{
    JsonFile(DataDirPath() + "data.json", &sStorage()).load();
}
