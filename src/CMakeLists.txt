target_sources(slime_mold PRIVATE
    main.cpp
    kernel.cu
)

add_subdirectory(utility)

target_link_libraries(slime_mold PRIVATE utility)