# bean_eater

一个用 C++ 和 SFML3.0 编写的吃豆人风格小游戏。

## 🎮 说明

- **方向键** 或 **WASD**：移动角色

- 在豆子消失前吃到豆子即可加分，没吃到会扣分

- 碰墙 或 得分为零时被扣分 则扣血，血量为0则游戏结束

- 幽灵会在达到对应分数后出现，达到目标分数幽灵会加速。被碰到会扣血

  

## 🛠️ 构建与运行

本项目使用 Windows 下 msys2 环境中的 LLVM Clang++ 编译器。
使用 CMake 构建与 CPack 打包

### 依赖项

- CMake 3.28 或更高版本
- SFML 3.*
- 支持 C++23 的编译器（GCC 13+ / Clang 16+ / MSVC 2022）

### 使用 CMakePresets（推荐）

项目提供了两个预设：`Debug` 和 `Release`。

### 打包与分发

使用 CPack 生成可独立运行的 ZIP 包（包含所有依赖 DLL 和资源文件）：

```
cd build/release   # 或你构建的目录
cpack
```

生成的 `bean_eater-0.0.x.zip` 解压后可直接运行 `bean_eater.exe`，无需额外安装 SFML库依赖。

## 🙏 致谢

- 字体：郑庆科南北词宫谱宋体
- 音乐：好友 ZhangLunch 自制 I eat so I'm here 和 Bean's demised
- 音效：我的世界早期史蒂夫受击音效、植物大战僵尸失败音效、不知名网友提供音效

- 特别感谢鼓励我、支持我，为我提建议的 TangBoat、ZhangLunch。他们的支持是我维护项目的最大动力


## 📜 许可证

本项目使用 Apache License 2.0 许可。详见仓库中的 LICENSE.md 文件。
