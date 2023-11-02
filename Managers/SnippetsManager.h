#pragma once

#include <optional>
#include <QString>
#include <QUuid>
#include "SnippetsHolderDesc.h"

class QWidget;
class QToolButton;
class QLineEdit;
class QPlainTextEdit;
class QVBoxLayout;
class QCheckBox;
class QApplication;

class SnippetsManager
{
public:

    static constexpr const char * _TYPE_      = "SnippetType";
    static constexpr const char * _ID_        = "ID"         ;
    static constexpr const char * _COMMENT_   = "Comment"    ;

    static constexpr const char * _TB_DEL_   = "tbDel"  ;
    static constexpr const char * _TB_COPY_  = "tbCopy" ;

    static constexpr const char * _TB_ADD_     = "tbAdd"    ;
    static constexpr const char * _TB_ADD_T_   = "tbAddT"   ;
    static constexpr const char * _CB_DISABLE_ = "cbDisable";

    static constexpr const char * _LE_TEXT_  = "leText" ;
    static constexpr const char * _PTE_TEXT_ = "pteText";

    static constexpr const char * _W_SNIPPET_ = "SnippetWidget";

    static constexpr const char * _INDEX_     = "index";

    static void sSetFusionPalette(QApplication *);

    static QVBoxLayout    *sAddControls    (QWidget *parent);

    static QWidget        *sGenItem      (QWidget        *parent = nullptr,
                                          SnippetDesc::Type type = SnippetDesc::Type::LineSnippet);

    static QToolButton    *sGetButton    (QWidget *parent, const QString &name);
    static QCheckBox      *sGetCheckBox  (QWidget *parent, const QString &name = _CB_DISABLE_);
    static QToolButton    *sGetDelButton (QWidget *parent);
    static QToolButton    *sGetCopyButton(QWidget *parent);
    static QLineEdit      *sGetLineEdit  (QWidget *parent);
    static QPlainTextEdit *sGetTextEdit  (QWidget *parent);

    static SnippetDesc::Type sGetType(QWidget *w);

    static bool    sIsLineEdit      (QWidget *parent);
    static void    sSetText         (QWidget *parent, QStringView text);
    static void    sSetEditorEnabled(QWidget *parent, bool enabled);

    static QString sText      (QWidget *parent);

    static QWidget *sCreateSnippet(QWidget *tabPage, SnippetDesc::Type type);

    static QWidget *sCreateSnippet(QWidget *tabPage, const SnippetDesc &snippetDesc);

    static std::optional<SnippetsTabDesc> sGetParentTab(const SnippetDesc &snippetDesc);

    static std::optional<SnippetsTabDesc> sGetTab(const QUuid &id);

    static void sSetTab(const SnippetsTabDesc &tabDesc, bool save = true);

    static std::optional<SnippetDesc> sGetSnippet(const QUuid &id);

    static void sSetSnippet(const QUuid &tabID, const SnippetDesc &snippetDesc, bool save = true);

    static void sSetSnippet(const SnippetDesc &snippetDesc, bool save = true);

    static void sRemoveSnippet(const QUuid &id, bool save = true);

    static SnippetsHolderDesc &sStorage();
    static void sSaveStorage();
    static void sLoadStorage();
};



