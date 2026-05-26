target_sources(slime_mold PRIVATE
    main.cpp
)

add_subdirectory(utility)

target_link_libraries(slime_mold PRIVATE utility)