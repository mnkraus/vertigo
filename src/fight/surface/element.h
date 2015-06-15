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

#ifndef SURFACE_ELEMENT_H
#define SURFACE_ELEMENT_H


#include "gfx/texture.h"
#include <QVector>
#include <QMap>
#include <QVector2D>
#include <QVector3D>
#include <QPoint>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>


namespace fight {


class Surface;


class Element
{
public:
    Element(Surface *surface, QRect rect);
    ~Element();

public:
    int numVertices(int textureId);
	void addVertex(int textureId, const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &texCoords);
    void addTriangle(int textureId, quint16 a, quint16 b, quint16 c);

    void draw();
    float maxZ() const { return m_maxZ; }
    float minZ() const { return m_minZ; }
    QVector3D center() const;
    bool testCollision(const QVector3D &center, float radius);

private:
    struct ElementSubset
    {
        QVector<QVector3D> vertices;
        QVector<QVector3D> normals;
        QVector<QVector2D> texCoords;
        QVector<quint16> indices;
    };

private:
    Surface *m_surface;
    QRect m_rect;
    float m_maxZ;
    float m_minZ;
    float *m_heights;

    QMap<int, ElementSubset> m_subsets;
};


} // namespace fight


#endif // SURFACE_ELEMENT_H
