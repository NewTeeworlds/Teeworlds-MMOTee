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
  `Level` int(11) NOT NULL DEFAULT '1',
  `Exp` int(11) NOT NULL,
  `MaxNum` int(11) NOT NULL DEFAULT '2',
  `Money` int(11) NOT NULL,
  `Relevance` int(11) NOT NULL DEFAULT '0',
  `MoneyAdd` int(11) NOT NULL DEFAULT '0',
  `ExpAdd` int(11) NOT NULL DEFAULT '0',
  `SpawnHouse` tinyint(4) NOT NULL DEFAULT '0',
  `ChairHouse` int(11) NOT NULL DEFAULT '0',
  `CreateDate` date NOT NULL,
  PRIMARY KEY (`ClanID`) USING BTREE,
  KEY `ClanID` (`ClanID`) USING BTREE,
  KEY `Clanname` (`Clanname`) USING BTREE,
  KEY `Level` (`Level`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_Clans`
--

LOCK TABLES `tw_Clans` WRITE;
/*!40000 ALTER TABLE `tw_Clans` DISABLE KEYS */;
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
) ENGINE=InnoDB AUTO_INCREMENT=71218 DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_Mail`
--

LOCK TABLES `tw_Mail` WRITE;
/*!40000 ALTER TABLE `tw_Mail` DISABLE KEYS */;
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
  `IsJailed` int(11) NOT NULL DEFAULT '0';
  `SummerHealingTimes` int(11) NOT NULL DEFAULT '0'; 
  `Class` int(11) NOT NULL DEFAULT '0',
  `ClanAdded` bigint(20) NOT NULL DEFAULT '0',
  `ClanID` int(11) NOT NULL,
  `Quest` int(11) NOT NULL DEFAULT '1',
  `QuestDat` int(11) NOT NULL DEFAULT '0',
  `Seccurity` tinyint(4) NOT NULL DEFAULT '0',
  `Killing` int(11) NOT NULL DEFAULT '0',
  `WinArea` int(11) NOT NULL DEFAULT '0',
  `SettingsChat` int(11) NOT NULL DEFAULT '0',
  `SettingsDrop` int(11) NOT NULL DEFAULT '0',
  `RegisterDate` datetime NOT NULL,
  `RegisterIp` varchar(64) NOT NULL,
  PRIMARY KEY (`UserId`) USING BTREE,
  KEY `UserId` (`UserId`) USING BTREE,
  KEY `Username` (`Username`) USING BTREE,
  KEY `UserId_2` (`UserId`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=1530 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_Users`
--

LOCK TABLES `tw_Users` WRITE;
/*!40000 ALTER TABLE `tw_Users` DISABLE KEYS */;
/*!40000 ALTER TABLE `tw_Users` ENABLE KEYS */;
UNLOCK TABLES;

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
  `item_desc` char(50) NOT NULL DEFAULT '- No description',
  PRIMARY KEY (`il_id`) USING BTREE,
  KEY `item_type` (`item_type`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=124 DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_uItemList`
--

LOCK TABLES `tw_uItemList` WRITE;
/*!40000 ALTER TABLE `tw_uItemList` DISABLE KEYS */;
INSERT INTO `tw_uItemList` VALUES 
(1,4,'Money Bag','- Random get 5-20000 silvers'),
(2,3,'Quest Pig Meat','- Need for quest'),
(3,3,'Quest Kwah Head','- Need for quest'),
(4,1,'Hook Damage','- Weapon / Upgrading'),
(5,1,'Gun','- Weapon / Upgrading'),
(6,1,'Shotgun','- Weapon / Upgrading'),
(7,1,'Grenade','- Weapon / Upgrading'),
(8,1,'Laser','- Weapon / Upgrading'),
(9,2,'Amulet Cleever','This for got x2 money bag in level up'),
(10,2,'Ring Selfine','This for don\'t get self damage'),
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
(27,5,'Formula Ring\'s','Item for craft'),
(28,1,'Module Explode Gun','- Weapon / Upgrading'),
(29,1,'Module Explode Shotgun','- Weapon / Upgrading'),
(30,1,'Module Explode Laser','- Weapon / Upgrading'),
(31,1,'Weapon Pressed','- All Weapon +5 ammo'),
(32,5,'Body Boomer','Item for craft'),
(33,2,'Rare Ring Boomer','Rare | Health +5% and item for craft'),
(34,5,'Formula Earring\'s','Item for craft'),
(35,5,'Formula Weapon\'s','Item for craft'),
(36,1,'Module Shotgun Slime','- Weapon / Upgrading'),
(37,1,'Rare Earrings Kwah','Item added +1 jump mabe stack'),
(38,5,'Foot Kwah','Item for craft'),
(39,4,'Book Money x2 (10min)','Book item\'s for added bonus'),
(40,4,'Book Experince x2 (10min)','Book item\'s for added bonus'),
(41,2,'Jioc Dual Ring\'s','Rare | Relation get -10'),
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
(123,10,'Title Enchanter','- Health Armor +1000');
/*!40000 ALTER TABLE `tw_uItemList` ENABLE KEYS */;
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
  `item_settings` smallint(5) NOT NULL DEFAULT '0',
  `item_count` int(11) NOT NULL DEFAULT '0',
  `item_enchant` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`it_id`) USING BTREE,
  KEY `item_owner` (`item_owner`) USING BTREE,
  KEY `item_type` (`item_type`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=32903 DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_uItems`
--

LOCK TABLES `tw_uItems` WRITE;
/*!40000 ALTER TABLE `tw_uItems` DISABLE KEYS */;
INSERT INTO `tw_uItems` VALUES (1,1,4,1,0,76,0);
/*!40000 ALTER TABLE `tw_uItems` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2020-11-08 13:27:23
