[Kurosio 的原版](https://www.teeworlds.com/forum/viewtopic.php?id=12612)  [github](https://github.com/kurosio/Teeworlds-Mmotee-Old)

# mmotee (老版本) 来自 Kurosio

# 新功能:
- [ ] **重要**: 将固定在代码中的中文文本移动到 `server_lang/` 中, 让世界各地的玩家可以更方便地参与项目开发, 用自己的语言进行游玩 (注:该操作完成以后. 项目有可能会进行更名)
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


