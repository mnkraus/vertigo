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

#include "trash.h"
#include "billboard.h"

#include <glm/gtx/norm.hpp>

namespace fight {

Effects Trash::trashCollection[9] = {Trash_0, Trash_0, Trash_1, Trash_1, Trash_2, Trash_2, Trash_3, Trash_3, Trash_4};

Trash::Trash(Scenario *scenario, Billboard *billboard, float angle) : 
    Effect(scenario, billboard, angle, 1)
{
    m_type = TrashObject;
}


void Trash::setPosition(const glm::vec3 &pos)
{
    Object::setPosition(pos);
    BoundingBox box = m_billboard->box();
    m_box = BoundingBox(pos + box.minPoint(), pos + box.maxPoint());
}


bool Trash::intersect(const glm::vec3 &start, const glm::vec3 &dir, float radius, float &distance, glm::vec3 &normal)
{
    return m_billboard->intersect(start - m_position, dir, distance);
}


void Trash::destroy()
{
    m_scenario->effectManager()->addEffect(Explosion_5, m_position, 0, glm::length2(m_box.dim()) > 15 ? 2 : 1);
    Object::destroy();
}


} // namespace fight
