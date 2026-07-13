#ifndef SECURITY_CORE_H
#define SECURITY_CORE_H

#include <stdint.h>
#include <stdbool.h>

// Bật cờ này để biên dịch với thư viện HAL thật khi nạp xuống Kit
#ifdef __arm__
#define USE_REAL_HARDWARE
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* State Machine cho hệ thống Pattern */
typedef enum {
    STATE_LOCKED = 0,
    STATE_VERIFYING,
    STATE_UNLOCKED,
    STATE_REGISTERING,
    STATE_CONFIRMING,
    STATE_LOCKED_TIMEOUT
} SecurityState_t;

/* Kích thước tối đa của một pattern */
#define MAX_PATTERN_LENGTH 9

/* Biến lưu trạng thái hiện tại của hệ thống */
extern SecurityState_t current_state;

/* Số lần nhập sai liên tiếp */
extern uint8_t failed_attempts;

/* Biến cờ thông báo yêu cầu chuyển sang màn hình Register từ nút Boot */
extern bool flag_request_register;

/* Khởi tạo hệ thống security, đọc pattern từ Flash (hoặc RAM mock) */
void Security_Init(void);

/* Xử lý pattern do người dùng vẽ (gọi từ C++ TouchGFX) 
   input_pattern: mảng chứa các điểm đã vẽ (0-8)
   length: số lượng điểm đã vẽ
   Trả về: true nếu thành công (mở khóa/đăng ký khớp), false nếu sai
*/
bool Security_ProcessPattern(const uint8_t* input_pattern, uint8_t length);

/* Kiểm tra nút Boot để chuyển sang chế độ đăng ký (giữ 3s)
   Hàm này cần được gọi liên tục trong Shadow_Loop() hoặc timer
*/
void Security_CheckBootButton(void);

#ifdef __cplusplus
}
#endif

#endif /* SECURITY_CORE_H */
