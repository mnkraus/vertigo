/***************************************************************************
*  Copyright (C) 2015  Philipp Nordhus                                    *
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

#ifndef UTIL_RECT_H
#define UTIL_RECT_H

#include <glm/vec2.hpp>
#include <glm/common.hpp>


namespace util {


template<typename T>
class Rectangle
{
public:
    Rectangle() : /*x(0), y(0),*/ width(0), height(0) { };
    Rectangle(T x, T y, T width, T height) : /*x(x), y(y),*/ posTest(x, y), width(width), height(height) { };
    Rectangle(const glm::tvec2<T> &pos, const glm::tvec2<T> &size) : x(pos.x), y(pos.y), width(size.x), height(size.y) { };

public:
    T left() const { return x; }
    T top() const { return y; }
    glm::tvec2<T> center() const { return glm::tvec2<T>(x + width/2, y + height/2); }

    glm::tvec2<T> pos() const { return glm::tvec2<T>(x, y); }
    glm::tvec2<T> size() const { return glm::tvec2<T>(width, height); }
    void setPos(const glm::tvec2<T> &pos) { x = pos.x; y = pos.y; }
    void setSize(const glm::tvec2<T> &size) { width = size.x; height = size.y; }

public:
    union
    {
        struct
        {
            glm::tvec2<T> posTest;
        };
        struct
        {
            T x;
            T y;
        };
    };
    T width;
    T height;
};


template<typename T>
class RectangleExclusive : public Rectangle<T>
{
public:
    //using Rectangle<T>::Rectangle<T>;
    RectangleExclusive() = default;
    RectangleExclusive(T x, T y, T width, T height) : Rectangle<T>(x, y, width, height) { };
    RectangleExclusive(const glm::tvec2<T> &pos, const glm::tvec2<T> &size) : Rectangle<T>(pos, size) { };

    RectangleExclusive(const Rectangle<T> &base) : Rectangle<T>(base) { };

    template<typename P>
    RectangleExclusive(const Rectangle<P> &base) : Rectangle<T>(base.pos(), base.size()) { };

public:

    T right() const { return this->x + this->width - 1; }
    T bottom() const { return this->y + this->height - 1; }
};


template<typename T>
class RectangleInclusive : public Rectangle<T>
{
public:
    //using Rectangle<T>::Rectangle<T>;
    RectangleInclusive() = default;
    RectangleInclusive(T x, T y, T width, T height) : Rectangle<T>(x, y, width, height) { };
    RectangleInclusive(const glm::tvec2<T> &pos, const glm::tvec2<T> &size) : Rectangle<T>(pos, size) { };

    RectangleInclusive(const Rectangle<T> &base) : Rectangle<T>(base) { };

    template<typename P>
    RectangleInclusive(const Rectangle<P> &base) : Rectangle<T>(base.pos(), base.size()) { };

public:

    T right() const { return this->x + this->width; }
    T bottom() const { return this->y + this->height; }
};


template<class R>
inline R roundRect(const R &rect)
{
    return R(glm::round(rect.pos()), glm::round(rect.size()));
}


typedef RectangleExclusive<int> Rect;

typedef RectangleInclusive<float> RectF;

typedef glm::ivec2 Point;

typedef glm::vec2 PointF;

typedef glm::ivec2 Size;

typedef glm::vec2 SizeF;


} // namespace util


#endif // UTIL_RECT_H
