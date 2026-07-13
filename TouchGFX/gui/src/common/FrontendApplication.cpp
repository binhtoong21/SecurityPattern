#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/transitions/NoTransition.hpp>
#include <gui/common/FrontendHeap.hpp>
#include <gui/registerscreen_screen/RegisterScreenView.hpp>
#include <gui/registerscreen_screen/RegisterScreenPresenter.hpp>
#include <gui/confirmscreen_screen/ConfirmScreenView.hpp>
#include <gui/confirmscreen_screen/ConfirmScreenPresenter.hpp>
#include <gui/unlockedscreen_screen/UnlockedScreenView.hpp>
#include <gui/unlockedscreen_screen/UnlockedScreenPresenter.hpp>

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : FrontendApplicationBase(m, heap)
{

}

void FrontendApplication::gotoRegisterScreenScreenNoTransition()
{
    transitionCallbackUser = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoRegisterScreenScreenNoTransitionImpl);
    pendingScreenTransitionCallback = &transitionCallbackUser;
}

void FrontendApplication::gotoRegisterScreenScreenNoTransitionImpl()
{
    touchgfx::makeTransition<RegisterScreenView, RegisterScreenPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoConfirmScreenScreenNoTransition()
{
    transitionCallbackUser = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoConfirmScreenScreenNoTransitionImpl);
    pendingScreenTransitionCallback = &transitionCallbackUser;
}

void FrontendApplication::gotoConfirmScreenScreenNoTransitionImpl()
{
    touchgfx::makeTransition<ConfirmScreenView, ConfirmScreenPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoUnlockedScreenScreenNoTransition()
{
    transitionCallbackUser = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoUnlockedScreenScreenNoTransitionImpl);
    pendingScreenTransitionCallback = &transitionCallbackUser;
}

void FrontendApplication::gotoUnlockedScreenScreenNoTransitionImpl()
{
    touchgfx::makeTransition<UnlockedScreenView, UnlockedScreenPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}
