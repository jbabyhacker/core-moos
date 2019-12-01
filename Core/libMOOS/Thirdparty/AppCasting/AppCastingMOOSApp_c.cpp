#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp_c.h"
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <functional>

class RustMoosApp : public AppCastingMOOSApp {
public:
    using AppCastingMOOSApp::Notify;
    using AppCastingMOOSApp::RegisterVariables;
    using AppCastingMOOSApp::Register;

    void *m_callbackTarget = nullptr;
    rust_bool_void_star_callback m_iterateCallback = nullptr;
    rust_bool_void_star_callback m_onStartUpCallback = nullptr;
    rust_bool_void_star_callback m_onConnectToServer = nullptr;

protected:
    bool Iterate() override {
        if (m_iterateCallback) {
            AppCastingMOOSApp::Iterate();
            m_iterateCallback(m_callbackTarget);
            AppCastingMOOSApp::PostReport();
            return true;
        } else {
            return false;
        }
    }

    bool OnStartUp() override {
        if (m_onStartUpCallback) {
            AppCastingMOOSApp::OnStartUp();
            return m_onStartUpCallback(m_callbackTarget);
        } else {
            return false;
        }
    }

    bool OnConnectToServer() override {
        if (m_onConnectToServer) {
            return m_onConnectToServer(m_callbackTarget);
        } else {
            return false;
        }
    }

private:

};

extern "C" {
RustMoosApp *newRustMoosApp() {
    return new RustMoosApp();
}

void deleteRustMoosApp(RustMoosApp *v) {
    delete v;
}

void RustMoosApp_setTarget(RustMoosApp *v, void* target) {
    v->m_callbackTarget = target;
}

void RustMoosApp_setIterateCallback(RustMoosApp *v, rust_bool_void_star_callback callback) {
    v->m_iterateCallback = callback;
}

void RustMoosApp_setOnStartUpCallback(RustMoosApp *v, rust_bool_void_star_callback callback) {
    v->m_onStartUpCallback = callback;
}

void RustMoosApp_onConnectToServer(RustMoosApp *v, rust_bool_void_star_callback callback) {
    v->m_onConnectToServer = callback;
}

bool RustMoosApp_notifyDouble(RustMoosApp *v, const char *sVar, const double& dfVal) {
    return v->Notify(sVar, dfVal);
}

bool RustMoosApp_run(RustMoosApp *v, const char *sName, const char *missionFile) {
    std::string cppName(sName);
    std::string cppMissionFile(missionFile);

    return v->Run(cppName, cppMissionFile);
}

bool RustMoosApp_register(RustMoosApp *v, const char *sVar, const double dfInterval) {
    std::string cppString(sVar);

    v->RegisterVariables();
    return v->Register(cppString, dfInterval);
}
}
