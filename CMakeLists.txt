cmake_minimum_required(VERSION 3.18)
project(bhopal)

set(CMAKE_CXX_STANDARD 23)

set(GIT_LIB_VERSION "V.1.0.0")
set(SFML_VERSION "SFML-3.0.2")
set(nlohman_VERSION "nlohmann-3.12.0")
set(boost_VERSION "boost-1.90.0")




# Jeśli folder SFML nie istnieje, pobierz i rozpakuj archiwum.
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/lib/${SFML_VERSION}/CMakeLists.txt")
    message(STATUS "SFML nie znaleziono w lib/${SFML_VERSION} Pobieranie i rozpakowywanie archiwum SFML...")
    set(SFML_ZIP "${CMAKE_BINARY_DIR}/${SFML_VERSION}.zip")
    file    (   DOWNLOAD
                "https://github.com/bajraan/BHOPAL-ComDisplayer/releases/download/${GIT_LIB_VERSION}/${SFML_VERSION}.zip"
                ${SFML_ZIP}
                SHOW_PROGRESS
            )
    if(${CMAKE_VERSION} VERSION_LESS "3.18")
        message(WARNING "Automatyczne rozpakowywanie archiwów ZIP wymaga CMake 3.18 lub wyższej. Proszę ręcznie rozpakować plik ${SFML_ZIP} do folderu ${CMAKE_SOURCE_DIR}/lib.")
    else()
        message(STATUS "Rozpakowywanie archiwum do: ${CMAKE_SOURCE_DIR}/lib")
        file(ARCHIVE_EXTRACT INPUT "${SFML_ZIP}" DESTINATION "${CMAKE_SOURCE_DIR}/lib")
        message(STATUS "Sukces! Sprawdź zawartość w: ${CMAKE_SOURCE_DIR}/lib")
    endif()
endif()
# add SFML as subproject
add_subdirectory(lib/${SFML_VERSION})




# Jeśli folder boost nie istnieje, pobierz i rozpakuj archiwum.
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/lib/${boost_VERSION}/boost/asio.hpp")
    message(STATUS "boost nie znaleziono w lib/${boost_VERSION}/boost/asio.hpp downloading and unziping boost...")
    set(BOOST_ZIP "${CMAKE_BINARY_DIR}/${boost_VERSION}.zip")
    file    (   DOWNLOAD
                "https://github.com/bajraan/BHOPAL-ComDisplayer/releases/download/${GIT_LIB_VERSION}/${boost_VERSION}.zip"
                ${BOOST_ZIP}
                SHOW_PROGRESS
            )
    if(${CMAKE_VERSION} VERSION_LESS "3.18")
        message(WARNING "Automatyczne rozpakowywanie archiwów ZIP wymaga CMake 3.18 lub wyższej. Proszę ręcznie rozpakować plik ${BOOST_ZIP} do folderu ${CMAKE_SOURCE_DIR}/lib.")
    else()
        file(ARCHIVE_EXTRACT INPUT ${BOOST_ZIP} DESTINATION "${CMAKE_SOURCE_DIR}/lib")
    endif()
endif()
# add Boost as header-only
include_directories(lib/${boost_VERSION})




# Jeśli folder nlohman nie istnieje, pobierz i rozpakuj archiwum.
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/lib/${nlohman_VERSION}/nlohman/json.hpp")
    message(STATUS "nlohman nie znaleziono w lib/${nlohman_VERSION}/nlohman/json.hpp downloading and unziping nlohman...")
    set(NLOHMAN_ZIP "${CMAKE_BINARY_DIR}/${nlohman_VERSION}.zip")
    file    (   DOWNLOAD
                "https://github.com/bajraan/BHOPAL-ComDisplayer/releases/download/${GIT_LIB_VERSION}/${nlohman_VERSION}.zip"
                ${NLOHMAN_ZIP}
                SHOW_PROGRESS
            )
    if(${CMAKE_VERSION} VERSION_LESS "3.18")
        message(WARNING "Automatyczne rozpakowywanie archiwów ZIP wymaga CMake 3.18 lub wyższej. Proszę ręcznie rozpakować plik ${NLOHMAN_ZIP} do folderu ${CMAKE_SOURCE_DIR}/lib.")
    else()
        file(ARCHIVE_EXTRACT INPUT ${NLOHMAN_ZIP} DESTINATION "${CMAKE_SOURCE_DIR}/lib")
    endif()
endif()
# add nlohmann as header-only
include_directories(lib/${nlohman_VERSION})




# Include project-specific include folders
include_directories(
    src
    src/Config
    src/Utils
    src/DataGrabber
    src/DataDisplayer

)

# Application source files
add_executable(BHOPAL

    src/Config/InterfaceConfigLoader.cpp
    src/DataGrabber/DataGrabbSerial.cpp
    src/DataDisplayer/DataDispSFML.cpp
    src/Utils/AppEnvironment.cpp
    src/main.cpp
    
)

# Połącz z bibliotekami SFML
target_link_libraries(BHOPAL
    sfml-graphics
    sfml-window
    sfml-system
)

if(WIN32)
    target_link_libraries(BHOPAL ws2_32)
endif()

# application output folder
set_target_properties(BHOPAL PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/application"
)

# run target by cmake -> cmake --build . -j8 --target run
add_custom_target(run
    COMMAND $<TARGET_FILE:BHOPAL>
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/application
    DEPENDS BHOPAL
    USES_TERMINAL
)