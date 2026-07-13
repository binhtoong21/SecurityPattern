// --- File: src/gui/src/lockscreen_screen/LockScreenPresenter.cpp ---
#include <gui/lockscreen_screen/LockScreenView.hpp>
#include <gui/lockscreen_screen/LockScreenPresenter.hpp>

LockScreenPresenter::LockScreenPresenter(LockScreenView& v)
    : view(v)
{
}

void LockScreenPresenter::activate()
{
}

void LockScreenPresenter::deactivate()
{
}

void LockScreenPresenter::processPattern(uint8_t* pattern, uint8_t length)
{
    model->verifyPattern(pattern, length);
}

void LockScreenPresenter::triggerRegister()
{
    model->triggerRegister();
}

void LockScreenPresenter::showError()
{
    view.showError();
}

void LockScreenPresenter::showLockout()
{
    view.showLockout();
}

void LockScreenPresenter::hideLockout()
{
    view.hideLockout();
}
