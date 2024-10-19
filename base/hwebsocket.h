#ifndef HV_WEBSOCKET_H_
#define HV_WEBSOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>

typedef struct ws_client {
    void *ws_cpp;
    void (*cb) (void *self, const char *msg);
} ws_client;

void ws_client_init(ws_client *ws, void (*cb) (void *self, const char *msg));

int ws_client_connect(ws_client *ws, const char *url);

void ws_client_send(ws_client *ws, const char *msg);

bool ws_client_is_connected(ws_client *ws);

void ws_client_close(ws_client *ws);

#ifdef __cplusplus
}
#endif

#endif // HV_WEBSOCKET_H_
