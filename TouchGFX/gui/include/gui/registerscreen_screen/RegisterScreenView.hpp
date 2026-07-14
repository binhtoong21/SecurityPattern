// --- File: src/gui/include/gui/registerscreen_screen/RegisterScreenView.hpp ---
#ifndef REGISTERSCREENVIEW_HPP
#define REGISTERSCREENVIEW_HPP

#include <gui_generated/registerscreen_screen/RegisterScreenViewBase.hpp>
#include <gui/registerscreen_screen/RegisterScreenPresenter.hpp>

#include <touchgfx/widgets/canvas/Line.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>

class RegisterScreenView : public RegisterScreenViewBase
{
public:
    RegisterScreenView();
    virtual ~RegisterScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    
    virtual void handleClickEvent(const touchgfx::ClickEvent& evt);
    virtual void handleDragEvent(const touchgfx::DragEvent& evt);
    virtual void handleTickEvent();

    void showError();

protected:
    uint8_t currentPattern[9];
    uint8_t patternLength;
    
    uint16_t errorTickCounter;
    bool isErrorState;
    
    touchgfx::Line patternLines[8];
    touchgfx::Line activeLine;
    touchgfx::PainterRGB565 linePainter;

    void checkDotTouch(int16_t x, int16_t y);
    void resetPattern();
    touchgfx::Widget* getDot(uint8_t index);
};

#endif // REGISTERSCREENVIEW_HPP
