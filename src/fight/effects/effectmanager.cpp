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

#include "effectmanager.h"
#include "billboard.h"
#include "../surface/surface.h"


namespace fight {


EffectManager::EffectManager(Scenario *scenario) :
    m_scenario(scenario)
{
    txt::DesFile file;
    
    file.load("vfx:sobjects/explosio.des");
    for (int i = 0; i < 27; i++)
        m_billboards.emplace(std::piecewise_construct,
            std::forward_as_tuple((Effects)(Explosion_0 + i)),
            std::forward_as_tuple(m_scenario->textureManager(), file, i));
    file.load("vfx:sobjects/shoot.des");
    for (int i = 0; i < 9; i++)
        m_billboards.emplace(std::piecewise_construct,
            std::forward_as_tuple((Effects)(Shoot_0 + i)),
            std::forward_as_tuple(m_scenario->textureManager(), file, i));
    file.load("vfx:sobjects/debris.des");
    for (int i = 0; i < 23; i++)
        m_billboards.emplace(std::piecewise_construct,
            std::forward_as_tuple((Effects)(Debris_0 + i)),
            std::forward_as_tuple(m_scenario->textureManager(), file, i));
    file.load("vfx:sobjects/trash.des");
    for (int i = 0; i < 5; i++)
        m_billboards.emplace(std::piecewise_construct,
            std::forward_as_tuple((Effects)(Trash_0 + i)),
            std::forward_as_tuple(m_scenario->textureManager(), file, i));
    file.load("vfx:sobjects/bubble.des");
    for (int i = 0; i < 3; i++)
        m_billboards.emplace(std::piecewise_construct,
            std::forward_as_tuple((Effects)(Bubble_0 + i)),
            std::forward_as_tuple(m_scenario->textureManager(), file, i));
}


Effect& EffectManager::addEffect(Effects effect, const glm::vec3 &position, float angle, float scale)
{
    m_effects.emplace_back(m_scenario, getBillboard(effect), angle, scale);
    m_effects.back().setPosition(position);
    return m_effects.back();
}


Projectile& EffectManager::addProjectile(Effects projectile, const glm::vec3 &position, const glm::vec3 &direction)
{
    m_projectiles.emplace_back(m_scenario, getBillboard(projectile));
    m_projectiles.back().setPosition(position);
    m_projectiles.back().setDirection(direction);
    return m_projectiles.back();
}


void EffectManager::update()
{
    m_effects.remove_if(updateObject);
    m_projectiles.remove_if(updateObject);
}


void EffectManager::draw()
{
    for (Effect &effect : m_effects)
        effect.draw();
    for (Projectile &projectile : m_projectiles)
        projectile.draw();
}


} // namespace fight
