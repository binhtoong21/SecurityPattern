// --- File: src/gui/include/gui/confirmscreen_screen/ConfirmScreenView.hpp ---
#ifndef CONFIRMSCREENVIEW_HPP
#define CONFIRMSCREENVIEW_HPP

#include <gui_generated/confirmscreen_screen/ConfirmScreenViewBase.hpp>
#include <gui/confirmscreen_screen/ConfirmScreenPresenter.hpp>

#include <touchgfx/widgets/canvas/Line.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>

class ConfirmScreenView : public ConfirmScreenViewBase
{
public:
    ConfirmScreenView();
    virtual ~ConfirmScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    
    virtual void handleClickEvent(const touchgfx::ClickEvent& evt);
    virtual void handleDragEvent(const touchgfx::DragEvent& evt);

    void showError();

protected:
    uint8_t currentPattern[9];
    uint8_t patternLength;
    
    touchgfx::Line patternLines[8];
    touchgfx::Line activeLine;
    touchgfx::PainterRGB565 linePainter;

    void checkDotTouch(int16_t x, int16_t y);
    void resetPattern();
    touchgfx::Widget* getDot(uint8_t index);
};

#endif // CONFIRMSCREENVIEW_HPP
