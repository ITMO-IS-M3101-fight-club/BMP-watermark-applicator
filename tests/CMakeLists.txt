add_executable(run_test
    main_test_checker.cpp
    ${PROJECT_SOURCE_DIR}/src/ImageBMP.cpp
)

target_include_directories(run_test PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(watermark_test
    main_watermark_test.cpp
    ${PROJECT_SOURCE_DIR}/src/ImageBMP.cpp
    ${PROJECT_SOURCE_DIR}/src/Watermark.cpp
)

target_include_directories(watermark_test PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)