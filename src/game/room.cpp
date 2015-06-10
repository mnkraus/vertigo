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

#include "chapter.h"
#include "room.h"
#include "gfx/colortable.h"
#include "gfx/image.h"
#include "ui/button.h"


namespace game {


Room::Room(int index, const QString &title, const QString &name,
    std::function<void()> funcClose,
    std::function<void(Dialog*)> funcStartDialog,
    std::function<void(Dialog*)> funcStartEnCom,
    std::function<void()> funcShowDeparture,
    std::function<void()> funcShowDepot,
    std::function<void()> funcHideCursor) :
    Frame(std::move(funcClose)),
    m_index(index),
    m_miniMovie("gfx:mvi/room", [this]() { miniMovieFinished(); }),
    m_name(name),
    m_dockMan(NULL),
    m_funcStartDialog(std::move(funcStartDialog)),
    m_funcStartEnCom(std::move(funcStartEnCom)),
    m_funcShowDeparture(std::move(funcShowDeparture)),
    m_funcShowDepot(std::move(funcShowDepot)),
    m_funcHideCursor(std::move(funcHideCursor))
{
    txt::DesFile file("dat:world/" + name + ".des");
    file.setSection("Room");

    const QString background = file.value("BackGround").toString();
    const QString backgroundSound = file.value("Sound").toString();
    m_backgroundImage = gfx::Image::loadPCX("gfx:pic/room/" + background + ".pcx").toRgb565();
    m_miniMovie.setColorTable(m_backgroundImage.colorTable());
    m_backgroundSound.load("sfx:snd/room/" + backgroundSound + ".pcm");

    setupFrame(m_backgroundImage.size() + QSize(18, 24), title, true);

    m_backgroundLabel = new ui::Label(this);
    m_backgroundLabel->setPosition(9, 15);
    m_backgroundLabel->setTexture(m_background);

    m_miniMovie.load(file);
    m_miniMovie.start();

    m_backgroundSound.playLoop();

    file.setSection("Dock");
    if (file.contains("Name")) {
        const QString name = file.value("Name").toString();
        const QString sound = file.value("Sound").toString();
        const int rate = file.value("Rate", 0).toInt();
        m_dockSound.load("sfx:snd/room/" + sound + ".pcm", rate);

        ui::Arrow *arrow = new ui::Arrow(file.value("Arrow").toString(), QPoint(file.value("X").toInt(), file.value("Y").toInt()), false, [this](int) { showDock(); }, m_backgroundLabel);
        arrow->setText(name);
    }

    int personId = 1;
    for (;;) {
        file.setSection(QString("Person%1").arg(personId));

        if (!file.contains("Arrow"))
            break;

        Person *person = new Person;

        person->arrow = new ui::Arrow(file.value("Arrow").toString(), QPoint(file.value("X").toInt(), file.value("Y").toInt()), false, [this](int n) { startDialog(n); }, m_backgroundLabel);
        person->arrow->hide();

        m_persons.insert(personId, person);

        personId++;
    }

    file.setSection("DockMan");
    if (!file.contains("Name"))
        file.setSection("WeaponMan");
    if (file.contains("Name"))
    {
        Person *person = new Person;

        person->arrow = new ui::Arrow(file.value("Arrow").toString(), QPoint(file.value("X").toInt(), file.value("Y").toInt()), false, [this](int n) { startDialog(n); }, m_backgroundLabel);
        person->arrow->hide();

        m_persons.insert(personId, person);
        m_dockMan = person;
        m_dockManName = file.value("Name").toString();
    }

    restart();
}


Room::~Room()
{
    qDeleteAll(m_persons);
}


void Room::restart()
{
    m_dockSound.stop();
    m_background.fromImage(m_backgroundImage);
    QList<Dialog*> dialogs = Chapter::get()->dialogs(m_index);

    foreach (Person* person, m_persons)
        person->arrow->hide();

    foreach (Dialog* dialog, dialogs) {
        Person *person = m_persons.value(dialog->person());
        if (!person) {
            foreach (person, m_persons) {
                if (person->arrow->isVisible() == false && person->female == dialog->isFemale())
                    break;
            }
        }

        Q_ASSERT(person);
        person->arrow->show();
        person->arrow->setText(dialog->name());
        person->arrow->setValue(dialog->id());
    }

    if (m_dockMan && !m_dockMan->arrow->isVisible())
    {
        m_dockMan->arrow->show();
        m_dockMan->arrow->setText(m_dockManName);
        m_dockMan->arrow->setValue(0);
    }

    dialogs = Chapter::get()->dialogsEnCom(true);
    if (!dialogs.isEmpty()) {
        m_funcStartEnCom(dialogs.first());
    }
}


void Room::draw()
{
    m_miniMovie.update(m_background);
    ui::Label::draw();
}


bool Room::mousePressEvent(const QPoint &pos, Qt::MouseButton button)
{
    if (!m_backgroundLabel->isEnabled()) {
        m_miniMovie.stopOneshot();
        return true;
    }

    if (button == Qt::RightButton) {
        closeFrame();
        return true;
    }

    return false;
}


void Room::showDock()
{
    m_funcHideCursor();
    m_backgroundLabel->disable();
    m_dockSound.play();
    m_miniMovie.playOneshot();
}


void Room::startDialog(int dialogId)
{
    if (dialogId > 0) {
        m_funcStartDialog(Chapter::get()->dialog(dialogId));
    } else {
        m_funcShowDepot();
    }
}

void Room::miniMovieFinished()
{
    m_funcShowDeparture();
    m_backgroundLabel->enable();
}

} // namespace game
