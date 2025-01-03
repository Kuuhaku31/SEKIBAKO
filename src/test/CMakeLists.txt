
project(test_base)

add_executable(${PROJECT_NAME} test_base/main.cpp)
include_directories(${THIRD_PARTY_INC_PATHS})

target_link_libraries(${PROJECT_NAME}
    third_party
)
