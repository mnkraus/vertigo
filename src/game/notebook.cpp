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

#include "notebook.h"

#include "chapter.h"

#include "gfx/colortable.h"
#include "gfx/image.h"

namespace game {

Notebook::Notebook(std::function<void()> funcClose) :
    m_lblBackground(this),
    m_lblMain(&m_lblBackground),
    m_lblMissions(&m_lblBackground, false),
    m_lblOptions(&m_lblBackground, false),
    m_lblMovies(&m_lblBackground, false),
    m_lblMoviePlayer(&m_lblBackground, false),
    m_lblMap(&m_lblBackground, false),
    m_btnMoviesAutopilot([this]() { Chapter::get()->toggleMovieAutopilot(); updateMovies(); }, &m_lblMovies),
    m_btnMoviesApproach([this]() { Chapter::get()->toggleMovieApproach(); updateMovies(); }, &m_lblMovies),
    m_btnMoviesHarbour([this]() { Chapter::get()->toggleMovieHarbour(); updateMovies(); }, &m_lblMovies),
    m_btnMoviePlayerHide([this]() { hideMoviePlayer(); }, &m_lblMoviePlayer),
    m_btnMissionsBack([this]() { hideMissions(); }, &m_lblMissions),
    m_listMissions(&m_lblMissions),
    m_funcClose(std::move(funcClose))
{
    const gfx::ColorTable colorTable("gfx:pal/notebook/notebook.pal");
    m_fontGreen.load("gfx:fnt/nfont1a.fnt", colorTable, true);
    m_fontYellow.load("gfx:fnt/nfont1b.fnt", colorTable, true);
    m_background.fromImage(gfx::Image::load("gfx:img/desktop/notebook/noteback.img", colorTable));

    QImage image(640, 480, QImage::Format_Indexed8);
    image.fill(0);
    image.setColorTable(QVector<QRgb>() << qRgba(0, 0, 0, 128));
    setTexture(image);

    m_lblBackground.setPosition(48, 48);
    m_lblBackground.setTexture(gfx::Image::loadPCX("gfx:pic/notebook/notebook.pcx"));

    {
        m_lblMain.setPosition(162, 73);
        m_lblMain.setTexture(m_background);

        createLabel(&m_lblMain, txt::Notebook_Title, 30);
        createLabel(&m_lblMain, txt::Notebook_TitleLine, 40);

        createButton([this]() { showMissions(); }, &m_lblMain, txt::Notebook_Missions, 90);
        createButton([]() {}, &m_lblMain, txt::Notebook_LoadSave, 110);
        createButton([this]() { showOptions(); }, &m_lblMain, txt::Notebook_Options, 130);
        createButton([this]() { showMap(); }, &m_lblMain, txt::Notebook_Map, 150);
        createButton(m_funcClose, &m_lblMain, txt::Notebook_Back, 170);
        createButton([]() { Chapter::get()->quit(); }, &m_lblMain, txt::Notebook_QuitGame, 210);
    }

    {
        m_lblOptions.setPosition(162, 73);
        m_lblOptions.setTexture(m_background);

        createLabel(&m_lblOptions, txt::Notebook_Options_Title, 30);
        createLabel(&m_lblOptions, txt::Notebook_Options_TitleLine, 40);

        createButton([]() {}, &m_lblOptions, txt::Notebook_Sound, 90);
        createButton([]() {}, &m_lblOptions, txt::Notebook_Graphics, 110);
        createButton([this]() { showMovies(); }, &m_lblOptions, txt::Notebook_Movies, 130);
        createButton([]() {}, &m_lblOptions, txt::Notebook_InputDevices, 150);
        createButton([this]() { showMoviePlayer(); }, &m_lblOptions, txt::Notebook_MoviePlayer, 170);
        createButton([this]() { hideOptions(); }, &m_lblOptions, txt::Notebook_Back, 220);
    }

    {
        m_lblMovies.setPosition(162, 73);
        m_lblMovies.setTexture(m_background);

        createLabel(&m_lblMovies, txt::Notebook_Movies_Title, 30);
        createLabel(&m_lblMovies, txt::Notebook_Movies_TitleLine, 40);

        setupButton(m_btnMoviesAutopilot, txt::Notebook_Movies_Autopilot_Yes, 90);
        setupButton(m_btnMoviesApproach, txt::Notebook_Movies_Approach_Yes, 110);
        setupButton(m_btnMoviesHarbour, txt::Notebook_Movies_Harbour_Yes, 130);

        createButton([this]() { hideMovies(); }, &m_lblMovies, txt::Notebook_Back, 170);

        updateMovies();
    }

    {
        m_lblMap.setPosition(164, 75);
        m_lblMap.setTexture(gfx::Image::load(QString("gfx:pic/notebook/%1.r16").arg(Chapter::get()->area()->map()), 304, 284));
    }

    {
        m_lblMoviePlayer.setPosition(162, 73);
        m_lblMoviePlayer.setTexture(m_background);

        createLabel(&m_lblMoviePlayer, txt::Notebook_MoviePlayer_Title, 10);
        createLabel(&m_lblMoviePlayer, txt::Notebook_MoviePlayer_TitleLine, 20);

        setupButton(m_btnMoviePlayerHide, txt::Notebook_Back, 30);
    }

    {
        m_lblMissions.setPosition(162, 73);
        m_lblMissions.setTexture(m_background);

        createLabel(&m_lblMissions, txt::Notebook_Missions_Title, 10);
        createLabel(&m_lblMissions, txt::Notebook_Missions_TitleLine, 20);

        m_listMissions.setFont(m_fontYellow);
        m_listMissions.setPosition(0, 50);
        m_listMissions.setSize(304, 220);

        m_btnMissionsBack.setFont(m_fontGreen);
        m_btnMissionsBack.setText(txt::StringTable::get(txt::Notebook_Back));
        m_btnMissionsBack.setPosition(180, 270);
    }
}

void Notebook::createLabel(ui::Widget *parent, txt::String text, float posY)
{
    m_labels.emplace_back(parent);
    setupLabel(m_labels.back(), text, posY);
}

void Notebook::createButton(std::function<void()> funcClick, ui::Widget *parent, txt::String text, float posY)
{
    m_buttons.emplace_back(funcClick, parent);
    setupButton(m_buttons.back(), text, posY);
}

void Notebook::setupLabel(ui::Label &label, txt::String text, float posY)
{
    label.setFont(m_fontYellow);
    label.setText(txt::StringTable::get(text));
    label.setPosition(0, posY);
    label.setWidth(304);
    label.setAlignment(ui::Label::AlignHCenter);
}

void Notebook::setupButton(ui::Button &button, txt::String text, float posY)
{
    setupLabel(button, text, posY);
    button.setFont(m_fontGreen);
}

void Notebook::showMissions()
{
    m_lblMain.hide();
    m_lblMissions.show();

    QStringList tasks;
    foreach (const Task &task, Chapter::get()->tasks()) {
        switch (task.type()) {
        case Task::Tip:
            tasks.append(txt::StringTable::get(txt::Notebook_Tip));
            break;

        case Task::Job:
            tasks.append(txt::StringTable::get(txt::Notebook_Job));
            break;

        case Task::Mission:
            tasks.append(txt::StringTable::get(txt::Notebook_Mission));
            break;
        }

        tasks.append(task.text());
        tasks.append("");
    }

    m_listMissions.setText(tasks);
}

void Notebook::hideMissions()
{
    m_lblMissions.hide();
    m_lblMain.show();
}

void Notebook::showOptions()
{
    m_lblMain.hide();
    m_lblOptions.show();
}

void Notebook::hideOptions()
{
    m_lblOptions.hide();
    m_lblMain.show();
}

void Notebook::showMovies()
{
    m_lblOptions.hide();
    m_lblMovies.show();
}

void Notebook::hideMovies()
{
    m_lblMovies.hide();
    m_lblOptions.show();
}

void Notebook::showMoviePlayer()
{
    m_lblOptions.hide();
    m_lblMoviePlayer.show();

    m_movieButtons.clear();
    const QSet<int>& movies = Chapter::get()->playedMovies();
    int y = 40;
    foreach (int movie, movies) {
        m_movieButtons.emplace_back([this, movie]() { Chapter::get()->playMovie(movie); }, &m_lblMoviePlayer);
        setupButton(m_movieButtons.back(), txt::String(txt::FirstMovie + movie), y);
        y += 10;
    }
    m_btnMoviePlayerHide.setPosition(0, y + 10);
}

void Notebook::hideMoviePlayer()
{
    m_lblMoviePlayer.hide();
    m_lblOptions.show();
}

void Notebook::showMap()
{
    m_lblMain.hide();
    m_lblMap.show();
}

void Notebook::hideMap()
{
    m_lblMap.hide();
    m_lblMain.show();
}

void Notebook::updateMovies()
{
    txt::String autopilot = txt::Notebook_Movies_Autopilot_No;
    if (Chapter::get()->movieAutopilot())
        autopilot = txt::Notebook_Movies_Autopilot_Yes;
    m_btnMoviesAutopilot.setText(txt::StringTable::get(autopilot));

    txt::String approach = txt::Notebook_Movies_Approach_No;
    if (Chapter::get()->movieApproach())
        approach = txt::Notebook_Movies_Approach_Yes;
    m_btnMoviesApproach.setText(txt::StringTable::get(approach));

    txt::String harbour = txt::Notebook_Movies_Harbour_No;
    if (Chapter::get()->movieHarbour())
        harbour = txt::Notebook_Movies_Harbour_Yes;
    m_btnMoviesHarbour.setText(txt::StringTable::get(harbour));
}

bool Notebook::mousePressEvent(const QPoint &pos, Qt::MouseButton button)
{
    if (m_lblMap.isVisible()) {
        hideMap();
        return true;
    }

    if (button == Qt::RightButton) {
        if (m_lblMissions.isVisible()) {
            hideMissions();
            return true;
        }
        if (m_lblOptions.isVisible()) {
            hideOptions();
            return true;
        }
        if (m_lblMovies.isVisible()) {
            hideMovies();
            return true;
        }
        if (m_lblMoviePlayer.isVisible()) {
            hideMoviePlayer();
            return true;
        }
        if (m_lblMain.isVisible()) {
            m_funcClose();
            return true;
        }
    }

    return false;
}

} // namespace game
