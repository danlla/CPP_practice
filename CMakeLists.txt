# CMakeList.txt: проект CMake для CPP_practice; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project ("CPP_practice")

# Добавьте источник в исполняемый файл этого проекта.
add_subdirectory(merge_sorting)
add_subdirectory(unordered_set_practice)
add_subdirectory(find_common_words)
add_subdirectory(avl_tree)
add_subdirectory(graphs)
add_subdirectory(median_filter)
add_subdirectory(brut_force)
add_subdirectory(collection)
add_subdirectory(aquarium)
add_subdirectory(qt-network)

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
