[Kurosio 的原版](https://www.teeworlds.com/forum/viewtopic.php?id=12612)  [github](https://github.com/kurosio/Teeworlds-Mmotee-Old)

# mmotee (老版本) 来自 Kurosio

# 新功能:
- [ ] **重要**将固定在代码中的中文文本移动到 `data/languages/json` 中, 让世界各地的玩家可以更方便地参与项目开发, 用自己的语言进行游玩 (注:该操作完成以后. 项目有可能会进行更名)
- [x] 投票界面、聊天界面的中文翻译
- [x] 去除 Pig, Kwah, Boomer 等等怪物在右上角击杀 HUD 的显示
- [x] **重要**: 修复在新发行版上构建失败的问题
- [x] 使用 `CMake` 替代过时的 `bam` 来构建
- [x] 将萃取上限提高至 50000,优化玩家体验
- [x] **重要**:加入密码修改功能
- [x] 修复卡锤问题 (测试中)
- [x] 批量领取邮件(一次领取当前显示的20个邮件)
- [x] 新地图(不断修改中)
- [x] 优化数据库结构,合并大量重复的邮箱信息,提升性能
- [x] 批量开箱
- [x] 修复一系列失效的控制台指令(例如 `ban`)
- [x] 增加一些有用的指令(例如数据库交互，修改玩家密码、物品等等)
- [x] 表情模块的手动开启与更换
- [ ] 增加新的 boss,按等级划分;提高 boss 战的奖励
- [ ] 增加 PvP 专用竞技场 & PvP 邀请赛
- [ ] 增加新的技能,例如来自 InfClass 的地雷,白洞,还有来自玩家构思的治疗,护盾等





**以下 Wiki 内容的一部分来自 [TeeWorldsCN Wiki](https://wiki.teeworlds.cn/mods:mmotee)**

# 目录：
1. [构建](#构建)
2. [聊天指令](#聊天指令一览)
3. [控制台指令(管理员)](#控制台指令)
4. [3种盒子的概率](#3种盒子的概率)
5. [各种称号的作用](#各种称号的作用)

## 构建:

依赖: (Debian 10)

	libmariadb-dev-compat libmariadb-dev libssl-dev libmysqlcppconn-dev libicu-dev

用 `bam` 构建 (需要版本 `0.4.x`)

	bam server_sql_release
**注意**: 只有 SQL 版本能构建成功。

用 `Docker` 构建([`bam-0.4.0`](https://github.com/matricks/bam/) 会被自动编译。)
**说明**:目前根目录下的 `bam` 是在 Debian 10 x86_64 上构建的 `bam-0.4.0`,默认直接使用。

    ./build.sh 

用 `CMake` 构建(测试中)

    mkdir build
    cd build
    cmake ..
    ninja build.ninja



## 聊天指令一览
()为可选项，<>为必填项
1. 注册与登录
  * `/login` (用户名) <密码> 登录
  * `/register` <用户名> <密码> 注册
  * `/password` <密码> 修改密码
2. 公会指令
  * `/newclan` <公会名称> 创建公会
  * `/invite` <玩家名称> 邀请玩家进入公会 (需要公会所有者权限)
3. 其他常用指令
  * `/cmdlist` 显示命令列表 (不全)
  * `/createboss` 创建boss
  * `/lang` (语言ID) 设置语言 (留空显示可用语言列表)

## 控制台指令
仅管理员可用
1. 用户控制
  * `ban <玩家ID> (时长) (原因)` 封禁玩家
  * `kick` <玩家ID> 踢出玩家

## 3种盒子的概率
游戏内共有 3 种盒子,打开它们有机会获得不同的物品,概率如下:
1. Farming Box 概率：
 - 92% 中，5 个种地经验值，2 个钱袋，5 个 Event Box 各占三分之一
 - 剩下 8% 中，Jump Impuls 占 50%,Rare Freeazer 和 Rare Slime Dirt 各占 25%
2. Event Box 概率：
 - 79/80 概率为钱袋
 - 1/80 概率为 Rare Event Hammer
3. Rand Craft Box 概率：
 - 95% 中的 50% 为 Body Boomer，50% 为 Foot Kwah
 - 5% 中，耳环蓝图、戒指蓝图、武器蓝图、Rare Slime Dirt各占 25%

 ## 各种称号的作用
 1. Title Enchant
  - 获得方法:成功附魔一个等级为 10 级的胸甲/鞋子
  - 加成:生命值 +1000,护盾 +1000
 2. Title PigPig
  - 获得方法:拥有 1000 个 Quest Pig Meat (即猪肉,通过打 Pig 掉落)
  - 加成:商店物品打 95 折
 3. Title Boss Die
  - 获得方法:打败一次 Slime
  - 加成:生命值 +1000
 4. Title Qusets
  - 获得方法:完成所有任务
  - 加成:生命值 +1500,护盾 +500
 5. Title Summer
  - 获得方法:成功合成 Skill Summer Healing
  - 加成: (?)
 6. Title Big Craft
  - 获得方法:成功合成 End Explosion 模块
  - 加成: (?)
 7. Title VIP
  - 获得方法:充钱(暂不开放)
  - 加成:获得翻倍黄金
