// --- File: src/gui/include/gui/model/Model.hpp ---
#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdint.h>

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener) { modelListener = listener; }

    void tick();

    // Gọi từ Presenter khi người dùng vẽ xong
    void verifyPattern(uint8_t* pattern, uint8_t length);

    // Hàm gọi giả lập từ Simulator để chuyển sang đăng ký
    void triggerRegister();

protected:
    ModelListener* modelListener;
    int unlocked_tick_count;
    int timeout_tick_count;
};

#endif // MODEL_HPP
