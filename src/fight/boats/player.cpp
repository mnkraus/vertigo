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

#include "player.h"
#include "fight/scenario.h"
#include "game/boat.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/geometric.hpp>


namespace fight {


Player::Player(Scenario *scenario, game::Boat *boat) :
    SimpleBoat(scenario, boat->boatFile(), ObjectInfo()),
    m_engineState(EngineRunning),
    m_firing(false),
    m_gun(scenario, boat->gun() - 3073)
{
    int i;
    txt::DesFile &file = boat->boatFile();

    file.setSection("defense");
    for (i = 0; i < 4; i++)
        m_shieldMax[i] = file.value(QString("kineticshield%1").arg(i)).toInt();

    int armor = 0;
    if (boat->armor() == 5121)
        armor = 2000;
    if (boat->armor() == 5122)
        armor = 3500;
    if (boat->armor() == 5123)
        armor = 6500;
    if (boat->armor() == 5124)
        armor = 7500;
    for (i = 0; i < 4; i++)
        m_shieldMax[i] += armor;

    for (i = 0; i < 4; i++)
        m_shield[i] = m_shieldMax[i];

    if (boat->silator() == 4101)
        m_noise -= 1;
    if (boat->nrskin() == 5126)
        m_noise -= 1;

    m_gun.addMuzzles(Vector3D(0, 3.0f, 0), boat->gun());
    m_gun.setDefect(boat->defects()[game::Boat::DefectGun]);
    if (boat->tur1() != 0)
    {
        m_tur1.reset(new Gun(m_scenario, boat->tur1() - 3073));
        m_tur1->addMuzzles(Vector3D(0, 3.0f, 0) + boat->getMounting("TUR1")->rel, boat->tur1());
        m_tur1->setDefect(boat->defects()[game::Boat::DefectTur1]);
    }
    if (boat->tur2() != 0)
    {
        m_tur2.reset(new Gun(m_scenario, boat->tur2() - 3073));
        m_tur2->addMuzzles(Vector3D(0, 3.0f, 0) + boat->getMounting("TUR2")->rel, boat->tur2());
        m_tur2->setDefect(boat->defects()[game::Boat::DefectTur2]);
    }

    sfx::SampleMap::get(sfx::Sample::Engine).playLoop();

    m_scenario->collisionManager().removeObject(this);
}


float Player::noise() const
{
    return (m_engineState != EngineOff ? ActiveObject::noise() : 0) + (m_scenario->sonar().isActive() || m_scenario->sonar().isActivating() ? 1 : 0);
}


float Player::range() const
{
    return m_gun.range();
}


void Player::damage(int kinetic, int shock, const Vector3D &position)
{
    Vector3D dirDamage = glm::normalize(position - m_position);
    float cosZ = glm::dot(dirDamage, dir());
    if (cosZ > 0.9)
        m_shield[0] -= kinetic;
    else if (cosZ < -0.9)
        m_shield[3] -= kinetic;
    else
    {
        float cosY = glm::dot(dirDamage, up());
        if (cosY > 0)
            m_shield[1] -= kinetic;
        else
            m_shield[2] -= kinetic;
    }
}


bool Player::update(float elapsedTime)
{
    updateOrientation(elapsedTime);
    updateVelocity(elapsedTime);

    Vector3D pos = m_scenario->position();

    if (m_engineState == EngineStartup && m_engineTime < m_scenario->time())
    {
        m_engineState = EngineRunning;
        sfx::SampleMap::get(sfx::Sample::Engine).playLoop();
    }
    if (m_engineState == EngineShutdown && m_engineTime < m_scenario->time())
        m_engineState = EngineOff;
    if (m_engineState == EngineRunning)
    {
        float pitch = glm::length(m_velocity)*0.036f;
        if (pitch > 1.0f)
            pitch = 1.0f;
        if (pitch < -0.5f)
            pitch = -0.5f;
        sfx::SampleMap::get(sfx::Sample::Engine).setPitch(1.0f + pitch);
    }

    m_gun.update(elapsedTime);
    if (m_firing && m_gun.state() == Gun::StateReady)
        m_gun.fire(pos, dir(), up(), right());
    if (m_tur1)
    {
        m_tur1->update(elapsedTime);
        if (m_firing && m_tur1->state() == Gun::StateReady)
            m_tur1->fire(pos, dir(), up(), right());
    }
    if (m_tur2)
    {
        m_tur2->update(elapsedTime);
        if (m_firing && m_tur2->state() == Gun::StateReady)
            m_tur2->fire(pos, dir(), up(), right());
    }

    return false;
}


void Player::engineToggle()
{
    if (m_engineState == EngineOff)
    {
        m_engineState = EngineStartup;
        m_engineTime = m_scenario->time() + 1;
        sfx::SampleMap::get(sfx::Sample::Engine_Startup).play();
    }
    if (m_engineState == EngineRunning)
    {
        m_engineState = EngineShutdown;
        m_engineTime = m_scenario->time() + 1;
        sfx::SampleMap::get(sfx::Sample::Engine_Shutdown).play();
        sfx::SampleMap::get(sfx::Sample::Engine).stop();
    }
}


void Player::fire()
{
    m_firing = true;
}


void Player::fireStop()
{
    m_firing = false;
}


} // namespace fight
