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

#ifndef GAME_BOAT_H
#define GAME_BOAT_H


#include <QString>
#include <QPoint>
#include <QList>


namespace game {


class Boat
{
public:
    Boat(int type);
    ~Boat();

public:
    struct Mounting
    {
        QString name;
        int side;
        int type;
        QPoint pos;
        QString dir;
    };

public:
    int type() const { return m_type; }
    const QString& name() const { return m_name; }
    const QString& cockpit() const { return m_cockpit; }
    const QString& flipMovie1() const { return m_flipMovie1; }
    const QString& flipMovie2() const { return m_flipMovie2; }

    const QList<Mounting*>& mountings() const { return m_mountings; }

    QList<int> getItems(const QString& mounting);
    void setItems(const QString& mounting, const QList<int> &items);
    bool canBuy(int model, const QString& mounting, int *oldModel);
    bool canSell(int model, const QString& mounting);
    bool isCompatible(int model);
    void buy(int model, const QString& mounting);
    void sell(int model, int index, const QString& mounting);

    const QList<int>& torpedoes() const { return m_torpedoes; }
    int magazine() const { return m_magazine; }
    int gun() const { return m_gun; }
    int engine() const { return m_engine; }
    int booster() const { return m_booster; }
    int silator() const { return m_silator; }
    int armor() const { return m_armor; }
    int nrskin() const { return m_nrskin; }
    int sensor() const { return m_sensor; }
    int tur1() const { return m_tur1; }
    int tur1soft() const { return m_tur1soft; }
    int tur2() const { return m_tur2; }
    int tur2soft() const { return m_tur2soft; }
    const QList<int>& buzzers() const { return m_buzzers; }
    int fixer() const { return m_fixer; }

private:
    void addMounting(const QString &name, int side, int type, int x, int y, const QString &dir);
    int getCapacity(int magazine);

private:
    int m_type;
    QString m_name;
    QString m_cockpit;
    QString m_flipMovie1;
    QString m_flipMovie2;

    QList<Mounting*> m_mountings;
    QList<int> m_compatibility;

    QList<int> m_torpedoes;
    int m_magazine;
    int m_gun;
    int m_engine;
    int m_booster;
    int m_silator;
    int m_armor;
    int m_nrskin;
    int m_sensor;
    int m_tur1;
    int m_tur1soft;
    int m_tur2;
    int m_tur2soft;
    QList<int> m_buzzers;
    int m_fixer;
};


} // namespace game


#endif // GAME_BOAT_H
