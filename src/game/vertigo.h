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

#ifndef GAME_VERTIGO_H
#define GAME_VERTIGO_H


#include <QStringList>


namespace game {


class Desktop;
class MainMenu;
class Movie;
class Window;


class Vertigo : public QObject
{
    Q_OBJECT

public:
    Vertigo();
    ~Vertigo();

public:
    bool start();

private slots:
    void update();
    void startGame();
    void movieFinished();

private:
    void playMovie(const QString &filename);

private:
    Window *m_window;
    MainMenu *m_mainMenu;
    Movie *m_movie;
    Desktop *m_desktop;
    QStringList m_movies;
};


} // namespace game


#endif // GAME_VERTIGO_H
