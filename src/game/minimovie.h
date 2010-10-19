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


#ifndef GAME_MINIMOVIE_H
#define GAME_MINIMOVIE_H


#include "gfx/texture.h"
#include "gfx/video.h"
#include "txt/desfile.h"


namespace game {


class MiniMovie
{
public:
    MiniMovie(const QString &path);
    ~MiniMovie();

public:
    void load(txt::DesFile &file);
    void start();
    void update(gfx::Texture texture);
    void setColorTable(const gfx::ColorTable &colorTable);

private:
    struct Video
    {
        gfx::Video video;
        int x;
        int y;
        int rndMax;
        int time;
    };

    QString m_path;
    QList<Video*> m_videos;
    QTime m_time;
    gfx::ColorTable m_colorTable;
};


} // namespace game


#endif // GAME_MINIMOVIE_H
