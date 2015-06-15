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

#ifndef UI_FRAME_H
#define UI_FRAME_H

#include "button.h"

#include "gfx/colortable.h"
#include "gfx/texture.h"

#include <functional>

namespace ui {

class Frame : public ui::Label
{
public:
    Frame(std::function<void()> &&funcClose, ui::Widget *parent = NULL);

public:
    void setupFrame(const QSize &size, const QString &title, bool closable);

protected:
    void closeFrame();

private:
    static int updateBorder(gfx::Texture texture, const gfx::ColorTable &colorTable, int x, int id);
    static QImage getBorder(const gfx::ColorTable &colorTable, int id);

private:
    std::function<void()> m_funcClose;
    ui::Label m_lblTitle;
    ui::Button m_btnClose;
};

} // namespace ui

#endif // UI_FRAME_H
