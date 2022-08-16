idf_component_register(SRCS
    Application.cxx
    Button.cxx
    StatusLed.cxx
    Wireless.cxx

    INCLUDE_DIRS ".")


add_subdirectory(components/oled-driver)
add_subdirectory(components/util)

target_link_libraries(${COMPONENT_LIB} PUBLIC 
                oled-driver
                util
                )
