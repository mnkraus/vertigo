/***************************************************************************
 *  Copyright (C) 2010  Philipp Nordhus                                    *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H


#include <QList>
#include <QObject>
#include <QRectF>


namespace ui {


class Widget : public QObject
{
public:
    Widget(Widget *parent = NULL);
    ~Widget();

public:
    void setVisible(bool);
    void hide();
    void show();
    bool isVisible() const { return m_visible; }
    void setPosition(float x, float y);
    void setSize(float w, float h);
    void setSize(const QSizeF &size);
    void setWidth(float w);
    const QRectF& rect() const { return m_rect; }
    QSizeF size() const { return m_rect.size(); }
    float width() const { return m_rect.width(); }
    float height() const { return m_rect.height(); }
    QRectF mapToGlobal(QRectF rect) const;

    void doDraw();
    bool doMousePressEvent(const QPointF &pos, Qt::MouseButton button);
    void doMouseReleaseEvent(const QPointF &pos, Qt::MouseButton button);
    void doMouseMoveEvent(const QPointF &pos);

private:
    virtual void draw() = 0;
    virtual bool mousePressEvent(const QPointF &pos, Qt::MouseButton button) { return false; }
    virtual void mouseReleaseEvent(const QPointF &pos, Qt::MouseButton button) {}
    virtual void mouseMoveEvent(const QPointF &pos) {}

private:
    Widget *m_parent;
    bool m_visible;
    QList<Widget*> m_children;
    QRectF m_rect;
};


inline void Widget::setSize(float w, float h)
{
    setSize(QSizeF(w, h));
}


} // namespace ui


#endif // UI_WIDGET_H
