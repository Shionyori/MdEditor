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

configure_file(res/default.md ${CMAKE_BINARY_DIR}/res/default.md COPYONLY)

# 可执行文件
qt_add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${PROJECT_SOURCES} ${PROJECT_RESOURCES})

# 在此处设置 cmark 库路径（仓库目录为 `3rdpart`，不是 `3rdparty`）
set(CMARK_ROOT "${CMAKE_SOURCE_DIR}/3rdpart/cmark-0.31.1")

# 优先使用随带的 CMake 配置包（会导入目标 `cmark::cmark`），
# 如果缺失则回退到按路径查找静态库/头文件。
find_package(cmark CONFIG PATHS "${CMARK_ROOT}/lib/cmake" NO_DEFAULT_PATH)
if(TARGET cmark::cmark)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
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
        ${CMARK_LIB}
    )
endif()

# 安装规则
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)