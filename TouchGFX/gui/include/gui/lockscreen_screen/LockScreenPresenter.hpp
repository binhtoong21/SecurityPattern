#ifndef LOCKSCREENPRESENTER_HPP
#define LOCKSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class LockScreenView;

class LockScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    LockScreenPresenter(LockScreenView& v);
    virtual void activate();
    virtual void deactivate();
    virtual ~LockScreenPresenter() {}

    void processPattern(uint8_t* pattern, uint8_t length);
    void triggerRegister();
    
    // Bắt sự kiện lỗi từ Model
    virtual void showError();
    virtual void showLockout();
    virtual void hideLockout();

private:
    LockScreenPresenter();
    LockScreenView& view;
};

#endif // LOCKSCREENPRESENTER_HPP
