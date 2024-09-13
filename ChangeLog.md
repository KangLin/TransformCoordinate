- v1.1.2
  - 修改作者格式
  - 使用 [RabbitCommon v2.2.6](https://github.com/KangLin/RabbitCommon/releases/tag/v2.2.6)
  
- v1.1.1
  - 修复 debian/rules 并行错误
  - 修复 debian/rules in aarch64
  - 使用 [RabbitCommon v2.2.5](https://github.com/KangLin/RabbitCommon/releases/tag/v2.2.5)

- v1.1.0
  - 修改 TransformCoordinateFiles

- v1.0.4
  - 修改提示信息

- v1.0.3
  - 重命名图标方案 rabbit-*
  - 修改 org.Rabbit.TransformCoordinate.desktop
  - 修改版本格式

- v1.0.2
  - debian: 判断系统中是否安装有RabbitCommon开发库，如果有，则安装到系统。如果没
    有，则从源码编译RabbitCommon，并安装到/opt/TransformCoordinate
  - 重命名 share/TransformCoordinate.desktop 到 share/org.Rabbit.TransformCoordinate.desktop

- v1.0.1
  - 修改文档
  - 修改自动化编译

- v1.0.0
  - 使用 [RabbitCommon v2.2.1](https://github.com/KangLin/RabbitCommon/releases/tag/v2.2.1)

- v0.0.11
  - 修复 Android 错误
  - 用 QFileDialog::getExistingDirectory 替换 RabbitCommon::CDir::GetOpenDirectory

- v0.0.10
  + 修改自动编译
    - 增加 github actions
  + 使用 RabbitCommon V2
    + 增加日志菜单
    + 增加设置样式菜单
  + 使用 CPack 打包

- v0.0.9
  + 修改更新
  + 增加 android 签名
  
- v0.0.8
  + 修改翻译
  + 修改自动化编译
  + 增加运行库安装
  
- v0.0.7
  + 修改翻译
  + 修改打开文件对话框
  + 修改状态栏提示
  + 修改 CMAKE_INSTALL_PREFIX 为 android
  + 修改翻译和 README
  + android 启动 Splash screen 隐藏
  
- v0.0.6
  + 坐标转换
  + 修改翻译
