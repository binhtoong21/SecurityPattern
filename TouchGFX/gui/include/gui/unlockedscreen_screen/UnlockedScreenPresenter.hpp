#ifndef UNLOCKEDSCREENPRESENTER_HPP
#define UNLOCKEDSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class UnlockedScreenView;

class UnlockedScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    UnlockedScreenPresenter(UnlockedScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~UnlockedScreenPresenter() {}

private:
    UnlockedScreenPresenter();

    UnlockedScreenView& view;
};

#endif // UNLOCKEDSCREENPRESENTER_HPP
