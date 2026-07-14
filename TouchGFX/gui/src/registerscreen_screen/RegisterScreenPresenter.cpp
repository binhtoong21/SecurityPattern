	#include <gui/registerscreen_screen/RegisterScreenView.hpp>
#include <gui/registerscreen_screen/RegisterScreenPresenter.hpp>

RegisterScreenPresenter::RegisterScreenPresenter(RegisterScreenView& v)
    : view(v)
{
}

void RegisterScreenPresenter::activate()
{
}

void RegisterScreenPresenter::deactivate()
{
}

void RegisterScreenPresenter::processPattern(uint8_t* pattern, uint8_t length)
{
    model->verifyPattern(pattern, length);
}

void RegisterScreenPresenter::showError()
{
    view.showError();
}
