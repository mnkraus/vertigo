#ifndef GAME_DIALOGFRAME_H
#define GAME_DIALOGFRAME_H


#include "dialog.h"
#include "ui/frame.h"


namespace game {


class DialogFrame : public ui::Frame
{
public:
    DialogFrame(Dialog* dialog, const QString &roomName, std::function<void()> &&funcClose);
    ~DialogFrame();

private:
    Dialog *m_dialog;
};


} // namespace game


#endif // GAME_DIALOGFRAME_H
