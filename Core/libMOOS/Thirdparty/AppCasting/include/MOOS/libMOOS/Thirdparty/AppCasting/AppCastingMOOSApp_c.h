//
// Created by jklein on 11/30/19.
//

#ifndef MOOS_APPCASTINGMOOSAPP_C_H
#define MOOS_APPCASTINGMOOSAPP_C_H


#ifdef __cplusplus
extern "C" {
#endif
    typedef struct RustMoosApp RustMoosApp;
    typedef bool (*rust_callback)(void);

    RustMoosApp* newRustMoosApp();

    void deleteRustMoosApp(RustMoosApp* v);

    void RustMoosApp_setIterateCallback(RustMoosApp* v, rust_callback callback);

    void RustMoosApp_setOnStartUpCallback(RustMoosApp *v, rust_callback callback);

    bool RustMoosApp_run1(RustMoosApp *v, const char* sName, int argc, char *argv[]);

#ifdef __cplusplus
};
#endif
#endif //MOOS_APPCASTINGMOOSAPP_C_H
