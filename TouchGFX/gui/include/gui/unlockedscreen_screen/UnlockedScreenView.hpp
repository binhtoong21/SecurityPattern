#ifndef UNLOCKEDSCREENVIEW_HPP
#define UNLOCKEDSCREENVIEW_HPP

#include <gui_generated/unlockedscreen_screen/UnlockedScreenViewBase.hpp>
#include <gui/unlockedscreen_screen/UnlockedScreenPresenter.hpp>

class UnlockedScreenView : public UnlockedScreenViewBase
{
public:
    UnlockedScreenView();
    virtual ~UnlockedScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // UNLOCKEDSCREENVIEW_HPP
