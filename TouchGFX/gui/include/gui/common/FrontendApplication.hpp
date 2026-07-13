#ifndef FRONTENDAPPLICATION_HPP
#define FRONTENDAPPLICATION_HPP

#include <gui_generated/common/FrontendApplicationBase.hpp>

class FrontendHeap;

using namespace touchgfx;

class FrontendApplication : public FrontendApplicationBase
{
public:
    FrontendApplication(Model& m, FrontendHeap& heap);
    virtual ~FrontendApplication() { }

    virtual void handleTickEvent()
    {
        model.tick();
        FrontendApplicationBase::handleTickEvent();
    }

    // RegisterScreen
    void gotoRegisterScreenScreenNoTransition();
    void gotoRegisterScreenScreenNoTransitionImpl();

    // ConfirmScreen
    void gotoConfirmScreenScreenNoTransition();
    void gotoConfirmScreenScreenNoTransitionImpl();

    // UnlockedScreen
    void gotoUnlockedScreenScreenNoTransition();
    void gotoUnlockedScreenScreenNoTransitionImpl();

protected:
    touchgfx::Callback<FrontendApplication> transitionCallbackUser;
private:
};

#endif // FRONTENDAPPLICATION_HPP
