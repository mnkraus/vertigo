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

#include "sonar.h"
#include "scenario.h"
#include "sfx/samplemap.h"

#include <glm/geometric.hpp>


namespace fight {


Sonar::Sonar(Scenario *scenario) :
    m_scenario(scenario),
    m_active(false),
    m_activatingDelay(0)
{
}


void Sonar::init(int sensor)
{
    // TODO: check values
    if (sensor == 6145)
    {
        m_delay = 12000;
        m_passiveRange = 180.0f;
        m_passiveRangeMult = 20.0f;
        m_activeRange = 180.0f;
        m_activeRangeMult = 20.0f;
        m_iff = false;
    }
    if (sensor == 6146)
    {
        m_delay = 10000;
        m_passiveRange = 200.0f;
        m_passiveRangeMult = 30.0f;
        m_activeRange = 180.0f;
        m_activeRangeMult = 20.0f;
        m_iff = true;
    }
    if (sensor == 6147)
    {
        m_delay = 8000;
        m_passiveRange = 220.0f;
        m_passiveRangeMult = 40.0f;
        m_activeRange = 180.0f;
        m_activeRangeMult = 20.0f;
        m_iff = true;
    }
    if (sensor == 6148)
    {
        m_delay = 6000;
        m_passiveRange = 240.0f;
        m_passiveRangeMult = 50.0f;
        m_activeRange = 180.0f;
        m_activeRangeMult = 20.0f;
        m_iff = true;
    }
}


void Sonar::activate()
{
    m_activatingDelay = m_delay;
}


void Sonar::deactivate()
{
    m_active = false;
    m_activatingDelay = 0;
    sfx::SampleMap::get(sfx::Sample::SonarDeactivated).play();
}


void Sonar::toggle()
{
    if (!m_active && m_activatingDelay <= 0)
        activate();
    else
        deactivate();
}


void Sonar::update(float elapsedTime)
{
    if (!m_active && m_activatingDelay > 0)
    {
        m_activatingDelay -= elapsedTime;
        if (m_activatingDelay <= 0)
        {
            m_active = true;
            sfx::SampleMap::get(sfx::Sample::SonarActivated).play();
        }
    }

    m_sonarEntries.clear();
    for (const auto &object_ptr : *m_scenario)
    {
        ActiveObject *object = dynamic_cast<ActiveObject *>(object_ptr.get());
        if (!object || !object->isEnabled())
            continue;

        float passiveRange = m_passiveRange + m_passiveRangeMult*object->noise();
        float activeRange = m_activeRange + m_activeRangeMult*object->noise();
        glm::vec3 dir = object->position() - m_scenario->position();
        float dist = glm::length(dir);
        dir /= dist;
        if (dist > passiveRange && (dist > activeRange || !m_active))
            continue;

        m_sonarEntries.emplace_back();
        SonarEntry &entry = m_sonarEntries.back();
        entry.object = object;
        entry.dir = dir;
        entry.isFriend = m_scenario->isFriend(0, object->iff());
        entry.isPassive = dist > activeRange || !m_active;
    }
}


} // namespace fight
