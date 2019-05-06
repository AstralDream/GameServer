# 游戏服务器框架

c++游戏服务器框架，上层绑定lua，功能分离，可复用性高，支持数据缓存，读写速度快   
![error](https://WKnvNs4Ypi8/s400/IMG_0012.jpg)

## 环境部署

``` 
$ 开发环境：VS2017
$ 需要安装：mysql
$ 辅助工具：协议生成工具，ExcelToJson工具
$ lua编辑工具：luastudio
```

## 服务器模块
### 1.Gate<网关服务器>  
- 主要处理客户端的包筛选，和一些防攻击逻辑，包的加密也可以在这层做
- 网关服务器会连接世界服务器
- 客户端直接连接网关服务器
- 由于网关服务器是可以多开的，所以当网关服务器都开启的情况下，客户端可以随机连接一个网   
### 2.DB<数据库服务器>   
- 主要连接mysql服务器，进行数据的存储
- 世界服务器会连接数据库服务器，来进入玩家数据的存储    
### 3.World<世界服务器>   
- 管理所有网关服务器的连接
- 然后去连接数据库服务器
- 主要功能是管理所有的玩家，处理所有的玩家逻辑，还有其它所有的功能逻辑模块

## 目录结构
### src:主要的代码目录   
- Core:主要是一些核心的代码和一些第3方库文件 
- Common:放一些可以共用的代码，比如协议文件和配置读取文件，还有一些共用的代码
- protocols：协议文件和协议工具，现在可以生成C#的文件和C++文件，其它语言可以扩展
- DB:数据库服务器的主要代码 
- Gate:网关服务器的主要代码
- World：世界服务器的主要代码   
### bin:主要生成后的exe文件，还有一些配置文件，和3方库dll  
-   DB:   
	$ config.json:数据服务器的启动配置   
	$ game.sql：生成数据库文件
	$ msgConfig.json：错误代码文件
	$ libmySQL.dll：mysql动态库   
-   Gate:   
	$ config.json:网关服务器的启动配置 
	$ msgConfig.json：错误代码文件    
-   World:   
	$ config.json:世界服务器的启动配置 
	$ msgConfig.json：错误代码文件
	$ config：逻辑表格的配置目录，里面有Excel转Json表格工具   
-   build：项目工程文件
-   client：是用来测试的机器人工程，可以不管



## 框架已经完成模块
1. 支持角色登陆，角色数据的存储都已经完成,可以直接进行逻辑开发
2. 协议只需要在xml中定义，用工具直接生成代码 ，协议交互都是以事件形式进行响应（具体使用配置文件暂时用json来读取，用工具转换成json来使用
3. 支持lua脚本，可以用脚本来做热更新，luabind都已经封装好了，函数的相互调用只要注册就可以
4. mysql中数据的查找和存储不用写sql语句，可以用结构体直接映射，不用拼语句那么麻烦，这些需要加的功能模块，可以直接继承Module类进行逻辑操作


**如果你喜欢这个游戏，请点星！！！，或与我共同改进此游戏** 