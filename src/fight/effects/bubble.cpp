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

#include "bubble.h"
#include "billboard.h"
#include "fight/scenario.h"
#include <glm/gtx/norm.hpp>


namespace fight {


Bubble::Bubble(Scenario *scenario, Billboard *billboard) :
    Effect(scenario, billboard, 0, 2)
{
    m_kineticShield = m_kineticShieldMax = billboard->kineticShield();
    m_kineticStrength = billboard->kineticStrength();
    m_shockStrength = billboard->shockStrength();
}


void Bubble::setPosition(const Vector3D &pos)
{
    m_originPos = pos;
    Object::setPosition(pos);
    m_time = glm::distance2(m_scenario->position(), m_position)/200.0f*qrand()/RAND_MAX;
}

bool Bubble::update(float elapsedTime)
{
    m_time += elapsedTime;
    m_position = m_originPos + Vector3D(/*(qrand()%10 - 5)/100.0f, (qrand()%10 - 5)/100.0f,*/0, 0, m_time*m_billboard->velocity());

    if (glm::distance2(m_scenario->position(), m_position) > 2500.0f)
    {
        disable();
        return true;
    }
    return false;
}


} // namespace fight
