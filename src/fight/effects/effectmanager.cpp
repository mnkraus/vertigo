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
#include "projectile.h"
#include "txt/desfile.h"
#include <QGLContext>


namespace fight {


EffectManager::EffectManager(gfx::TextureManager &texMan)
{
    txt::DesFile file;
    
    file.load("vfx:sobjects/explosio.des");
    for (int i = 0; i < 27; i++)
    {
        Billboard *billboard = new Billboard(texMan, file, i);
        m_billboards.insert((Effects)(Explosion_0 + i), billboard);
    }
    file.load("vfx:sobjects/shoot.des");
    for (int i = 0; i < 9; i++)
    {
        Billboard *billboard = new Billboard(texMan, file, i);
        m_billboards.insert((Effects)(Shoot_0 + i), billboard);
    }
    file.load("vfx:sobjects/debris.des");
    for (int i = 0; i < 23; i++)
    {
        Billboard *billboard = new Billboard(texMan, file, i);
        m_billboards.insert((Effects)(Debris_0 + i), billboard);
    }
    file.load("vfx:sobjects/trash.des");
    for (int i = 0; i < 5; i++)
    {
        Billboard *billboard = new Billboard(texMan, file, i);
        m_billboards.insert((Effects)(Trash_0 + i), billboard);
    }
    file.load("vfx:sobjects/bubble.des");
    for (int i = 0; i < 3; i++)
    {
        Billboard *billboard = new Billboard(texMan, file, i);
        m_billboards.insert((Effects)(Bubble_0 + i), billboard);
    }
}


EffectManager::~EffectManager()
{
    qDeleteAll(m_effects);
    qDeleteAll(m_billboards.values());
}


Effect* EffectManager::create(Effects effect)
{
    return new Effect(getBillboard(effect), 0);
}


void EffectManager::addEffect(Effects effect, const QVector3D &position)
{
    Effect *object = create(effect);
    object->setPosition(position);
    m_effects << object;
}


void EffectManager::addProjectile(Effects effect, const QVector3D &position, const QVector3D &direction)
{
    Projectile *object = new Projectile(getBillboard(effect));
    object->setPosition(position);
    object->setDirection(direction);
    m_effects << object;
}


void EffectManager::draw()
{
    for (int i = 0; i < m_effects.count(); i++)
    {
        if (m_effects[i]->atEnd())
        {
            delete m_effects[i];
            m_effects.removeAt(i);
            i--;
            continue;
        }
        m_effects[i]->draw();
    }
}


} // namespace fight
