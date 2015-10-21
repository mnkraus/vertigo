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

#ifndef FIGHT_PLAYER_H
#define FIGHT_PLAYER_H


#include "simpleboat.h"
#include "fight/weapons/gun.h"
#include <memory>


namespace game { class Boat; }


namespace fight {


class Player : public SimpleBoat
{
public:
    enum EngineState { EngineOff, EngineStartup, EngineRunning, EngineShutdown };

public:
    Player(Scenario *scenario, game::Boat *boat);

public:
    int shield(int index) const { return m_shield[index]; };
    int shieldMax(int index) const { return m_shieldMax[index]; };
    bool engineRunning() const { return m_engineState == EngineRunning; }
    float engineThrottle() const { return m_engineThrottle; }
    float noise() const;
    float range() const;

public:
    virtual void damage(int kinetic, int shock, const Vector3D &position);
    bool update(float elapsedTime);

    void engineToggle();
    void setEngineThrottle(float throttle);

    void fire();
    void fireStop();

private:
    int m_shield[4];
    int m_shieldMax[4];

    EngineState m_engineState;
    float m_engineTime;
    float m_engineThrottle;
    float m_engineThrottleTarget;

    bool m_firing;
    Gun m_gun;
    std::unique_ptr<Gun> m_tur1;
    std::unique_ptr<Gun> m_tur2;
};


} // namespace fight


#endif // FIGHT_PLAYER_H
