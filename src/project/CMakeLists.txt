
project(main_project)

set(SRC_PATH
    ${CMAKE_CURRENT_LIST_DIR}
)

set(INC_PATH
    ${THIRD_PARTY_INC_PATHS}
)

foreach(p ${SRC_PATH})
    aux_source_directory(${p} SRCS)
endforeach()

message(STATUS "Main Project SRCS: ${SRCS}")
message(STATUS "Main Project INC_PATH: ${INC_PATH}")

add_executable(${PROJECT_NAME} ${SRCS})
include_directories(${INC_PATH})

target_link_libraries(${PROJECT_NAME}
    third_party
)
