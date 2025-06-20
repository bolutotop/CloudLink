# CloudLinker

CloudLinker是一个基于Qt的现代化即时通讯和云存储应用程序，提供AI对话、即时通讯和网络磁盘等功能。
## 使用的开源库
- [ElaWidgetTools](https://github.com/Liniyous/ElaWidgetTools) - 提供现代化的UI组件和图标
- [Qt Material Widgets](https://github.com/laserpants/qt-material-widgets) - Material Design风格的Qt部件

## 功能特点

### 1. AI模型对话
- 集成AI模型对话功能
- 智能对话界面
- 实时响应

### 2. 即时通讯
- 私人聊天
- 好友系统
- 实时消息推送
- 聊天气泡界面
- 支持文本消息
- 用户注册和登录系统
- 在线用户查看
- 好友管理（添加、删除、刷新好友列表）
- 在线状态实时提醒

### 3. 网络磁盘
- 文件上传下载
- 文件夹管理（创建、删除、重命名）
- 目录浏览
- 文件共享(待开发)

### 4. 用户界面
- 现代化Material Design和FluentUI风格
- 流畅的动画效果
- 响应式设计
- 切换主题支持
## 技术架构

### 客户端 (CloudLinkerClient)
- 基于Qt 6.9.0开发
- 采用Material Design风格UI和FluentUI组件
- 模块化设计
- 日志系统
- AI对话模块配置

### 服务器端 (CloudLinkerServer)
- 基于Qt框架的TCP服务器
- SQLite数据库
- 线程池管理
- 日志系统

## 系统要求

- 操作系统：Windows
- Qt版本：6.9.0或更高
- 编译器：MinGW 64-bit
- 数据库：SQLite

## 项目结构

```
CloudLinker/
├── CloudLinkerClient/           # 客户端代码
│   ├── AI_Model_Dialogue/      # AI对话模块
│   ├── Friend_chat/           # 好友聊天模块
│   ├── Network_disk/         # 网络磁盘模块
│   ├── Logger/               # 日志模块
│   └── libs/                 # UI组件库
│
├── CloudLinkerServer/          # 服务器代码
│   ├── Logger/               # 日志模块
│   ├── cloud.db             # SQLite数据库
│   └── TreadPool.h          # 线程池实现
```
### 运行
1. 首先启动服务器端程序
2. 然后启动客户端程序
3. 在客户端登录界面输入账号密码

## 开发说明

### 客户端开发
- 使用Qt的信号槽机制实现界面交互
- 采用Material Design风格组件库
- 实现了自定义的聊天气泡组件
- 网络请求使用Qt的网络模块

### 服务器开发
- 使用Qt的TCP服务器框架
- 实现了线程池处理并发请求
- 使用SQLite数据库存储用户数据
- 实现了自定义的通信协议

## 日志系统

项目包含完整的日志系统，记录：
- 用户登录信息
- 系统运行状态
- 错误信息
- 网络连接状态
- 客户端日志位置：`logs/client/`
- 客户端日志格式：`[日期]进程ID_applog.txt`
- 服务器日志位置：`logs/server/`
- 客户端日志格式：`[日期]applog.txt`
- 日志文件位于程序运行目录的logs文件夹下。


## 配置文件

- 客户端配置：`client.config`
- 服务器配置：`client.config`

配置文件包含服务器地址、端口重要参数。

## 协议说明

客户端和服务器之间使用自定义协议通信，主要包括：
- 用户认证
- 消息传输
- 文件传输
- 状态同步

## 安全性
- 文件传输验证
- 用户认证机制

## 未来计划
- [ ] 添加群聊功能
- [ ] 支持更多文件类型
- [ ] 优化AI模型响应速度
- [ ] 添加更多社交功能
- [ ] 实现端到端加密
- [ ] 添加拓展群聊功能
- [ ] 优化为FTP文件传输,传输界面设计
- [ ] AI模型选择
- [ ] 文件共享功能
## 注意事项
- 确保数据库文件`cloud.db`存在且有正确的读写权限
- 检查网络连接和防火墙设置

- 项目使用了ElaWidgetTools库提供的图标，主要包括：
- ArrowLeftLong - 返回按钮
- FolderPlus - 新建文件夹
- FolderArrowUp - 文件上传
- FolderArrowDown - 文件下载
- ShareNodes - 文件分享
- ArrowRotateRight - 刷新
- 需要手动添加加到ui_cloudlinkerclient.h对应的pushbutton的构造中，我重新编写了一下ElaIconButton的构造，默认生成了一个的图标,不然会导致构建错误
- 日志类需要改一下 logger.cpp 9行，把默认的日志目录改成自己的目录 QString customLogDir = "C:/Users/55047/Desktop/myproject/CloudLinker/logs";
- AI对话模型 ai_model_dialogue.cpp 37行改为自己的deepseek APIket     request.setRawHeader("Authorization","Bearer 自己的APIkey");
- 将CloudLinker\CloudLinkerClient\libs下的ElaWidgetTools文件夹复制到客户端输出目录中
## 客户端界面
### 登录界面
- ![Logo 描述文字](/Image/Quicker_20250620_182334.png)

### 注册界面
- ![Logo 描述文字](/Image/Quicker_20250620_182419.png)

### 聊天界面
- ![Logo 描述文字](/Image/Quicker_20250620_184246.png)

### 网盘界面
- ![Logo 描述文字](/Image/Quicker_20250620_185315.png)

### AI聊天界面
- ![Logo 描述文字](/Image/Quicker_20250620_185540.png)

