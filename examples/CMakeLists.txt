list(APPEND EXAMPLES
    hloop_test
    htimer_test
    pipe_test
    nc
    tinyhttpd
    tinyproxyd
    tcp_client_test
    jsonrpc_client
)

include_directories(.. ../base ../ssl ../event ../util)

add_executable(hloop_test hloop_test.c)
target_link_libraries(hloop_test ${HV_LIBRARIES})

add_executable(htimer_test htimer_test.c)
target_link_libraries(htimer_test ${HV_LIBRARIES})

add_executable(pipe_test pipe_test.c)
target_link_libraries(pipe_test ${HV_LIBRARIES})

add_executable(nc nc.c)
target_link_libraries(nc ${HV_LIBRARIES})

add_executable(tinyhttpd tinyhttpd.c)
target_link_libraries(tinyhttpd ${HV_LIBRARIES})

add_executable(tinyproxyd tinyproxyd.c)
target_link_libraries(tinyproxyd ${HV_LIBRARIES})

add_executable(tcp_client_test tcp_client_test.c)
target_link_libraries(tcp_client_test ${HV_LIBRARIES})

add_executable(jsonrpc_client jsonrpc/jsonrpc_client.c jsonrpc/cJSON.c)
target_compile_definitions(jsonrpc_client PRIVATE CJSON_HIDE_SYMBOLS)
target_link_libraries(jsonrpc_client ${HV_LIBRARIES})

if(WITH_KCP)
    glob_headers_and_sources(KCPTUN_SMUX_FILES   kcptun/smux)
    glob_headers_and_sources(KCPTUN_CLIENT_FILES kcptun/client)

    # kcptun_client
    add_executable(kcptun_client ${KCPTUN_SMUX_FILES} ${KCPTUN_CLIENT_FILES})
    target_link_libraries(kcptun_client ${HV_LIBRARIES})

    list(APPEND EXAMPLES kcptun_client)
endif()

if(WITH_EVPP)
    include_directories(../cpputil ../evpp)

    # hmain_test
    add_executable(hmain_test hmain_test.cpp)
    target_link_libraries(hmain_test ${HV_LIBRARIES})

    # nmap
    glob_headers_and_sources(NMAP_FILES nmap)
    add_executable(nmap ${NMAP_FILES})
    target_compile_definitions(nmap PRIVATE PRINT_DEBUG)
    target_link_libraries(nmap ${HV_LIBRARIES})

    list(APPEND EXAMPLES hmain_test nmap)
if(WITH_HTTP)
    include_directories(../http)

    # wrk
    add_executable(wrk wrk.cpp)
    target_link_libraries(wrk ${HV_LIBRARIES})

    list(APPEND EXAMPLES wrk)

if(WITH_HTTP_CLIENT)
    include_directories(../http/client)

    # curl
    set(CURL_TARGET_NAME curl)
    if(WITH_CURL)
        set(CURL_TARGET_NAME hv_curl)
    endif()
    add_executable(${CURL_TARGET_NAME} curl.cpp)
    if(WITH_CURL)
        set_target_properties(${CURL_TARGET_NAME} PROPERTIES OUTPUT_NAME curl)
    endif()
    target_link_libraries(${CURL_TARGET_NAME} ${HV_LIBRARIES})

    # wget
    add_executable(wget wget.cpp)
    target_link_libraries(wget ${HV_LIBRARIES})

    # http_client_test
    add_executable(http_client_test http_client_test.cpp)
    target_link_libraries(http_client_test ${HV_LIBRARIES})

    # websocket_client_test
    add_executable(websocket_client_test websocket_client_test.cpp)
    target_link_libraries(websocket_client_test ${HV_LIBRARIES})

    list(APPEND EXAMPLES ${CURL_TARGET_NAME} wget http_client_test websocket_client_test)
endif()

endif()
endif()

add_custom_target(examples DEPENDS ${EXAMPLES})
