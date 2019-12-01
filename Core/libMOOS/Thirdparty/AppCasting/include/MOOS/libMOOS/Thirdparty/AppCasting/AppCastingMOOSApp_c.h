#ifndef MOOS_APPCASTINGMOOSAPP_C_H
#define MOOS_APPCASTINGMOOSAPP_C_H


#ifdef __cplusplus
extern "C" {
#endif

typedef struct RustMoosApp RustMoosApp;
typedef bool (*rust_bool_void_star_callback)(void *callback_target);

RustMoosApp *newRustMoosApp();

void deleteRustMoosApp(RustMoosApp *v);

void RustMoosApp_setTarget(RustMoosApp *v, void* target);

void RustMoosApp_setIterateCallback(RustMoosApp *v, rust_bool_void_star_callback callback);

void RustMoosApp_setOnStartUpCallback(RustMoosApp *v, rust_bool_void_star_callback callback);

void RustMoosApp_onConnectToServer(RustMoosApp *v, rust_bool_void_star_callback callback);

bool RustMoosApp_run(RustMoosApp *v, const char *sName, const char *missionFile);

bool RustMoosApp_register(RustMoosApp *v, const char *sVar, const double dfInterval);

bool RustMoosApp_notifyDouble(RustMoosApp *v, const char *sVar, const double& dfVal);

#ifdef __cplusplus
};
#endif
#endif //MOOS_APPCASTINGMOOSAPP_C_H
