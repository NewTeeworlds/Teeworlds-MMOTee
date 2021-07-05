[Kurosio 的原版](https://www.teeworlds.com/forum/viewtopic.php?id=12612)

# mmotee (老版本) 来自 Kurosio

# 新功能:
* 投票界面、聊天界面的中文翻译
* ...
# 目录：
1. [构建](#构建)
2. [聊天指令](#聊天指令一览)
3. [控制台指令(管理员)](#控制台指令(管理员))


# 构建:

依赖: (Debian 10)

	libmariadb-dev-compat libmariadb-dev libssl-dev libmysqlcppconn-dev

用 `bam` 构建 (需要版本 `0.4.x`)

	bam server_sql_release

用 Docker 构建([`bam-0.4.0`](https://github.com/matricks/bam/) 会被自动编译。)

    ./build.sh 

**注意**: 只有 SQL 版本能构建成功。

# 聊天指令一览： ()为可选项，<>为必填项
1. 注册与登录
  * `/login` (用户名) <密码> 登录
  * `/register` <用户名> <密码> 注册
2. 交换物品
  * `/giveitem` <玩家ID> <物品ID> <物品数量> 给某人物品 
  * `/sendmail` <玩家ID> <物品ID> <物品数量> 通过邮件向某人发送物品
  * `/givedonate` <玩家ID> <黄金数量> 捐款(?)
3. 公会指令
  * `/newclan` <公会名称> 创建公会
  * `/invite` <玩家名称> 邀请玩家进入公会 (需要公会所有者权限)
4. 其他常用指令
  * `/cmdlist` 显示命令列表 (不全)
  * `/createboss` 创建boss
  * `/lang` (语言ID) 设置语言 (留空显示可用语言列表)
5. 未知指令
  * `/sd` <声音ID> 设置声音(?)
  * `/trah` <玩家ID> 与某玩家做爱(雾)

# 控制台指令(管理员)
1. 用户控制
  * `ban <玩家ID> (时长) (原因)` 封禁玩家