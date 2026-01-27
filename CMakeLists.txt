# 设置最小 CMake 版本
cmake_minimum_required(VERSION 3.19)

# 设置项目名称和版本
project(MarkdownEditor VERSION 0.1 LANGUAGES CXX)

# Qt 元对象系统
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 查找 Qt6
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    WebEngineWidgets
    WebChannel
)

# 源文件
set(PROJECT_SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h    
    src/utils/MarkdownRenderer.cpp
    src/utils/MarkdownRenderer.h
    src/component/Menubar.cpp
    src/component/Menubar.h
    src/component/Toolbar.cpp
    src/component/Toolbar.h
    src/component/Editor.cpp
    src/component/Editor.h
    src/component/Preview.cpp
    src/component/Preview.h
    src/view/ViewPage.cpp
    src/view/ViewPage.h
    src/view/FavoritePage.cpp
    src/view/FavoritePage.h
    src/view/SettingPage.cpp
    src/view/SettingPage.h
)

# 资源文件
qt_add_resources(PROJECT_RESOURCES
    res/icons/icons.qrc
)

# 可执行文件
qt_add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${PROJECT_SOURCES} ${PROJECT_RESOURCES})

# 确保将整个 res 目录（包含 default.md 和 icons）复制到可执行文件同级目录
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"
    COMMENT "Copying all resources to output directory..."
)

# 在此处设置 cmark 库路径（仓库目录为 `3rdpart`，不是 `3rdparty`）
set(CMARK_ROOT "${CMAKE_SOURCE_DIR}/3rdpart/cmark-0.31.1/msvc")

# 优先使用随带的 CMake 配置包（会导入目标 `cmark::cmark`），
# 如果缺失则回退到按路径查找静态库/头文件。
find_package(cmark CONFIG PATHS "${CMARK_ROOT}/lib/cmake" NO_DEFAULT_PATH)
if(TARGET cmark::cmark)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::WebEngineWidgets
        Qt6::WebChannel
        cmark::cmark
    )
else()
    find_library(CMARK_LIB cmark PATHS "${CMARK_ROOT}/lib")
    find_path(CMARK_INC cmark.h PATHS "${CMARK_ROOT}/include")
    if(NOT CMARK_LIB OR NOT CMARK_INC)
        message(FATAL_ERROR "Could not find CMARK library or headers. Set CMARK_ROOT or install cmark.")
    endif()
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMARK_INC})
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::WebEngineWidgets
        Qt6::WebChannel
        ${CMARK_LIB}
    )
endif()

# Windows 下自动部署 Qt 依赖 (修复 fehlenden WebEngineProcess)
if(WIN32)
    # 尝试定位 windeployqt
    if(TARGET Qt6::Core)
        get_target_property(_qt_core_lib Qt6::Core LOCATION)
        get_filename_component(_qt_bin_dir "${_qt_core_lib}" DIRECTORY)
    endif()

    find_program(WINDEPLOYQT_EXE windeployqt HINTS "${_qt_bin_dir}")

    if(WINDEPLOYQT_EXE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${WINDEPLOYQT_EXE}"
                --no-compiler-runtime
                --no-opengl-sw
                --verbose 0
                "$<TARGET_FILE:${PROJECT_NAME}>"
            COMMENT "Running windeployqt..."
        )
    endif()
endif()

# 安装规则
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)