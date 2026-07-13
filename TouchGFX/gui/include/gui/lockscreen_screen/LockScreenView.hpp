// --- File: src/gui/include/gui/lockscreen_screen/LockScreenView.hpp ---
#ifndef LOCKSCREENVIEW_HPP
#define LOCKSCREENVIEW_HPP

#include <gui_generated/lockscreen_screen/LockScreenViewBase.hpp>
#include <gui/lockscreen_screen/LockScreenPresenter.hpp>

#include <touchgfx/widgets/canvas/Line.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>

class LockScreenView : public LockScreenViewBase
{
public:
    LockScreenView();
    virtual ~LockScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    
    // Ghi đè sự kiện cảm ứng
    virtual void handleClickEvent(const touchgfx::ClickEvent& evt);
    virtual void handleDragEvent(const touchgfx::DragEvent& evt);
    
    // Ghi đè sự kiện bàn phím để test Simulator
    virtual void handleKeyEvent(uint8_t key);
    
    virtual void handleTickEvent();

    void showError();
    void showLockout();
    void hideLockout();

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

#endif // LOCKSCREENVIEW_HPP
