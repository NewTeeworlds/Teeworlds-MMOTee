[Orignial by Kurosio](https://www.teeworlds.com/forum/viewtopic.php?id=12612)

# mmotee (old version) by Kurosio

Feature(s):
* Vote menu translation in Chinese
* ...

# 0 build:

Dependencies: (Debian 10)

	libmariadb-dev-compat libmariadb-dev libssl-dev libmysqlcppconn-dev

build with bam (version 0.4.x required)

	bam server_sql_release

build with docker ([`bam-0.4.0`](https://github.com/matricks/bam/) will be automatically compiled.)

    ./build.sh 

Notice: only sql version could be build.

# 聊天指令一览： ()为可选项，<>为必填项
## 1.注册与登录
`/login` (用户名) <密码> 登录
`/register` <用户名> <密码> 注册
## 2.交换物品
`/giveitem` <玩家ID> <物品ID> <物品数量> 给某人物品 
`/sendmail` <玩家ID> <物品ID> <物品数量> 通过邮件向某人发送物品
`/givedonate` <玩家ID> <黄金数量> 捐款(?)
## 3.公会指令
`/newclan` <公会名称> 创建公会
`/invite` <玩家名称> 邀请玩家进入公会 (需要公会所有者权限)
## 4.其他常用指令
`/cmdlist` 显示命令列表 (不全)
`/createboss` 创建boss
`/lang` (语言ID) 设置语言 (留空显示可用语言列表)
## 5.未知指令
`/sd` <声音ID> 设置声音(?)
`/trah` <玩家ID> 与某玩家做爱(雾)