#ifndef CONFIRMSCREENPRESENTER_HPP
#define CONFIRMSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ConfirmScreenView;

class ConfirmScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ConfirmScreenPresenter(ConfirmScreenView& v);
    virtual void activate();
    virtual void deactivate();
    virtual ~ConfirmScreenPresenter() {}

    void processPattern(uint8_t* pattern, uint8_t length);
    virtual void showError();

private:
    ConfirmScreenPresenter();
    ConfirmScreenView& view;
};

#endif // CONFIRMSCREENPRESENTER_HPP
