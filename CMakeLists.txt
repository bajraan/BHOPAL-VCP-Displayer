cmake_minimum_required(VERSION 3.18)
project(bhopal)

set(CMAKE_CXX_STANDARD 17)

# Jeśli folder SFML nie istnieje, pobierz i rozpakuj archiwum.
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/lib/SFML-3.0.0/CMakeLists.txt")
    message(STATUS "SFML nie znaleziono w lib/SFML-3.0.0. Pobieranie i rozpakowywanie archiwum SFML...")
    set(SFML_ZIP "${CMAKE_BINARY_DIR}/SFML-3.0.0.zip")
    file    (   DOWNLOAD
                "https://github.com/bajraan/BHOPAL-ComDisplayer/releases/download/Library/SFML-3.0.0.zip"
                ${SFML_ZIP}
                SHOW_PROGRESS
            )
    if(${CMAKE_VERSION} VERSION_LESS "3.18")
        message(WARNING "Automatyczne rozpakowywanie archiwów ZIP wymaga CMake 3.18 lub wyższej. Proszę ręcznie rozpakować plik ${SFML_ZIP} do folderu ${CMAKE_SOURCE_DIR}/lib.")
    else()
        file(ARCHIVE_EXTRACT INPUT ${SFML_ZIP} DESTINATION "${CMAKE_SOURCE_DIR}/lib")
    endif()
endif()


# Jeśli folder boost nie istnieje, pobierz i rozpakuj archiwum.
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/lib/boost-1.87.0/CMakeLists.txt")
    message(STATUS "boost nie znaleziono w lib/boost-1.87.0 Pobieranie i rozpakowywanie archiwum SFML...")
    set(BOOST_ZIP "${CMAKE_BINARY_DIR}/boost-1.87.0.zip")
    file    (   DOWNLOAD
                "https://github.com/bajraan/BHOPAL-ComDisplayer/releases/download/Library/boost-1.87.0.zip"
                ${BOOST_ZIP}
                SHOW_PROGRESS
            )
    if(${CMAKE_VERSION} VERSION_LESS "3.18")
        message(WARNING "Automatyczne rozpakowywanie archiwów ZIP wymaga CMake 3.18 lub wyższej. Proszę ręcznie rozpakować plik ${BOOST_ZIP} do folderu ${CMAKE_SOURCE_DIR}/lib.")
    else()
        file(ARCHIVE_EXTRACT INPUT ${BOOST_ZIP} DESTINATION "${CMAKE_SOURCE_DIR}/lib")
    endif()
endif()


# Dodaj Boost (nagłówki tylko)
include_directories(lib/boost-1.87.0)

# Dodaj SFML jako subproject
add_subdirectory(lib/SFML-3.0.0)

# Twój plik źródłowy
add_executable(BHOPAL src/main.cpp)

# Połącz z bibliotekami SFML
target_link_libraries(BHOPAL
    sfml-graphics
    sfml-window
    sfml-system
)

if(WIN32)
    target_link_libraries(BHOPAL ws2_32)
endif()

# Folder wyjściowy aplikacji
set_target_properties(BHOPAL PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/application"
)
