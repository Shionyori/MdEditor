
# MarkdownEditor

一个轻量级的本地 Markdown 编辑器（学习用），使用 C++ 与 Qt 构建，基于 CMake 管理工程。

**主要特性**
- 实时 Markdown 编辑与预览
- 支持常见 Markdown 语法（基于 cmark）
- 资源文件与图标打包（使用 Qt 资源系统）

**需求**
- C++17 编译器
- CMake 3.10+
- Qt 6.5.3
- 已包含的第三方库：`3rdpart/cmark-0.31.1`（源码与构建配置已随仓库提供）

**构建（通用）**
建议在项目根目录执行以下命令（示例使用 Ninja）：

```powershell
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

如果使用 MinGW + Ninja / MSYS2，可同样指定相应生成器；若使用 Visual Studio，请在 CMake 配置时选择相应的 Visual Studio 生成器并用 `--config Release` 指定配置。

构建完成后，二进制文件位于 `build` 目录（例如 Windows 下为 `build/MarkdownEditor.exe`）。

**运行**
- 在构建目录下直接运行可执行文件，或通过 IDE 的调试/运行配置启动。
