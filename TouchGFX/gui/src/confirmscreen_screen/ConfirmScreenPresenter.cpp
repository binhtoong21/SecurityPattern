#include <gui/confirmscreen_screen/ConfirmScreenView.hpp>
#include <gui/confirmscreen_screen/ConfirmScreenPresenter.hpp>

ConfirmScreenPresenter::ConfirmScreenPresenter(ConfirmScreenView& v)
    : view(v)
{
}

void ConfirmScreenPresenter::activate()
{
}

void ConfirmScreenPresenter::deactivate()
{
}

void ConfirmScreenPresenter::processPattern(uint8_t* pattern, uint8_t length)
{
    model->verifyPattern(pattern, length);
}

void ConfirmScreenPresenter::showError()
{
    view.showError();
}
