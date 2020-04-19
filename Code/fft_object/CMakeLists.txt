cmake_minimum_required(VERSION 3.2)

add_compile_options(-Wall -Wextra -Wpedantic)

project(PitchPerfector)

#add_custom_target(target COMMAND qmake && make run
#                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#)

execute_process(COMMAND chmod +x ${CMAKE_CURRENT_SOURCE_DIR}/installDependencies.sh)
execute_process(COMMAND .${CMAKE_CURRENT_SOURCE_DIR}/installDependencies.sh)

#add_custom_target(install_dependencies COMMAND chmod +x installDependencies.sh && ./installDependencies.sh
#                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#)

# -------------------------------------------
if (Qt5Gui_FOUND)
  get_target_property(QT_INCLUDE_DIR Qt5::Gui INTERFACE_INCLUDE_DIRECTORIES)
endif ()

find_path ( QWT_INCLUDE_DIR
  NAMES qwt_plot.h
  HINTS ${QT_INCLUDE_DIR}
  PATH_SUFFIXES qwt qwt-qt5 qwt6
)

set ( QWT_INCLUDE_DIRS ${QWT_INCLUDE_DIR} )

# version
set ( _VERSION_FILE ${QWT_INCLUDE_DIR}/qwt_global.h )
if ( EXISTS ${_VERSION_FILE} )
  file ( STRINGS ${_VERSION_FILE} _VERSION_LINE REGEX "define[ ]+QWT_VERSION_STR" )
  if ( _VERSION_LINE )
    string ( REGEX REPLACE ".*define[ ]+QWT_VERSION_STR[ ]+\"([^\"]*)\".*" "\\1" QWT_VERSION_STRING "${_VERSION_LINE}" )
  endif ()
endif ()
unset ( _VERSION_FILE )

find_library ( QWT_LIBRARY
  NAMES qwt qwt-qt5
  HINTS ${QT_LIBRARY_DIR}
)

set ( QWT_LIBRARIES ${QWT_LIBRARY} )

include ( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Qwt REQUIRED_VARS QWT_LIBRARY QWT_INCLUDE_DIR VERSION_VAR QWT_VERSION_STRING )

if (Qwt_FOUND AND NOT TARGET Qwt::Qwt)
  add_library(Qwt::Qwt UNKNOWN IMPORTED)
  set_target_properties(Qwt::Qwt PROPERTIES
                        INTERFACE_INCLUDE_DIRECTORIES "${QWT_INCLUDE_DIRS}"
                        IMPORTED_LOCATION "${QWT_LIBRARIES}")
endif ()

mark_as_advanced (
  QWT_LIBRARY
  QWT_INCLUDE_DIR
)

# -------------------------------------------

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(Qt5_DIR /usr/lib/x86_64-linux-gnu/cmake/Qt5)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

#install(TARGETS librtaudio-dev)
#install(TARGETS libfftw3-dev)

#find_package(Qt5 5.1.0 CONFIG REQUIRED Core Widgets Gui)
#find_package(Qt5 CONFIG REQUIRED Core Gui Widgets)
#find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)

include_directories(.)
include_directories(/usr/include)
include_directories(/usr/include/rtaudio)
include_directories(/usr/lib)

add_executable(PitchPerfector main.cpp window.h audioStreamer.h fft.h vocoder.h helper.h dispatch.h voxFilter.h window.cpp audioStreamer.cpp fft.cpp vocoder.cpp helper.cpp dispatch.cpp voxFilter.cpp)

#target_link_libraries(PitchPerfector Qt5::Widgets)
target_link_libraries(PitchPerfector rtaudio)
target_link_libraries(PitchPerfector fftw3)
target_link_libraries(PitchPerfector /usr/lib/libqwt-qt5.so)


