idf_component_register(SRCS
    Application.cxx
    Button.cxx
    StatusLed.cxx
    Wireless.cxx

    INCLUDE_DIRS ".")

add_subdirectory(components/date)
add_subdirectory(components/util)

target_link_libraries(${COMPONENT_LIB} PUBLIC 
                date
                util
                )
