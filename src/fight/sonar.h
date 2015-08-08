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

#ifndef SONAR_H
#define SONAR_H


#include "objects/activeobject.h"


namespace fight {


class Scenario;
class Object;


class Sonar
{
public:
    Sonar(Scenario *scenario);

public:
    struct SonarEntry
    {
        ActiveObject *object;
        glm::vec3 dir;
        bool isFriend;
        bool isPassive;
    };

public:
    void init(int sensor);
    void activate();
    void deactivate();
    void toggle();
    const bool isActive() { return m_active; }
    const bool isActivating() { return m_activatingDelay > 0; }
    void update(float elapsedTime);

private:
    Scenario *m_scenario;
    float m_delay;
    float m_passiveRange;
    float m_passiveRangeMult;
    float m_activeRange;
    float m_activeRangeMult;
    bool m_iff;

    bool m_active;
    float m_activatingDelay;
    std::vector<SonarEntry> m_sonarEntries;
};


}


#endif // SONAR_H
