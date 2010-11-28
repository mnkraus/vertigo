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

#include "mine.h"


namespace fight {


Mine::Mine(Scenario *scenario, const QString &name) :
    Object(scenario, name)
{
    txt::DesFile file(QString("vfx:sobjects/%1.des").arg(name));
    file.setSection("cluster");
    m_base = scenario->moduleManager().get(file.value("name").toString());

    file.setSection("size");
    m_scale = file.value("scale").toFloat() / 16;
}


} // namespace fight
