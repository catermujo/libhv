#include "WebSocketClient.h"

#include "hwebsocket.h"

void ws_client_init(ws_client *ws, void (*cb) (void *self, const char *msg)) {
    ws->ws_cpp = new hv::WebSocketClient();
    ws->cb = cb;
}

int ws_client_connect(ws_client *ws, const char *url) {
    hv::WebSocketClient *ws_cpp = (hv::WebSocketClient*) ws->ws_cpp;

    ws_cpp->onopen = [ws_cpp]() {
        const HttpResponsePtr& resp = ws_cpp->getHttpResponse();
        printf("connection opened.\n%s\n", resp->body.c_str());
        // printf("response:\n%s\n", resp->Dump(true, true).c_str());
    };
    ws_cpp->onmessage = [ws_cpp, ws](const std::string& msg) {
        ws->cb(ws, msg.data());
    };
    ws_cpp->onclose = []() {
        printf("connection closed.\n");
    };

    // ping every 16ms ("""5 frames""")
    ws_cpp->setPingInterval(80);

    // reconnect: 1,2,4,8,10,10,10...
    reconn_setting_t reconn;
    reconn_setting_init(&reconn);
    reconn.min_delay = 1000;
    reconn.max_delay = 10000;
    reconn.delay_policy = 2;
    ws_cpp->setReconnect(&reconn);

    /*
    auto req = std::make_shared<HttpRequest>();
    req->method = HTTP_POST;
    req->headers["Origin"] = "http://example.com";
    req->json["app_id"] = "123456";
    req->json["app_secret"] = "abcdefg";
    printf("request:\n%s\n", req->Dump(true, true).c_str());
    setHttpRequest(req);
    */

    http_headers headers;
    headers["Origin"] = "http://example.com/";
    return ws_cpp->open(url, headers);
}

void ws_client_send(ws_client *ws, const char *msg) {
    hv::WebSocketClient *ws_cpp = (hv::WebSocketClient*) ws->ws_cpp;
    ws_cpp->send(msg);
}

bool ws_client_is_connected(ws_client *ws) {
    hv::WebSocketClient *ws_cpp = (hv::WebSocketClient*) ws->ws_cpp;
    return ws_cpp->isConnected();
}

void ws_client_close(ws_client *ws) {
    hv::WebSocketClient *ws_cpp = (hv::WebSocketClient*) ws->ws_cpp;
    delete ws_cpp;
}
