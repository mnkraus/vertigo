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

#ifndef FIGHT_OBJECT_H
#define FIGHT_OBJECT_H


#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <QGLContext>
#include "modulemanager.h"
#include "scenario.h"
#include "boundingbox.h"


namespace fight {


class Scenario;
class CollisionCache;


class Object
{
public:
    Object(Scenario *scenario);
    Object(Scenario *scenario, const QString &name, float scale = 1/32.0f);
    ~Object();

public:
    void setEnabled(bool);
    void enable();
    void disable();
    bool isEnabled() const { return m_enabled; }
    virtual void setPosition(const QVector3D &pos);
    QVector3D position() const { return m_position; }

    const BoundingBox& box() const { return m_box; }
    bool isStatic() const { return m_static; }
    CollisionCache *collisionCache() const { return m_collisionCache; }
    void setCollisionCache(CollisionCache *cache);

public:
    virtual void update();
    virtual void draw();
    virtual bool intersect(const QVector3D &start, const QVector3D &dir, float radius, float &distance, QVector3D &normal);

protected:
    Scenario *m_scenario;
    bool m_enabled;
    Module m_base;
    float m_scale;
    QVector3D m_position;

    BoundingBox m_box;
    bool m_static;
    CollisionCache *m_collisionCache;
};


} // namespace fight


#endif // FIGHT_OBJECT_H
