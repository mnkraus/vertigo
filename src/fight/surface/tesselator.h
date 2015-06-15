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

#ifndef SURFACE_TESSELATOR_H
#define SURFACE_TESSELATOR_H


#include <QList>
#include <QVector2D>
#include <QVector3D>

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>


namespace fight {


class BetaSpline;
class Element;
class Surface;


class Tesselator
{
public:
    Tesselator(int MaxLevel, Surface *surface);
    ~Tesselator();

public:
	float heightAt(const QVector2D &pos);
	float heightAt(const QVector2D &pos, QVector3D &normal);
    Element* tesselate(QRect rect, int level, QVector3D scale, QByteArray &textureMap, QByteArray &textureDir, int mapping);
    bool intersect(const QVector3D &start, const QVector3D &end, float radius, QVector3D &position, QVector3D &normal);

private:
    void InitIndices(int MaxLevel);
    void PrepareElementSubset(int level, QVector3D scale, int x, int y, int textureId, QVector2D t0, QVector2D tu, QVector2D tv, Element *element);

private:
	std::vector<BetaSpline> m_splines;
	quint16 *indices;

    Surface *m_surface;
};


} // namespace fight


#endif // SURFACE_TESSELATOR_H
