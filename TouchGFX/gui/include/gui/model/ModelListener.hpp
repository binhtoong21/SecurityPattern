#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m) { model = m; }

    // Phương thức ảo để View có thể override và hiển thị lỗi
    virtual void showError() {}
    virtual void showLockout() {}
    virtual void hideLockout() {}

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
