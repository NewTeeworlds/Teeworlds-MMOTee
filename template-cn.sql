-- MySQL dump 10.17  Distrib 10.3.25-MariaDB, for debian-linux-gnu (x86_64)
--
-- Host: 10.66.173.119    Database: mmohard
-- ------------------------------------------------------
-- Server version	5.7.18-txsql-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `tw_Clans`
--

DROP TABLE IF EXISTS `tw_Clans`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tw_Clans` (
  `ClanID` int(11) NOT NULL AUTO_INCREMENT,
  `Clanname` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT 'NOPE',
  `LeaderName` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT 'NOPE',
  `LeaderID` int(11) NOT NULL,
  `AdminName` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT 'NOPE',
  `Level` int(11) NOT NULL DEFAULT '1',
  `Exp` int(11) NOT NULL DEFAULT '0',
  `MaxNum` int(11) NOT NULL DEFAULT '2',
  `Money` bigint(32) NOT NULL DEFAULT '0',
  `Relevance` int(11) NOT NULL DEFAULT '0',
  `MoneyAdd` int(11) NOT NULL DEFAULT '0',
  `ExpAdd` int(11) NOT NULL DEFAULT '0',
  `SpawnHouse` tinyint(4) NOT NULL DEFAULT '0',
  `ChairHouse` int(11) NOT NULL DEFAULT '0',
  `CreateDate` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`ClanID`) USING BTREE,
  KEY `ClanID` (`ClanID`) USING BTREE,
  KEY `Clanname` (`Clanname`) USING BTREE,
  KEY `Level` (`Level`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_Clans`
--

LOCK TABLES `tw_Clans` WRITE;
/*!40000 ALTER TABLE `tw_Clans` DISABLE KEYS */;
INSERT INTO `tw_Clans` VALUES (1,'Star','天上的星星',1,'Nope',4,2595550,20,5001,16164,53,131,0,0,'2021-08-04 14:00:00');
/*!40000 ALTER TABLE `tw_Clans` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_Mail`
--

DROP TABLE IF EXISTS `tw_Mail`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tw_Mail` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `IDOwner` int(11) NOT NULL,
  `MailType` int(11) NOT NULL DEFAULT '0',
  `ItemID` int(11) DEFAULT NULL,
  `ItemCount` int(11) NOT NULL,
  PRIMARY KEY (`ID`) USING BTREE,
  KEY `IDOwner` (`IDOwner`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_Mail`
--

LOCK TABLES `tw_Mail` WRITE;
/*!40000 ALTER TABLE `tw_Mail` DISABLE KEYS */;
INSERT INTO `tw_Mail` VALUES (1,1,1,1,10);
/*!40000 ALTER TABLE `tw_Mail` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_Materials`
--

DROP TABLE IF EXISTS `tw_Materials`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tw_Materials` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `Materials` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_Materials`
--

LOCK TABLES `tw_Materials` WRITE;
/*!40000 ALTER TABLE `tw_Materials` DISABLE KEYS */;
INSERT INTO `tw_Materials` VALUES (1,16956962),
(2,13580910),
(3,1156871);
/*!40000 ALTER TABLE `tw_Materials` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_Users`
--

DROP TABLE IF EXISTS `tw_Users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tw_Users` (
  `UserId` int(11) NOT NULL AUTO_INCREMENT,
  `Username` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `Nick` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `Email` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `PasswordHash` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `Level` int(11) NOT NULL DEFAULT '1',
  `Exp` int(11) NOT NULL DEFAULT '0',
  `Money` bigint(32) NOT NULL DEFAULT '100',
  `Gold` bigint(20) NOT NULL DEFAULT '0',
  `Donate` int(11) NOT NULL DEFAULT '0',
  `Rel` int(11) NOT NULL DEFAULT '0',
  `Jail` tinyint(1) NOT NULL DEFAULT '0',
  `IsJailed` int(11) NOT NULL DEFAULT '0',
  `JailLength` int(11) NOT NULL DEFAULT '0',
  `SummerHealingTimes` int(11) NOT NULL DEFAULT '0',
  `Class` int(11) NOT NULL DEFAULT '0',
  `ClanAdded` bigint(20) NOT NULL DEFAULT '0',
  `ClanID` int(11) NOT NULL DEFAULT '0',
  `Quest` int(11) NOT NULL DEFAULT '1',
  `QuestDat` int(11) NOT NULL DEFAULT '0',
  `Seccurity` tinyint(4) NOT NULL DEFAULT '0',
  `Killing` int(11) NOT NULL DEFAULT '0',
  `WinArea` int(11) NOT NULL DEFAULT '0',
  `SettingsChat` int(11) NOT NULL DEFAULT '0',
  `SettingsDrop` int(11) NOT NULL DEFAULT '0',
  `RegisterDate` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `RegisterIp` varchar(64) NOT NULL,
  PRIMARY KEY (`UserId`) USING BTREE,
  KEY `UserId` (`UserId`) USING BTREE,
  KEY `Username` (`Username`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_Users`
--

LOCK TABLES `tw_Users` WRITE;
/*!40000 ALTER TABLE `tw_Users` DISABLE KEYS */;
INSERT INTO `tw_Users` VALUES 
(1,'test','天上的星星','null','505a47881334859c582f2731b59fac1e',212,1355885,8203,22959,0,0,0,0,0,0,2,0,1,7,0,1,53301,1,0,0,'2021-08-04 14:00:00','127.0.0.1');
/*!40000 ALTER TABLE `tw_Users` ENABLE KEYS */;
UNLOCK TABLES;

-- 密码是978798

--
-- Table structure for table `tw_uClass`
--

DROP TABLE IF EXISTS `tw_uClass`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tw_uClass` (
  `UserID` int(11) NOT NULL,
  `Username` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `Upgrade` int(11) NOT NULL DEFAULT '0',
  `SkillPoint` int(11) NOT NULL DEFAULT '0',
  `Damage` int(11) NOT NULL DEFAULT '0',
  `Speed` int(11) NOT NULL DEFAULT '0',
  `Health` int(11) NOT NULL DEFAULT '0',
  `HPRegen` int(11) NOT NULL DEFAULT '0',
  `AmmoRegen` int(11) NOT NULL DEFAULT '0',
  `Ammo` int(11) NOT NULL DEFAULT '0',
  `Spray` int(11) NOT NULL DEFAULT '0',
  `Mana` int(11) NOT NULL DEFAULT '0',
  `HammerRange` int(11) NOT NULL DEFAULT '0',
  `Pasive2` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`UserID`) USING BTREE,
  KEY `Username` (`Username`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_uClass`
--

LOCK TABLES `tw_uClass` WRITE;
/*!40000 ALTER TABLE `tw_uClass` DISABLE KEYS */;
INSERT INTO `tw_uClass` VALUES (1,'test',600,300,1000,400,1000,400,400,500,30,200,8,8);
/*!40000 ALTER TABLE `tw_uClass` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_uItemList`
--

DROP TABLE IF EXISTS `tw_uItemList`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tw_uItemList` (
  `il_id` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `item_type` int(11) DEFAULT '0',
  `item_name` char(50) NOT NULL,
  `item_desc` char(100) NOT NULL DEFAULT '- 无描述',
  PRIMARY KEY (`il_id`) USING BTREE,
  KEY `item_type` (`item_type`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=130 DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_uItemList`
--

LOCK TABLES `tw_uItemList` WRITE;
/*!40000 ALTER TABLE `tw_uItemList` DISABLE KEYS */;
INSERT INTO `tw_uItemList` VALUES 
(1,4,'钱袋','- 随机获取5-20000白银'),
(2,3,'猪肉','- 任务物品'),
(3,3,'Kwah 的头','- 任务物品'),
(4,1,'钩子伤害','- 武器/武器加成'),
(5,1,'手枪','- 武器'),
(6,1,'霰弹枪','- 武器'),
(7,1,'榴弹炮','- 武器'),
(8,1,'激光枪','- 武器'),
(9,2,'克里文护身符','- 每次升级获得20个钱袋'),
(10,2,'自我免伤戒指','- 不会受到来自自己的伤害(如爆炸)'),
(11,5,'表情-快乐模块','- 表情(自动使用)。集齐所有表情可以合成表情模块。'),
(12,5,'表情-愤怒模块','- 表情(自动使用)。集齐所有表情可以合成表情模块。'),
(13,5,'表情-惊讶模块','- 表情(自动使用)。集齐所有表情可以合成表情模块。'),
(14,5,'表情-眨眼模块','- 表情(自动使用)。集齐所有表情可以合成表情模块。'),
(15,5,'表情-痛苦模块','- 表情(自动使用)。集齐所有表情可以合成表情模块。'),
(16,4,'公会创建票','- 拥有才可创建公会'),
(17,2,'表情模块','- 表情(自动使用)。'),
(18,4,'α 箱子','- 注册奖励'),
(19,4,'技能点箱子','- VIP 奖励,包含20升级点+10技能点'),
(20,2,'皮革','- 用于合成'),
(21,2,'特效-生命恢复','- 神器｜点满生命恢复获得的特效'),
(22,2,'特效-伤害','- 神器｜点满伤害获得的特效'),
(23,2,'特效-射速','- 神器｜点满射速获得的特效'),
(24,2,'特效-子弹恢复','- 神器｜点满子弹恢复获得的特效'),
(25,2,'神器-Slime 的戒指','- 神器|在不受自身伤害的情况下让你进行榴弹跳'),
(26,5,'Slime 的尸体','- 用于合成'),
(27,5,'戒指蓝图','- 用于合成'),
(28,1,'手枪模块-爆炸','- 让你的子弹爆炸!'),
(29,1,'霰弹枪模块：爆炸','- 让你的子弹爆炸!'),
(30,1,'激光枪模块：爆炸','- 让你的激光爆炸!'),
(31,1,'弹夹','- 所有武器最大子弹上限+5'),
(32,5,'Boomer 的尸体','用于合成'),
(33,2,'神器-Boomer的戒指','神器 | 生命上限+5%/用于合成'),
(34,5,'耳环蓝图','用于合成耳环'),
(35,5,'武器蓝图','用于合成'),
(36,1,'模块：Slime 的霰弹枪','- 让你的子弹能够反弹!'),
(37,1,'神器-Kwah的耳环','跳跃段数+1'),
(38,5,'Kwah 的脚','用于合成'),
(39,4,'黄金书(10分钟)','黄金获取*2'),
(40,4,'经验书(10分钟)','经验获取*2'),
(41,2,'双重联合戒指','神器｜让守卫的愤怒值每秒额外-10'),
(42,1,'钩子模块：爆炸','- 钩子勾住的敌人会爆炸'),
(43,1,'自动锤子/刀模块','- 长按可一直挥动锤子/刀'),
(44,1,'自动手枪','- 长按手枪可一直射击'),
(45,1,'手枪模块：穿墙','- 手枪子弹可穿过可勾的墙体'),
(46,1,'霰弹枪模块：穿墙','- 霰弹枪子弹可穿过可勾的墙体'),
(47,1,'榴弹模块：穿墙','- 榴弹可穿过可勾的墙体'),
(48,2,'白房门票','- 用它进入白房'),
(49,1,'神器-冰冻','- 1%概率冻结敌人1秒(Slime 3秒)'),
(50,4,'合成用物品盲盒','- 随机获得合成用物品'),
(51,4,'随机盲盒','- 随机获得钱袋、神器'),
(52,1,'神器-随机锤子','- 特效 & 暴击伤害+5'),
(53,1,'手枪模块：反弹','- 手枪子弹落地可以反弹'),
(54,1,'超能霰弹枪','- 让你的霰弹枪前后齐发！'),
(55,1,'榴弹模块：反弹','- 榴弹落地可以反弹'),
(56,4,'月球 氧气/水','- 用于月球区域'),
(57,1,'追踪锤','- 锤子可以发射远程追踪攻击'),
(58,3,'僵尸的脑子','- 任务物品'),
(59,5,'僵尸的眼睛','- 用于合成'),
(60,5,'僵尸的大眼睛','- 用于合成'),
(61,5,'Skelet 的骨头','- 用于合成'),
(62,5,'Skelet 强化骨','- 用于合成'),
(63,3,'Skelet 的头骨','- 任务物品'),
(64,5,'Nimf 的炉子','- 用于合成'),
(65,5,'Nimf 的耳朵','- 用于合成'),
(66,3,'Nimf 的尸体','- 任务物品'),
(67,4,'公会经验包','- 公会经验+50000'),
(68,2,'灵魂','- 可以使用自定义皮肤'),
(69,4,'升级点重置','- 重置升级点'),
(70,4,'技能点重置','- 仅重置技能点'),
(71,1,'终极爆炸','- 增强爆炸效果'),
(72,10,'聊天','- 基本设置'),
(73,10,'捡起物品','- 基本设置'),
(74,10,'技能-激光墙','- 魔能技能'),
(75,10,'技能-治疗','- 魔能技能'),
(76,4,'VIP 包','- 包含钱袋,专属特效等物品'),
(77,10,'PVP设置','- VIP用'),
(78,2,'VIP 特效','- VIP 专属特效'),
(79,12,'称号 VIP','- 黄金与经验加倍'),
(80,10,'技能-光剑','- 魔能技能'),
(81,12,'称号-Boss克星','- 最大生命 +1000'),
(82,12,'称号-猪猪','- 商店打95折'),
(83,12,'称号-合成大师','- 无增益'),
(84,10,'技能-沐浴阳光','- 立即回复大量生命'),
(85,5,'日曜','- 用于合成 技能-沐浴阳光'),
(86,12,'称号-阳光','- 活动称号,无加成'),
(87,4,'土豆','- 获得25经验'),
(88,4,'番茄','- 获得15经验'),
(89,4,'胡萝卜','- 获得10经验'),
(90,2,'超级跳跃','- 第一段跳高度增加'),
(91,10,'农耕经验','- 农耕经验'),
(92,4,'农耕盲盒','- 随机获得农耕经验、随机盲盒、超级跳、神器等'),
(93,12,'称号 任务达人','- +1500生命、500护甲'),
(94,5,'木材','- 用于合成'),
(95,5,'铜矿石','- 用于合成'),
(96,6,'铜镐','- 用于挖矿'),
(97,5,'铁矿石','- 用于合成'),
(98,6,'铁镐','- 用于挖矿'),
(99,5,'金矿石','- 用于合成'),
(100,6,'金镐','- 用于挖矿'),
(101,5,'钻石','- 用于合成'),
(102,6,'钻石镐','- 用于挖矿'),
(103,10,'挖矿经验','- 挖矿经验'),
(104,5,'大号木头','- 用于合成'),
(105,5,'龙矿石','- 用于合成'),
(106,3,'灵魂碎片','- 在线奖励中随机得到,25个灵魂碎片可以合成1个灵魂'),
(107,6,'材料','- 用于附魔、商店等'),
(108,10,'萃取经验','- 萃取经验'),
(109,1,'爱心榴弹','- 50%几率消耗1魔能'),
(110,15,'皮革胸甲','- 生命+100,护甲+100'),
(111,16,'皮革鞋子','- 生命+50,护甲+50'),
(112,17,'伤害增强','- 增加伤害'),
(113,15,'铜质胸甲','- 生命+150,护甲+150'),
(114,16,'铜质鞋子','- 生命+100,护甲 +100'),
(115,15,'铁质胸甲','- 生命+200,护甲 +200'),
(116,16,'铁质鞋子','- 生命+150,护甲 +150'),
(117,15,'金质胸甲','- 生命+250,护甲+250'),
(118,16,'金质鞋子','- 生命+150,护甲+150'),
(119,15,'钻石胸甲','- 生命+300,护甲+300'),
(120,16,'钻石鞋子','- 生命+250,护甲+250'),
(121,15,'龙胸甲','- 生命+500,护甲+500'),
(122,16,'龙鞋子','- 生命+400,护甲+400'),
(123,10,'魔法师称号','- 生命+1000,护甲+1000'),
(124,6,'龙斧','- 砍树速度加倍'),
(125,6,'龙锄头','- 种地速度提升'),
(126,5,'精铁','- 用于合成'),
(127,5,'锡矿','- 用于合成'),
(128,2,'自定义颜色','让你使用自己的皮肤颜色'),
(129,4,'卷心菜','- 获得35经验'),
(130,4,'金券','- 更方便地交易黄金!'),
(131,10,'技能扩展','- 增加部分技能的上限'),
(132,12,'充值称号 - GG爆!','- 暴击伤害99%几率增加50倍'),
(133,12,'充值称号 - TANK!','- 生命上限增加70倍'),
(134,12,'特殊称号 - TeeFun!','- TeeFun成员都得有');
-- (131,10,'中秋节称号','- 射速+10'),
-- (132,5,'活动用品','用于合成');
/*!40000 ALTER TABLE `tw_uItemList` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_uItemList_en`
--

DROP TABLE IF EXISTS `tw_uItemList_en`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tw_uItemList_en` (
  `il_id` int(4) unsigned NOT NULL AUTO_INCREMENT,
  `item_type` int(11) DEFAULT '0',
  `item_name` char(50) NOT NULL,
  `item_desc` char(100) NOT NULL DEFAULT '- No description',
  PRIMARY KEY (`il_id`) USING BTREE,
  KEY `item_type` (`item_type`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=126 DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;


--
-- Dumping data for table `tw_uItemList_en`
--

LOCK TABLES `tw_uItemList_en` WRITE;
/*!40000 ALTER TABLE `tw_uItemList_en` DISABLE KEYS */;
INSERT INTO `tw_uItemList_en` VALUES 
(1,4,'Money Bag','- Random get 5-20000 silvers'),
(2,3,'Quest Pig Meat','- Need for quest'),
(3,3,'Quest Kwah Head','- Need for quest'),
(4,1,'Hook Damage','- Weapon / Upgrading'),
(5,1,'Gun','- Weapon / Upgrading'),
(6,1,'Shotgun','- Weapon / Upgrading'),
(7,1,'Grenade','- Weapon / Upgrading'),
(8,1,'Laser','- Weapon / Upgrading'),
(9,2,'Amulet Cleever','This for got x2 money bag in level up'),
(10,2,'Ring Selfine','This for don not get self damage'),
(11,5,'Module Happy','Emote module if all modules + bonus'),
(12,5,'Module Evil','Emote module if all modules + bonus'),
(13,5,'Module Surprise','Emote module if all modules + bonus'),
(14,5,'Module Blink','Emote module if all modules + bonus'),
(15,5,'Module Pain','Emote module if all modules + bonus'),
(16,4,'Ticket Create Clan','Ticket for created clan'),
(17,2,'Module Emote','Emote module if all modules + bonus'),
(18,4,'Alpha Box','This bonus in alpha'),
(19,4,'SP Upgrade Box','This bonus in alpha'),
(20,2,'Leather','Craft Item'),
(21,2,'Rare Draw HP Regen','Rare | This drawing item'),
(22,2,'Rare Draw Damage','Rare | This drawing item'),
(23,2,'Rare Draw Handle','Rare | This drawing item'),
(24,2,'Rare Draw Ammo Regen','Rare | This drawing item'),
(25,2,'Rare Ring Slime','Rare | Not self damage and jump'),
(26,5,'Slime Dirt','Item for craft'),
(27,5,'Formula Ring','Item for craft'),
(28,1,'Module Explode Gun','- Weapon / Upgrading'),
(29,1,'Module Explode Shotgun','- Weapon / Upgrading'),
(30,1,'Module Explode Laser','- Weapon / Upgrading'),
(31,1,'Weapon Pressed','- All Weapon +5 ammo'),
(32,5,'Body Boomer','Item for craft'),
(33,2,'Rare Ring Boomer','Rare | Health +5% and item for craft'),
(34,5,'Formula Earring','Item for craft'),
(35,5,'Formula Weapon','Item for craft'),
(36,1,'Module Shotgun Slime','- Weapon / Upgrading'),
(37,1,'Rare Earrings Kwah','Item added +1 jump mabe stack'),
(38,5,'Foot Kwah','Item for craft'),
(39,4,'Book Money x2 (10min)','Book item for added bonus'),
(40,4,'Book Experince x2 (10min)','Book item for added bonus'),
(41,2,'Jioc Dual Ring','Rare | Relation get -10'),
(42,1,'Module Hook Explode','- Module hook damage in explode'),
(43,1,'Auto Hammer','- Auto Fire Hammer'),
(44,1,'Auto Gun','- Auto Fire Gun'),
(45,1,'Module Ghost Gun','- Fire Ghost in Wall'),
(46,1,'Module Ghost Shotgun','- Fire Ghost in Wall'),
(47,1,'Module Ghost Grenade','- Fire Ghost in Wall'),
(48,2,'White Ticket','- Need for Enter in White'),
(49,1,'Rare Freeazer','- %1 freeze for 1sec'),
(50,4,'Rand Box Craft','- This box gived rand craft item'),
(51,4,'Event Box','Event Box'),
(52,1,'Rare Event Hammer','- Snap & added +5 crit damage'),
(53,1,'Bounce Gun','- Gun Ammo Bounce'),
(54,1,'Hybrid SGUN','- Hybrid Gun & Shotgun'),
(55,1,'Bounce Grenade','- Grenade Ammo Bounce'),
(56,4,'Moon O2 / H2O','- This need for area moon'),
(57,1,'Lamp Hammer','- Content 18+'),
(58,3,'Zombie Brain','- Quest Item'),
(59,5,'Zombie Eye','- Craft Item'),
(60,5,'Big Zombie Eye','- Craft Item'),
(61,5,'Skelet Bone','- Craft Item'),
(62,5,'Skelet Fortified Bone','- Craft Item'),
(63,3,'Skelet Skull','- Quest Item'),
(64,5,'Nimf Hearth','- Craft Item'),
(65,5,'Nimf Ears','- Craft Item'),
(66,3,'Nimf Body','- Quest Item'),
(67,4,'Clan Exp Bag','- Added for clan 50000 exp'),
(68,2,'Customized','- This item for custom skin'),
(69,4,'Reseting Upgrades','- This item used'),
(70,4,'Reseting Passive Skills','- Reseting only pasive skill'),
(71,1,'The end Explosion','- This item added end Explosion'),
(72,10,'Output to the chat','- Settings'),
(73,10,'Pick Item','- Settings'),
(74,10,'Skill Walls','- Settings'),
(75,10,'Skill Heal','- Settings'),
(76,4,'VIP PACKAGE','- Added more Bonuses'),
(77,10,'AntiPVP','- Settings'),
(78,2,'VIP Draw Snap','- VIP Item'),
(79,12,'Title VIP','- VIP x2 Money & Exp'),
(80,10,'Skill Sword','- Settings'),
(81,12,'Title Boss?Die','- Spawn HP +1000'),
(82,12,'Title Pig Pig','- Price -5%'),
(83,12,'Title Big Craft','- Not Bonuses'),
(84,10,'Skill Summer Healing','- Event Skill Healing'),
(85,5,'Sun Ray','- Event Craft Item'),
(86,12,'Title Summer','- Title Special Events'),
(87,4,'Potato','- You got 25 exp'),
(88,4,'Tomate','- You got 15 exp'),
(89,4,'Carrot','- You got 10 exp'),
(90,2,'Jump Impuls','- Impuls jump'),
(91,10,'Farming Exp','- Farming Exp'),
(92,4,'Farming Box','- Random Reward'),
(93,12,'Title Questing System','- 1500 HP | 500 Armor'),
(94,5,'Wood','- Craft Item'),
(95,5,'Cooper Ore','- Craft Item'),
(96,6,'Cooper Pickaxe','- Miner Item'),
(97,5,'Iron Ore','- Craft Item'),
(98,6,'Iron Pickaxe','- Miner Item'),
(99,5,'Gold Ore','- Craft Item'),
(100,6,'Gold Pickaxe','- Miner Item'),
(101,5,'Diamond Ore','- Craft Item'),
(102,6,'Diamond Pickaxe','- Miner Item'),
(103,10,'Miner Exp','- Miner Exp'),
(104,5,'Big Wood','- Craft Item'),
(105,5,'Dragon Ore','- Craft Item'),
(106,3,'Particle Soul','- Event Back to school'),
(107,6,'Material','- Work Item'),
(108,10,'Loader Exp','- Loader Exp'),
(109,1,'Pizdamet','- 50% -1 mana'),
(110,15,'Leather body','(Armor +100, Health +100)'),
(111,16,'Leather feet','- Health +50 Armor +50'),
(112,17,'Stabilized Clasic','- Damaage +1'),
(113,15,'Cooper body','- Health Armor +150'),
(114,16,'Cooper feet','- Health Armor +100'),
(115,15,'Iron body','- Health Armor +200'),
(116,16,'Iron feet','- Health Armor +150'),
(117,15,'Gold body','- Health Armor +250'),
(118,16,'Gold feet','- Health Armor +150'),
(119,15,'Diamond body','- Health Armor +300'),
(120,16,'Diamond feet','- Health Armor +250'),
(121,15,'Dragon body','- Health Armor +500'),
(122,16,'Dragon feet','- Health Armor +400'),
(123,10,'Title Enchanter','- Health Armor +1000'),
(124,6,'Dragon Axe','Cutting wood faster'),
(125,6,'Dragon Hoe','Farming faster'),
(126,5,'Fine Iron','- Item for crafting'),
(127,5,'Stannum Ore','- Item for crafting'),
(128,2,'Custom Color','Use customized tee color'),
(129,4,'Cabbage','- You got 35 exp'),
(130,4,'Gold Ticket','- Item for exchanging Gold'),
(131,10,'Extend Limit','- Item for extending upgrade limit'),
(132,12,'Donate Title - GG Bond!','- 暴击伤害99%几率增加50倍'),
(133,12,'Donate Title - TANK!','- 生命上限增加70倍'),
(134,12,'Special Title - TeeFun!','- TeeFun成员都得有');
-- (131,10,'Title Moon','- Special Title for Mid-Autumn Festival'),
-- (132,5,'Activity Item','For Crafting');
/*!40000 ALTER TABLE `tw_uItemList_en` ENABLE KEYS */;
UNLOCK TABLES;


--
-- Table structure for table `tw_uItems`
--

DROP TABLE IF EXISTS `tw_uItems`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tw_uItems` (
  `it_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `il_id` int(4) unsigned NOT NULL,
  `item_type` int(11) NOT NULL,
  `item_owner` bigint(20) NOT NULL DEFAULT '0',
  `item_settings` bigint(20) NOT NULL DEFAULT '0',
  `item_count` int(11) NOT NULL DEFAULT '0',
  `item_enchant` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`it_id`) USING BTREE,
  KEY `item_owner` (`item_owner`) USING BTREE,
  KEY `item_type` (`item_type`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_uItems`
--

LOCK TABLES `tw_uItems` WRITE;
/*!40000 ALTER TABLE `tw_uItems` DISABLE KEYS */;
INSERT INTO `tw_uItems` VALUES (1,1,4,1,0,76,0);
/*!40000 ALTER TABLE `tw_uItems` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_UserStatus`
--

DROP TABLE IF EXISTS `tw_UserStatus`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tw_UserStatus` (
  `ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `date` DATETIME NOT NULL DEFAULT NOW(),
  `IP` varchar(64) NOT NULL,
  `Nick` varchar(32) NOT NULL,
  `ban` tinyint NOT NULL DEFAULT '0',
  `banreason` char(32) DEFAULT '',
  `banlength` int(11) NOT NULL DEFAULT '0',
  `online` tinyint NOT NULL DEFAULT '0',
  `serverid` tinyint NOT NULL DEFAULT '0',
  `lastupdate` DATETIME NOT NULL DEFAULT NOW(),
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_UserStatus`
--

LOCK TABLES `tw_UserStatus` WRITE;
/*!40000 ALTER TABLE `tw_UserStatus` DISABLE KEYS */;
INSERT INTO `tw_UserStatus` VALUES (1,'2021-01-01 09:05:32','127.0.0.1','天上的星星',0,'',0,0,0,'2021-01-01 09:05:32');
/*!40000 ALTER TABLE `tw_UserStatus` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

