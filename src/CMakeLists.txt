
add_executable(search_engine
    main.cpp
    ConverterJSON.cpp
    InvertedIndex.cpp
    SearchServer.cpp
)

target_include_directories(search_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)
