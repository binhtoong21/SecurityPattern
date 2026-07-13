#include <gui/lockscreen_screen/LockScreenView.hpp>
#include <touchgfx/Color.hpp>

// Định nghĩa helper macro cho ngắn gọn
#define CHECK_DOT(dot_widget, index) \
    if (dot_widget.getRect().intersect(x, y)) { \
        bool found = false; \
        for (int j = 0; j < patternLength; j++) { \
            if (currentPattern[j] == index) { found = true; break; } \
        } \
        if (!found && patternLength < 9) { \
            currentPattern[patternLength++] = index; \
            dot_widget.setAlpha(255); \
            dot_widget.invalidate(); \
            if (patternLength > 1) { \
                touchgfx::Widget* prevDot = getDot(currentPattern[patternLength - 2]); \
                int startX = prevDot->getX() + prevDot->getWidth() / 2; \
                int startY = prevDot->getY() + prevDot->getHeight() / 2; \
                int endX = dot_widget.getX() + dot_widget.getWidth() / 2; \
                int endY = dot_widget.getY() + dot_widget.getHeight() / 2; \
                patternLines[patternLength - 2].invalidate(); \
                patternLines[patternLength - 2].setStart(startX, startY); \
                patternLines[patternLength - 2].setEnd(endX, endY); \
                patternLines[patternLength - 2].setVisible(true); \
                patternLines[patternLength - 2].invalidate(); \
            } \
        } \
    }

LockScreenView::LockScreenView() : patternLength(0), errorTickCounter(0), isErrorState(false) {}

void LockScreenView::setupScreen()
{
    LockScreenViewBase::setupScreen();
    
    linePainter.setColor(touchgfx::Color::getColorFromRGB(0, 150, 255)); // Màu xanh dương nhạt giống Android
    
    for (int i = 0; i < 8; i++) {
        patternLines[i].setPosition(0, 0, 240, 320);
        patternLines[i].setPainter(linePainter);
        patternLines[i].setLineWidth(6);
        patternLines[i].setLineEndingStyle(touchgfx::Line::ROUND_CAP_ENDING);
        patternLines[i].setVisible(false);
        // Chèn vào phía sau các dot để Line nằm dưới Dot
        insert(&dot0, patternLines[i]); 
    }
    
    activeLine.setPosition(0, 0, 240, 320);
    activeLine.setPainter(linePainter);
    activeLine.setLineWidth(6);
    activeLine.setLineEndingStyle(touchgfx::Line::ROUND_CAP_ENDING);
    activeLine.setVisible(false);
    insert(&dot0, activeLine);

    resetPattern();
}

touchgfx::Widget* LockScreenView::getDot(uint8_t index)
{
    switch(index) {
        case 0: return &dot0;
        case 1: return &dot1;
        case 2: return &dot2;
        case 3: return &dot3;
        case 4: return &dot4;
        case 5: return &dot5;
        case 6: return &dot6;
        case 7: return &dot7;
        case 8: return &dot8;
        default: return &dot0;
    }
}

void LockScreenView::tearDownScreen()
{
    LockScreenViewBase::tearDownScreen();
}

void LockScreenView::resetPattern()
{
    patternLength = 0;
    // Làm mờ đi (alpha = 100) để biết là chưa chọn
    dot0.setAlpha(100); dot0.invalidate();
    dot1.setAlpha(100); dot1.invalidate();
    dot2.setAlpha(100); dot2.invalidate();
    dot3.setAlpha(100); dot3.invalidate();
    dot4.setAlpha(100); dot4.invalidate();
    dot5.setAlpha(100); dot5.invalidate();
    dot6.setAlpha(100); dot6.invalidate();
    dot7.setAlpha(100); dot7.invalidate();
    dot8.setAlpha(100); dot8.invalidate();

    for (int i = 0; i < 8; i++) {
        patternLines[i].invalidate();
        patternLines[i].setVisible(false);
    }
    activeLine.invalidate();
    activeLine.setVisible(false);
    
    isErrorState = false;
    errorTickCounter = 0;
    linePainter.setColor(touchgfx::Color::getColorFromRGB(0, 150, 255));
    
    textError.setVisible(false);
    textError.invalidate();
    textLockout.setVisible(false);
    textLockout.invalidate();
    textArea1.setVisible(true);
    textArea1.invalidate();
}

void LockScreenView::checkDotTouch(int16_t x, int16_t y)
{
    CHECK_DOT(dot0, 0);
    CHECK_DOT(dot1, 1);
    CHECK_DOT(dot2, 2);
    CHECK_DOT(dot3, 3);
    CHECK_DOT(dot4, 4);
    CHECK_DOT(dot5, 5);
    CHECK_DOT(dot6, 6);
    CHECK_DOT(dot7, 7);
    CHECK_DOT(dot8, 8);
}

void LockScreenView::handleClickEvent(const touchgfx::ClickEvent& evt)
{
    LockScreenViewBase::handleClickEvent(evt);

    if (isErrorState) return; // Không cho vẽ tiếp khi đang báo lỗi

    if (evt.getType() == touchgfx::ClickEvent::PRESSED) {
        resetPattern();
        checkDotTouch(evt.getX(), evt.getY());
    }
    else if (evt.getType() == touchgfx::ClickEvent::RELEASED) {
        activeLine.invalidate();
        activeLine.setVisible(false);

        if (patternLength > 0) {
            presenter->processPattern(currentPattern, patternLength);
        }
    }
}

void LockScreenView::handleDragEvent(const touchgfx::DragEvent& evt)
{
    LockScreenViewBase::handleDragEvent(evt);
    
    if (isErrorState) return; // Không cho vẽ tiếp khi đang báo lỗi
    
    checkDotTouch(evt.getNewX(), evt.getNewY());

    if (patternLength > 0 && patternLength < 9) {
        touchgfx::Widget* lastDot = getDot(currentPattern[patternLength - 1]);
        int startX = lastDot->getX() + lastDot->getWidth() / 2;
        int startY = lastDot->getY() + lastDot->getHeight() / 2;
        
        // Lấy bounding box tối thiểu của Line CŨ và mở rộng 4 pixel để xóa sạch bóng ma
        touchgfx::Rect oldRect = activeLine.getMinimalRect();
        oldRect.x -= 4; oldRect.y -= 4; oldRect.width += 8; oldRect.height += 8;
        invalidateRect(oldRect);

        activeLine.setStart(startX, startY);
        activeLine.setEnd(evt.getNewX(), evt.getNewY());
        activeLine.setVisible(true);

        // Lấy bounding box tối thiểu của Line MỚI và mở rộng
        touchgfx::Rect newRect = activeLine.getMinimalRect();
        newRect.x -= 4; newRect.y -= 4; newRect.width += 8; newRect.height += 8;
        invalidateRect(newRect);
    }
}

void LockScreenView::handleKeyEvent(uint8_t key)
{
    LockScreenViewBase::handleKeyEvent(key);
    // Nếu người dùng gõ phím 'r' hoặc 'R' trên bàn phím máy tính
    if (key == 'r' || key == 'R') {
        presenter->triggerRegister();
    }
}

void LockScreenView::showError()
{
    isErrorState = true;
    errorTickCounter = 0;
    
    // Đổi màu các đường nối thành màu đỏ
    linePainter.setColor(touchgfx::Color::getColorFromRGB(255, 0, 0));
    for (int i = 0; i < 8; i++) {
        if (patternLines[i].isVisible()) {
            patternLines[i].invalidate(); // Ép màn hình vẽ lại line với màu đỏ
        }
    }
    
    // Ẩn chữ thường, hiện chữ báo lỗi
    textArea1.setVisible(false);
    textArea1.invalidate();
    
    textError.setVisible(true);
    textError.invalidate();
}

void LockScreenView::handleTickEvent()
{
    LockScreenViewBase::handleTickEvent();
    
    // Lưu ý: timeout 30s được xử lý ở Model, ở đây ta chỉ đếm tick cho hiệu ứng đỏ 1 giây
    if (isErrorState && !textLockout.isVisible()) {
        errorTickCounter++;
        // Đợi 120 tick (tương đương 2 giây ở 60fps)
        if (errorTickCounter >= 120) {
            resetPattern(); // Reset lại toàn bộ về trạng thái bình thường
        }
    }
}

void LockScreenView::showLockout()
{
    resetPattern();
    isErrorState = true; // Chặn người dùng tương tác
    
    // Ẩn tất cả các điểm
    dot0.setVisible(false); dot0.invalidate();
    dot1.setVisible(false); dot1.invalidate();
    dot2.setVisible(false); dot2.invalidate();
    dot3.setVisible(false); dot3.invalidate();
    dot4.setVisible(false); dot4.invalidate();
    dot5.setVisible(false); dot5.invalidate();
    dot6.setVisible(false); dot6.invalidate();
    dot7.setVisible(false); dot7.invalidate();
    dot8.setVisible(false); dot8.invalidate();
    
    // Đổi chữ
    textArea1.setVisible(false); textArea1.invalidate();
    textError.setVisible(false); textError.invalidate();
    textLockout.setVisible(true); textLockout.invalidate();
}

void LockScreenView::hideLockout()
{
    isErrorState = false;
    
    // Hiện lại các điểm
    dot0.setVisible(true); dot0.invalidate();
    dot1.setVisible(true); dot1.invalidate();
    dot2.setVisible(true); dot2.invalidate();
    dot3.setVisible(true); dot3.invalidate();
    dot4.setVisible(true); dot4.invalidate();
    dot5.setVisible(true); dot5.invalidate();
    dot6.setVisible(true); dot6.invalidate();
    dot7.setVisible(true); dot7.invalidate();
    dot8.setVisible(true); dot8.invalidate();
    
    // Đổi lại chữ
    textLockout.setVisible(false); textLockout.invalidate();
    resetPattern();
}
