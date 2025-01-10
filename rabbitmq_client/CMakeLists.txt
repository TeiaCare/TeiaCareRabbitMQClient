set(TARGET_NAME teiacare_rabbitmq_client)
add_library(${TARGET_NAME})
add_library(teiacare::rabbitmq_client ALIAS ${TARGET_NAME})

find_package(rabbitmq-c REQUIRED)

configure_file(
    src/version.cpp.in
    ${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp
)

set(TARGET_HEADERS
    include/teiacare/rabbitmq_client/channel.hpp
    include/teiacare/rabbitmq_client/rabbitmq_client.hpp
    include/teiacare/rabbitmq_client/version.hpp
)

set(TARGET_SOURCES
    src/channel.cpp
    src/rabbitmq_client.cpp
    src/version.cpp
)

list(APPEND ALL_SOURCES ${TARGET_HEADERS} ${TARGET_SOURCES})
list(TRANSFORM ALL_SOURCES_ABS_PATH PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)
target_sources(${TARGET_NAME} PUBLIC ${TARGET_HEADERS} PRIVATE ${TARGET_SOURCES})
target_include_directories(${TARGET_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:CMAKE_INSTALL_INCLUDEDIR>
)
target_link_libraries(${TARGET_NAME}
    PUBLIC
        rabbitmq::rabbitmq-static
)

set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${${PROJECT_NAME}_VERSION} SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR})
install(TARGETS ${TARGET_NAME})
install(DIRECTORY include DESTINATION .)

if(TC_ENABLE_WARNINGS_ERROR)
    add_warnings(${TARGET_NAME})
    add_warnings_as_errors(${TARGET_NAME})
endif()

#################################################################
# Unit Tests
if(TC_ENABLE_UNIT_TESTS)
    set(TEST_TARGET_NAME teiacare_rabbitmq_client_unittests)
    find_package(GTest REQUIRED)
    include(GoogleTest)

    set(UNIT_TESTS_SRC
        tests/main.cpp
    )

    add_executable(${TEST_TARGET_NAME})
    target_sources(${TEST_TARGET_NAME} PRIVATE ${UNIT_TESTS_SRC})
    target_compile_features(${TEST_TARGET_NAME} PUBLIC cxx_std_20)
    target_link_libraries(${TEST_TARGET_NAME}
        PRIVATE
            GTest::gtest
            GTest::gmock
            teiacare::rabbitmq_client
    )

    gtest_discover_tests(${TEST_TARGET_NAME})
    install(TARGETS ${TEST_TARGET_NAME} DESTINATION unit_tests)

    if(TC_ENABLE_WARNINGS_ERROR)
        add_warnings(${TEST_TARGET_NAME})
        add_warnings_as_errors(${TEST_TARGET_NAME})
    endif()
endif()

#################################################################
# Example
if(TC_ENABLE_EXAMPLES)
    include(examples)
    add_example(teiacare_rabbitmq_client simple_client)
endif()
