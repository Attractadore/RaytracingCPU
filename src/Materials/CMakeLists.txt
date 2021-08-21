function(add_material MATERIAL)
    add_library(
        ${MATERIAL}Material SHARED
        "${MATERIAL}Material.cpp"
    )
    target_link_libraries(
        ${MATERIAL}Material
        PRIVATE SceneLib
    )
endfunction()

add_material(Marble)
add_material(Blue)
add_material(Gold)
add_material(Grey)
