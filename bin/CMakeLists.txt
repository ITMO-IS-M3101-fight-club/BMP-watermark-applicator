add_executable(watermark
    main.cpp
    ${PROJECT_SOURCE_DIR}/src/ImageBMP.cpp
    ${PROJECT_SOURCE_DIR}/src/Watermark.cpp
)

target_include_directories(watermark PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)