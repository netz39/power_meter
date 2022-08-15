idf_component_register(SRCS 
    wrappers/BinarySemaphore.cxx
    wrappers/Mutex.cxx
    wrappers/sync.cxx
    wrappers/Task.cxx

    Application.cxx
    StatusLed.cxx
    Wireless.cxx

    INCLUDE_DIRS ".")


add_subdirectory(components/oled-driver)

target_link_libraries(${COMPONENT_LIB} PUBLIC 
                oled-driver
                )
