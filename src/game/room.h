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

#ifndef GAME_ROOM_H
#define GAME_ROOM_H


#include "minimovie.h"
#include "sfx/sound.h"
#include "ui/frame.h"


namespace game {


class Room : public ui::Frame
{
    Q_OBJECT

public:
    Room(const QString &title, const QString &name);

signals:
    void startDialog(const QString &name);
    void showDeparture();
    void hideCursor();

public:
    void restart();

protected:
    void draw();
    bool mousePressEvent(const QPoint &pos, Qt::MouseButton button);

private slots:
    void showDock();

private:
    MiniMovie m_miniMovie;
    QString m_name;
    QImage m_backgroundImage;
    gfx::Texture m_background;
    sfx::Sound m_backgroundSound;
    sfx::Sound m_dockSound;
};


} // namespace game


#endif // GAME_ROOM_H
