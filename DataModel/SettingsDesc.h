#pragma once

#include "qserializer.h"
#include <QRect>

class Rect : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    public:
    Rect(const QRect& r = QRect())
    : x{r.x()}
    , y{r.y()}
    , w{r.width()}
    , h{r.height()}
    {

    }

    Rect operator = (const QRect& r)
    {
        x = r.x();
        y = r.y();
        w = r.width();
        h = r.height();
        return *this;
    }

    QRect rect() const
    {
        return QRect(x, y, w, h);
    }

    QS_FIELD(int, x)
    QS_FIELD(int, y)
    QS_FIELD(int, w)
    QS_FIELD(int, h)
};

class WindowDesc : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
public:
    WindowDesc() {}
    QS_OBJECT(Rect, coord)
    QS_FIELD(Qt::WindowFlags, flags)
};


class SettingsDesc : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
public:
    SettingsDesc() {}
    QS_OBJECT(WindowDesc, winSettings)
};
