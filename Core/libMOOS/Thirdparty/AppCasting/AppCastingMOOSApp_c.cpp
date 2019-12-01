//
// Created by jklein on 11/30/19.
//

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp_c.h"
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <functional>

using IterateCallback = std::function<bool(void*)>;
using OnStartUpCallback = std::function<bool(void*)>;

class RustMoosApp : public AppCastingMOOSApp {
public:
    void setIterateCallback(IterateCallback callback) {
        m_iterateCallback = callback;
    }

    void setOnStartUpCallback(OnStartUpCallback callback) {
        m_onStartUpCallback = callback;
    }

    bool callRegister(const std::string& sVar, const double dfInterval) {
        return Register(sVar, dfInterval);
    }

    void* m_callbackTarget = nullptr;

protected:
    bool Iterate() override {
        if(m_iterateCallback) {
            return m_iterateCallback(m_callbackTarget);
        } else {
            return false;
        }
    }

    bool OnStartUp() override {
        if(m_onStartUpCallback) {
            return m_onStartUpCallback(m_callbackTarget);
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
    if(!v->m_callbackTarget) {
        v->m_callbackTarget = v;
    }
    v->setIterateCallback(callback);
}

void RustMoosApp_setOnStartUpCallback(RustMoosApp *v, rust_callback callback) {
    if(!v->m_callbackTarget) {
        v->m_callbackTarget = v;
    }
    v->setOnStartUpCallback(callback);
}

bool RustMoosApp_run1(RustMoosApp *v, const char* sName, const char* missionFile) {
    std::string cppName(sName);
    std::string cppMissionFile(missionFile);

    return v->Run(cppName, cppMissionFile);
}

bool RustMoosApp_register(RustMoosApp *v, const char *sVar, const double dfInterval) {
    std::string cppString(sVar);

    return v->callRegister(cppString, dfInterval);
}
}
