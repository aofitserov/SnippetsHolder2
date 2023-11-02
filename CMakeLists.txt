cmake_minimum_required(VERSION 3.5)

project(SnippetsHolder2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(OPERATION_SYSTEM "Linux")
else()
    set(OPERATION_SYSTEM "Win")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set (Root                     ${CMAKE_CURRENT_SOURCE_DIR})
set (DataModel                ${Root}/DataModel)
set (Managers                 ${Root}/Managers)
set (Helpers                  ${Root}/Helpers)
set (Types                    ${Root}/Types)

set(BuildTypeDir ${CMAKE_BUILD_TYPE}_${OPERATION_SYSTEM})
set (BIN_DIR "${Root}/Build/Bin/${BuildTypeDir}")

include_directories(${Root}
                    ${DataModel}
                    ${Managers}
                    ${Helpers}
                    ${Types})

set(PROJECT_SOURCES
        main.cpp
        MainWidget.cpp
        MainWidget.h
        MainWidget.ui
        ${DataModel}/SettingsDesc.h
        ${DataModel}/SnippetDesc.h
        ${DataModel}/SnippetsHolderDesc.h
        ${DataModel}/SnippetsHolderModel.h
        ${DataModel}/SnippetsTabDesc.h
        ${Managers}/qserializer.h
        ${Managers}/SnippetsManager.cpp
        ${Managers}/SnippetsManager.h
        ${Helpers}/JsonFile.cpp
        ${Helpers}/JsonFile.h
        ${Helpers}/TabEditorWatcher.cpp
        ${Helpers}/TabEditorWatcher.h
        ${Helpers}/MouseWatcher.cpp
        ${Helpers}/MouseWatcher.h
        ${Types}/SingletonSharedPtrHolder.h

)

add_executable(${PROJECT_NAME} WIN32
    ${PROJECT_SOURCES}
)

message("o---------- ${PROJECT_NAME} ------------o")
message("o-> PROJECT_NAME: ${PROJECT_NAME}")
message("o-> CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message("o-> CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message("o-> PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
message("o-> PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message("o-> CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message("o-> CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message("o-> BIN_DIR: ${BIN_DIR}")
message("o----------------------------------------o")

target_link_libraries(SnippetsHolder2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(${PROJECT_NAME}
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR}
  LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR}
  RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)

add_compile_definitions(QS_HAS_JSON)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstrict-aliasing -Wall -Wextra -Wno-comment -Wdouble-promotion -Wswitch-default -Wswitch-enum -Wuninitialized -Wstrict-aliasing -Wfloat-equal -Wshadow -Wplacement-new -Wcast-align -Wconversion -Wlogical-op")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wduplicated-cond -Wduplicated-branches -Wrestrict -Wnull-dereference -Wno-terminate")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")
set(CMAKE_VERBOSE_MAKEFILE ON)
