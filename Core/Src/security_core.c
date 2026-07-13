#ifdef __arm__
#include "security_core.h"
#else
#include "../Inc/security_core.h"
#endif

#include <string.h>
#include <stdio.h>

/* Biến lưu trạng thái hiện tại của hệ thống */
SecurityState_t current_state = STATE_LOCKED;
bool flag_request_register = false;
uint8_t failed_attempts = 0;

/* Mảng chứa pattern đã lưu */
static uint8_t saved_pattern[MAX_PATTERN_LENGTH] = {0, 1, 2, 5, 8}; // Pattern mặc định: L ngược
static uint8_t saved_pattern_len = 5;

/* Mảng tạm dùng khi đang ở bước Confirming */
static uint8_t temp_register_pattern[MAX_PATTERN_LENGTH];
static uint8_t temp_register_len = 0;

/* ------------------------------------------------------------------ */
static void Storage_WritePattern(const uint8_t* pattern, uint8_t length) {
    // Phiên bản mock: Chỉ lưu vào biến RAM, không ghi vào Flash
    (void)pattern; (void)length;
}

static void Storage_ReadPattern(void) {
    // Phiên bản mock: Không đọc từ Flash, giữ nguyên default pattern trên RAM
}
/* ------------------------------------------------------------------ */

void Security_Init(void) {
    Storage_ReadPattern();
    current_state = STATE_LOCKED;
    flag_request_register = false;
    failed_attempts = 0;
}

bool Security_ProcessPattern(const uint8_t* input_pattern, uint8_t length) {
    if (length == 0 || length > MAX_PATTERN_LENGTH) {
        return false;
    }

    switch (current_state) {
        case STATE_LOCKED:
            current_state = STATE_VERIFYING;
            
            if (length == saved_pattern_len) {
                if (memcmp(input_pattern, saved_pattern, length) == 0) {
                    current_state = STATE_UNLOCKED;
                    failed_attempts = 0;
                    return true;
                }
            }
            failed_attempts++;
            if (failed_attempts >= 5) {
                current_state = STATE_LOCKED_TIMEOUT;
            } else {
                current_state = STATE_LOCKED;
            }
            return false;

        case STATE_REGISTERING:
            if (length < 4) {
                return false; 
            }
            memcpy(temp_register_pattern, input_pattern, length);
            temp_register_len = length;
            current_state = STATE_CONFIRMING;
            return true;

        case STATE_CONFIRMING:
            if (length == temp_register_len) {
                if (memcmp(input_pattern, temp_register_pattern, length) == 0) {
                    memcpy(saved_pattern, temp_register_pattern, length);
                    saved_pattern_len = length;
                    
                    Storage_WritePattern(saved_pattern, saved_pattern_len);
                    
                    current_state = STATE_LOCKED;
                    return true;
                }
            }
            current_state = STATE_REGISTERING;
            return false;

        default:
            return false;
    }
}

void Security_CheckBootButton(void) {
    // Phiên bản mock: Sẽ được trigger từ TouchGFX Keyboard (giả lập phím R)
    // Không đọc GPIO PA0 ở phiên bản mock này
}
