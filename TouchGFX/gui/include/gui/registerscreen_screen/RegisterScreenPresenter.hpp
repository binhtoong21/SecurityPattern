#ifndef REGISTERSCREENPRESENTER_HPP
#define REGISTERSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class RegisterScreenView;

class RegisterScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    RegisterScreenPresenter(RegisterScreenView& v);
    virtual void activate();
    virtual void deactivate();
    virtual ~RegisterScreenPresenter() {}

    void processPattern(uint8_t* pattern, uint8_t length);
    virtual void showError();

private:
    RegisterScreenPresenter();
    RegisterScreenView& view;
};

#endif // REGISTERSCREENPRESENTER_HPP
