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

#include "boat.h"
#include "items.h"
#include "txt/desfile.h"


namespace game {


Boat::Boat(int id)
{
	txt::DesFile file;

    if (id == 0)
    {
        file.load("vfx:sobjects/hiob.des");
        m_flipMovie1 = "hiob1.mvi";
        m_flipMovie2 = "hiob2.mvi";

		addMounting("DEFE", 1, 280, 173, "TopLeft");
		addMounting("GENE", 1, 80, 132, "BottomRight");
		addMounting("GUN", 2, 324, 188, "BottomLeft");
		addMounting("TORP", 2, 276, 129, "TopLeft");
    }

    file.setSection("Ship");
    m_name = file.value("Name").toString();
    file.setSection("Cockpit");
    m_cockpit = file.value("Name").toString();

	file.setSection("Compatibility");
    for (int comp = 0; file.contains(QString("Comp%1").arg(comp)); comp++)
    {
        QString str = file.value(QString("Comp%1").arg(comp)).toString();
        str.truncate(str.indexOf(' '));
		int model = str.toInt();
		if (model != 99999)
			m_compatibility << model;
	}
}

Boat::~Boat()
{
	qDeleteAll(m_mountings);
}

void Boat::addMounting(const QString &name, int side, int x, int y, const QString &dir)
{
	Mounting *mounting = new Mounting();
	mounting->name = name;
	mounting->side = side;
	mounting->pos = QPoint(x, y);
	mounting->dir = dir;
	m_mountings << mounting;
}

int Boat::getCapacity(int magazine)
{
	if (magazine == 1025)
		return 4;
	if (magazine == 1026)
		return 6;
	if (magazine == 1027)
		return 8;
	if (magazine == 1028)
		return 10;
	if (magazine == 1029)
		return 6;
	if (magazine == 1030)
		return 8;
	if (magazine == 1031)
		return 10;
	if (magazine == 1032)
		return 12;
	return 0;
}


QList<int> Boat::getItems(const QString& mounting)
{
	QList<int> list;
	if (mounting == "DEFE")
	{
		list << m_armor;
		if (m_nrskin)
			list << m_nrskin;
		list << m_sensor;
		foreach (int buzz, m_buzzers)
			list << buzz;
		if (m_fixer)
			list << m_fixer;
	}
	if (mounting == "GENE")
	{
		list << m_engine;
		if (m_booster)
			list << m_booster;
		if (m_silator)
			list << m_silator;
	}
	if (mounting == "GUN")
	{
		list << m_gun;
	}
	if (mounting == "TORP")
	{
		list << m_magazine;
		foreach (int torp, m_torpedoes)
			list << torp;
	}
	if (mounting == "TUR1")
	{
		if (m_tur1)
			list << m_tur1;
		if (m_tur1soft)
			list << m_tur1soft;
	}
	if (mounting == "TUR2")
	{
		if (m_tur2)
			list << m_tur2;
		if (m_tur2soft)
			list << m_tur2soft;
	}
	return list;
}


bool Boat::canBuy(int model, const QString& mounting, int *oldModel)
{
	Items::Item *item = Items::get()->get(model);
	*oldModel = -1;
	if (mounting == "DEFE")
	{
		if (item->type == Items::Armor)
			*oldModel = m_armor;
		if (item->type == Items::NRSkin)
			*oldModel = m_nrskin;
		if (item->type == Items::Sensor)
			*oldModel = m_sensor;
		if (item->type == Items::Buzzer && m_buzzers.count() < 8)
			return true;
		if (item->type == Items::Fixer)
			*oldModel = m_fixer;
	}
	if (mounting == "GENE")
	{
		if (item->type == Items::Engine)
			*oldModel = m_engine;
		if (item->type == Items::Booster)
			*oldModel = m_booster;
		if (item->type == Items::Silator)
			*oldModel = m_silator;
	}
	if (mounting == "GUN")
	{
		if (item->type == Items::Gun)
			*oldModel = m_gun;
	}
	if (mounting == "TORP")
	{
		if (item->type == Items::Magazine)
		{
			*oldModel = m_magazine;
			if (getCapacity(model) < m_torpedoes.count())
				return false;
		}
		if (item->type == Items::Torpedo)
		{
			if (m_torpedoes.count() < getCapacity(m_magazine))
				return true;
		}
	}
	if (mounting == "TUR1")
	{
		if (item->type == Items::Gun)
			*oldModel = m_tur1;
		if (item->type == Items::Software)
			*oldModel = m_tur1soft;
	}
	if (mounting == "TUR2")
	{
		if (item->type == Items::Gun)
			*oldModel = m_tur2;
		if (item->type == Items::Software)
			*oldModel = m_tur2soft;
	}
	if (*oldModel == 0)
		return true;
	if (*oldModel > 0 && *oldModel != model)
		return true;
	return false;
}


bool Boat::canSell(int model, const QString& mounting)
{
	Items::Item *item = Items::get()->get(model);
	if (mounting == "DEFE")
	{
		if (item->type == Items::NRSkin)
			return true;
		if (item->type == Items::Buzzer)
			return true;
		if (item->type == Items::Fixer)
			return true;
	}
	if (mounting == "GENE")
	{
		if (item->type == Items::Booster)
			return true;
		if (item->type == Items::Silator)
			return true;
	}
	if (mounting == "GUN")
	{
	}
	if (mounting == "TORP")
	{
		if (item->type == Items::Torpedo)
			return true;
	}
	if (mounting == "TUR1")
	{
		if (item->type == Items::Gun)
			return true;
		if (item->type == Items::Software)
			return true;
	}
	if (mounting == "TUR2")
	{
		if (item->type == Items::Gun)
			return true;
		if (item->type == Items::Software)
			return true;
	}
	return false;
}


bool Boat::isCompatible(int model)
{
	return m_compatibility.indexOf(model) >= 0;
}


} // namespace game
