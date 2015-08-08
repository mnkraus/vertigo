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

#include "collisionmanager.h"
#include "objects/object.h"

#include <glm/geometric.hpp>

namespace fight {


CollisionCache::CollisionCache()
{
}


void CollisionCache::addObject(Object *object, bool collision, const glm::vec3 &position, const glm::vec3 &normal)
{
    m_entries.emplace_back();
    CacheEntry &entry = m_entries.back();
    entry.object = object;
    entry.collision = collision;
    entry.position = position;
    entry.normal = normal;
}


bool CollisionCache::testObject(Object *object, bool &collision, glm::vec3 &position, glm::vec3 &normal)
{
    for (const CacheEntry &entry :  m_entries)
        if (entry.object == object)
        {
            collision = entry.collision;
            position = entry.position;
            normal = entry.normal;
            return true;
        }
    return false;
}


CollisionManager::CollisionManager()
{
}


void CollisionManager::addObject(Object *object)
{
    m_objects.push_back(object);
}


Object* CollisionManager::testCollision(const glm::vec3 &start, const glm::vec3 &end, float radius, glm::vec3 &position, glm::vec3 &normal, CollisionCache *collisionCache)
{
    glm::vec3 dir = end - start;
    float distance = glm::length(dir);
    dir /= distance;

    Object *collisionObject = NULL;
    for (Object *object : m_objects)
        if (object->isEnabled() && object->box().test(end, radius))
        {
            bool collision = false;
            float d;
            glm::vec3 pos, norm;

            if (object->isStatic() && collisionCache)
            {
                if (collisionCache->testObject(object, collision, pos, norm))
                    d = glm::dot(pos - start, dir);
                else
                {
                    collision = object->intersect(start, dir, radius, d, norm);
                    collisionCache->addObject(object, collision, start + dir*d, norm);
                }
            }
            else
                collision = object->intersect(start, dir, radius, d, norm);

            if (collision && distance > d)
            {
                distance = d;
                position = start;
                if (distance > 0)
                    position += dir*distance;
                normal = norm;
                collisionObject = object;
            }
        }
    return collisionObject;
}


} // namespace fight
