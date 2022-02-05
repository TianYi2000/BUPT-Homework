# Distributed-Temperature-Control-System
某快捷廉价酒店响应节能绿色环保理念，推行自助计费式中央温控系统，使得入住的客户可以根据要求设定温度和风速的调节，同时可以显示所需支付的金额。客户退房时酒店须出具空调使用的账单及详单。空调运行期间，空调管理员能够监控各房间空调的使用状态，需要的情况下可以生成格式统计报表。


## 后端目录结构
```
source code/
├── class （后端设计）
│   │ └── Config.py: 存储运行参数
│   │ └── Service.py: 控制从控机服务并记录服务数据
│   │ └── ServiceQueue.py: 服务队列，记录当前正在服务的对象
│   │ └── WaitQueue.py: 等待队列，记录当前正在等待的对象
│   │ └── Dispatcher.py: 调度算法，根据规则调度对象并传递操作信息
│   │   
├── database （数据库操作）
│   ├── server
│   │   └── server.py: 
│   │   └── sqldb.py: 数据库操作，如对表的增删改查
└── README.md
```
## 前端目录结构
```
web/
├── README.md
├── package-lock.json:锁定依赖配置文件
├── package.json:依赖配置文件
├── src
│   ├── app.vue:项目启动文件
│   ├── config
│   │   ├── config.js:配置文件
│   │   └── env.js:环境配置文件
│   ├── libs
│   │   └── util.js:js工具库
│   ├── main.js:框架入口
│   ├── router.js:前端路由
│   ├── styles
│   │   └── common.css:公用css文件
│   ├── template
│   │   └── index.ejs:公用模板
│   ├── vendors.js
│   └── views
│       ├── index.vue:客户端界面
│       └── main.vue:主控机页面
├── webpack.base.config.js:基础环境配置
├── webpack.dev.config.js:开发环境配置
└── webpack.prod.config.js:发布环境配置
```
