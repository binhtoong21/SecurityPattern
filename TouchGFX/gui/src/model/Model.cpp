// --- File: src/gui/src/model/Model.cpp ---
#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <gui/common/FrontendApplication.hpp>

extern "C" {
#ifdef __arm__
#include "security_core.h"
#else
// Simulator PC x86 không có Core/Inc trong Include Path
#include "../../../../Core/Inc/security_core.h"
#endif
}

// MẸO: Trình biên dịch PC Simulator (MinGW x86) mặc định không biên dịch các file trong Core/Src.
// Ta include thẳng nội dung file .c vào Model.cpp khi chạy Simulator để test.
#ifndef __arm__
#include "../../../../Core/Src/security_core.c"
#endif

Model::Model() : modelListener(0), unlocked_tick_count(0), timeout_tick_count(0) {}

void Model::tick()
{
    // Gọi hàm kiểm tra nút bấm (trong thực tế chạy ở ngắt hoặc thread khác, 
    // nhưng để tiện cho Simulator ta gọi ở đây)
    Security_CheckBootButton();

    if (flag_request_register) {
        flag_request_register = false;
        // Chuyển sang màn hình Register
        static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotoRegisterScreenScreenNoTransition();
    }

    // Tự động khóa lại sau 3 giây (60 fps * 3s = 180 ticks)
    if (current_state == STATE_UNLOCKED) {
        unlocked_tick_count++;
        if (unlocked_tick_count >= 180) {
            current_state = STATE_LOCKED;
            unlocked_tick_count = 0;
            static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotoLockScreenScreenNoTransition();
        }
    } else {
        unlocked_tick_count = 0;
    }

    // Xử lý đếm thời gian khóa 30 giây (1800 ticks ở 60fps)
    if (current_state == STATE_LOCKED_TIMEOUT) {
        if (timeout_tick_count == 0 && modelListener != 0) {
            modelListener->showLockout();
        }
        timeout_tick_count++;
        if (timeout_tick_count >= 1800) {
            current_state = STATE_LOCKED;
            failed_attempts = 0;
            timeout_tick_count = 0;
            if (modelListener != 0) {
                modelListener->hideLockout();
            }
        }
    } else {
        timeout_tick_count = 0;
    }
}

void Model::verifyPattern(uint8_t* pattern, uint8_t length)
{
    SecurityState_t previous_state = current_state;
    bool result = Security_ProcessPattern(pattern, length);
    
    if (result) {
        if (current_state == STATE_UNLOCKED) {
            static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotoUnlockedScreenScreenNoTransition();
        } else if (current_state == STATE_CONFIRMING) {
            static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotoConfirmScreenScreenNoTransition();
        } else if (current_state == STATE_LOCKED) {
            // Confirm thành công -> quay về Lock
            static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotoLockScreenScreenNoTransition();
        }
    } else {
        // Vẽ sai
        if (modelListener != 0) {
            modelListener->showError();
        }
        
        // Chỉ chuyển màn hình về RegisterScreen nếu trước đó đang ở ConfirmScreen và vẽ sai
        if (previous_state == STATE_CONFIRMING && current_state == STATE_REGISTERING) {
            static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotoRegisterScreenScreenNoTransition();
        }
    }
}

void Model::triggerRegister()
{
    flag_request_register = true;
    current_state = STATE_REGISTERING;
    static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotoRegisterScreenScreenNoTransition();
}
