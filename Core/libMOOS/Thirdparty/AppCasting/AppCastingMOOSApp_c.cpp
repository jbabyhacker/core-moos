#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp_c.h"
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <functional>
#include <set>

class RustMoosApp : public AppCastingMOOSApp {
public:
    using AppCastingMOOSApp::Notify;
    using AppCastingMOOSApp::RegisterVariables;
    using AppCastingMOOSApp::Register;

    void *m_callbackTarget = nullptr;
    rust_bool_void_star_callback m_iterateCallback = nullptr;
    rust_bool_void_star_callback m_onStartUpCallback = nullptr;
    rust_bool_void_star_callback m_onConnectToServerCallback = nullptr;
    on_new_mail_callback m_onNewMailCallback = nullptr;
    std::set<std::string> m_doubleVarNames;
    std::set<std::string> m_stringVarNames;

protected:
    bool Iterate() override {
        bool success = false;
        AppCastingMOOSApp::Iterate();
        if (m_iterateCallback) {
            success = m_iterateCallback(m_callbackTarget);
        }
        AppCastingMOOSApp::PostReport();
        return success;
    }

    bool OnNewMail(MOOSMSG_LIST &NewMail) override {
        AppCastingMOOSApp::OnNewMail(NewMail);

        MOOSMSG_LIST::iterator p;
        for(p=NewMail.begin(); p!=NewMail.end();) {
            CMOOSMsg &msg = *p;
            if(m_doubleVarNames.count(msg.GetKey()) > 0) {
                if(msg.IsDouble()) {
                    if(m_onNewMailCallback) {
                        m_onNewMailCallback(m_callbackTarget, msg.GetKey().c_str(), DOUBLE, msg.GetDouble(), nullptr);
                    }
                }
                p = NewMail.erase(p);
            } else if (m_stringVarNames.count(msg.GetKey()) > 0) {
                if(msg.IsString()) {
                    if(m_onNewMailCallback) {
                        m_onNewMailCallback(m_callbackTarget, msg.GetKey().c_str(), STRING, 0, msg.GetString().c_str());
                    }
                }
                p = NewMail.erase(p);
            } else {
                ++p;
            }
        }

        m_onNewMailCallback(m_callbackTarget, nullptr, DONE, 0, nullptr);

        return true;
    }

    bool OnStartUp() override {
        AppCastingMOOSApp::OnStartUp();
        if (m_onStartUpCallback) {
            return m_onStartUpCallback(m_callbackTarget);
        } else {
            return false;
        }
    }

    bool OnConnectToServer() override {
        if (m_onConnectToServerCallback) {
            bool success = m_onConnectToServerCallback(m_callbackTarget);
            RegisterVariables();
            return success;
        }

        return false;
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

void RustMoosApp_setOnConnectToServerCallback(RustMoosApp *v, rust_bool_void_star_callback callback) {
    v->m_onConnectToServerCallback = callback;
}

void RustMoosApp_setOnNewMailCallback(RustMoosApp *v, on_new_mail_callback callback) {
    v->m_onNewMailCallback = callback;
}

bool RustMoosApp_notifyDouble(RustMoosApp *v, const char *sVar, const double dfVal) {
    return v->Notify(sVar, dfVal);
}

bool RustMoosApp_run(RustMoosApp *v, const char *sName, const char *missionFile) {
    std::string cppName(sName);
    std::string cppMissionFile(missionFile);

    return v->Run(cppName, cppMissionFile);
}

bool RustMoosApp_register(RustMoosApp *v, const char *sVar, const DataType kind, const double dfInterval) {
    std::string cppString(sVar);

    switch(kind) {
        case DOUBLE:
            v->m_doubleVarNames.insert(cppString);
            break;
        case STRING:
            v->m_stringVarNames.insert(cppString);
            break;
        case DONE:

            break;
    }

    return v->Register(cppString, dfInterval);
}
}
