idf_component_register(SRCS
    Application.cxx
    Button.cxx
    HttpClient.cxx
    PulseDetector.cxx
    StatusLed.cxx
    sync.cxx
    Wireless.cxx

    REQUIRES
        driver
        esp_http_client
        esp_wifi
        mbedtls
        nvs_flash

    INCLUDE_DIRS ".")

add_subdirectory(components/date)
add_subdirectory(components/util)

target_link_libraries(${COMPONENT_LIB} PUBLIC 
                date
                util
                )
