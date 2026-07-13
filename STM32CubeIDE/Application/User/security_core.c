#ifdef __arm__
#include "security_core.h"
#else
#include "../Inc/security_core.h"
#endif
#ifdef __arm__
#include "stm32f4xx_hal.h"
#endif
#include <string.h>
#include <stdio.h>

/* Biến trạng thái toàn cục */
SecurityState_t current_state = STATE_LOCKED;
bool flag_request_register = false;

/* ---- Mock Flash Storage (Phase 1) ---- */
/* Ở Phase 1 Simulator, ta lưu pattern vào RAM.
   Sau này ở Phase 2 sẽ thay bằng hàm đọc/ghi Flash thực sự.
*/
static uint8_t saved_pattern[MAX_PATTERN_LENGTH] = {0, 1, 2, 5, 8}; // Pattern mặc định: L ngược
static uint8_t saved_pattern_len = 5;

/* Mảng tạm dùng khi đang ở bước Confirming */
static uint8_t temp_register_pattern[MAX_PATTERN_LENGTH];
static uint8_t temp_register_len = 0;

/* ------------------------------------------------------------------ */

void Security_Init(void) {
    /* Trong tương lai (Phase 2):
       Đọc từ Flash sector 11. Nếu magic number đúng thì lấy pattern.
       Hiện tại (Phase 1): Sử dụng pattern mặc định trong RAM.
    */
    current_state = STATE_LOCKED;
    flag_request_register = false;
}

bool Security_ProcessPattern(const uint8_t* input_pattern, uint8_t length) {
    if (length == 0 || length > MAX_PATTERN_LENGTH) {
        return false;
    }

    switch (current_state) {
        case STATE_LOCKED:
            /* User vừa vẽ xong ở màn hình khóa, chuyển sang VERIFYING */
            current_state = STATE_VERIFYING;

            /* So sánh với pattern đã lưu */
            if (length == saved_pattern_len) {
                if (memcmp(input_pattern, saved_pattern, length) == 0) {
                    /* Khớp -> Mở khóa thành công */
                    current_state = STATE_UNLOCKED;
                    return true;
                }
            }
            /* Không khớp -> Quay lại khóa */
            current_state = STATE_LOCKED;
            return false;

        case STATE_REGISTERING:
            /* User vẽ pattern lần 1 -> Lưu vào mảng tạm -> Sang CONFIRMING */
            memcpy(temp_register_pattern, input_pattern, length);
            temp_register_len = length;
            current_state = STATE_CONFIRMING;
            return true;

        case STATE_CONFIRMING:
            /* User vẽ pattern lần 2 để xác nhận */
            if (length == temp_register_len) {
                if (memcmp(input_pattern, temp_register_pattern, length) == 0) {
                    /* Khớp -> Lưu thành pattern chính thức */
                    memcpy(saved_pattern, temp_register_pattern, length);
                    saved_pattern_len = length;

                    /* Phase 2: Gọi HAL_FLASH_Program ở đây để lưu vĩnh viễn */

                    /* Xong -> Quay về Locked */
                    current_state = STATE_LOCKED;
                    return true;
                }
            }
            /* Không khớp -> Bắt vẽ lại từ đầu (REGISTERING) */
            current_state = STATE_REGISTERING;
            return false;

        default:
            return false;
    }
}

/* Biến phục vụ đếm thời gian nút Boot */
static uint32_t boot_button_press_time = 0;
static bool boot_button_is_pressed = false;

/* Giả lập đọc nút Boot (USER BUTTON trên kit STM32F429I-DISC1 là PA0) */
/* Nhưng trong Simulator (Phase 1), có thể ta sẽ dùng một flag từ TouchGFX gửi xuống
   hoặc gọi hàm này từ một nút bấm trên UI để test. */
void Security_CheckBootButton(void) {
    /* Trong thư viện HAL, PA0 là nút bấm trên kit F429-DISC1.
       Tùy thuộc vào config CubeMX, thường là GPIO_PIN_0 trên GPIOA.
       Ở Phase 1 (Simulator), GPIO không hoạt động thực sự trừ khi có phần giả lập.
    */

    // GPIO_PinState state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
    // Giả sử ta đang có một hàm mock hoặc test trên kit thật:

    #ifdef USE_REAL_HARDWARE
    GPIO_PinState state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
    if (state == GPIO_PIN_SET) { // Nút bấm là Active High trên F429I-DISC1
        if (!boot_button_is_pressed) {
            boot_button_is_pressed = true;
            boot_button_press_time = HAL_GetTick();
        } else {
            /* Đã bấm và đang giữ, kiểm tra xem quá 3s chưa */
            if (HAL_GetTick() - boot_button_press_time >= 3000) {
                if (current_state == STATE_LOCKED || current_state == STATE_UNLOCKED) {
                    /* Trigger cờ để TouchGFX biết cần đổi màn hình */
                    flag_request_register = true;
                    current_state = STATE_REGISTERING;
                }
                /* Reset để không trigger liên tục */
                boot_button_is_pressed = false;
                boot_button_press_time = HAL_GetTick();
            }
        }
    } else {
        boot_button_is_pressed = false;
    }
    #endif
}
