# Include filepaths for source and include.
include( ${MODULE_ROOT_DIR}/otaFilePaths.cmake )
include( ${MODULE_ROOT_DIR}/otaDependencyFilePaths.cmake )

# ====================  Define your project name (edit) ========================
set(project_name "fuzz_aws_ota")

# ================= Create the library under test here (edit) ==================

# set( OTA_C_TMP_BASE "${CMAKE_BINARY_DIR}/ota" )

# # Strip static constraints so unit tests may call internal functions
# execute_process( COMMAND sed "s/^static //"
#                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#                  INPUT_FILE "${MODULE_ROOT_DIR}/source/ota.c"
#                  OUTPUT_FILE ${OTA_C_TMP_BASE}.c
# )

# # list the files you would like to test here
# list(APPEND real_source_files
#     ${OTA_C_TMP_BASE}.c
#     "${MODULE_ROOT_DIR}/source/ota_interface.c"
#     "${MODULE_ROOT_DIR}/source/ota_base64.c"
#     "${MODULE_ROOT_DIR}/source/ota_mqtt.c"
#     "${MODULE_ROOT_DIR}/source/ota_http.c"
#     "${MODULE_ROOT_DIR}/source/ota_cbor.c"
#     "${MODULE_ROOT_DIR}/source/portable/os/ota_os_posix.c"
#     ${TINYCBOR_SOURCES}
#     ${JSON_SOURCES}
# )
# list the directories the module under test includes
# list(APPEND real_include_directories
#     "../unit-test"
#     ${OTA_INCLUDE_PUBLIC_DIRS}
#     ${OTA_INCLUDE_PRIVATE_DIRS}
#     ${OTA_INCLUDE_OS_POSIX_DIRS}
#     ${JSON_INCLUDE_PUBLIC_DIRS}
#     ${TINYCBOR_INCLUDE_DIRS}
# )

# =====================  Create UnitTest Code here (edit)  =====================

# list the directories your test needs to include
# list(APPEND test_include_directories "../unit-test")


# =============================  (end edit)  ===================================

set(real_name "aws_ota_real")

# create_real_library(${real_name}
#     "${real_source_files}"
#     "${real_include_directories}"
#     ""
# )
# Suppress warnings in dependency folder
# set_source_files_properties(
#     ${JSON_SOURCES}
#     ${TINYCBOR_SOURCES}
#     PROPERTIES COMPILE_FLAGS
#     "-w"
# )
target_include_directories(${real_name}
    SYSTEM PRIVATE
    ${TINYCBOR_INCLUDE_DIRS}
    ${JSON_INCLUDE_PUBLIC_DIRS}
    ${test_include_directories}
)

list(APPEND utest_link_list
    -v
    -lpthread
    lib${real_name}.a
    -lrt
)

list(APPEND utest_dep_list
    ${real_name}
)

# create_test(ota_cbor_fuzz 
#     "ota_cbor_fuzz.c"
#     "${utest_link_list}"
#     "${utest_dep_list}"
#     "${test_include_directories}"
# )

add_executable(ota_cbor_fuzz ota_cbor_fuzz.c)
target_include_directories(ota_cbor_fuzz PRIVATE ${test_include_directories})
target_link_directories(ota_cbor_fuzz PRIVATE "${CMAKE_BINARY_DIR}/unit-test/lib")
target_link_libraries(ota_cbor_fuzz 
    ${utest_link_list}
    ${utest_dep_list}
    )

target_compile_options(ota_cbor_fuzz PRIVATE -fsanitize=fuzzer)
target_link_options(ota_cbor_fuzz PRIVATE -fsanitize=fuzzer)