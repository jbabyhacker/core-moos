//
// Created by jklein on 11/30/19.
//

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp_c.h"
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <functional>

using IterateCallback = std::function<bool(void)>;
using OnStartUpCallback = std::function<bool(void)>;

class RustMoosApp : public AppCastingMOOSApp {
public:
    void setIterateCallback(IterateCallback callback) {
        m_iterateCallback = callback;
    }

    void setOnStartUpCallback(OnStartUpCallback callback) {
        m_onStartUpCallback = callback;
    }

protected:
    bool Iterate() override {
        if(m_iterateCallback) {
            return m_iterateCallback();
        } else {
            return false;
        }
    }

    bool OnStartUp() override {
        if(m_onStartUpCallback) {
            return m_onStartUpCallback();
        } else {
            return false;
        }
    }

private:
    IterateCallback m_iterateCallback;
    OnStartUpCallback  m_onStartUpCallback;
};

extern "C" {
RustMoosApp* newRustMoosApp() {
    return new RustMoosApp();
}

void deleteRustMoosApp(RustMoosApp* v) {
    delete v;
}

void RustMoosApp_setIterateCallback(RustMoosApp* v, rust_callback callback) {
    v->setIterateCallback(callback);
}

void RustMoosApp_setOnStartUpCallback(RustMoosApp *v, rust_callback callback) {
    v->setOnStartUpCallback(callback);
}

bool RustMoosApp_run1(RustMoosApp *v, const char* sName, int argc, char *argv[]) {
    std::string cppString(sName);

    return v->Run(cppString);
}
}
