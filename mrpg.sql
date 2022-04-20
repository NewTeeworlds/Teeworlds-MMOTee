-- MySQL dump 10.13  Distrib 8.0.26, for Linux (x86_64)
--
-- Host: localhost    Database: t
-- ------------------------------------------------------
-- Server version	8.0.26-0ubuntu1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `enum_behavior_mobs`
--

DROP TABLE IF EXISTS `enum_behavior_mobs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `enum_behavior_mobs` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Behavior` varchar(32) NOT NULL DEFAULT 'Standard',
  PRIMARY KEY (`ID`),
  KEY `Behavior` (`Behavior`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `enum_behavior_mobs`
--

LOCK TABLES `enum_behavior_mobs` WRITE;
/*!40000 ALTER TABLE `enum_behavior_mobs` DISABLE KEYS */;
INSERT INTO `enum_behavior_mobs` VALUES (3,'Sleepy'),(2,'Slime'),(1,'Standard');
/*!40000 ALTER TABLE `enum_behavior_mobs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `enum_effects_list`
--

DROP TABLE IF EXISTS `enum_effects_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `enum_effects_list` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Name` varchar(16) CHARACTER SET utf8mb4 DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `Name` (`Name`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `enum_effects_list`
--

LOCK TABLES `enum_effects_list` WRITE;
/*!40000 ALTER TABLE `enum_effects_list` DISABLE KEYS */;
INSERT INTO `enum_effects_list` VALUES (3,'Fire'),(2,'Poison'),(1,'Slowdown');
/*!40000 ALTER TABLE `enum_effects_list` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `enum_emotes`
--

DROP TABLE IF EXISTS `enum_emotes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `enum_emotes` (
  `ID` int NOT NULL,
  `Emote` varchar(64) NOT NULL DEFAULT 'nope',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `enum_emotes`
--

LOCK TABLES `enum_emotes` WRITE;
/*!40000 ALTER TABLE `enum_emotes` DISABLE KEYS */;
INSERT INTO `enum_emotes` VALUES (0,'Normal Emote'),(1,'Pain Emote'),(2,'Happy Emote'),(3,'Surprise Emote'),(4,'Angry Emote'),(5,'Blink Emote');
/*!40000 ALTER TABLE `enum_emotes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `enum_items_functional`
--

DROP TABLE IF EXISTS `enum_items_functional`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `enum_items_functional` (
  `FunctionID` int NOT NULL AUTO_INCREMENT,
  `Name` varchar(64) NOT NULL,
  PRIMARY KEY (`FunctionID`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `enum_items_functional`
--

LOCK TABLES `enum_items_functional` WRITE;
/*!40000 ALTER TABLE `enum_items_functional` DISABLE KEYS */;
INSERT INTO `enum_items_functional` VALUES (-1,'Not have function'),(0,'Equip hammer(Only equip type)'),(1,'Equip gun(Only equip type)'),(2,'Equip shotgun(Only equip type)'),(3,'Equip grenade(Only equip type)'),(4,'Equip rifle(Only equip type)'),(5,'Equip miner(Only equip type)'),(6,'Equip wings(Only equip type)'),(7,'Equip discord(Only equip type)'),(8,'Once use item x1'),(9,'Several times use item x99'),(10,'Settings(Only settings or modules type)'),(11,'Plants item'),(12,'Mining item');
/*!40000 ALTER TABLE `enum_items_functional` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `enum_items_types`
--

DROP TABLE IF EXISTS `enum_items_types`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `enum_items_types` (
  `TypeID` int NOT NULL AUTO_INCREMENT,
  `Name` varchar(32) NOT NULL,
  PRIMARY KEY (`TypeID`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `enum_items_types`
--

LOCK TABLES `enum_items_types` WRITE;
/*!40000 ALTER TABLE `enum_items_types` DISABLE KEYS */;
INSERT INTO `enum_items_types` VALUES (-1,'Invisible'),(1,'Useds'),(2,'Crafts'),(3,'Modules'),(4,'Others'),(5,'Settings'),(6,'Equipping'),(7,'Decorations'),(8,'Potions');
/*!40000 ALTER TABLE `enum_items_types` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `enum_mmo_proj`
--

DROP TABLE IF EXISTS `enum_mmo_proj`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `enum_mmo_proj` (
  `ID` int NOT NULL,
  `Name` varchar(64) NOT NULL,
  KEY `ID` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `enum_mmo_proj`
--

LOCK TABLES `enum_mmo_proj` WRITE;
/*!40000 ALTER TABLE `enum_mmo_proj` DISABLE KEYS */;
INSERT INTO `enum_mmo_proj` VALUES (0,'Magitech Gun'),(1,'Magitech Shotgun'),(2,'Magitech Grenade'),(-1,'No Proj'),(3,'Heavenly Gun'),(4,'Heavenly Shotgun'),(5,'Heavenly Grenade'),(6,'Goblin Gun'),(7,'Goblin Shotgun'),(8,'Goblin Grenade'),(0,'Magitech Gun'),(1,'Magitech Shotgun'),(2,'Magitech Grenade'),(-1,'No Proj'),(3,'Heavenly Gun'),(4,'Heavenly Shotgun'),(5,'Heavenly Grenade'),(6,'Goblin Gun'),(7,'Goblin Shotgun'),(8,'Goblin Grenade'),(0,'Magitech Gun'),(1,'Magitech Shotgun'),(2,'Magitech Grenade'),(-1,'No Proj'),(3,'Heavenly Gun'),(4,'Heavenly Shotgun'),(5,'Heavenly Grenade'),(6,'Goblin Gun'),(7,'Goblin Shotgun'),(8,'Goblin Grenade');
/*!40000 ALTER TABLE `enum_mmo_proj` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `enum_quest_interactive`
--

DROP TABLE IF EXISTS `enum_quest_interactive`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `enum_quest_interactive` (
  `ID` int NOT NULL,
  `Name` varchar(64) NOT NULL,
  KEY `ID` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `enum_quest_interactive`
--

LOCK TABLES `enum_quest_interactive` WRITE;
/*!40000 ALTER TABLE `enum_quest_interactive` DISABLE KEYS */;
INSERT INTO `enum_quest_interactive` VALUES (1,'Randomly accept or refuse with the item'),(2,'Pick up items that NPC will drop.'),(1,'Randomly accept or refuse with the item'),(2,'Pick up items that NPC will drop.'),(1,'Randomly accept or refuse with the item'),(2,'Pick up items that NPC will drop.');
/*!40000 ALTER TABLE `enum_quest_interactive` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `enum_talk_styles`
--

DROP TABLE IF EXISTS `enum_talk_styles`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `enum_talk_styles` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Style` varchar(64) NOT NULL DEFAULT 'nope',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  KEY `ID_2` (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `enum_talk_styles`
--

LOCK TABLES `enum_talk_styles` WRITE;
/*!40000 ALTER TABLE `enum_talk_styles` DISABLE KEYS */;
INSERT INTO `enum_talk_styles` VALUES (0,'Basic Talking'),(1,'Aggresive Talking'),(2,'Joyful Talking');
/*!40000 ALTER TABLE `enum_talk_styles` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `enum_worlds`
--

DROP TABLE IF EXISTS `enum_worlds`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `enum_worlds` (
  `WorldID` int NOT NULL,
  `Name` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `RespawnWorld` int DEFAULT NULL,
  `MusicID` int NOT NULL DEFAULT '-1',
  PRIMARY KEY (`WorldID`),
  KEY `WorldID` (`WorldID`),
  KEY `Name` (`Name`),
  KEY `SafeZoneWorldID` (`RespawnWorld`),
  KEY `WorldID_2` (`WorldID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `enum_worlds`
--

LOCK TABLES `enum_worlds` WRITE;
/*!40000 ALTER TABLE `enum_worlds` DISABLE KEYS */;
INSERT INTO `enum_worlds` VALUES (0,'Pier Elfinia',NULL,53),(1,'Way to the Elfinia',1,54),(2,'Elfinia',2,53),(3,'Elfinia Deep cave',2,54),(4,'Elfia home room',2,53),(5,'Elfinia occupation of goblins',5,54),(6,'Elfinia Abandoned mine',NULL,56),(7,'Diana home room',2,53),(8,'Noctis Resonance',NULL,55),(9,'Departure',9,53),(10,'Underwater of Neptune',10,55),(11,'Yugasaki',11,-1),(12,'StarKay',10,55);
/*!40000 ALTER TABLE `enum_worlds` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_accounts`
--

DROP TABLE IF EXISTS `tw_accounts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_accounts` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Username` varchar(64) NOT NULL,
  `Password` varchar(64) NOT NULL,
  `PasswordSalt` varchar(64) DEFAULT NULL,
  `RegisterDate` varchar(64) NOT NULL,
  `LoginDate` varchar(64) NOT NULL DEFAULT 'First log in',
  `RegisteredIP` varchar(64) NOT NULL DEFAULT '0.0.0.0',
  `LoginIP` varchar(64) NOT NULL DEFAULT '0.0.0.0',
  `Language` varchar(8) NOT NULL DEFAULT 'en',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  KEY `Password` (`Password`),
  KEY `Username` (`Username`)
) ENGINE=InnoDB AUTO_INCREMENT=36 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_accounts`
--

LOCK TABLES `tw_accounts` WRITE;
/*!40000 ALTER TABLE `tw_accounts` DISABLE KEYS */;
INSERT INTO `tw_accounts` VALUES (1,'haxx','2de29cade41dc0d57c612ca9def7375cf0648a37cd3889301420d6587e8a8906','Z7ZekB2KFCFV4XbUcTVLH8YM','2021-10-17 04:17:25','2021-10-24 08:09:12','113.76.108.69','14.116.69.186','cn'),(2,'Moker','af397d9206e778217eb879bfa1fab4d400a99a6a61fdbcefe810febaf5ab8499','eN4G24gXS3mZeGBYTHeHgPeG','2021-10-17 04:19:47','2021-10-24 15:40:40','218.17.207.27','218.17.207.27','cn'),(3,'e26174222','e4fb1ad95a2e1db737953d2c32258d7e752abf97e637ef14ac836de1747f510c','V6ES4LKt8P2ZF8LV6S6BpjDN','2021-10-17 04:33:32','2021-10-24 01:53:35','61.227.76.108','114.47.75.128','cn'),(4,'hack','fd04f8463c79ba032966939003f7bb126adda5afa5debe542ea4c4c0e1a2532d','LtmnnStB7ZkYDeTq5F8VA5UG','2021-10-17 05:02:49','2021-10-24 19:04:37','113.76.108.69','113.76.108.179','cn'),(5,'Ninecloud','cf60b9d457bdb2351b197b310b8e2a4515b0e766af9ca68db41c68a40f4585d0','q5aC5bptKNbAMS52VHf9GM2f','2021-10-17 05:13:23','2021-10-24 14:37:21','220.163.70.155','182.242.17.28','cn'),(6,'15875294901','0b6cb540cf656315f09b6eaf5879023e759744ef8474d0bfce30c743bbaf8e0d','ZMLa8BSA4oTPbE6BNtPLNg6t','2021-10-17 07:08:59','2021-10-17 21:17:11','120.235.120.104','120.235.120.104','en'),(7,'高木','acd5365ac645bc89feb65825dc2fc867f8d03d583e2902db427a7e0a8c7dd829','FqPKmfKVMW4BpRaBkahje3Yg','2021-10-17 07:19:47','2021-10-23 21:16:51','183.167.25.234','183.167.29.54','en'),(8,'3496240200','de599eb9abbd15e01984e4fda3b70948ce75f79967143e0a874ded552763f93b','t78RdnVSRt8ASkceBtPA76nE','2021-10-17 12:15:08','2021-10-18 22:44:55','117.179.165.47','111.41.250.206','cn'),(9,'zczczc332','aac0ece9cfe8b0a31b12f9098a0bc979245b3b984111e2564e11e92551fb6477','bj2KMNDnU4SYoPPFYnRgdFeL','2021-10-17 13:13:11','2021-10-17 21:17:39','119.77.194.118','119.77.194.118','en'),(10,'zczczc332','4a81f2fd15546715e1521846d4583f660e35060b6618041441a887dba72888b5','bcjKWdX9Y2cDdA4cNt6eSgpD','2021-10-17 13:23:04','2021-10-24 14:29:13','119.77.194.118','119.77.194.118','cn'),(11,'3496240200','68a1b820b5733bbc7c82c07f8999e1e0f8bc53ff3aabdb5f046787ae7b1ebd44','a53knqg5LofkUHYfU7ULV8NN','2021-10-17 13:25:35','2021-10-17 21:25:54','117.179.165.47','117.179.165.47','en'),(12,'ak111aa','658f51c080f9f24a154e543364e0b974d484f87c113d85cf89c78e42ad3c9eff','j8AjTZ324apNtSceAPCdhB3A','2021-10-17 13:35:18','2021-10-17 21:35:33','111.41.242.91','111.41.242.91','cn'),(13,'卅白','2e4f1240a534833d4a5a5c5681137f7c3b2447182099e38b0a9c18395d2e4ec3','34t3tekAnFhaRB4DCNKGKT3M','2021-10-17 13:42:03','2021-10-19 21:31:45','114.222.3.104','114.222.3.104','cn'),(14,'天帝','fb82c9fac7b7e779fd5c4d54342eddd0c8e3655198dd98a98025575c2fea2bd2','WcVF6kC57t6Eg4aWhMUFTkBY','2021-10-17 13:57:33','2021-10-24 15:33:45','113.116.158.190','113.88.134.178','cn'),(15,'i3slkiller','92ddab0d3a577d2e75a0f380e9c92199c6f8ccd657f8c615d6338645dd6f3fec','C4cX4j2dXdMSTjjE27jdWTLC','2021-10-18 13:42:25','2021-10-18 21:42:26','188.146.41.62','188.146.41.62','en'),(16,'KASARES','0da5900dc11207bac76f2c9ad764d6a5090ac4ae7d065836931edc8764d97a61','oHW9mCdP37AN3DNFfpFb6KYt','2021-10-18 13:54:30','2021-10-18 21:54:40','148.81.137.6','148.81.137.6','en'),(17,'anuspeanus','ee05bcf0d17c9599fef199e0c554f6e058a9f1226052a6a6eb0bbebe29c30566','oZ2oHTnfdfR8hTZ7hVHNDEae','2021-10-19 20:52:14','2021-10-20 04:52:25','178.66.158.217','178.66.158.217','en'),(18,'zczczc332','f37b1d35b4035368685a2c0c74d704da4c5c0f80e903f45ef51b1ddf3852c00d','eNkB2KTfgoYXAc28HmHYhRT2','2021-10-20 08:31:45','2021-10-20 16:31:59','119.77.194.118','119.77.194.118','en'),(19,'hx198181','10ba54fa4801699bc6de0c559c14edc540813495570160cee2cedaf902440e21','T6eNcVkjeR3aVXcDHG69enbN','2021-10-20 11:33:18','2021-10-20 19:33:25','27.17.210.224','27.17.210.224','en'),(20,'kuro','2e728cd0acf3ba96a877d2ca9b609107f6aeb2450f98b91cbc671166a7bee81e','2Aqd3PCeCoakPaYApcFNe2DL','2021-10-22 05:49:49','2021-10-23 15:02:36','91.132.21.32','91.132.21.32','en'),(21,'SCIENTISTBEA','3391c21fbf874123fad4227b9c97b6563086a0b08053bbe73d16d5298f8bc34a','tj6dqngnNpX94e64m8NpBWAt','2021-10-22 08:42:29','2021-10-24 16:18:06','117.155.38.42','117.155.38.42','cn'),(22,'孤疯月影','88e324d10f52c704562c6c23909d750073b98939f104e991e6195c2a06b3bc2d','6NXZ5VYff9SbXjtTBMAMqoCD','2021-10-22 09:01:49','2021-10-24 17:50:50','36.101.194.32','36.101.194.204','cn'),(23,'hx1981','07fc016f93e7ff099f26072b2c9873b861c3e109b32d50a2c0dc95a60417b582','c7aaS96E3AR7RN6m8FUbacqU','2021-10-22 10:12:31','2021-10-24 10:07:28','27.17.210.224','27.17.210.224','cn'),(24,'HUMAN','3b633d0505766981ee715f9747a33449d99ccf897667b3cebe26e64ef3e94157','WWgNkL6A6kDp7G8NZ5bj8Bf8','2021-10-22 10:30:43','2021-10-22 18:31:07','46.219.230.155','46.219.230.155','en'),(25,'Ankerlol666','9bd5cba9e3da55f684489b9543771c0d877910bfdd7d60b6c7201989c15f6a53','GYkNLYaVj9e4BaqLS2f8tDcT','2021-10-22 12:33:12','2021-10-22 20:33:42','186.104.175.6','186.104.175.6','en'),(26,'tenhel33','81a4073817c4bb4f0b6e5e41ce06578e7fc9177f4208807856ef6be9b6298e70','PERM6ZhW3H6haf39Z4nZdY3U','2021-10-22 21:09:44','2021-10-23 05:10:57','185.16.31.211','185.16.31.211','en'),(27,'shengyan','f551fc65f230a353895f8bbf1cabb06ea3c1d2f337deb18e73f3b6401ca1a3c9','nK4ot5RmeNWHPg76gMDqM2GU','2021-10-23 07:54:30','2021-10-24 15:06:33','49.80.111.146','114.226.154.93','cn'),(28,'imyyee','f240248fbbc8072d6e5d9389e42f04fd73fa18fd239ebf454fcefaef479a9ade','R4cKqeCFMH3AK8fVge5bGAdF','2021-10-23 07:54:43','2021-10-23 15:54:50','110.229.69.8','110.229.69.8','en'),(29,'nothingxd','bc2fb40b6f767882353223f6d2ecfa7e79c6d9569862657956b283b396f1fef7','cLE5pnMf5obVg24NHtZ9oV2F','2021-10-23 08:35:20','2021-10-23 16:35:39','5.114.177.234','5.114.177.234','en'),(30,'101024','7b306a032ca35acf136c61b20e5a5f667041114f0413a3ebe5c612515ba9b70f','tRVNPX4X4KYBLAeV6ofFWGtg','2021-10-23 08:50:11','2021-10-24 16:00:56','112.10.71.240','112.10.190.165','en'),(31,'tk700','e2e8824099ee4392595a46ce2b1f70510f30574c77f3edf6adf3cc0801620567','R2C8aTYhWhEfhSng7CE2oKSU','2021-10-23 10:19:52','2021-10-23 18:20:02','178.27.67.96','178.27.67.96','en'),(32,'star','76f2cc739dc1308da8c678e2744c066631bb559d879c5aedcca2949984c936e2','B9BPWqK7GGcW6WhTHMR3SaHS','2021-10-23 13:29:07','2021-10-23 21:29:24','101.224.56.251','101.224.56.251','en'),(33,'someteeNG','6db0b39d1a4ab44ec91ad7512953dab5beed4f902006d4ed252a619e64c93bcc','F7FctA6o2Mn2YmGg3PnDEWFj','2021-10-24 00:53:38','2021-10-24 08:53:45','89.201.197.179','89.201.197.179','en'),(34,'bigcat','661fa17cb38bff30732222f4e67afe8790748a09cfcfcba45a9e7ad76cb720ba','TZ4SPSP53dXtUD2d6ZA2dgK5','2021-10-24 02:22:22','2021-10-24 10:22:42','112.1.238.41','112.1.238.41','en'),(35,'天际','95987b33cd3921bd95dd0dc4410e90ad73fc2f1a83fbb6cd94c2516d1d4284e3','hNU54MNY7oFjZfVFnPhEjhmC','2021-10-24 04:59:23','2021-10-24 14:23:14','115.151.240.189','115.151.240.189','en');
/*!40000 ALTER TABLE `tw_accounts` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_accounts_aethers`
--

DROP TABLE IF EXISTS `tw_accounts_aethers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_accounts_aethers` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `UserID` int NOT NULL,
  `AetherID` int NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  KEY `OwnerID` (`UserID`),
  KEY `TeleportID` (`AetherID`),
  CONSTRAINT `tw_accounts_aethers_ibfk_1` FOREIGN KEY (`UserID`) REFERENCES `tw_accounts_data` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_accounts_aethers_ibfk_2` FOREIGN KEY (`AetherID`) REFERENCES `tw_aethers` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=59 DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_accounts_aethers`
--

LOCK TABLES `tw_accounts_aethers` WRITE;
/*!40000 ALTER TABLE `tw_accounts_aethers` DISABLE KEYS */;
INSERT INTO `tw_accounts_aethers` VALUES (1,1,2),(2,2,2),(3,4,2),(4,8,2),(5,8,1),(6,6,2),(7,9,2),(8,10,2),(9,2,1),(10,5,2),(11,12,2),(12,6,1),(13,13,2),(14,13,1),(15,12,1),(16,14,2),(17,10,1),(18,14,1),(19,3,2),(20,3,1),(21,4,1),(22,2,3),(23,15,2),(24,10,3),(25,16,2),(26,13,3),(27,3,3),(28,14,3),(29,17,2),(30,19,2),(31,20,2),(32,21,2),(33,21,1),(34,22,2),(35,22,1),(36,23,2),(37,23,1),(38,24,2),(39,25,2),(40,4,3),(41,5,1),(42,26,2),(43,26,1),(44,27,2),(45,28,2),(46,27,1),(47,29,2),(48,30,2),(49,28,1),(50,31,2),(51,1,1),(52,30,1),(53,7,2),(54,7,1),(55,32,2),(56,33,2),(57,35,1),(58,35,2);
/*!40000 ALTER TABLE `tw_accounts_aethers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_accounts_data`
--

DROP TABLE IF EXISTS `tw_accounts_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_accounts_data` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Nick` varchar(32) NOT NULL,
  `DiscordID` varchar(64) NOT NULL DEFAULT 'null',
  `WorldID` int DEFAULT NULL,
  `Level` int NOT NULL DEFAULT '1',
  `Exp` int NOT NULL DEFAULT '0',
  `GuildID` int DEFAULT NULL,
  `GuildDeposit` int NOT NULL DEFAULT '0',
  `GuildRank` int DEFAULT NULL,
  `Upgrade` int NOT NULL DEFAULT '0',
  `DiscordEquip` int NOT NULL DEFAULT '-1',
  `SpreadShotgun` int NOT NULL DEFAULT '3',
  `SpreadGrenade` int NOT NULL DEFAULT '1',
  `SpreadRifle` int NOT NULL DEFAULT '1',
  `Dexterity` int NOT NULL DEFAULT '0',
  `CriticalHit` int NOT NULL DEFAULT '0',
  `DirectCriticalHit` int NOT NULL DEFAULT '0',
  `Hardness` int NOT NULL DEFAULT '0',
  `Tenacity` int NOT NULL DEFAULT '0',
  `Lucky` int NOT NULL DEFAULT '0',
  `Piety` int NOT NULL DEFAULT '0',
  `Vampirism` int NOT NULL DEFAULT '0',
  `AmmoRegen` int NOT NULL DEFAULT '0',
  `Ammo` int NOT NULL DEFAULT '0',
  `Efficiency` int NOT NULL DEFAULT '0',
  `Extraction` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `Nick` (`Nick`),
  KEY `MemberID` (`GuildID`),
  KEY `DiscordID` (`DiscordID`),
  KEY `tw_accounts_data_ibfk_3` (`WorldID`),
  KEY `GuildRank` (`GuildRank`),
  KEY `Level` (`Level`),
  KEY `Exp` (`Exp`),
  CONSTRAINT `tw_accounts_data_ibfk_3` FOREIGN KEY (`WorldID`) REFERENCES `enum_worlds` (`WorldID`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `tw_accounts_data_ibfk_4` FOREIGN KEY (`GuildRank`) REFERENCES `tw_guilds_ranks` (`ID`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `tw_accounts_data_ibfk_5` FOREIGN KEY (`ID`) REFERENCES `tw_accounts` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=36 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_accounts_data`
--

LOCK TABLES `tw_accounts_data` WRITE;
/*!40000 ALTER TABLE `tw_accounts_data` DISABLE KEYS */;
INSERT INTO `tw_accounts_data` VALUES (1,'FlowerFell-Sans','null',1,9,1081,1,0,NULL,40,-1,3,1,1,9,10,10,0,0,0,0,11,0,0,0,0),(2,'Moker','null',0,24,6152,1,0,NULL,0,15003,3,3,1,0,0,0,0,0,0,110,0,30,0,0,0),(3,'BAO','null',5,13,469,1,0,NULL,0,-1,3,1,1,15,10,15,30,0,5,20,7,18,0,0,0),(4,'Flower','868837923020210177',3,21,6162,1,12058321,NULL,90,15003,10,1,1,50,10,10,36,0,21,100,65,26,24,0,0),(5,'Ninecloud','null',2,5,39,NULL,0,NULL,40,-1,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0),(6,'骨头小哲','null',2,4,116,NULL,0,NULL,0,-1,3,1,1,4,5,4,4,0,4,4,4,1,0,0,0),(7,'高木','null',2,4,8,NULL,0,NULL,20,-1,3,1,1,0,0,0,0,0,0,5,5,0,0,0,0),(8,'000','null',2,5,382,NULL,0,NULL,0,-1,3,1,1,2,3,2,4,0,2,0,1,26,0,0,0),(9,'ZrushFuns','null',1,2,4,NULL,0,NULL,10,-1,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0),(10,'嬌兔來打球','null',5,31,19514,1,0,NULL,40,15003,3,2,1,0,0,0,0,0,0,160,0,0,0,0,0),(11,'nameless tee','null',0,1,0,NULL,0,NULL,0,-1,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0),(12,'321','null',1,3,93,NULL,0,NULL,0,-1,3,1,1,20,0,0,0,0,0,0,0,0,0,0,0),(13,'卅白','null',2,11,718,NULL,0,NULL,0,-1,3,1,1,10,10,10,20,0,0,30,0,20,0,0,0),(14,'天帝','null',5,17,461,NULL,0,NULL,10,15003,3,1,1,0,10,10,50,0,0,80,0,0,0,0,0),(15,'i3slkiller','null',0,2,2,NULL,0,NULL,10,-1,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0),(16,'kasarse','null',0,1,0,NULL,0,NULL,0,-1,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0),(17,'anuspeanus','null',0,1,0,NULL,0,NULL,0,-1,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0),(18,'嬌兔來打糗','null',0,1,0,NULL,0,NULL,0,-1,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0),(19,'HX1492','null',1,2,19,NULL,0,NULL,10,-1,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0),(20,'kurosio','null',0,2,46,NULL,0,NULL,10,-1,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0),(21,'SCIENTISTBEAR','null',2,6,615,NULL,0,NULL,0,-1,3,1,1,17,6,3,7,0,10,3,2,2,0,0,0),(22,'孤疯月影','null',2,11,724,NULL,0,NULL,0,15003,3,1,1,0,0,0,0,0,0,20,10,40,1,0,0),(23,'HX1493','null',2,14,1375,NULL,0,NULL,0,15003,3,1,1,2,0,0,7,0,9,90,11,11,0,0,0),(24,'HackedHuman','null',0,1,0,NULL,0,NULL,0,-1,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0),(25,'Ankerlol666','null',0,1,0,NULL,0,NULL,0,-1,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0),(26,'Wanceko','null',2,3,45,NULL,0,NULL,20,-1,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0),(27,'shengyanLM','null',2,20,8606,1,100000100,NULL,50,-1,3,1,1,8,10,7,5,0,5,30,25,50,0,0,0),(28,'im gay','null',NULL,5,1186,NULL,0,NULL,66,-1,3,1,1,1,0,0,2,0,1,0,0,0,0,0,0),(29,'NothingXD','null',1,2,2,NULL,0,NULL,10,-1,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0),(30,'101024','null',2,5,218,NULL,0,NULL,0,-1,3,1,1,6,7,6,6,0,5,3,3,4,0,0,0),(31,'chilly tonka','null',1,2,3,NULL,0,NULL,10,-1,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0),(32,'StarOnTheSky','null',0,2,2,NULL,0,NULL,10,-1,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0),(33,'someteeNG','null',0,1,0,NULL,0,NULL,0,-1,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0),(34,'big cat','null',0,1,0,NULL,0,NULL,0,-1,3,1,1,0,0,0,0,0,0,0,0,0,0,0,0),(35,'天际','null',2,5,385,NULL,0,NULL,37,-1,3,1,1,1,0,2,0,0,0,0,0,0,0,0,0);
/*!40000 ALTER TABLE `tw_accounts_data` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_accounts_farming`
--

DROP TABLE IF EXISTS `tw_accounts_farming`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_accounts_farming` (
  `UserID` int NOT NULL,
  `Level` int NOT NULL DEFAULT '1',
  `Exp` int NOT NULL DEFAULT '0',
  `Quantity` int NOT NULL DEFAULT '1',
  `Upgrade` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`UserID`),
  UNIQUE KEY `AccountID` (`UserID`),
  CONSTRAINT `tw_accounts_farming_ibfk_1` FOREIGN KEY (`UserID`) REFERENCES `tw_accounts_data` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_accounts_farming`
--

LOCK TABLES `tw_accounts_farming` WRITE;
/*!40000 ALTER TABLE `tw_accounts_farming` DISABLE KEYS */;
INSERT INTO `tw_accounts_farming` VALUES (1,1,1,1,0),(2,12,21,1,11),(3,1,2,1,0),(4,3,28,1,2),(5,1,0,1,0),(6,1,0,1,0),(7,1,0,1,0),(8,1,0,1,0),(9,1,0,1,0),(10,8,739,1,7),(11,1,0,1,0),(12,1,0,1,0),(13,1,0,1,0),(14,9,665,1,8),(15,1,0,1,0),(16,1,0,1,0),(17,1,0,1,0),(18,1,0,1,0),(19,1,0,1,0),(20,1,0,1,0),(21,2,36,1,1),(22,2,22,1,1),(23,2,2,1,1),(24,1,0,1,0),(25,1,0,1,0),(26,1,0,1,0),(27,1,3,1,0),(28,1,4,1,0),(29,1,0,1,0),(30,2,2,1,1),(31,1,0,1,0),(32,1,0,1,0),(33,1,0,1,0),(34,1,0,1,0),(35,1,0,1,0);
/*!40000 ALTER TABLE `tw_accounts_farming` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_accounts_items`
--

DROP TABLE IF EXISTS `tw_accounts_items`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_accounts_items` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `ItemID` int NOT NULL,
  `Value` int NOT NULL,
  `Settings` int NOT NULL,
  `Enchant` int NOT NULL,
  `Durability` int NOT NULL DEFAULT '100',
  `UserID` int NOT NULL,
  PRIMARY KEY (`ID`),
  KEY `OwnerID` (`UserID`),
  KEY `ItemID` (`ItemID`),
  CONSTRAINT `tw_accounts_items_ibfk_1` FOREIGN KEY (`ItemID`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_accounts_items_ibfk_2` FOREIGN KEY (`UserID`) REFERENCES `tw_accounts_data` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=607 DEFAULT CHARSET=utf8mb4 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_accounts_items`
--

LOCK TABLES `tw_accounts_items` WRITE;
/*!40000 ALTER TABLE `tw_accounts_items` DISABLE KEYS */;
INSERT INTO `tw_accounts_items` VALUES (1,2,1,1,0,100,1),(2,39,1,1,0,100,1),(3,2,1,0,1,100,2),(4,39,1,0,0,100,2),(9,3,1,0,0,100,2),(10,1,2380,0,0,100,2),(13,1,118,0,0,100,1),(15,2,1,0,0,100,4),(16,39,1,1,0,100,4),(18,2,1,0,0,100,5),(20,3,1,0,0,100,4),(22,21,1,0,0,100,4),(23,9,136,0,0,100,4),(24,15,1011,0,0,100,4),(25,14,12,0,0,100,4),(27,2,1,1,0,100,3),(30,39,1,0,0,100,8),(31,2,1,1,0,100,8),(34,3,1,1,0,100,8),(35,1,265,0,0,100,8),(37,9,4,0,0,100,8),(38,15,3,0,0,100,8),(39,14,3,0,0,100,8),(40,39,1,1,0,100,6),(43,39,1,1,0,100,9),(44,2,1,1,0,100,9),(45,3,1,1,0,100,6),(46,1,283,0,0,100,6),(50,3,1,1,0,100,9),(51,1,16,0,0,100,9),(52,9,1,0,0,100,9),(53,21,1,0,0,100,9),(54,2,1,0,1,100,10),(55,39,1,0,0,100,10),(58,3,1,1,0,100,10),(59,1,16482,0,0,100,10),(61,39,1,1,0,100,11),(62,2,1,0,0,100,11),(63,39,1,1,0,100,5),(64,15,3,0,0,100,6),(65,14,3,0,0,100,6),(67,27,1,1,5,100,2),(69,15,10,0,0,100,10),(70,14,39,0,0,100,10),(72,3,1,1,0,100,5),(73,1,82,0,0,100,5),(74,2,1,1,0,100,12),(75,39,1,1,0,100,12),(76,21,1,0,0,100,5),(79,3,1,1,0,100,12),(80,1,100,0,0,100,12),(82,29,1,0,0,100,8),(83,2,1,1,0,100,13),(84,39,1,1,0,100,13),(85,27,1,1,0,100,6),(86,9,1,0,0,100,12),(87,22,1,0,1,100,12),(88,15,9,0,0,100,2),(89,14,10,0,0,100,2),(92,9,1,0,0,100,6),(94,3,1,1,0,100,13),(96,22,137,0,21893,100,10),(98,15,3,0,0,100,12),(99,14,3,0,0,100,12),(100,27,1,1,0,100,8),(102,2,1,1,1,100,14),(103,39,1,1,0,100,14),(104,21,1,0,0,100,1),(105,27,1,1,0,100,13),(107,27,1,1,0,100,10),(109,3,1,1,0,100,14),(113,22,314,0,21893,100,2),(114,4,1,1,0,100,8),(117,22,5,0,0,100,8),(118,29,77,0,9321,100,10),(120,22,68,0,6512,100,13),(121,27,1,1,2,100,14),(123,22,72,0,21893,100,14),(126,14,11,0,0,100,13),(127,15,11,0,0,100,13),(129,50,1,0,0,100,2),(130,4,1,1,0,100,2),(131,15,11,0,0,100,14),(132,14,11,0,0,100,14),(133,5,1,1,0,100,13),(136,50,1,0,0,100,10),(137,4,1,1,0,100,10),(139,6,1,1,0,100,14),(142,5,1,1,0,100,10),(143,6,1,1,0,100,2),(144,39,1,0,0,100,3),(147,3,1,1,0,100,3),(148,1,3042,0,0,100,3),(151,14,11,0,0,100,3),(153,27,1,1,0,100,3),(155,25,8,0,0,100,10),(156,22,102,0,5969,100,3),(158,4,1,1,0,100,3),(159,25,8,0,0,100,2),(160,48,1,1,3,100,2),(161,32,1,1,2,100,10),(162,4,1,1,0,100,13),(163,29,261,0,21893,100,2),(164,50,1,0,0,100,3),(165,27,1,1,0,100,4),(166,22,41,0,21893,100,4),(168,29,71,0,21893,100,4),(169,4,1,0,0,100,4),(172,6,1,1,0,100,4),(174,10,100,0,0,100,4),(175,50,1,0,0,100,4),(176,7,17,0,0,100,4),(178,11,1,0,0,100,4),(179,12,1,0,0,100,4),(180,13,1,0,0,100,4),(181,5,1,1,0,100,3),(182,32,1,1,2,100,2),(183,49,1,1,0,100,10),(184,32,1,1,0,100,3),(186,6,1,1,0,100,3),(188,36,1,1,0,100,3),(189,29,44,0,0,100,3),(190,32,1,1,0,100,13),(191,29,38,0,12,100,13),(192,49,1,1,2,100,2),(193,6,1,1,0,100,13),(194,5,1,1,0,100,2),(195,48,1,1,3,100,10),(196,50,1,0,0,100,14),(197,4,1,1,0,100,14),(199,36,1,1,1,100,10),(200,25,8,0,0,100,13),(201,48,1,1,0,100,3),(202,47,1,1,3,100,2),(203,48,1,1,0,100,13),(204,1,477,0,0,100,13),(205,35,23,0,21893,100,2),(206,9,14,0,0,100,10),(208,25,8,0,0,100,3),(209,9,6,0,0,100,13),(211,36,1,1,0,100,13),(212,47,1,1,0,100,3),(215,2,1,1,0,100,15),(216,39,1,1,0,100,15),(219,16,3,0,0,100,13),(220,3,1,1,0,100,15),(221,1,15,0,0,100,15),(222,21,1,0,0,100,15),(223,2,1,1,0,100,16),(224,39,1,1,0,100,16),(225,30,7,0,0,100,13),(227,47,1,1,1,100,10),(228,28,1,1,0,100,10),(229,47,1,1,0,100,13),(230,28,1,1,2,100,2),(231,36,1,1,3,100,2),(232,9,11,0,0,100,3),(233,35,4,0,0,100,14),(234,35,7,0,0,100,13),(235,5,1,1,0,100,14),(236,28,1,1,0,100,3),(239,35,10,0,809458447,100,3),(241,25,8,0,0,100,14),(242,42,2,0,32687,100,2),(243,42,29,0,32687,100,10),(244,32,1,1,1,100,14),(245,42,2,0,0,100,3),(246,49,1,1,0,100,3),(248,36,1,1,0,100,14),(249,1,8783,0,0,100,14),(250,29,24,0,22006,100,14),(251,9,59,0,0,100,14),(253,26,1,1,1,10,2),(254,30,47,0,22006,100,2),(256,26,1,1,1,41,10),(258,7,88,0,0,100,10),(259,7,2041,0,0,100,2),(260,30,49,0,9424,100,10),(261,31,271,0,0,100,2),(263,26,1,1,0,100,3),(264,7,116,0,0,100,3),(265,31,801,0,0,100,10),(266,31,12,0,0,100,3),(267,43,5,0,0,100,2),(268,43,5,0,0,100,10),(269,9,75,0,0,100,2),(270,28,1,1,0,100,14),(272,35,12,0,21977,100,10),(274,10009,1,1,0,100,4),(276,10020,1,1,0,100,4),(277,10021,1,1,0,100,4),(278,18,25,0,0,100,4),(279,28,1,0,1,100,4),(280,33,1,1,1,100,4),(281,34,1,1,1,100,4),(282,35,32,0,21893,100,4),(283,49,1,1,1,100,4),(284,47,1,1,0,100,14),(285,10022,1,1,1,100,4),(286,25,8,0,0,100,4),(287,43,5,0,0,100,4),(291,26,1,1,0,97,4),(292,49,1,0,0,100,13),(293,42,1,0,0,100,13),(297,42,23,0,32687,100,14),(299,48,1,1,0,100,14),(300,2,1,1,0,100,17),(301,39,1,1,0,100,17),(302,23,9,0,0,100,17),(303,2,1,1,0,100,18),(304,39,1,1,0,100,18),(305,2,1,1,0,100,19),(306,39,1,1,0,100,19),(309,3,1,1,0,100,19),(310,1,25,0,0,100,19),(311,21,1,0,0,100,19),(313,2,1,1,0,100,20),(314,39,1,1,0,100,20),(317,3,1,1,0,100,20),(318,1,43,0,0,100,20),(320,29,1,0,0,100,20),(321,2,1,1,0,100,21),(322,39,1,1,0,100,21),(325,3,1,1,0,100,21),(326,1,333,0,0,100,21),(328,15,3,0,0,100,21),(329,14,3,0,0,100,21),(330,9,8,0,0,100,21),(331,27,1,0,0,100,21),(332,2,1,1,0,100,22),(333,39,1,1,0,100,22),(334,29,6,0,0,100,21),(337,3,1,1,0,100,22),(338,1,1064,0,0,100,22),(340,15,3,0,0,100,22),(341,14,3,0,0,100,22),(342,27,1,1,0,100,22),(345,20,76,0,0,100,4),(347,5,1,1,0,100,22),(348,9,4,0,0,100,22),(351,22,14,0,22006,100,22),(352,10019,1,1,5,100,2),(353,10019,1,1,3,100,10),(354,10019,1,1,0,100,4),(358,17,69,0,21893,100,4),(359,15003,1,1,0,100,4),(360,2,1,1,0,100,23),(361,39,1,1,0,100,23),(364,3,1,1,0,100,23),(365,1,2038,0,0,100,23),(368,15003,1,1,0,100,2),(370,15003,1,1,0,100,22),(371,15003,1,1,0,100,10),(374,10011,1,1,0,100,2),(375,39,1,1,0,100,24),(376,2,1,0,0,100,24),(377,27,1,1,0,100,23),(379,23,6,0,0,100,24),(383,2,1,1,0,100,25),(384,39,1,1,0,100,25),(385,22,119,0,6384,100,23),(386,29,14,0,3,100,23),(387,16,3,0,0,100,4),(388,50,1,0,0,100,23),(389,30,29,0,21893,100,4),(390,36,1,1,0,100,23),(391,9,3,0,0,100,5),(392,15,3,0,0,100,5),(393,14,3,0,0,100,5),(394,4,1,1,0,100,23),(395,27,1,1,0,100,5),(396,37,10,0,6384,100,14),(397,42,9,0,21977,100,4),(398,37,10,0,32687,100,10),(399,49,1,0,0,100,14),(400,37,2,0,420,100,4),(401,37,3,0,6384,100,2),(402,2,1,1,0,100,26),(403,39,1,1,0,100,26),(406,3,1,1,0,100,26),(407,1,88,0,0,100,26),(409,14,3,0,0,100,26),(410,15,3,0,0,100,26),(411,22,1,0,3,100,26),(413,7,40,0,0,100,22),(414,4,1,0,0,100,22),(415,6,1,1,0,100,22),(418,49,1,1,0,100,22),(419,10018,1,1,0,100,22),(422,15003,1,1,0,100,23),(425,32,1,1,0,100,4),(426,46,1,0,0,100,4),(427,45,1,0,0,100,10),(428,32,1,1,0,100,23),(429,5,1,1,0,100,23),(430,6,1,1,0,100,23),(431,7,1,0,0,100,23),(432,35,11,0,0,100,23),(434,1,2943,0,0,100,4),(436,15003,1,1,0,100,14),(437,9,26,0,0,100,23),(438,22,12,0,21893,100,21),(440,20,629,0,0,100,2),(441,20,53,0,0,100,21),(443,47,1,1,0,100,23),(444,20,19,0,0,100,23),(445,2,1,0,0,100,27),(446,39,1,1,0,100,27),(447,2,1,1,0,100,28),(448,39,1,1,0,100,28),(453,3,1,1,0,100,27),(456,29,2,0,0,100,27),(457,3,1,1,0,100,28),(458,1,184,0,0,100,28),(460,9,4,0,0,100,27),(461,10000,1,1,0,100,27),(462,15,3,0,0,100,28),(463,14,3,0,0,100,28),(464,4,1,0,5,100,27),(465,5,1,1,10,100,27),(466,6,1,1,10,100,27),(467,10021,1,1,10,100,27),(468,14,3,0,0,100,27),(469,15,3,0,0,100,27),(470,28,1,1,0,100,27),(471,36,1,1,0,100,27),(472,47,1,1,0,100,27),(473,49,1,1,0,100,27),(474,2,1,1,0,100,29),(475,39,1,1,0,100,29),(478,18,115,0,0,100,14),(479,3,1,0,0,100,29),(480,1,15,0,0,100,29),(483,26,1,1,1,47,14),(484,20,4,0,0,100,28),(485,7,227,0,0,100,14),(486,2,1,1,0,100,30),(487,39,1,1,0,100,30),(488,27,1,1,0,100,28),(493,21,1,0,0,100,30),(494,28,1,0,0,100,23),(496,2,1,1,0,100,31),(497,39,1,1,0,100,31),(500,3,1,1,0,100,31),(501,1,25,0,0,100,31),(502,21,1,0,0,100,31),(503,29,1,0,0,100,31),(504,14,3,0,0,100,1),(505,15,3,0,0,100,1),(507,27,1,1,0,100,1),(508,22,5,0,0,100,1),(510,7,1,0,0,100,21),(513,4,1,1,1,100,1),(514,5,1,1,100,100,1),(516,22,4,0,21893,100,30),(517,15,3,0,0,100,30),(518,14,3,0,0,100,30),(519,50,1,0,0,100,1),(520,50,1,0,0,100,21),(521,4,1,1,0,100,21),(522,5,1,1,0,100,21),(523,3,1,1,1000,100,1),(524,20,1,0,0,100,1),(525,29,1,0,5949,100,1),(526,9,2,0,0,100,1),(528,10021,1,1,0,100,22),(529,17,30,0,0,100,1),(530,15003,1,0,0,100,1),(532,20,1,0,0,100,10),(536,2,1,1,0,100,7),(537,39,1,1,0,100,7),(540,3,1,1,0,100,7),(541,1,199,0,0,100,7),(543,9,1,0,0,100,7),(544,2,1,1,0,100,32),(545,39,1,1,0,100,32),(547,27,1,1,0,100,7),(550,3,1,1,0,100,32),(551,1,15,0,0,100,32),(553,31,116,0,0,100,14),(554,6,1,1,0,100,1),(556,20,2,0,0,100,3),(558,20,20,0,0,100,30),(559,29,3,0,22006,100,22),(560,18,558,0,0,100,10),(563,2,1,1,0,100,33),(564,39,1,1,0,100,33),(565,23,12,0,0,100,33),(566,20,629,0,0,100,14),(567,30,10,0,22006,100,14),(568,15,3,0,0,100,23),(569,14,3,0,0,100,23),(570,25,8,0,0,100,23),(571,2,1,1,0,100,34),(572,39,1,1,0,100,34),(573,18,1,0,0,100,2),(574,17,5,0,9424,100,10),(575,22,3,0,0,100,27),(578,27,1,1,0,100,27),(580,29,1,0,0,100,5),(581,2,1,1,0,100,35),(582,39,1,1,0,100,35),(585,3,1,1,0,100,35),(586,1,530,0,0,100,35),(588,22,9,0,21893,100,35),(589,27,1,0,0,100,35),(592,29,7,0,21893,100,35),(593,17,10,0,21893,100,35),(594,30,2,0,21893,100,35),(595,22,1,0,21893,100,5),(596,50,1,0,0,100,35),(597,9,6,0,0,100,35),(598,4,1,1,0,100,5),(600,1,34,0,0,100,27),(601,18,3,0,0,100,27),(602,27,1,1,0,100,30),(603,3,1,1,0,100,30),(604,4,1,1,0,100,30),(605,1,58,0,0,100,30),(606,9,1,0,0,100,30);
/*!40000 ALTER TABLE `tw_accounts_items` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_accounts_mailbox`
--

DROP TABLE IF EXISTS `tw_accounts_mailbox`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_accounts_mailbox` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `ItemID` int DEFAULT NULL,
  `ItemValue` int DEFAULT NULL,
  `Enchant` int DEFAULT NULL,
  `Name` varchar(64) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `UserID` int NOT NULL,
  `IsRead` tinyint NOT NULL DEFAULT '0',
  `FromSend` varchar(32) NOT NULL DEFAULT 'Game',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  KEY `OwnerID` (`UserID`),
  KEY `tw_accounts_inbox_ibfk_2` (`ItemID`),
  CONSTRAINT `tw_accounts_mailbox_ibfk_1` FOREIGN KEY (`UserID`) REFERENCES `tw_accounts_data` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_accounts_mailbox_ibfk_2` FOREIGN KEY (`ItemID`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=36 DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_accounts_mailbox`
--

LOCK TABLES `tw_accounts_mailbox` WRITE;
/*!40000 ALTER TABLE `tw_accounts_mailbox` DISABLE KEYS */;
/*!40000 ALTER TABLE `tw_accounts_mailbox` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_accounts_mining`
--

DROP TABLE IF EXISTS `tw_accounts_mining`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_accounts_mining` (
  `UserID` int NOT NULL,
  `Level` int NOT NULL DEFAULT '1',
  `Exp` int NOT NULL DEFAULT '0',
  `Upgrade` int NOT NULL DEFAULT '0',
  `Quantity` int NOT NULL DEFAULT '1',
  PRIMARY KEY (`UserID`),
  UNIQUE KEY `AccountID` (`UserID`),
  CONSTRAINT `tw_accounts_mining_ibfk_1` FOREIGN KEY (`UserID`) REFERENCES `tw_accounts_data` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_accounts_mining`
--

LOCK TABLES `tw_accounts_mining` WRITE;
/*!40000 ALTER TABLE `tw_accounts_mining` DISABLE KEYS */;
INSERT INTO `tw_accounts_mining` VALUES (1,1,0,0,1),(2,8,376,0,8),(3,3,4,0,3),(4,1,0,0,1),(5,1,0,0,1),(6,1,0,0,1),(7,1,0,0,1),(8,1,0,0,1),(9,1,0,0,1),(10,6,676,0,6),(11,1,0,0,1),(12,1,0,0,1),(13,1,0,0,1),(14,5,9,0,5),(15,1,0,0,1),(16,1,0,0,1),(17,1,0,0,1),(18,1,0,0,1),(19,1,0,0,1),(20,1,0,0,1),(21,1,0,0,1),(22,1,0,0,1),(23,1,0,0,1),(24,1,0,0,1),(25,1,0,0,1),(26,1,0,0,1),(27,1,0,0,1),(28,1,0,0,1),(29,1,0,0,1),(30,1,0,0,1),(31,1,0,0,1),(32,1,0,0,1),(33,1,0,0,1),(34,1,0,0,1),(35,1,0,0,1);
/*!40000 ALTER TABLE `tw_accounts_mining` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_accounts_quests`
--

DROP TABLE IF EXISTS `tw_accounts_quests`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_accounts_quests` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `QuestID` int DEFAULT NULL,
  `UserID` int NOT NULL,
  `Step` int NOT NULL DEFAULT '1',
  `Type` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  UNIQUE KEY `UK_tw_accounts_quests` (`QuestID`,`UserID`),
  KEY `OwnerID` (`UserID`),
  KEY `tw_accounts_quests_ibfk_4` (`QuestID`),
  CONSTRAINT `tw_accounts_quests_ibfk_3` FOREIGN KEY (`UserID`) REFERENCES `tw_accounts_data` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_accounts_quests_ibfk_4` FOREIGN KEY (`QuestID`) REFERENCES `tw_quests_list` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=389 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_accounts_quests`
--

LOCK TABLES `tw_accounts_quests` WRITE;
/*!40000 ALTER TABLE `tw_accounts_quests` DISABLE KEYS */;
INSERT INTO `tw_accounts_quests` VALUES (1,5,1,1,2),(2,5,2,1,2),(3,1,1,1,2),(4,1,2,1,2),(5,2,2,1,2),(6,2,1,1,2),(7,3,1,1,2),(8,5,4,1,2),(9,1,4,1,2),(10,2,4,1,2),(11,3,4,1,2),(12,4,4,1,2),(13,1,8,1,2),(14,10,2,1,2),(15,2,8,1,2),(16,5,8,1,2),(17,3,8,1,2),(18,10,8,1,2),(19,6,8,1,2),(20,50,8,1,1),(21,7,8,1,1),(22,55,8,1,1),(23,11,8,1,2),(24,4,8,1,1),(25,1,6,1,2),(26,6,2,1,2),(27,1,9,1,2),(28,2,6,1,2),(29,50,2,1,2),(30,7,2,1,2),(31,2,9,1,1),(32,3,9,1,1),(33,1,10,1,2),(34,3,6,1,2),(35,2,10,1,2),(36,3,10,1,2),(37,11,2,1,2),(38,4,6,1,1),(39,12,2,1,2),(40,5,5,1,2),(41,1,5,1,2),(42,5,6,1,2),(43,13,2,1,2),(44,6,6,1,2),(45,4,10,1,2),(46,2,5,1,2),(47,10,6,1,2),(48,55,2,1,2),(49,3,5,1,2),(50,1,12,1,2),(51,2,12,1,2),(52,7,6,1,1),(53,50,6,1,1),(54,3,12,1,2),(55,3,2,1,2),(56,11,6,1,2),(57,12,6,1,2),(58,55,6,1,1),(59,13,6,1,1),(60,5,13,1,2),(61,1,13,1,2),(62,4,2,1,2),(63,5,12,1,1),(64,2,13,1,2),(65,4,12,1,1),(66,12,8,1,2),(67,10,12,1,1),(68,50,12,1,1),(69,6,13,1,2),(70,7,13,1,1),(71,55,13,1,1),(72,55,12,1,1),(73,10,13,1,2),(74,50,13,1,1),(75,5,10,1,2),(76,5,14,1,2),(77,11,13,1,2),(78,13,8,1,2),(79,10,10,1,2),(80,12,13,1,2),(81,50,10,1,2),(82,6,10,1,2),(83,13,13,1,2),(84,7,10,1,2),(85,1,14,1,2),(86,11,10,1,2),(87,12,10,1,2),(88,55,10,1,2),(89,13,10,1,2),(90,2,14,1,2),(91,14,13,1,2),(92,8,13,1,2),(93,15,13,1,2),(94,16,13,1,2),(95,10,14,1,2),(96,6,14,1,2),(97,14,8,1,2),(98,8,8,1,1),(99,15,8,1,2),(100,7,14,1,2),(101,50,14,1,1),(102,11,14,1,2),(103,12,14,1,2),(104,55,14,1,2),(105,13,14,1,2),(106,16,8,1,1),(107,14,2,1,2),(108,15,2,1,2),(109,14,14,1,2),(110,15,14,1,2),(111,8,14,1,2),(112,16,14,1,2),(113,3,13,1,2),(114,14,10,1,2),(115,8,10,1,2),(116,4,13,1,2),(117,15,10,1,2),(118,3,14,1,2),(119,16,2,1,2),(120,16,10,1,2),(121,4,14,1,2),(122,1,3,1,2),(123,2,3,1,2),(124,3,3,1,2),(125,4,3,1,2),(126,10,3,1,2),(127,50,3,1,1),(128,6,3,1,2),(129,11,3,1,2),(130,12,3,1,2),(131,7,3,1,2),(132,13,3,1,2),(133,8,2,1,2),(134,14,3,1,2),(135,8,3,1,2),(136,15,3,1,2),(137,16,3,1,2),(138,9,10,1,2),(139,10,4,1,2),(140,50,4,1,2),(141,6,4,1,2),(142,11,4,1,2),(143,12,4,1,2),(144,7,4,1,2),(145,13,4,1,2),(146,9,2,1,2),(147,9,3,1,2),(148,9,13,1,2),(149,17,2,1,2),(150,18,2,1,2),(151,19,2,1,2),(152,17,10,1,2),(153,20,2,1,2),(154,17,13,1,2),(155,18,10,1,2),(156,60,2,1,2),(157,61,2,1,2),(158,18,13,1,2),(159,19,10,1,2),(160,1,15,1,2),(161,19,13,1,2),(162,2,15,1,1),(163,3,15,1,1),(164,20,10,1,2),(165,60,10,1,2),(166,61,10,1,2),(167,1,16,1,1),(168,20,13,1,2),(169,60,13,1,2),(170,61,13,1,1),(171,17,3,1,2),(172,18,3,1,2),(173,19,3,1,2),(174,20,3,1,2),(175,60,3,1,2),(176,61,3,1,1),(177,21,10,1,2),(178,9,14,1,2),(179,21,2,1,2),(180,21,3,1,1),(181,5,3,1,2),(182,55,3,1,1),(183,14,4,1,2),(184,8,4,1,2),(185,15,4,1,2),(186,62,10,1,1),(187,16,4,1,2),(188,62,2,1,2),(189,17,14,1,2),(190,17,4,1,2),(191,18,14,1,2),(192,21,13,1,1),(193,19,14,1,2),(194,20,14,1,2),(195,60,14,1,2),(196,61,14,1,1),(197,21,14,1,2),(198,1,17,1,1),(199,1,19,1,2),(200,2,19,1,1),(201,3,19,1,1),(202,1,20,1,2),(203,2,20,1,2),(204,1,21,1,2),(205,2,21,1,2),(206,5,21,1,2),(207,3,21,1,2),(208,4,21,1,1),(209,10,21,1,2),(210,50,21,1,1),(211,11,21,1,2),(212,12,21,1,2),(213,55,21,1,1),(214,13,21,1,2),(215,1,22,1,2),(216,5,22,1,2),(217,2,22,1,2),(218,3,22,1,2),(219,4,22,1,1),(220,10,22,1,2),(221,6,22,1,2),(222,50,22,1,1),(223,7,22,1,1),(224,11,22,1,2),(225,12,22,1,2),(226,55,22,1,1),(227,13,22,1,2),(228,55,4,1,1),(229,1,23,1,2),(230,2,23,1,2),(231,5,23,1,2),(232,10,23,1,2),(233,50,23,1,1),(234,6,23,1,2),(235,7,23,1,1),(236,11,23,1,2),(237,12,23,1,2),(238,55,23,1,1),(239,13,23,1,2),(240,1,24,1,1),(241,14,22,1,2),(242,15,22,1,2),(243,16,22,1,1),(244,14,23,1,2),(245,8,23,1,2),(246,15,23,1,2),(247,16,23,1,1),(248,18,4,1,2),(249,1,25,1,1),(250,5,25,1,1),(251,19,4,1,2),(252,20,4,1,2),(253,60,4,1,2),(254,61,4,1,1),(255,4,5,1,1),(256,10,5,1,2),(257,55,5,1,1),(258,11,5,1,2),(259,12,5,1,2),(260,13,5,1,2),(261,22,14,1,2),(262,22,2,1,2),(263,22,10,1,2),(264,21,4,1,2),(265,22,4,1,2),(266,1,26,1,2),(267,2,26,1,2),(268,3,26,1,2),(269,4,26,1,1),(270,5,26,1,1),(271,10,26,1,1),(272,50,26,1,1),(273,55,26,1,1),(274,9,4,1,2),(275,9,23,1,2),(276,14,21,1,2),(277,15,21,1,2),(278,16,21,1,1),(279,8,21,1,1),(280,6,21,1,2),(281,7,21,1,1),(282,1,27,1,2),(283,1,28,1,2),(284,2,27,1,2),(285,5,28,1,1),(286,2,28,1,2),(287,3,28,1,2),(288,4,28,1,1),(289,10,27,1,2),(290,6,27,1,2),(291,55,27,1,1),(292,50,27,1,1),(293,5,27,1,2),(294,3,27,1,2),(295,6,28,1,2),(296,50,28,1,1),(297,4,27,1,1),(298,7,27,1,1),(299,7,28,1,1),(300,1,29,1,2),(301,5,29,1,1),(302,2,29,1,1),(303,10,28,1,2),(304,11,28,1,2),(305,12,28,1,1),(306,5,30,1,2),(307,1,30,1,2),(308,55,28,1,1),(309,2,30,1,2),(310,3,30,1,2),(311,1,31,1,2),(312,5,31,1,1),(313,2,31,1,1),(314,3,31,1,1),(315,4,1,1,1),(316,50,1,1,1),(317,55,1,1,1),(318,6,1,1,2),(319,7,1,1,1),(320,10,1,1,2),(321,11,1,1,2),(322,12,1,1,2),(323,13,1,1,2),(324,14,1,1,2),(325,8,1,1,1),(326,15,1,1,2),(327,16,1,1,1),(328,4,30,1,1),(329,10,30,1,2),(330,6,30,1,2),(331,7,30,1,1),(332,50,30,1,1),(333,11,30,1,2),(334,12,30,1,2),(335,55,30,1,1),(336,1,7,1,2),(337,2,7,1,2),(338,10,7,1,2),(339,50,7,1,1),(340,6,7,1,2),(341,11,7,1,2),(342,5,32,1,1),(343,12,7,1,2),(344,7,7,1,1),(345,55,7,1,1),(346,13,7,1,1),(347,1,32,1,2),(348,2,32,1,1),(349,5,7,1,1),(350,13,30,1,2),(351,14,30,1,2),(352,8,30,1,1),(353,15,30,1,2),(354,16,30,1,1),(355,5,33,1,1),(356,1,33,1,1),(357,3,23,1,2),(358,4,23,1,2),(359,11,27,1,2),(360,12,27,1,2),(361,13,27,1,2),(362,14,27,1,2),(363,8,27,1,1),(364,15,27,1,2),(365,16,27,1,1),(366,1,35,1,2),(367,2,35,1,2),(368,5,35,1,2),(369,10,35,1,2),(370,6,35,1,2),(371,50,35,1,1),(372,7,35,1,1),(373,55,35,1,1),(374,11,35,1,2),(375,12,35,1,2),(376,13,35,1,2),(377,14,35,1,2),(378,8,35,1,1),(379,15,35,1,2),(380,16,35,1,1),(381,50,5,1,1),(382,6,5,1,2),(383,7,5,1,1),(384,14,5,1,2),(385,8,5,1,1),(386,15,5,1,2),(387,16,5,1,1),(388,8,22,1,1);
/*!40000 ALTER TABLE `tw_accounts_quests` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_accounts_skills`
--

DROP TABLE IF EXISTS `tw_accounts_skills`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_accounts_skills` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `SkillID` int NOT NULL,
  `UserID` int NOT NULL,
  `Level` int NOT NULL,
  `UsedByEmoticon` int DEFAULT '-1',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  KEY `SkillID` (`SkillID`),
  KEY `OwnerID` (`UserID`),
  CONSTRAINT `tw_accounts_skills_ibfk_1` FOREIGN KEY (`SkillID`) REFERENCES `tw_skills_list` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_accounts_skills_ibfk_2` FOREIGN KEY (`UserID`) REFERENCES `tw_accounts_data` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=22 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_accounts_skills`
--

LOCK TABLES `tw_accounts_skills` WRITE;
/*!40000 ALTER TABLE `tw_accounts_skills` DISABLE KEYS */;
INSERT INTO `tw_accounts_skills` VALUES (1,1,2,2,-1),(2,1,10,4,13),(3,1,3,2,2),(4,1,13,4,-1),(5,2,3,2,-1),(6,6,2,1,-1),(7,1,14,2,-1),(8,6,10,1,12),(9,2,2,1,-1),(10,5,3,1,-1),(11,2,4,10,-1),(12,2,14,1,-1),(13,5,4,4,-1),(14,1,22,1,3),(15,4,4,1,-1),(16,6,4,4,-1),(17,1,23,1,-1),(18,3,4,4,-1),(19,1,4,8,2),(20,5,10,1,-1),(21,6,22,1,2);
/*!40000 ALTER TABLE `tw_accounts_skills` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_aethers`
--

DROP TABLE IF EXISTS `tw_aethers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_aethers` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Name` varchar(64) NOT NULL DEFAULT 'Teleport name',
  `WorldID` int NOT NULL,
  `TeleX` int NOT NULL,
  `TeleY` int NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  KEY `WorldID` (`WorldID`),
  CONSTRAINT `tw_aethers_ibfk_1` FOREIGN KEY (`WorldID`) REFERENCES `enum_worlds` (`WorldID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_aethers`
--

LOCK TABLES `tw_aethers` WRITE;
/*!40000 ALTER TABLE `tw_aethers` DISABLE KEYS */;
INSERT INTO `tw_aethers` VALUES (1,'Crossroad',2,8033,7089),(2,'Pier',0,3680,1150),(3,'Guard post',5,1536,4396),(4,'Yugasaki',11,7070,1980);
/*!40000 ALTER TABLE `tw_aethers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_attributs`
--

DROP TABLE IF EXISTS `tw_attributs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_attributs` (
  `ID` int NOT NULL,
  `Name` varchar(32) NOT NULL,
  `FieldName` varchar(32) NOT NULL DEFAULT 'unfield',
  `Price` int NOT NULL,
  `Type` int NOT NULL COMMENT '0.tank1.healer2.dps3.weapon4.hard5.jobs 6. others',
  `Divide` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_attributs`
--

LOCK TABLES `tw_attributs` WRITE;
/*!40000 ALTER TABLE `tw_attributs` DISABLE KEYS */;
INSERT INTO `tw_attributs` VALUES (1,'Shotgun Spread','SpreadShotgun',100,3,0),(2,'Grenade Spread','SpreadGrenade',100,3,0),(3,'Rifle Spread','SpreadRifle',100,3,0),(4,'Strength','unfield',0,4,10),(5,'Dexterity','Dexterity',1,2,5),(6,'Crit Dmg','CriticalHit',1,2,5),(7,'Direct Crit Dmg','DirectCriticalHit',1,2,5),(8,'Hardness','Hardness',1,0,5),(9,'Lucky','Lucky',1,0,5),(10,'Piety','Piety',1,1,5),(11,'Vampirism','Vampirism',1,1,5),(12,'Ammo Regen','AmmoRegen',1,3,5),(13,'Ammo','Ammo',30,3,0),(14,'Efficiency','unfield',-1,5,0),(15,'Extraction','unfield',-1,5,0),(16,'Hammer Power','unfield',-1,4,10),(17,'Gun Power','unfield',-1,4,10),(18,'Shotgun Power','unfield',-1,4,10),(19,'Grenade Power','unfield',-1,4,10),(20,'Rifle Power','unfield',-1,4,10),(21,'Lucky items','unfield',-1,6,5);
/*!40000 ALTER TABLE `tw_attributs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_bots_info`
--

DROP TABLE IF EXISTS `tw_bots_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_bots_info` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Name` varchar(32) NOT NULL DEFAULT 'Bot name',
  `SkinName` varchar(128) NOT NULL DEFAULT '''bear standard standard standard standard standard''' COMMENT 'body marking deco hands feet eyes',
  `SkinColor` varchar(128) NOT NULL DEFAULT '''0 0 0 0 0 0''' COMMENT 'body marking deco hands feet eyes	',
  `SlotHammer` int DEFAULT NULL,
  `SlotGun` int DEFAULT NULL,
  `SlotShotgun` int DEFAULT NULL,
  `SlotGrenade` int DEFAULT NULL,
  `SlotRifle` int DEFAULT NULL,
  `SlotWings` int DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  KEY `SlotWings` (`SlotWings`),
  KEY `SlotHammer` (`SlotHammer`),
  KEY `SlotGun` (`SlotGun`),
  KEY `tw_bots_world_ibfk_4` (`SlotShotgun`),
  KEY `SlotGrenade` (`SlotGrenade`),
  KEY `SlotRifle` (`SlotRifle`),
  CONSTRAINT `tw_bots_info_ibfk_1` FOREIGN KEY (`SlotWings`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_info_ibfk_2` FOREIGN KEY (`SlotHammer`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_info_ibfk_3` FOREIGN KEY (`SlotGun`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_info_ibfk_4` FOREIGN KEY (`SlotShotgun`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_info_ibfk_5` FOREIGN KEY (`SlotGrenade`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_info_ibfk_6` FOREIGN KEY (`SlotRifle`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=60 DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_bots_info`
--

LOCK TABLES `tw_bots_info` WRITE;
/*!40000 ALTER TABLE `tw_bots_info` DISABLE KEYS */;
INSERT INTO `tw_bots_info` VALUES (1,'Captain','puar warstripes hair standard standard standardreal','16187160 -16645889 131327 16177260 7624169 65408',10010,NULL,NULL,NULL,NULL,NULL),(2,'Sailor','standard bear hair standard standard standard','1082745 -15634776 65408 1082745 1147174 65408',NULL,NULL,NULL,NULL,NULL,NULL),(3,'Carpenter','puar tiger2  duotone standard moustache','15007529 -14563483 65408 3827951 1310537 65422',NULL,NULL,NULL,NULL,NULL,NULL),(4,'Worker','standard cammostripes  standard standard standard','1821867 -14840320 65408 750848 1944919 65408',NULL,NULL,NULL,NULL,NULL,NULL),(5,'Mr. Worker','trela cammostripes  standard standard standard','1662583 -14840320 65408 750848 1944919 65408',NULL,NULL,NULL,NULL,NULL,NULL),(6,'Diana','raccoon coonfluff  standard standard standard','965254 -15235151 65408 1769643 1305243 1085234',NULL,NULL,NULL,NULL,NULL,NULL),(7,'Mr. Guard','flokes flokes_gray  standard standard standard','1638493 -15138752 65408 1769472 1638422 255',NULL,NULL,NULL,NULL,NULL,NULL),(8,'Brother','bear panda1 hair standard standard standard','9834574 -6411543 65408 1769630 1835070 41215',NULL,NULL,NULL,NULL,NULL,NULL),(9,'Green slime','bear downdony hair duotone standard standard','3981441 -29333592 5313052 14500779 5468601 6321790',NULL,NULL,NULL,NULL,NULL,NULL),(10,'Apostle Elfia','kitty bear  duotone standard standard','9568256 -1695744161 12254015 3972544 15204215 7470034',10024,NULL,NULL,NULL,NULL,10017),(11,'Craftsman','bear tiger1 twinpen duotone standard colorable','12770027 828507979 11162385 6849346 44458 8581506',NULL,NULL,NULL,NULL,NULL,NULL),(12,'Auctionist','kitty saddo twinbopp duotone standard colorable','12201075 -855657567 2205432 3349551 6943484 13531062',NULL,NULL,NULL,NULL,NULL,NULL),(13,'Teacher','mouse purelove unibop duotone standard moustache','8467692 -1394954365 12408709 11534535 3010026 5627093',NULL,NULL,NULL,NULL,NULL,NULL),(14,'Nurse','flokes downdony hair duotone standard standard','15920331 1593835335 4147122 3795484 16279737 10976418',NULL,NULL,NULL,NULL,10023,10015),(15,'Gunsmith Eric','kitty cammostripes hair duotone standard standard','5209108 1463395762 12628238 8169037 3830859 2771259',NULL,NULL,NULL,NULL,NULL,NULL),(16,'Mr. Sentry','raccoon tripledon hair duotone standard standard','10060032 1690185928 11278269 5677608 886610 13831075',NULL,NULL,NULL,NULL,NULL,NULL),(18,'Daughter Sailor','kitty whisker hair duotone standard standard','3981441 -18874784 5313052 14500779 8614329 6321790',NULL,NULL,NULL,NULL,NULL,NULL),(19,'Miner','standard toptri hair duotone standard colorable','16718165 -2012161487 3858792 7940502 3052250 1873584',NULL,NULL,NULL,NULL,NULL,NULL),(20,'Customs officer','kitty cammo1 hair duotone standard standardreal','6083870 -2125570185 6324018 7470633 16541982 2000684',NULL,NULL,NULL,NULL,NULL,NULL),(21,'Seller artifact','flokes saddo hair duotone standard colorable','13769006 -35537528 12473004 465468 14009515 803609',NULL,NULL,NULL,NULL,NULL,NULL),(22,'Goblin','bear hipbel  duotone standard standard','6301461 -1695632164 12254015 3972544 9710385 7470034',NULL,NULL,NULL,NULL,NULL,NULL),(23,'Blue slime','bear bear  duotone standard standard','9610633 -1695632164 12254015 3972544 9729630 7470034',NULL,NULL,NULL,NULL,NULL,NULL),(25,'Adventurer Koto','bear belly1 hair2 duotone standard standardreal','2307609 -167295050 13142259 5885245 9371648 7807949',NULL,NULL,NULL,NULL,NULL,NULL),(26,'Messenger','puar duodonny  duotone standard negative','7018125 -2047512102 9555542 2060938 1379442 14886533',NULL,NULL,NULL,NULL,NULL,NULL),(27,'Salantra','kitty bear  duotone standard colorable','15557993 -346596366 16130358 701247 11272539 4707488',NULL,NULL,NULL,NULL,NULL,NULL),(28,'Pink Slime','bear bear hair2 duotone standard standard','14212759 -2065039105 16307984 12212874 16062570 8064920',NULL,NULL,NULL,NULL,NULL,NULL),(29,'Dead miner','kitty stripe hair duotone standard colorable','15995954 121878199 10365700 1972392 8301545 1240658',NULL,NULL,NULL,NULL,NULL,NULL),(30,'Hobgoblin','bear cammo2 twinmello duotone standard negative','4331554 -1830111313 6962262 8759142 9574495 10700700',NULL,NULL,NULL,NULL,NULL,NULL),(31,'Orc','bear downdony  duotone standard standard','4136201 -406977393 5258828 12493916 2359072 5233408',NULL,NULL,NULL,NULL,NULL,NULL),(32,'Leader Orcs','monkey twinbelly  duotone standard standardreal','16495882 -1326302539 9549353 8668187 2063263 1406656',10019,10020,10021,10022,10023,10016),(33,'Twins','bear bear  duotone standard standard','9683673 672388260 12653934 1134912 4360420 8598003',NULL,NULL,NULL,NULL,NULL,NULL),(34,'Kappa','standard sidemarks unibop duotone standard colorable','4821534 821909703 1852219 1132310 5264192 15588285',NULL,NULL,NULL,NULL,NULL,NULL),(35,'Orc warrior','fox cammostripes  standard zilly!0007 zilly!0007','65408 -16711808 65408 65408 65408 65408',NULL,NULL,NULL,NULL,NULL,NULL),(36,'Brown slime','bear duodonny hair duotone standard standard','1091872 -394592059 12674866 8116231 1151 14198436',NULL,NULL,NULL,NULL,NULL,NULL),(37,'Officer Henry','flokes warstripes hair2 duotone standard standard','5482497 -1837124528 15809857 10752857 7539007 3358899',NULL,NULL,NULL,NULL,NULL,NULL),(38,'Daughter Maria','flokes duodonny hair duotone standard standardreal','2653584 -36345458 3470770 9908420 14444105 738196',NULL,NULL,NULL,NULL,NULL,NULL),(39,'Noctis','kitty mice hair duotone standard standard','11017984 1596522751 5772960 14417927 5570560 9381961',10010,NULL,NULL,NULL,NULL,10017),(40,'Chocobo','kitty bear hair duotone standard standard','2490221 -1792213144 2096979 2157678 1735229 1103953',NULL,NULL,NULL,NULL,NULL,NULL),(41,'Benny','bear tricircular hair duotone standard colorable','16066652 -1087601154 3829858 1314019 641021 1376511',NULL,NULL,NULL,NULL,NULL,NULL),(42,'Rogalia','monkey donny unibop duotone standard colorable','8347469 -852808362 15666037 15456343 5845918 7003270',NULL,NULL,NULL,NULL,NULL,NULL),(43,'Kengo','monkey donny unibop duotone standard colorable','8347469 -852808362 15666037 15456343 5845918 7003270',NULL,NULL,NULL,NULL,NULL,NULL),(44,'Goshii','bear whisker hair2 duotone standard standard','10571316 -1915300733 1789119 12372457 338092 9300965',10019,10020,10021,10022,10023,10018),(45,'Maid','flokes downdony twinmello duotone standard standardreal','4260095 -1698513476 10819474 1784648 14990515 10338195',NULL,NULL,NULL,NULL,NULL,NULL),(46,'Yasue San','mouse flokes unibop duotone standard colorable','8925949 -1578020608 9437184 14815652 862504 8671829',NULL,NULL,NULL,NULL,NULL,NULL),(47,'Dead soul','bear sidemarks horns duotone standard standardreal','692505 -1831891179 13771590 7098987 3994129 8711018',NULL,NULL,NULL,NULL,NULL,NULL),(48,'Skeleton','standard mixture1 twinpen duotone standard colorable','9830655 1328873727 4260095 11171578 11796735 4135487',NULL,NULL,NULL,NULL,NULL,NULL),(49,'Librarian','trela bear twinmello duotone standard sunglasses','11686168 -1962207864 7675723 14445598 12185649 15772890',NULL,NULL,NULL,NULL,NULL,NULL),(50,'Neptune','spiky cammo1 twinmello duotone standard standardreal','7526428 -1490205206 9650234 16143024 5143432 7378889',10019,10020,10021,10022,10023,10006),(51,'Farmer','standard cammostripes  standard standard standard','1821867 -14840320 65408 750848 1944919 65408',NULL,NULL,NULL,NULL,NULL,NULL),(53,'Master','puar mice  duotone standard moustache','11763922 -15685931 65408 1102450 1232060 1376256',NULL,NULL,NULL,NULL,NULL,NULL),(54,'shengyan','trela cammostripes  standard standard standard','1662583 -12814320 65601 752848 1944919 65408',NULL,NULL,NULL,NULL,NULL,10007),(55,'Ninecloud','trela cammostripes  standard standard standard','1662583 -12814320 65601 752848 1944919 65408',10010,NULL,NULL,NULL,NULL,10015),(57,'STAR On The SKY','bluekitty twincross  standard standard negative','1662583 -12814320 65601 752848 1944919 65408',10010,10001,4,10003,10023,10015),(58,'DONKEY On The SKY','default twincross  standard standard negative','1111113 -12814320 66666 666666 6666666 66666',NULL,NULL,10021,NULL,NULL,NULL),(59,'MC_TYH','random twincross  standard standard negative','1166666 -12666666 66666 666666 6666666 66666',10019,10020,10021,10022,10023,10015);
/*!40000 ALTER TABLE `tw_bots_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_bots_mobs`
--

DROP TABLE IF EXISTS `tw_bots_mobs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_bots_mobs` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `BotID` int NOT NULL DEFAULT '-1',
  `WorldID` int DEFAULT NULL,
  `PositionX` int NOT NULL,
  `PositionY` int NOT NULL,
  `Effect` varchar(16) DEFAULT NULL,
  `Behavior` varchar(32) NOT NULL DEFAULT 'Standard',
  `Level` int NOT NULL DEFAULT '1',
  `Power` int NOT NULL DEFAULT '10',
  `Spread` int NOT NULL DEFAULT '0',
  `Number` int NOT NULL DEFAULT '1',
  `Respawn` int NOT NULL DEFAULT '1',
  `Boss` tinyint(1) NOT NULL DEFAULT '0',
  `it_drop_0` int DEFAULT NULL,
  `it_drop_1` int DEFAULT NULL,
  `it_drop_2` int DEFAULT NULL,
  `it_drop_3` int DEFAULT NULL,
  `it_drop_4` int DEFAULT NULL,
  `it_drop_count` varchar(64) NOT NULL DEFAULT '[0][0][0][0][0]',
  `it_drop_chance` varchar(64) NOT NULL DEFAULT '[0][0][0][0][0]',
  PRIMARY KEY (`ID`),
  KEY `MobID` (`BotID`),
  KEY `it_drop_0` (`it_drop_0`),
  KEY `it_drop_1` (`it_drop_1`),
  KEY `it_drop_2` (`it_drop_2`),
  KEY `it_drop_3` (`it_drop_3`),
  KEY `it_drop_4` (`it_drop_4`),
  KEY `WorldID` (`WorldID`),
  KEY `Effect` (`Effect`),
  KEY `Behavior` (`Behavior`),
  CONSTRAINT `tw_bots_mobs_ibfk_1` FOREIGN KEY (`BotID`) REFERENCES `tw_bots_info` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_mobs_ibfk_10` FOREIGN KEY (`it_drop_1`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_mobs_ibfk_11` FOREIGN KEY (`it_drop_2`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_mobs_ibfk_12` FOREIGN KEY (`it_drop_3`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_mobs_ibfk_13` FOREIGN KEY (`it_drop_4`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_mobs_ibfk_15` FOREIGN KEY (`Effect`) REFERENCES `enum_effects_list` (`Name`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_mobs_ibfk_16` FOREIGN KEY (`Behavior`) REFERENCES `enum_behavior_mobs` (`Behavior`) ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_mobs_ibfk_8` FOREIGN KEY (`WorldID`) REFERENCES `enum_worlds` (`WorldID`) ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_mobs_ibfk_9` FOREIGN KEY (`it_drop_0`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=24 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_bots_mobs`
--

LOCK TABLES `tw_bots_mobs` WRITE;
/*!40000 ALTER TABLE `tw_bots_mobs` DISABLE KEYS */;
INSERT INTO `tw_bots_mobs` VALUES (1,36,1,4049,890,'Slowdown','Slime',2,8,0,5,5,0,22,29,NULL,NULL,NULL,'|1|1|0|0|0|','|1.08|1.08|0|0|0|'),(2,23,3,3057,2577,'Slowdown','Slime',4,18,0,12,5,0,29,NULL,NULL,NULL,NULL,'|1|0|0|0|0|','|3.2|0|0|0|0|'),(3,9,3,1890,1160,'Slowdown','Slime',2,10,0,12,5,0,22,NULL,NULL,NULL,NULL,'|1|0|0|0|0|','|3.2|0|0|0|0|'),(4,28,3,3057,2577,'Slowdown','Slime',10,240,0,1,320,1,29,22,NULL,NULL,NULL,'|3|3|0|0|0|','|100|100|0|0|0|'),(5,22,5,1345,2600,NULL,'Standard',8,15,1,14,5,0,30,NULL,NULL,NULL,NULL,'|1|0|0|0|0|','|1.56|0|0|0|0|'),(7,29,6,2825,2430,NULL,'Standard',12,50,1,10,1,0,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|','|0|0|0|0|0|'),(8,30,6,4840,2560,'Fire','Standard',12,80,2,8,1,0,30,NULL,NULL,NULL,NULL,'|1|0|0|0|0|','|4.56|0|0|0|0|'),(9,31,6,1150,3700,'Poison','Standard',12,50,1,10,1,0,42,NULL,NULL,NULL,NULL,'|1|0|0|0|0|','|3.47|0|0|0|0|'),(10,22,6,1440,5100,NULL,'Standard',12,60,1,10,1,0,30,NULL,NULL,NULL,NULL,'|1|0|0|0|0|','|3.96|0|0|0|0|'),(11,32,6,3960,4595,'Fire','Standard',15,800,1,1,1,1,37,42,NULL,NULL,NULL,'|1|1|0|0|0|','|50|75|0|0|0|'),(12,34,3,1570,2915,NULL,'Standard',6,27,0,8,5,0,35,NULL,NULL,NULL,NULL,'|1|0|0|0|0|','|4.1|0|0|0|0|'),(13,35,5,1345,2600,NULL,'Standard',14,620,1,1,400,1,30,42,NULL,NULL,NULL,'|1|1|0|0|0|','|100|25|0|0|0|'),(14,40,8,3665,390,'Fire','Standard',18,100,1,10,1,0,44,NULL,NULL,NULL,NULL,'|1|0|0|0|0|','|7.5|0|0|0|0|'),(15,41,8,5610,2865,'Poison','Standard',19,120,1,10,1,0,44,NULL,NULL,NULL,NULL,'|1|0|0|0|0|','|3.5|0|0|0|0|'),(16,42,8,2400,3150,'Fire','Standard',20,110,1,10,1,0,44,NULL,NULL,NULL,NULL,'|1|0|0|0|0|','|5.25|0|0|0|0|'),(17,43,8,1720,3180,'Fire','Standard',20,115,1,10,1,0,44,NULL,NULL,NULL,NULL,'|1|0|0|0|0|','|7.5|0|0|0|0|'),(18,44,8,2800,1640,'Fire','Standard',25,2090,1,1,1,1,44,NULL,NULL,NULL,NULL,'|3|0|0|0|0|','|100|0|0|0|0|'),(19,47,10,2030,1260,'Fire','Standard',15,90,2,16,1,0,46,NULL,NULL,NULL,NULL,'|1|0|0|0|0|','|2.57|0|0|0|0|'),(20,48,10,4446,577,'Fire','Standard',16,110,2,16,1,0,46,NULL,NULL,NULL,NULL,'|1|0|0|0|0|','|2.57|0|0|0|0|'),(21,49,10,61,1539,'Fire','Sleepy',16,120,2,8,1,0,46,NULL,NULL,NULL,NULL,'|1|0|0|0|0|','|3.67|0|0|0|0|'),(22,50,10,6545,700,'Fire','Standard',20,1820,3,1,1,1,45,NULL,NULL,NULL,NULL,'|1|0|0|0|0|','|50|0|0|0|0|');
/*!40000 ALTER TABLE `tw_bots_mobs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_bots_npc`
--

DROP TABLE IF EXISTS `tw_bots_npc`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_bots_npc` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `BotID` int NOT NULL DEFAULT '-1',
  `PosX` int NOT NULL,
  `PosY` int NOT NULL,
  `GivesQuestID` int DEFAULT NULL,
  `DialogData` varchar(4096) NOT NULL DEFAULT 'empty',
  `Function` int NOT NULL DEFAULT '-1',
  `Static` int NOT NULL,
  `Emote` int NOT NULL DEFAULT '0' COMMENT '1.Pain 2.Happy 3.Surprise 4.Angry 5.Blink	',
  `Number` int NOT NULL DEFAULT '1',
  `WorldID` int DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `MobID` (`BotID`),
  KEY `WorldID` (`WorldID`),
  KEY `tw_bots_npc_ibfk_3` (`Emote`),
  KEY `tw_bots_npc_ibfk_5` (`GivesQuestID`),
  CONSTRAINT `tw_bots_npc_ibfk_1` FOREIGN KEY (`BotID`) REFERENCES `tw_bots_info` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_npc_ibfk_3` FOREIGN KEY (`Emote`) REFERENCES `enum_emotes` (`ID`),
  CONSTRAINT `tw_bots_npc_ibfk_4` FOREIGN KEY (`WorldID`) REFERENCES `enum_worlds` (`WorldID`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_npc_ibfk_5` FOREIGN KEY (`GivesQuestID`) REFERENCES `tw_quests_list` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=30 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_bots_npc`
--

LOCK TABLES `tw_bots_npc` WRITE;
/*!40000 ALTER TABLE `tw_bots_npc` DISABLE KEYS */;
INSERT INTO `tw_bots_npc` VALUES (1,1,1985,977,NULL,'[{\"text\":\"**He said surprised**: What a wonderful weather today!\",\"style\":\"joyful\",\"emote\":\"happy\"},{\"text\":\"[p]Yes, I think so too.\",\"emote\":\"happy\"},{\"text\":\"Well, how do you like on our ship [Player]? Captures the ocean, i think I\'ll stay here a little longer, it\'s wonderful here.\"},{\"text\":\"[p]I also really like it. Okay, I\'ll go, I wish you a good rest [Talked]!\"},{\"text\":\"And I wish you good luck too [Player]!\",\"style\":\"joyful\",\"emote\":\"happy\"}]',-1,1,2,1,0),(2,2,1022,1073,NULL,'[{\"text\":\"**He looks very tired**: Very hot, impossible to work.\",\"style\":\"aggresive\",\"emote\":\"pain\"}]',-1,0,5,2,0),(3,3,2691,1009,1,'[{\"emote\":\"blink\",\"text\":\"[p]Hello, I arrived from a small village beyond the mountains and the ocean, my name is [Player], I have a desire to become an adventurer. But I do not know about this place, could you help me?\"},{\"text\":\"Hello [Player], Yes of course I will help you, explain where you need to go, but you help my workers, they do not cope at all.\"},{\"emote\":\"happy\",\"style\":\"joyful\",\"text\":\"[p]What you need help with, I\'m good at dealing with predators without meat you will not leave! *laughter*\"},{\"emote\":\"blink\",\"text\":\"Oh, you\'re a Joker, no thanks go a little further guys will explain everything to you!\"},{\"text\":\"[p]OK, thanks!\"}]',-1,1,0,1,0),(4,4,5312,1073,NULL,'empty',-1,1,0,1,0),(5,11,10092,8561,NULL,'empty',-1,0,0,1,2),(6,12,5693,8369,NULL,'empty',-1,0,0,1,2),(7,13,6471,7569,NULL,'empty',-1,0,0,1,2),(8,14,6451,7345,NULL,'empty',0,1,2,1,2),(9,15,9464,6833,55,'[{\"text\":\"Hey, pst! You, yes, you!\",\"emote\":\"angry\",\"style\":\"aggresive\"},{\"text\":\"[p]Yeah? How can I help?\",\"emote\":\"blink\"},{\"text\":\"You cannot help me, kid! I just need...to TEST you, yes...!\",\"emote\":\"blink\"}]',-1,0,4,1,2),(10,16,264,1009,3,'[{\"text\":\"[p]Hi my name is [Player], [Talked] are you working?\",\"emote\":\"blink\"},{\"text\":\"Hello [Player], Yes, I am on duty as a guard, and I also trade a little\"},{\"text\":\"[p]Can you tell me how difficult it is to get into the service?\",\"emote\":\"blink\"},{\"text\":\"If that\'s what you want, it\'s easy. And if you don\'t then you won\'t be able to get in ;)\",\"emote\":\"happy\",\"style\":\"joyful\"},{\"text\":\"By the way [Player], I need help, there are slugs nearby, again raging, and I can\'t leave the post. At the same time you will test your skills young adventurer.\",\"emote\":\"blink\"},{\"text\":\"[p]Yes, no problem, wait [Talked]!!\",\"emote\":\"happy\",\"style\":\"joyful\"}]',-1,0,4,1,1),(11,2,1234,689,5,'[{\"text\":\"**He says with a look of horror on his face**: Help me find my daughter, she went to the village for food and never returned.\",\"emote\":\"pain\"},{\"text\":\"[p]To the village? [Talked] Yes of course I will help. That\'s where I need to go!\"},{\"text\":\"Thanks a lot\",\"emote\":\"pain\"}]',-1,1,1,1,0),(12,14,419,1009,NULL,'empty',0,1,2,1,1),(13,19,5739,7473,6,'[{\"text\":\"Hello, would you like to learn the basics of mining?\"},{\"text\":\"[p]Yes, of course, I don\'t mind\"},{\"text\":\"All right follow me I\'ll explain the basics to you!\"}]',-1,1,0,1,2),(14,20,3759,8209,10,'[{\"text\":\"Hello, and so you\'re new here?\"},{\"text\":\"[p]Yes.\",\"emote\":\"blink\"},{\"text\":\"I need to register you. Please introduce yourself, and the purpose of your arrival, and the time you to stay here\"},{\"text\":\"[p]My name is [Player]. I came here to become an adventurer. I can\'t tell you the exact time of stay.\"},{\"text\":\"And who for you should know the time of your stay here.\",\"style\":\"aggresive\",\"emote\":\"angry\"},{\"text\":\"Okay well, I will write you down, in case of violation of the rules of our village, we have every right to prohibit your presence here, and report to superiors.\",\"emote\":\"blink\"},{\"text\":\"Have a good day!\",\"style\":\"joyful\",\"emote\":\"happy\"},{\"text\":\"Thank\'s [Talked]!\",\"emote\":\"blink\",\"action_step\":1}]',-1,1,0,1,2),(15,21,6218,6417,NULL,'empty',-1,1,0,1,2),(16,27,4590,977,8,'[{\"text\":\"[Stranger]Greetings, wanderer. *bowing*\"},{\"text\":\"I am the Deputy of the Apostle, and I also protect her.\"},{\"text\":\"I think you\'ve already heard, that the monsters are raging.\"},{\"text\":\"I have a request for you [Player]\"},{\"text\":\"[p]Which one?\"},{\"text\":\"Help in the southern part to win over the monsters. We can\'t drive them away but we can scare them away.\",\"emote\":\"blink\"},{\"text\":\"[p]Of course I will.\"},{\"text\":\"Thank\'s [Player]!\",\"emote\":\"happy\",\"style\":\"joyful\"}]',-1,1,2,1,4),(17,14,1448,4433,NULL,'empty',0,1,2,1,5),(18,37,7781,7921,50,'[{\"text\":\"[p]You look awful today, [Talked]! What happend?\"},{\"text\":\"Oh.. don\'t you worry about me, boy..\",\"style\":\"aggresive\",\"emote\":\"angry\"},{\"text\":\"[p]But I DO worry!\",\"emote\":\"blink\"},{\"text\":\"I\'m tired of these fight I have with my wife, it\'s personal bussiness.\",\"emote\":\"blink\"},{\"text\":\"[p]I didn\'t want to be a pain to you, Officer. I will leave you to your problems now.\",\"emote\":\"blink\"},{\"text\":\"No! Wait!\",\"emote\":\"blink\"}]',-1,1,4,1,2),(19,39,2851,3473,60,'[{\"text\":\"Hello [Player], I have come to you from the Final Fantasy universe, my name is [Talked].\"},{\"text\":\"I can\'t say for sure how long I will be here, but for now I will be happy to know your world, and I will be happy to show my world\"},{\"text\":\"[p]Are you serious? Did the author smoke dope?\",\"emote\":\"blink\"},{\"text\":\"Maybe so, I have a couple of things that you can get from me, but not for free. I\'ll need the fragments I lost\"}]',-1,1,3,1,5),(20,51,9335,4881,NULL,'empty',-1,1,0,1,2),(21,53,7282,4880,NULL,'empty',-1,1,0,1,2),(22,13,7042,1410,NULL,'empty',-1,0,2,1,11),(23,14,7048,884,NULL,'empty',0,0,2,1,11),(24,12,7101,1169,NULL,'empty',-1,1,5,1,11),(25,11,5710,1937,NULL,'[{\"text\":\"Welcome home, sir! **she made a bow**\",\"style\":\"joyful\",\"emote\":\"happy\"}]',-1,0,4,1,11),(26,45,3800,1521,NULL,'empty',-1,1,2,1,11),(27,54,3568,8140,NULL,'[{\"text\":\"Hello, [Player], I am the Chinese translator of MRPG.\",\"style\":\"joyful\",\"emote\":\"happy\"}]',-1,1,3,1,2),(28,55,240,1005,NULL,'[{\"text\":\"Hello, [Player], I am the Chinese translator of MRPG.\",\"style\":\"joyful\",\"emote\":\"happy\"}]',-1,1,3,1,1),(29,59,881,2380,NULL,'[{\"text\":\"Hello, [Player], I am the Chinese translator of MRPG.\",\"style\":\"joyful\",\"emote\":\"happy\"}]',-1,1,3,1,3);
/*!40000 ALTER TABLE `tw_bots_npc` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_bots_quest`
--

DROP TABLE IF EXISTS `tw_bots_quest`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_bots_quest` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `BotID` int NOT NULL DEFAULT '-1',
  `QuestID` int NOT NULL DEFAULT '-1',
  `Step` int NOT NULL DEFAULT '1',
  `WorldID` int DEFAULT NULL,
  `GenerateSubName` tinyint NOT NULL DEFAULT '0',
  `PosX` int NOT NULL,
  `PosY` int NOT NULL,
  `DialogData` varchar(4096) NOT NULL DEFAULT 'empty',
  `RequiredItemID1` int DEFAULT NULL,
  `RequiredItemID2` int DEFAULT NULL,
  `RewardItemID1` int DEFAULT NULL,
  `RewardItemID2` int DEFAULT NULL,
  `RequiredDefeatMobID1` int DEFAULT NULL,
  `RequiredDefeatMobID2` int DEFAULT NULL,
  `Amount` varchar(64) NOT NULL DEFAULT '|0|0|0|0|0|0|',
  `InteractionType` int DEFAULT NULL,
  `InteractionTemp` int DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `MobID` (`BotID`),
  KEY `it_need_0` (`RequiredItemID1`),
  KEY `tw_bots_quest_ibfk_3` (`RequiredItemID2`),
  KEY `tw_bots_quest_ibfk_4` (`RewardItemID1`),
  KEY `it_reward_1` (`RewardItemID2`),
  KEY `QuestID` (`QuestID`),
  KEY `tw_bots_quest_ibfk_6` (`RequiredDefeatMobID1`),
  KEY `tw_bots_quest_ibfk_7` (`RequiredDefeatMobID2`),
  KEY `WorldID` (`WorldID`),
  KEY `interactive_type` (`InteractionType`),
  CONSTRAINT `tw_bots_quest_ibfk_1` FOREIGN KEY (`BotID`) REFERENCES `tw_bots_info` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_quest_ibfk_10` FOREIGN KEY (`InteractionType`) REFERENCES `enum_quest_interactive` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_quest_ibfk_2` FOREIGN KEY (`RequiredItemID1`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_quest_ibfk_3` FOREIGN KEY (`RequiredItemID2`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_quest_ibfk_4` FOREIGN KEY (`RewardItemID1`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_quest_ibfk_5` FOREIGN KEY (`RewardItemID2`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_quest_ibfk_6` FOREIGN KEY (`RequiredDefeatMobID1`) REFERENCES `tw_bots_info` (`ID`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_quest_ibfk_7` FOREIGN KEY (`RequiredDefeatMobID2`) REFERENCES `tw_bots_info` (`ID`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_quest_ibfk_8` FOREIGN KEY (`QuestID`) REFERENCES `tw_quests_list` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_bots_quest_ibfk_9` FOREIGN KEY (`WorldID`) REFERENCES `enum_worlds` (`WorldID`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=73 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_bots_quest`
--

LOCK TABLES `tw_bots_quest` WRITE;
/*!40000 ALTER TABLE `tw_bots_quest` DISABLE KEYS */;
INSERT INTO `tw_bots_quest` VALUES (1,5,1,1,0,0,3925,1169,'[{\"text\":\"[p]Hello, I was sent to you by a carpenter to help you.\"},{\"text\":\"**He asks with a smile**: Hello, have you just arrived?\"},{\"text\":\"[p]Yes, I came to you to become an adventurer, I need money.\"},{\"text\":\"Well, I\'ll tell you [Player], things have been very bad here lately, the residents are living in fear. Recently, a little girl was killed, as we understand there is a dead force involved.\",\"emote\":\"pain\"},{\"text\":\"[p]**With horror in his eyes**: What ... Dead People\",\"style\":\"aggresive\",\"emote\":\"surprise\"},{\"text\":\"**He said hurriedly**: I don\'t know for sure. Over time, the residents will tell you everything, or you will feel for yourself what is happening there.\"},{\"text\":\"[p]I didn\'t expect it to be this bad. Well, what can I help you with?\",\"emote\":\"blink\",\"action_step\":1},{\"text\":\"Come with me, our guys dropped boards, help me collect them.\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(2,5,1,2,0,0,5599,1009,'[{\"text\":\"Okay generally help me collect all the boards! I will reward you for your help.\",\"action_step\":1},{\"text\":\"[p]**Weakness in asks**: Oh, hell, they\'re heavy, I\'ve got them all.\",\"emote\":\"pain\"},{\"text\":\"Great job! Your muscles won\'t hurt you anyway [Player] *laughter*\",\"emote\":\"happy\",\"style\":\"joyful\"},{\"text\":\"[p]*laughter*\",\"emote\":\"happy\",\"style\":\"joyful\"},{\"text\":\"Now hand them out to my boys and let them go on with their business.\"},{\"text\":\"[p]Good [Talked]!\"}]',23,NULL,23,NULL,NULL,NULL,'|12|0|12|0|0|0|',2,NULL),(3,4,1,3,0,1,4121,1137,'[{\"text\":\"[p]Here you go, your boss told me to give it to you!\",\"action_step\":1},{\"text\":\"Thanks for the help.\"}]',23,NULL,NULL,NULL,NULL,NULL,'|4|0|0|0|0|0|',2,NULL),(4,4,1,3,0,1,6489,1137,'[{\"text\":\"[p]Here you go, your boss told me to give it to you!\",\"action_step\":1},{\"text\":\"Thanks for the help.\"}]',23,NULL,NULL,NULL,NULL,NULL,'|4|0|0|0|0|0|',2,NULL),(5,4,1,3,0,1,2430,977,'[{\"text\":\"[p]Here you go, your boss told me to give it to you!\",\"action_step\":1},{\"text\":\"Thanks for the help.\"}]',23,NULL,NULL,NULL,NULL,NULL,'|4|0|0|0|0|0|',2,NULL),(6,5,1,4,0,0,6742,1041,'[{\"text\":\"[p]Is tired. But I did it!\",\"emote\":\"pain\"},{\"text\":\"Well done, take this. I think you need it if you\'re going to be an adventurer. Go straight, and you will go straight to the residents, if you ask where to go.\",\"emote\":\"happy\",\"style\":\"joyful\",\"action_step\":1},{\"text\":\"[p]**With a smile**: Well thank you, good luck with your work!\",\"emote\":\"happy\",\"style\":\"joyful\"}]',NULL,NULL,3,NULL,NULL,NULL,'|0|0|1|0|0|0|',NULL,NULL),(7,6,2,1,1,0,841,977,'[{\"text\":\"[p][Stranger]Hi, my name is [Player], how do I find the nearest village?\",\"emote\":\"pain\"},{\"text\":\"**She concerned**: Hello, my name is [Talked].\",\"emote\":\"happy\"},{\"text\":\"I\'ll be happy to help you settle in, but could you give my brother a note? He works now on the ship that you arrived on, i could have done it myself, but they won\'t let me in.\",\"emote\":\"pain\"},{\"text\":\"[p]Yes, of course i help, will you be here [Talked]?\",\"emote\":\"happy\",\"style\":\"joyful\",\"action_step\":1},{\"text\":\"I\'ll be here waiting!\"}]',NULL,NULL,21,NULL,NULL,NULL,'|0|0|1|0|0|0|',NULL,NULL),(8,8,2,2,0,0,411,1009,'[{\"text\":\"[p]Hello, are you Diana\'s brother?\",\"emote\":\"pain\"},{\"text\":\"**He said rudely**: Yes. Did something happen to her?..\",\"style\":\"aggresive\",\"emote\":\"surprise\"},{\"text\":\"[p]No, don\'t worry, she just asked me to pass you the notebook.\",\"emote\":\"pain\",\"action_step\":1},{\"text\":\"Well, thank you very much, and tell her I\'ll be seeing her this evening.\",\"emote\":\"happy\",\"style\":\"joyful\"},{\"text\":\"[p]Yes, good luck with your work!\",\"emote\":\"happy\",\"style\":\"joyful\"}]',21,NULL,NULL,NULL,NULL,NULL,'|1|0|0|0|0|0|',NULL,NULL),(9,6,2,3,1,0,841,977,'[{\"text\":\"**She says impatiently**: Oh, you\'re already here, you\'re fast!\"},{\"text\":\"[p]Really?\",\"emote\":\"happy\",\"style\":\"joyful\"},{\"text\":\"Yes. How is he?\"},{\"text\":\"[p]He told me that he is busy now, and will visit you later in the evening!\"},{\"text\":\"I\'m so happy! They won\'t let me in because of the problems that are happening. Dead men kidnap girls, and then they find them dead.\",\"emote\":\"pain\"},{\"text\":\"[p]**He sounds surprised**: Only girls? Why the dead?\",\"emote\":\"blink\"},{\"text\":\"**She\'s upset**: Honestly, I don\'t know. The dead or who it is, our headman says that most likely the dead ;(\",\"emote\":\"pain\"},{\"text\":\"[p]I came to you to become an adventurer, as I know you have magical creatures.\"},{\"text\":\"**She sounds like she knows it**: An adventurer? We have a Guild of adventurers, but to become an adventurer you need to pass the tests.\",\"emote\":\"surprise\"},{\"text\":\"[p]Tests?....\",\"emote\":\"blink\"},{\"text\":\"Yes the test [Player]. First, you will need to collect the minimum equipment. To apply and pay the fee. And only after that you will be assigned the position of adventurer.\"},{\"text\":\"[p]Is everything so strict? Yes, I think I can handle it.\",\"emote\":\"happy\",\"style\":\"joyful\"},{\"text\":\"All right come on I\'ll show you our village [Player].\",\"emote\":\"happy\",\"style\":\"joyful\",\"action_step\":1},{\"text\":\"[p]Yes come on ;)\",\"emote\":\"happy\",\"style\":\"joyful\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(10,6,2,4,1,0,2207,465,'[{\"text\":\" **She made it sound like there was something wrong with them**: Oh, my God, where are the slime from?.\",\"emote\":\"pain\"},{\"text\":\"[p]I don\'t know [Talked], here even have slimes. Wahoo\",\"emote\":\"blink\"},{\"text\":\"Well, Yes [Player], but now they are becoming more frequent\",\"emote\":\"pain\"},{\"text\":\"[p]**You speak proudly**: All right [Talked], I\'ll clear the way!\",\"emote\":\"happy\",\"style\":\"joyful\",\"action_step\":1},{\"text\":\"[p]**You said in disgust**: Phew, the slime is so disgusting...\",\"emote\":\"angry\",\"style\":\"aggresive\"},{\"text\":\"**She says with a smile**: Well you could use a bath [Player] ;)\",\"emote\":\"happy\",\"style\":\"joyful\"},{\"text\":\"[p]I will not refuse such an honor ;)\",\"emote\":\"happy\",\"style\":\"joyful\"},{\"text\":\"Ah here is and decided, we\'ll get there I\'ll show you where I live. And I\'ll introduce you to the places. And you wash :)\"},{\"text\":\"[p]All right, shall we continue on our way?\",\"emote\":\"surprise\"},{\"text\":\"Yes let\'s go\"}]',NULL,NULL,NULL,NULL,36,NULL,'|0|0|0|0|12|0|',NULL,NULL),(11,16,3,1,1,0,525,1009,'[{\"text\":\"**He said with surprise**: Did you defeat the slugs on the instructions of our master? Take this the master asked me to give you.\",\"action_step\":1},{\"text\":\"[p]Yes, I could. They were very slippery.\",\"emote\":\"happy\",\"style\":\"joyful\"},{\"text\":\"Hehe.. What did you think?\",\"emote\":\"happy\",\"style\":\"joyful\"},{\"text\":\"[p]Well, not so difficult, at the same time I warmed up.\"},{\"text\":\"Okay I need to serve, good luck to you, don\'t give up ;)\",\"emote\":\"happy\",\"style\":\"joyful\"}]',NULL,NULL,15,14,36,NULL,'|0|0|3|3|16|0|',NULL,NULL),(12,18,5,1,2,0,5215,7409,'[{\"text\":\"**She said very rudely**: Who the hell are you? What do you want?\",\"emote\":\"angry\",\"style\":\"aggresive\"},{\"text\":\"[p]Do you know that your father is looking for you?\"},{\"text\":\"I hate this old guy, he doesn\'t have time for me......\",\"emote\":\"angry\",\"style\":\"aggresive\"},{\"text\":\"He can only go there. Do it. And we can\'t spend time together...\",\"emote\":\"angry\",\"style\":\"aggresive\"},{\"text\":\"Enrages...\",\"emote\":\"angry\",\"style\":\"aggresive\"},{\"text\":\"[p]Calm down let\'s go back together and tell him everything? Maybe he just cares about you like that.\",\"emote\":\"happy\",\"style\":\"joyful\",\"action_step\":1},{\"text\":\"All right, let\'s go back.\",\"emote\":\"pain\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(13,18,5,2,0,0,2390,977,'[{\"text\":\"[p]Are you ready?\",\"action_step\":1},{\"text\":\"Yes..\",\"emote\":\"pain\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(14,2,5,2,0,0,2162,977,'[{\"emote\":\"angry\",\"text\":\"So where was she?.\"},{\"text\":\"[p]You just don\'t get angry, everything\'s fine!\"},{\"text\":\"[p]In fact, she didn\'t want to come back, she was offended that you weren\'t spending time together. You can\'t rest.\"},{\"action_step\":1,\"emote\":\"surprise\",\"text\":\"Damn why didn\'t she ask for it herself? Okay, don\'t worry thanks for the work :) Everything will be fine, we will decide something with daughter\"},{\"emote\":\"happy\",\"text\":\"[p]Well, nice :) Good luck to you\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(15,19,6,1,2,0,7915,8401,'[{\"text\":\"To extract any ore [Player], you will need a pickaxe.\"},{\"text\":\"You can only get it by creating. But in some cases merchants can sell.\"},{\"text\":\"Also, the picks have a durability of 100-0. If you durability reach 0. Don\'t worry it won\'t break.\"},{\"text\":\"But you can\'t work with break pickaxe it. You can repair it at any store.\"},{\"text\":\"You can also improve picks this will depend on their effectiveness.\"},{\"text\":\"Each ore has its own strength. If you have a weak pick and the strength of the ore is high, it can take a long time to extract it. So don\'t forget about the improvements. Or craft even better picks.\"},{\"text\":\"Everything is clearly explained, I will not repeat it again, if you need you will understand everything!\"},{\"action_step\":1,\"text\":\"[p]Yes of course thank you for the explanation\"},{\"text\":\"Well don\'t worry, I\'ll give you a couple of tasks to get used to in this area\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(16,19,7,1,2,0,9859,8561,'[{\"text\":\"[p]Did you want something?\"},{\"text\":\"I think it\'s time for you to get your first pickaxe.\"},{\"text\":\"I think as crafted something, you don\'t need to explain!\"},{\"text\":\"So let\'s get right to the point, try to make a pickaxe bring it to me, I\'ll look at it.\"},{\"action_step\":1,\"text\":\"[p]Done. (you need to have an idea of how to craft items, you can see them near Craftsman in votes)\"},{\"text\":\"Well, it\'s not bad, so use it for the first time.\"},{\"text\":\"All right so let\'s start with your first ore?\"}]',26,NULL,26,NULL,NULL,NULL,'|1|0|1|0|0|0|',NULL,NULL),(17,6,10,1,2,0,6615,8433,'[{\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"Well, as [Player], passed customs?\"},{\"emote\":\"blink\",\"text\":\"[p]Yes [Talked]. He was very rude.\"},{\"text\":\"Well, you have to understand that we also have a lot of problems lately, too many monsters, problems with kidnappings.\"},{\"action_step\":1,\"text\":\"[p]Come on, show me [Here] seats!\"},{\"text\":\"Oh sure. By the way, in our village, the chief Apostle.\"},{\"emote\":\"surprise\",\"text\":\"[p]Wait an Apostle? They are ancient beings sent by the gods. Do they exist?\"},{\"text\":\"Yes [Player]. They exist to keep order.\"},{\"emote\":\"blink\",\"text\":\"Okay [Player] let\'s go and show you our [Here]\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(18,6,10,2,2,0,9953,8561,'[{\"emote\":\"happy\",\"text\":\"**She said with a smile**: Here, you can do a craft items, make your own equipment potions, or what to eat.\"},{\"text\":\"[Player] even if you provide items, the craftsman you must pay for the use of the equipment\"},{\"text\":\"[p]Yes I think I can use it. Thanks for the explanation!\"},{\"action_step\":1,\"text\":\"Let\'s move on!\"},{\"emote\":\"happy\",\"text\":\"[p]Yes let\'s go :)\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(19,6,10,3,2,0,8574,7665,'[{\"text\":\"So here we have someone\'s house.\"},{\"text\":\"You can also buy houses, decorate them or use the safety Deposit box.\"},{\"action_step\":1,\"text\":\"[p]Sounds good [Talked]!\"},{\"emote\":\"happy\",\"text\":\"**She likes to talk**: Well, okay, let\'s move on!\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(20,6,10,4,2,0,8123,7089,'[{\"text\":\"This is the ether, it allows you to use instantaneous movement around the world.\"},{\"text\":\"But only there in the area where you have already visited.\"},{\"text\":\"[p]Is it free [Talked]?\"},{\"text\":\"Not always, there is nothing free in our world!\"},{\"action_step\":1,\"emote\":\"blink\",\"text\":\"**Apparently she didn\'t like something**: Okay [Player] let\'s move on!\"},{\"text\":\"[p]Yes let\'s go [Talked]!\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(21,6,10,5,2,0,6815,7569,'[{\"text\":\"Here you will be able to raise his skills adventurer, to explore something new. Our world is divided into 3 classes (Healer, Tank, DPS)\"},{\"emote\":\"surprise\",\"text\":\"[p]So I can become something?\"},{\"emote\":\"blink\",\"text\":\"[Player], you can be anyone you want (as well as fine-tune your game mode)\"},{\"emote\":\"blink\",\"text\":\"[p][Talked] so I can be both a Tank and a Defender?\"},{\"text\":\"Yes quite but do not forget about the main characteristics (Strength, Hardness, idk)\"},{\"action_step\":1,\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"[p]Thank you for a clear explanation :)\"},{\"emote\":\"blink\",\"text\":\"That\'s a small part of what I\'ve been able to tell you.\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(22,6,10,6,2,0,6364,7345,'[{\"text\":\"And here, you will be treated. \"},{\"style\":\"1\",\"emote\":\"blink\",\"text\":\"**She said very jealously**: True, I hate those nurses, all the guys from the village are taken away....\"},{\"action_step\":1,\"emote\":\"blink\",\"text\":\"Don\'t marry them...\"},{\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"[p]Yes I will not marry anyone [Talked]! Thanks for the care!\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(23,6,10,7,2,0,5021,7441,'[{\"emote\":\"blink\",\"text\":\"**Yawning she said**: Okay I\'m tired I\'ll go home.\"},{\"action_step\":1,\"text\":\"And you don\'t disappear, and [Player] go wash up :)\"},{\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"[p]Well have a good day [Talked] :)\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(24,6,11,1,2,0,6834,7569,'[{\"text\":\"Are you ready?\"},{\"emote\":\"surprise\",\"text\":\"[p]What, ready?\"},{\"text\":\"Introduce you [Player] to the Apostle of our village. \"},{\"emote\":\"surprise\",\"text\":\"[p][Talked] so what\'s it to them for me?\"},{\"style\":\"aggresive\",\"emote\":\"angry\",\"text\":\"**She said rudely**: No, don\'t think so bad, they actually think of us as their children.\"},{\"text\":\"[p]Why so rude? I understand you, okay, I\'m worried, but we\'ll see what happens.\"},{\"emote\":\"pain\",\"text\":\"I\'m sorry, but she\'s really close to me.\"},{\"emote\":\"blink\",\"text\":\"[p]Are you one of them? Hm...\"},{\"action_step\":1,\"style\":\"aggresive\",\"emote\":\"angry\",\"text\":\"Fool let\'s go already.\"},{\"emote\":\"pain\",\"text\":\"[p]Well ... Let\'s go\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(25,6,11,2,2,0,5722,6353,'[{\"action_step\":1,\"emote\":\"blink\",\"text\":\"Well, just don\'t mess with as a joke, it\'s Apostle, he is watching over us for many years.\"},{\"text\":\"[p]I understand [Talked].\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(26,10,11,3,2,0,5325,6289,'[{\"text\":\"[p]Hello dear Apostle *bowing*. I have come to you to wisdom.\"},{\"text\":\"Hello, lift your head. I knew you\'d come. But I\'m not sure you\'re the one I saw.\"},{\"text\":\"[p]See it?\"},{\"text\":\"I can\'t communicate with my brothers and sisters, since our paths have diverged, but we have a common mind.\"},{\"text\":\"I had a vision about a year ago that there would soon be a man who was as tenacious as the Apostles.\"},{\"text\":\"But in the end, he was going to die. My visions have always come true, but when they will come true is unknown.\"},{\"text\":\"So please be careful [Player]. I\'m worried about everyone, but I\'ve also been weak lately.\"},{\"text\":\"[p]I .. Not to worry, I can handle it even if I turn out to be one of your visions\"},{\"action_step\":1,\"text\":\"I hope this is not the case, get used to it, then go to Adventurer Koto.\"},{\"text\":\"[p]Thanks a lot.\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(27,25,12,1,2,0,5421,8273,'[{\"text\":\"[p]Hello [Talked], the Apostle sent me to you. I want to be an adventurer!\"},{\"style\":\"aggresive\",\"emote\":\"blink\",\"text\":\"**He as if he doesn\'t want me to become an adventurer**: Will you be strong enough? Well, let\'s see, I think you already understand that you first need to test, only after I say whether you are worthy or not to become an Adventurer?\"},{\"text\":\"[p]Well, what exactly is required?\"},{\"style\":\"aggresive\",\"emote\":\"blink\",\"text\":\"If you pass my first test, pay the fee, I encourage you, and send you to the exam. And then we will decide whether you are worthy or not\"},{\"text\":\"[p]Okay, where do we start [Talked]?\"},{\"action_step\":1,\"text\":\"Ha ha where do we start? Don\'t joke with me young fighter. First, take this and go to the southern part of the village.\"},{\"text\":\"[p]Well\"}]',NULL,NULL,27,NULL,NULL,NULL,'|0|0|1|0|0|0|',NULL,NULL),(28,25,12,2,2,0,10822,6737,'[{\"style\":\"aggresive\",\"emote\":\"angry\",\"text\":\"You did.\"},{\"emote\":\"blink\",\"text\":\"[p]What could I do?\"},{\"style\":\"aggresive\",\"emote\":\"angry\",\"text\":\"I thought you were joking, you wouldn\'t take it seriously, but you came.\"},{\"action_step\":1,\"text\":\"[p]Well, let\'s go.\"},{\"text\":\"Let\'s go.\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(29,25,13,1,3,0,676,1169,'[{\"style\":\"aggresive\",\"emote\":\"angry\",\"text\":\"Why are you shivering [Player]?\"},{\"action_step\":1,\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"Am I that scary? Don\'t worry I\'m kind *laughs*. Well, let\'s see how you cope. Kill for me ....\"},{\"emote\":\"blink\",\"text\":\"[p]I managed it, and it wasn\'t difficult.\"},{\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"I noticed how well you did. But don\'t relax this is just the beginning ^^\"},{\"emote\":\"blink\",\"text\":\"[p]Ah.. Okay [Talked].\"},{\"text\":\"Well today we\'re finished you can relax. You\'ll find me as soon as you\'re ready.\"},{\"emote\":\"happy\",\"text\":\"[p]Okay, well, have a good day.\"}]',NULL,NULL,NULL,NULL,9,NULL,'|0|0|0|0|32|0|',NULL,NULL),(30,26,13,2,3,0,500,1361,'[{\"text\":\"Hello, I am the personal Ambassador of the Apostle.\"},{\"text\":\"She wants to see you!\"},{\"action_step\":1,\"text\":\"Please follow me to village, I will be waiting for you there\"},{\"text\":\"[p]Okay, I\'m coming\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(31,16,4,1,1,0,1530,1073,'[{\"text\":\"Listen there\'s another request!\"},{\"action_step\":1,\"text\":\"[Player] if you find some gel, I\'ll be happy to exchange them. Brown Slimes,Blue Slimes and Pink Slime drop Gel. You can take your time, i\'ll wait!\"},{\"text\":\"[p]Here, it wasn\'t easy but I got it [Talked].\"},{\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"Thank you very much for your help. All right I\'ll go back to my post, good luck to you [Player]!\"},{\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"[p]Thank\'s [Talked]\"}]',29,NULL,25,NULL,NULL,NULL,'|18|0|8|0|0|0|',NULL,NULL),(32,26,13,3,2,0,3780,6449,'[{\"action_step\":1,\"text\":\"[Player] you can go in!\"},{\"text\":\"[p]Well [Talked], thank you for seeing me off!\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(33,27,8,1,4,0,4235,945,'[{\"action_step\":1,\"text\":\"[Player] first we need to win over the slime. Dirty but necessary.\"},{\"text\":\"[p]I report everything, I coped with the task\"},{\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"Well done, but that\'s not all.\"},{\"text\":\"[p]Anything else?\"},{\"text\":\"Yes I think you know about the ethers find me next to him\"}]',NULL,NULL,NULL,NULL,23,NULL,'|0|0|0|0|50|0|',NULL,NULL),(34,27,8,2,2,0,7975,7089,'[{\"text\":\"I think you know that these crystals are actually not just for beauty. \"},{\"text\":\"You use them as a quick transition. But in fact, these are crystals (all the light of good prosperity) precisely because they exist.\"},{\"action_step\":1,\"text\":\"Me when once before as became defender of the Apostle, I was able to meet with the crystal. Yes, he has a mind. From it warmth on the soul, all the sins immediately fall from the soul. I\'m sure you\'ll meet him one day.\"},{\"text\":\"[p]Thanks. I will remember that. (Auther text: This is how the player\'s rapprochement with the deities began)\"},{\"text\":\"Well, I\'ll wait for you in the Apostle\'s chambers. We will continue the expulsion of the monsters\"}]',NULL,NULL,32,NULL,NULL,NULL,'|0|0|1|0|0|0|',NULL,NULL),(35,27,9,1,4,0,4243,945,'[{\"text\":\"Our priority is higher now.\"},{\"action_step\":1,\"text\":\"[Player] you need to defeat the main slug, in the southern part.\"},{\"text\":\"[p]That\'s it, I\'ve met my goal. Clearing dirt.\"},{\"text\":\"Well done, we\'ll meet again.\"}]',NULL,NULL,NULL,NULL,28,NULL,'|0|0|0|0|1|0|',NULL,NULL),(36,10,14,1,4,0,4391,977,'[{\"text\":\"I want to talk to you [Player].\"},{\"emote\":\"blink\",\"text\":\"I know that your path will not stop here. One day you will become stronger and move on.\"},{\"text\":\"I have a request to you, if you meet other apostles, it is better to avoid them. Don\'t trust them, we apostles are not as pure as you think\"},{\"emote\":\"blink\",\"text\":\"[p]Maybe you, too, then the enemy really is?\"},{\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"I don\'t know *laughter*. The villagers I think will clearly tell you how it really is\"},{\"text\":\"Diana told me about you in General terms.\"},{\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"**She said it with concern**: You turn out to be a Joker, and a vulgar *laughter*\"},{\"emote\":\"blink\",\"text\":\"[p]No [Talked]......\"},{\"text\":\"Always listen to your heart. I think one day you will be able to bring light. But the main thing is perseverance!\"},{\"action_step\":1,\"text\":\"Do not delay with your title of adventurer. You can go now!\"},{\"text\":\"[p]I\'m trying, thank you very much.\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(37,6,15,1,2,0,8081,7889,'[{\"text\":\"[p]You look sad today [Talked].\"},{\"action_step\":1,\"emote\":\"pain\",\"text\":\"Oh man I\'m too confused today.... [Player] help me collect lost.\"},{\"emote\":\"happy\",\"text\":\"[p]Take your nose up [Talked], what\'s wrong with you?\"},{\"emote\":\"pain\",\"text\":\"My brother has to stop by today, and I want to cook something to feed him, but everything falls out of my hands..\"},{\"emote\":\"happy\",\"text\":\"[p]So let me help you?\"},{\"text\":\"And you I look [Player] today cheerful, well help. I will not remain in debt\"},{\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"[p]Of course [Talked] ;)\"}]',20,NULL,NULL,NULL,NULL,NULL,'|10|0|0|0|0|0|',2,NULL),(38,6,15,2,2,0,8193,6065,'[{\"text\":\"Well here\'s my house! Ah as my not my, father is a master, and even brother. In General, our family\"},{\"action_step\":1,\"text\":\"All right, come on in, I\'ll cook, and I\'ll ask you to bring something. All the help me need\"},{\"text\":\"[p]I will help you, especially since you are my first close friend in this village, I already trust you, how you can be abandoned.\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(39,6,16,1,7,0,4454,881,'[{\"text\":\"Well [Player], let\'s get started, little by little\"},{\"emote\":\"happy\",\"text\":\"[p]Your house comfortable! [Talked]\"},{\"action_step\":1,\"text\":\"[p]Do you really think so? Well thank you.\"},{\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"All right let\'s get down to cooking\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(40,6,16,2,7,0,4671,881,'[{\"text\":\"Well, let\'s get started [Player]!\"},{\"text\":\"[p]Oh sure [Talked]\"},{\"action_step\":1,\"text\":\"[Player] get me some first, we\'ll use it as oil\"},{\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"[p]I think everyone has their own way of making butter *laughter*\"},{\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"Yeah you\'re right, good sense of humor damn *laughter*\"}]',29,NULL,NULL,NULL,NULL,NULL,'|18|0|0|0|0|0|',NULL,NULL),(41,6,16,3,7,0,4590,881,'[{\"text\":\"So now I need meat. How to cook without meat.\"},{\"action_step\":1,\"text\":\"I think you\'ve heard of Kappa, they live near water, their meat is very tasty. So get it for me (you can self find them habitat)\"},{\"emote\":\"happy\",\"text\":\"[p]All delicious fine meat, looks great, take it!\"},{\"emote\":\"happy\",\"text\":\"Well i let\'s start cooking :)\"},{\"text\":\"[p]**You see like she didn\'t know how to cook**: Are you sure you can cook?\"},{\"style\":\"aggresive\",\"emote\":\"angry\",\"text\":\"**She said angrily**: You know what I\'ll tell you [Player]... You don\'t know how to compliment girls at all..\"},{\"emote\":\"blink\",\"text\":\"[p]**Someone knocks on the door**: Who could it be? lovers?\"},{\"emote\":\"surprise\",\"text\":\"Oh it\'s probably my brother.\"}]',35,NULL,NULL,NULL,NULL,NULL,'|14|0|0|0|0|0|',NULL,NULL),(42,8,16,4,7,0,4142,881,'[{\"emote\":\"blink\",\"text\":\"**He said angrily and rudely**: Oh, what people, I\'ve seen you somewhere.\"},{\"action_step\":1,\"emote\":\"blink\",\"text\":\"[p]You must have imagined it. \"},{\"text\":\"Oh well.\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(43,6,16,4,7,0,4590,881,'[{\"emote\":\"happy\",\"text\":\"[Player] you look sad.\"},{\"text\":\"[p]Okay [Talked]. I\'ll probably go extra here\"},{\"emote\":\"blink\",\"text\":\"Yes calm down you why so decided to?\"},{\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"[p]Yes, everything is fine. I just have things to do, too. I\'ll go!\"},{\"emote\":\"pain\",\"text\":\"You offend me [Talked].....\"},{\"action_step\":1,\"text\":\"[p]Sorry I\'m really busy [Talked].\"},{\"emote\":\"blink\",\"text\":\"Well, take care of yourself.\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(44,25,17,1,2,0,8128,7889,'[{\"text\":\"So let\'s continue our training.\"},{\"text\":\"[p]Yes, of course, it\'s about time!\"},{\"style\":\"aggresive\",\"emote\":\"angry\",\"text\":\"**He speaks rudely**: I don\'t really like you. But let\'s see what you can achieve. Let\'s go back to the Deep Cave.\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(45,25,17,2,3,0,762,1169,'[{\"text\":\"**He speaks proudly**: Well, now we have more serious goals.\"},{\"action_step\":1,\"text\":\"Exterminate the stronger slime. I\'ll see.\"},{\"emote\":\"blink\",\"text\":\"[p]Everything is ready, they are vile I do not like slime, maybe something new?\"},{\"text\":\"Well, I\'ll think about your request. Follow me.\"}]',NULL,NULL,NULL,NULL,23,NULL,'|0|0|0|0|32|0|',NULL,NULL),(46,25,17,3,2,0,6316,7569,'[{\"action_step\":1,\"text\":\"Take it, use it wisely. You will need it very much in the future.\"},{\"emote\":\"surprise\",\"text\":\"[p]**You were surprised by the gift provided**: Oh, thank you very much. Do I owe you something?\"},{\"emote\":\"blink\",\"text\":\"I hope you make it to the end, at least I hope you do. I didn\'t think you\'d be Apostle\'s favorite.\"},{\"emote\":\"surprise\",\"text\":\"[p]Favorite [Talked]?\"},{\"style\":\"aggresive\",\"emote\":\"angry\",\"text\":\"**He said very rudely**: I\'ll go, meet you later.\"}]',NULL,NULL,9,NULL,NULL,NULL,'|0|0|50|0|0|0|',NULL,NULL),(47,27,18,1,2,0,5244,6289,'[{\"text\":\"Hello [Player]. Are you free?\"},{\"text\":\"[p]Yes, of course. Everything okay?\"},{\"text\":\"Apostle asked you to see her!\"},{\"action_step\":1,\"text\":\"[p]Well!\"},{\"text\":\"Thank you, I\'m going for a walk!\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|1|0|',NULL,NULL),(48,10,18,2,4,0,4453,977,'[{\"emote\":\"pain\",\"text\":\"**She speaks in fear**: Hello [Player]. Today I received news that in the southern part of our village, goblins attacked. \"},{\"emote\":\"pain\",\"text\":\"And it\'s not just goblins. This must be the army. The leader brought them.\"},{\"emote\":\"surprise\",\"text\":\"[p]Maybe they kidnapped the residents and terrorized you, and now they decided to attack?\"},{\"action_step\":1,\"text\":\"I think so too. Anyway, I talked to Koko. About your appointment as an adventurer. We should prepare for resistance. Communication nodes they need to be destroyed\"},{\"text\":\"[p]Well, I did the job [Talked]!\"},{\"text\":\"Good job thank\'s, you should meet with Koko, he will explain everything to you, good luck!\"},{\"text\":\"[p]Well, thank you!\"}]',NULL,NULL,16,NULL,34,NULL,'|0|0|3|0|32|0|',NULL,NULL),(49,25,19,1,4,0,4727,977,'[{\"text\":\"Get ready let\'s go to the Goblin-occupied zone.\"},{\"text\":\"[p]I\'m ready to sort of out right now.\"},{\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"Well [Player], you\'re fun, Yes, but it\'s worth getting ready.\"},{\"action_step\":1,\"emote\":\"blink\",\"text\":\"[p]Well, what do need [Talked]?\"},{\"text\":\"I\'ll be waiting for you next to Craftsman.\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(50,25,19,2,2,0,9079,8305,'[{\"text\":\"So first we need potions.\"},{\"action_step\":1,\"text\":\"Go get supplies, and at the same time exterminate a few slugs so that we can get there without any problems.\"},{\"emote\":\"pain\",\"text\":\"[p]**You sound very tired**: We can all move, but the problem is that I\'m tired..\"},{\"style\":\"aggresive\",\"emote\":\"blink\",\"text\":\"Nothing to worry about [Player]. You need to toughen up. All right let\'s move out.\"}]',29,22,15,14,23,9,'|16|16|8|8|40|40|',NULL,NULL),(51,25,19,3,3,0,4509,1265,'[{\"action_step\":1,\"text\":\"We\'ll get to the guard post now. We\'ll move out there to exterminate the goblins.\"},{\"text\":\"[p]Well [Talked].\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(52,37,50,1,2,0,7781,7921,'[{\"emote\":\"blink\",\"text\":\"[p]**Grinning**: Huh?\"},{\"emote\":\"pain\",\"text\":\"We can\'t heal her..\"},{\"emote\":\"blink\",\"text\":\"[p]I don\'t understand..\"},{\"emote\":\"pain\",\"text\":\"Our daughter, Maria.. She\'s sick!\"},{\"text\":\"Why don\'t you take her to the Nurse?!\"},{\"action_step\":1,\"emote\":\"pain\",\"text\":\"I tried, but she said she can\'t do anything.. I found something in an old book of mine.. A gel treatment. But I also need Kappa Meat for it..\"},{\"emote\":\"happy\",\"text\":\"Tha..nk.. You..\"},{\"emote\":\"blink\",\"text\":\"Well.. don\'t look at me, heal Maria!\"}]',29,35,NULL,NULL,NULL,NULL,'|20|20|0|0|0|0|',NULL,NULL),(53,38,50,2,2,0,7671,7921,'[{\"emote\":\"pain\",\"text\":\"F..Father?\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(54,37,50,3,2,0,7781,7921,'[{\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"MARIA! You\'re well!\"},{\"emote\":\"happy\",\"text\":\"[p]*with joy* Hi, Maria!\"},{\"text\":\"How can I ever make it up to you?\"},{\"action_step\":1,\"text\":\"[p]Well Sir.. I might need a discount from your old friend, the Craftsman.\"},{\"emote\":\"happy\",\"text\":\"Sure, I\'ll.. let him know you deserve it!\"}]',NULL,NULL,43,NULL,NULL,NULL,'|0|0|5|0|0|0|',NULL,NULL),(55,15,55,1,2,0,9463,6833,'[{\"emote\":\"blink\",\"text\":\"[p]Sure..What can I do?\"},{\"emote\":\"blink\",\"text\":\"I heard you helped the craftsman with his deliveries..I have no time for small things like mining cooper, can you do it for me? I have to save the world in the main time..\"},{\"action_step\":1,\"emote\":\"blink\",\"text\":\"[p] *a bit angry* Ok, I will do it.\"},{\"emote\":\"happy\",\"text\":\"Oh, it\'s you kid!\"},{\"style\":\"aggresive\",\"emote\":\"angry\",\"text\":\"[p]*you can\'t control yourself* HEY. I\'m NOT A KID!\"},{\"text\":\"Hey.. easy, just a joke, did you get what I wanted?\"},{\"emote\":\"blink\",\"text\":\"[p]Yes.. *puts a heavy cooper bag on Erik\'s table*\"},{\"style\":\"aggresive\",\"emote\":\"angry\",\"text\":\"Good, now get out of here!\"},{\"style\":\"aggresive\",\"emote\":\"angry\",\"text\":\"[p]Wait.. YOU NOT GONNA PAY ME??\"},{\"style\":\"aggresive\",\"emote\":\"angry\",\"text\":\"Haha! I am messing with you.\"}]',31,NULL,NULL,NULL,NULL,NULL,'|50|0|0|0|0|0|',NULL,NULL),(56,39,60,1,5,0,2852,3473,'[{\"text\":\"[p]So can you tell me how you got here?\"},{\"emote\":\"blink\",\"text\":\"**He\'s stuttered**: A long story..\"},{\"emote\":\"blink\",\"text\":\"We tried a new potion mixed with ether, but something went wrong, my hiccups are part of this experiment, and I ended up here after I drank it\"},{\"text\":\"**Sound in the tablet**: Noctis... Noctiis. Do you hear me?\"},{\"text\":\"**Noctis**: I can hear you, how do I get out of here?\"},{\"text\":\"**Sound in the tablet**: Collect all the fragments that ended up here. The connection is lost....\"},{\"text\":\"[p]What were you talking to just now?\"},{\"action_step\":1,\"text\":\"A tablet made from a magic crystal. In General, as I thought I need your help in collecting fragments\"},{\"text\":\"[p]Of course I\'ll help\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(57,39,60,2,5,0,3216,3537,'[{\"text\":\"Look at what I can give you in return for your help. (Vote menu shop list)\"},{\"text\":\"[p]Wow, [Talked] where can I find them?\"},{\"text\":\"In a distorted dimension.\"},{\"action_step\":1,\"emote\":\"blink\",\"text\":\"[p]Where is it? How do I get there?\"},{\"text\":\"[Player] way, so we\'ll be preparing to get there.\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(58,39,61,1,5,0,2944,3505,'[{\"action_step\":1,\"text\":\"So first [Player], I\'ll need these things from you. They resonate between our worlds\"},{\"text\":\"[p]Take it [Talked].\"},{\"text\":\"Well thank you, it will be a little easier now.\"},{\"text\":\"**Sound in the tablet**: Noctis. How you to get the fragments we will be able to pick you up.\"},{\"text\":\"**Noctis**: Okay, Sid..\"}]',31,35,NULL,NULL,NULL,NULL,'|40|40|0|0|0|0|',NULL,NULL),(59,39,62,1,5,0,2944,3505,'[{\"action_step\":1,\"text\":\"Next thing I need to create a resonance. Bring me these items.\"},{\"text\":\"[p]Everything is ready?. [Talked] take it\"},{\"text\":\"[Player] yes.. almost, follow me and we\'ll start resonating.\"}]',41,37,NULL,NULL,NULL,NULL,'|1|8|0|0|0|0|',NULL,NULL),(60,39,62,2,5,0,4566,4273,'[{\"text\":\"All ready.\"},{\"text\":\"Resonance received. I will wait for you with fragments\"},{\"action_step\":1,\"emote\":\"blink\",\"text\":\"[p]Can come to resonance?\"},{\"text\":\"Yes. Don\'t worry nothing will happen to you.\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(61,46,20,1,5,0,2528,4305,'[{\"action_step\":1,\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"I welcome you [Talked], nice to meet you.\"},{\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"Glad to meet you, i\'am [Player]!\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(62,25,20,1,5,0,2409,4305,'[{\"text\":\"Him you will spend time here. I have to go to the village.\"},{\"action_step\":1,\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"Need to stand on defense in the village, good luck to you I will wait for good news from you.\"},{\"emote\":\"happy\",\"text\":\"Good luck to you [Talked].\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(63,46,20,2,5,0,2528,4305,'[{\"text\":\"I came from a far Eastern country, I have some problems in the country so I need to become stronger.\"},{\"text\":\"[p]I came for similar purposes. But I would like to visit your country.\"},{\"emote\":\"happy\",\"text\":\"**He speaks with a smile**: If you wish I will be sailing back soon I can take you with me\"},{\"action_step\":1,\"text\":\"[p]I\'d love to, but I don\'t know, i have here made friends. We\'ll see.\"},{\"emote\":\"blink\",\"text\":\"Well, if we still live of course. And the got to talking and drive away goblins need.\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(64,46,20,3,5,0,3867,3089,'[{\"action_step\":1,\"text\":\"To begin with, we must exterminate many of them as possible. To get to them in zone\"},{\"emote\":\"pain\",\"text\":\"[p]**Very tired voice**: [Talked]. I\'m so tired, there are so many of them.....\"},{\"emote\":\"pain\",\"text\":\"**He sounds very tired**: Yes, I am also very tired, but we have to deal with it.\"},{\"text\":\"All right let\'s keep going before they pile up again.\"}]',NULL,NULL,NULL,NULL,35,22,'|0|0|0|0|5|30|',NULL,NULL),(65,46,20,4,5,0,122,3377,'[{\"text\":\"Hmmm.. I see a passageway. I think we can go through there and find out where they\'re coming from.\"},{\"action_step\":1,\"emote\":\"blink\",\"text\":\"[p]I\'m worried about something.. Where did this hole?\"},{\"emote\":\"blink\",\"text\":\"I don\'t know either, honestly. Maybe the goblins have dug through and are coming out. In any case we need to find out.\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(66,46,21,1,6,0,881,1521,'[{\"text\":\"I think this is where they live. \"},{\"text\":\"In any case, if we can not cope with them, there is an option to block the way. But I think they\'ll dig it up again in time.\"},{\"action_step\":1,\"style\":\"aggresive\",\"emote\":\"blink\",\"text\":\"[p]**In anger**: LET\'s BREAK UP THESE FREAKS ALREADY... \"},{\"style\":\"aggresive\",\"emote\":\"happy\",\"text\":\"I\'m only for your idea. Let\'s go!\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(67,46,21,2,6,0,3121,4114,'[{\"action_step\":1,\"text\":\"And here is their leader.\"},{\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"[p]Did we manage?\"},{\"emote\":\"blink\",\"text\":\"We don\'t know yet we need to get out of here....\"},{\"text\":\"I\'ll be waiting for you where we fought the goblins.\"}]',NULL,NULL,NULL,NULL,32,NULL,'|0|0|0|0|1|0|',NULL,NULL),(68,46,21,3,5,0,1714,4433,'[{\"action_step\":1,\"text\":\"[p]Finally we got out how do you think we managed?\"},{\"emote\":\"blink\",\"text\":\"I don\'t know [Player], we have to report this news to the village.\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|1|0|',NULL,NULL),(69,46,22,1,5,0,970,4529,'[{\"emote\":\"happy\",\"text\":\"Well, I think my work here is done [Player].\"},{\"emote\":\"blink\",\"text\":\"[p]Your [Talked] next trip will be to your homeland?\"},{\"emote\":\"happy\",\"text\":\"Yes, I think it\'s time to go home, I missed it.\"},{\"emote\":\"happy\",\"text\":\"[p]Will you take me with you?\"},{\"emote\":\"happy\",\"text\":\"Yes, of course. Say goodbye to your friends or maybe one of them will go with you?\"},{\"action_step\":1,\"text\":\"No, I don\'t want to expose them to danger.\"},{\"text\":\"Well\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(70,10,22,2,5,0,2337,4305,'[{\"emote\":\"pain\",\"text\":\"[p]Well, goodbye [Talked], I\'ll keep going. I want to be stronger than I was before.\"},{\"emote\":\"happy\",\"text\":\"All right [Player]. Good luck don\'t forget that we will always be happy to welcome you as a guest ;)\"},{\"action_step\":1,\"emote\":\"happy\",\"text\":\"[p]Don\'t forget, don\'t miss me ;)\"},{\"text\":\"Don\'t worry, Diana is very attached to you. I think she will be offended. Talk to she!\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(71,6,22,3,5,0,2546,4305,'[{\"emote\":\"pain\",\"text\":\"Why didn\'t you tell [Player] me you were going to leave?\"},{\"emote\":\"pain\",\"text\":\"[p]I\'m Sorry [Talked]. I wanted to tell you..\"},{\"emote\":\"pain\",\"text\":\"**She said sadly**: Why? Will you take me with you?\"},{\"emote\":\"pain\",\"text\":\"[p]I don\'t want to put you in danger!\"},{\"emote\":\"pain\",\"text\":\"I want to travel with you!\"},{\"action_step\":1,\"emote\":\"blink\",\"text\":\"[p]Ok, I\'ll take you. Now let me to say Yasue San.\"},{\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"**She said joyfully**: True? Thank! ;)\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL),(72,46,22,4,5,0,4654,4241,'[{\"emote\":\"blink\",\"text\":\"Well, ready to go?\"},{\"action_step\":1,\"emote\":\"blink\",\"text\":\"[p]Yes, will I take Diana with me?\"},{\"style\":\"joyful\",\"emote\":\"happy\",\"text\":\"Yes, of course, set sail:) I will show you my homeland!\"}]',NULL,NULL,NULL,NULL,NULL,NULL,'|0|0|0|0|0|0|',NULL,NULL);
/*!40000 ALTER TABLE `tw_bots_quest` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_crafts_list`
--

DROP TABLE IF EXISTS `tw_crafts_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_crafts_list` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `ItemID` int DEFAULT NULL,
  `ItemValue` int NOT NULL,
  `RequiredItemID0` int DEFAULT NULL,
  `RequiredItemID1` int DEFAULT NULL,
  `RequiredItemID2` int DEFAULT NULL,
  `RequiredItemsValues` varchar(32) NOT NULL DEFAULT '0 0 0',
  `Price` int NOT NULL DEFAULT '100',
  `WorldID` int NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  KEY `CraftIID` (`ItemID`),
  KEY `Craft_Item_0` (`RequiredItemID0`),
  KEY `Craft_Item_1` (`RequiredItemID1`),
  KEY `Craft_Item_2` (`RequiredItemID2`),
  KEY `WorldID` (`WorldID`),
  CONSTRAINT `tw_crafts_list_ibfk_1` FOREIGN KEY (`ItemID`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_crafts_list_ibfk_2` FOREIGN KEY (`RequiredItemID0`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `tw_crafts_list_ibfk_3` FOREIGN KEY (`RequiredItemID1`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `tw_crafts_list_ibfk_4` FOREIGN KEY (`RequiredItemID2`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_crafts_list`
--

LOCK TABLES `tw_crafts_list` WRITE;
/*!40000 ALTER TABLE `tw_crafts_list` DISABLE KEYS */;
INSERT INTO `tw_crafts_list` VALUES (1,15,3,22,29,18,'9 9 16',50,2),(2,26,1,30,NULL,NULL,'24 0 0',150,2),(3,33,1,37,31,NULL,'3 30 0',2500,2),(4,34,1,37,31,NULL,'8 50 0',2700,2),(5,10019,1,37,30,31,'18 48 24',7200,2),(6,10020,1,37,30,31,'14 38 18',7200,2),(7,10021,1,37,30,31,'14 38 18',7200,2),(8,10022,1,37,30,31,'14 38 18',7200,2),(9,10023,1,37,30,31,'14 38 18',7200,2),(10,10016,1,37,NULL,NULL,'40 0 0',14400,2),(11,14,3,22,29,18,'9 9 16',50,2),(12,41,1,42,30,18,'32 16 64',3600,2);
/*!40000 ALTER TABLE `tw_crafts_list` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_dungeons`
--

DROP TABLE IF EXISTS `tw_dungeons`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_dungeons` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Name` varchar(64) NOT NULL DEFAULT 'Unknown',
  `Level` int NOT NULL DEFAULT '1',
  `DoorX` int NOT NULL DEFAULT '0',
  `DoorY` int NOT NULL DEFAULT '0',
  `RequiredQuestID` int NOT NULL DEFAULT '-1',
  `Story` tinyint NOT NULL DEFAULT '0',
  `WorldID` int NOT NULL,
  PRIMARY KEY (`ID`),
  KEY `WorldID` (`WorldID`),
  CONSTRAINT `tw_dungeons_ibfk_1` FOREIGN KEY (`WorldID`) REFERENCES `enum_worlds` (`WorldID`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_dungeons`
--

LOCK TABLES `tw_dungeons` WRITE;
/*!40000 ALTER TABLE `tw_dungeons` DISABLE KEYS */;
INSERT INTO `tw_dungeons` VALUES (1,'Abandoned mine',10,1105,1521,20,1,6),(2,'Resonance Noctis',18,1157,528,62,0,8),(3,'Kingdom Neptune',15,1084,532,22,1,10),(4,'StarKay',15,788,391,20,0,12);
/*!40000 ALTER TABLE `tw_dungeons` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_dungeons_door`
--

DROP TABLE IF EXISTS `tw_dungeons_door`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_dungeons_door` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Name` varchar(64) NOT NULL DEFAULT 'Write here name dungeon',
  `PosX` int NOT NULL,
  `PosY` int NOT NULL,
  `BotID` int NOT NULL,
  `DungeonID` int NOT NULL,
  PRIMARY KEY (`ID`),
  KEY `tw_dungeons_door_ibfk_1` (`DungeonID`),
  KEY `tw_dungeons_door_ibfk_2` (`BotID`),
  CONSTRAINT `tw_dungeons_door_ibfk_1` FOREIGN KEY (`DungeonID`) REFERENCES `tw_dungeons` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_dungeons_door_ibfk_2` FOREIGN KEY (`BotID`) REFERENCES `tw_bots_info` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_dungeons_door`
--

LOCK TABLES `tw_dungeons_door` WRITE;
/*!40000 ALTER TABLE `tw_dungeons_door` DISABLE KEYS */;
INSERT INTO `tw_dungeons_door` VALUES (1,'Write here name dungeon',4302,1940,29,1),(2,'Write here name dungeon',1808,3600,30,1),(3,'Write here name dungeon',750,4850,31,1),(4,'Write here name dungeon',2255,4850,22,1),(5,'Write here name dungeon',5233,530,40,2),(6,'Write here name dungeon',4432,2929,41,2),(7,'Write here name dungeon',1550,1970,42,2),(8,'Write here name dungeon',1647,1970,43,2),(9,'Write here name dungeon',3213,369,47,3),(10,'Write here name dungeon',400,881,48,3),(11,'Write here name dungeon',4432,1490,49,3);
/*!40000 ALTER TABLE `tw_dungeons_door` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_dungeons_records`
--

DROP TABLE IF EXISTS `tw_dungeons_records`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_dungeons_records` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `UserID` int NOT NULL,
  `DungeonID` int NOT NULL,
  `Seconds` int NOT NULL,
  `PassageHelp` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`),
  KEY `tw_dungeons_records_ibfk_1` (`UserID`),
  KEY `DungeonID` (`DungeonID`),
  KEY `Seconds` (`Seconds`),
  CONSTRAINT `tw_dungeons_records_ibfk_1` FOREIGN KEY (`UserID`) REFERENCES `tw_accounts_data` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_dungeons_records_ibfk_2` FOREIGN KEY (`DungeonID`) REFERENCES `tw_dungeons` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_dungeons_records`
--

LOCK TABLES `tw_dungeons_records` WRITE;
/*!40000 ALTER TABLE `tw_dungeons_records` DISABLE KEYS */;
INSERT INTO `tw_dungeons_records` VALUES (1,10,1,217,2359),(2,14,1,494,4947),(3,2,1,494,25182),(4,4,1,95,4523),(5,4,3,190,11650),(6,10,3,190,130),(7,2,3,147,1474);
/*!40000 ALTER TABLE `tw_dungeons_records` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_guilds`
--

DROP TABLE IF EXISTS `tw_guilds`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_guilds` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Name` varchar(32) NOT NULL DEFAULT 'Member name',
  `UserID` int DEFAULT NULL,
  `Level` int NOT NULL DEFAULT '1',
  `Experience` int NOT NULL DEFAULT '0',
  `Bank` int NOT NULL DEFAULT '0',
  `Score` int NOT NULL DEFAULT '0',
  `AvailableSlots` int NOT NULL DEFAULT '2',
  `ChairExperience` int NOT NULL DEFAULT '1',
  `ChairMoney` int NOT NULL DEFAULT '1',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  KEY `OwnerID` (`UserID`),
  KEY `Bank` (`Bank`),
  KEY `Level` (`Level`),
  KEY `Experience` (`Experience`),
  CONSTRAINT `tw_guilds_ibfk_1` FOREIGN KEY (`UserID`) REFERENCES `tw_accounts_data` (`ID`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_guilds`
--

LOCK TABLES `tw_guilds` WRITE;
/*!40000 ALTER TABLE `tw_guilds` DISABLE KEYS */;
INSERT INTO `tw_guilds` VALUES (1,'Host',1,22,8757,1571821,0,6,20,5);
/*!40000 ALTER TABLE `tw_guilds` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_guilds_decorations`
--

DROP TABLE IF EXISTS `tw_guilds_decorations`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_guilds_decorations` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `PosX` int NOT NULL,
  `PosY` int NOT NULL,
  `HouseID` int NOT NULL,
  `DecoID` int NOT NULL,
  `WorldID` int DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `tw_guilds_decorations_ibfk_2` (`DecoID`),
  KEY `tw_guilds_decorations_ibfk_3` (`WorldID`),
  KEY `HouseID` (`HouseID`),
  CONSTRAINT `tw_guilds_decorations_ibfk_2` FOREIGN KEY (`DecoID`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_guilds_decorations_ibfk_3` FOREIGN KEY (`WorldID`) REFERENCES `enum_worlds` (`WorldID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_guilds_decorations_ibfk_4` FOREIGN KEY (`HouseID`) REFERENCES `tw_guilds_houses` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_guilds_decorations`
--

LOCK TABLES `tw_guilds_decorations` WRITE;
/*!40000 ALTER TABLE `tw_guilds_decorations` DISABLE KEYS */;
/*!40000 ALTER TABLE `tw_guilds_decorations` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_guilds_history`
--

DROP TABLE IF EXISTS `tw_guilds_history`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_guilds_history` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `GuildID` int NOT NULL,
  `Text` varchar(64) NOT NULL,
  `Time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`ID`),
  KEY `MemberID` (`GuildID`),
  CONSTRAINT `tw_guilds_history_ibfk_1` FOREIGN KEY (`GuildID`) REFERENCES `tw_guilds` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=159 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_guilds_history`
--

LOCK TABLES `tw_guilds_history` WRITE;
/*!40000 ALTER TABLE `tw_guilds_history` DISABLE KEYS */;
INSERT INTO `tw_guilds_history` VALUES (1,1,'Guild raised level to \'2\'.','2021-10-18 13:29:16'),(2,1,'\'Flower\' added to bank 999999gold.','2021-10-18 13:29:50'),(3,1,'\'Available slots\' level up to \'3\'.','2021-10-18 13:29:53'),(4,1,'Guild raised level to \'3\'.','2021-10-18 13:31:49'),(5,1,'Guild raised level to \'4\'.','2021-10-18 13:36:42'),(6,1,'Guild raised level to \'5\'.','2021-10-18 14:05:32'),(7,1,'\'嬌兔來打球\' exit or kicked.','2021-10-18 14:29:05'),(8,1,'Guild raised level to \'6\'.','2021-10-18 17:10:18'),(9,1,'Guild raised level to \'7\'.','2021-10-18 17:51:14'),(10,1,'Guild raised level to \'8\'.','2021-10-18 18:49:21'),(11,1,'Guild raised level to \'9\'.','2021-10-18 20:09:19'),(12,1,'Guild raised level to \'10\'.','2021-10-18 20:36:22'),(13,1,'Guild raised level to \'11\'.','2021-10-18 21:32:57'),(14,1,'Guild raised level to \'12\'.','2021-10-18 22:42:48'),(15,1,'Guild raised level to \'13\'.','2021-10-19 07:16:01'),(16,1,'Guild raised level to \'14\'.','2021-10-19 11:51:56'),(17,1,'Guild raised level to \'15\'.','2021-10-19 16:34:00'),(18,1,'Guild raised level to \'16\'.','2021-10-19 17:45:40'),(19,1,'Bought a house on \'Elfinia\'.','2021-10-19 20:18:40'),(20,1,'Guild raised level to \'17\'.','2021-10-19 20:42:43'),(21,1,'Guild raised level to \'18\'.','2021-10-20 17:32:13'),(22,1,'\'Chair experience\' level up to \'2\'.','2021-10-22 20:27:02'),(23,1,'\'Available slots\' level up to \'4\'.','2021-10-22 20:27:10'),(24,1,'\'Available slots\' level up to \'5\'.','2021-10-22 20:27:14'),(25,1,'\'Chair experience\' level up to \'3\'.','2021-10-22 20:27:16'),(26,1,'\'Available slots\' level up to \'6\'.','2021-10-22 20:27:18'),(27,1,'\'Chair experience\' level up to \'4\'.','2021-10-22 20:28:00'),(28,1,'\'Chair experience\' level up to \'5\'.','2021-10-22 20:28:03'),(29,1,'\'Chair experience\' level up to \'6\'.','2021-10-22 20:28:05'),(30,1,'\'Chair experience\' level up to \'7\'.','2021-10-22 20:28:07'),(31,1,'\'Chair experience\' level up to \'8\'.','2021-10-22 20:28:09'),(32,1,'\'Chair experience\' level up to \'9\'.','2021-10-22 20:28:10'),(33,1,'Guild raised level to \'19\'.','2021-10-22 21:07:40'),(34,1,'Guild raised level to \'20\'.','2021-10-23 04:30:08'),(35,1,'\'Flower\' added to bank 58324gold.','2021-10-23 10:54:09'),(36,1,'\'Flower\' added to bank 999999gold.','2021-10-23 10:54:41'),(37,1,'\'Chair experience\' level up to \'10\'.','2021-10-23 10:54:43'),(38,1,'\'Chair experience\' level up to \'11\'.','2021-10-23 10:54:45'),(39,1,'\'Chair experience\' level up to \'12\'.','2021-10-23 10:54:47'),(40,1,'\'Chair experience\' level up to \'13\'.','2021-10-23 10:54:49'),(41,1,'\'Chair experience\' level up to \'14\'.','2021-10-23 10:54:51'),(42,1,'Guild raised level to \'21\'.','2021-10-23 16:36:45'),(43,1,'\'shengyanLM\' added to bank 100gold.','2021-10-23 17:18:53'),(44,1,'\'Flower\' added to bank 9999999gold.','2021-10-23 17:20:28'),(45,1,'\'Chair experience\' level up to \'15\'.','2021-10-23 17:20:32'),(46,1,'\'Chair experience\' level up to \'16\'.','2021-10-23 17:20:35'),(47,1,'\'Chair experience\' level up to \'17\'.','2021-10-23 17:20:37'),(48,1,'\'Chair experience\' level up to \'18\'.','2021-10-23 17:20:38'),(49,1,'\'Chair experience\' level up to \'19\'.','2021-10-23 17:20:40'),(50,1,'\'Chair experience\' level up to \'20\'.','2021-10-23 17:20:43'),(51,1,'\'Chair experience\' level up to \'21\'.','2021-10-23 17:20:45'),(52,1,'\'Chair experience\' level up to \'22\'.','2021-10-23 17:20:47'),(53,1,'\'Chair experience\' level up to \'23\'.','2021-10-23 17:20:49'),(54,1,'\'Chair experience\' level up to \'24\'.','2021-10-23 17:20:50'),(55,1,'\'Chair experience\' level up to \'25\'.','2021-10-23 17:20:51'),(56,1,'\'Chair experience\' level up to \'26\'.','2021-10-23 17:20:51'),(57,1,'\'Chair experience\' level up to \'27\'.','2021-10-23 17:20:52'),(58,1,'\'Chair experience\' level up to \'28\'.','2021-10-23 17:20:52'),(59,1,'\'Chair experience\' level up to \'29\'.','2021-10-23 17:20:53'),(60,1,'\'Chair experience\' level up to \'30\'.','2021-10-23 17:20:53'),(61,1,'\'Chair experience\' level up to \'31\'.','2021-10-23 17:20:54'),(62,1,'\'Chair experience\' level up to \'32\'.','2021-10-23 17:20:54'),(63,1,'\'Chair experience\' level up to \'33\'.','2021-10-23 17:20:55'),(64,1,'\'Chair experience\' level up to \'34\'.','2021-10-23 17:20:56'),(65,1,'\'Chair experience\' level up to \'35\'.','2021-10-23 17:20:56'),(66,1,'\'Chair experience\' level up to \'36\'.','2021-10-23 17:20:57'),(67,1,'\'Chair experience\' level up to \'37\'.','2021-10-23 17:20:57'),(68,1,'\'shengyanLM\' added to bank 10000000gold.','2021-10-23 17:21:09'),(69,1,'\'Chair experience\' level up to \'38\'.','2021-10-23 17:21:16'),(70,1,'\'Chair experience\' level up to \'39\'.','2021-10-23 17:21:17'),(71,1,'\'Chair experience\' level up to \'40\'.','2021-10-23 17:21:18'),(72,1,'\'Chair experience\' level up to \'41\'.','2021-10-23 17:21:19'),(73,1,'\'Chair experience\' level up to \'42\'.','2021-10-23 17:21:20'),(74,1,'\'Chair experience\' level up to \'43\'.','2021-10-23 17:21:21'),(75,1,'\'Chair experience\' level up to \'44\'.','2021-10-23 17:21:21'),(76,1,'\'Chair experience\' level up to \'45\'.','2021-10-23 17:21:22'),(77,1,'\'Chair experience\' level up to \'46\'.','2021-10-23 17:21:22'),(78,1,'\'Chair experience\' level up to \'47\'.','2021-10-23 17:21:23'),(79,1,'\'Chair experience\' level up to \'48\'.','2021-10-23 17:21:23'),(80,1,'\'Chair experience\' level up to \'49\'.','2021-10-23 17:21:24'),(81,1,'\'Chair experience\' level up to \'50\'.','2021-10-23 17:21:24'),(82,1,'\'Chair experience\' level up to \'51\'.','2021-10-23 17:21:25'),(83,1,'\'shengyanLM\' added to bank 10000000gold.','2021-10-23 17:21:36'),(84,1,'\'shengyanLM\' added to bank 10000000gold.','2021-10-23 17:21:39'),(85,1,'\'shengyanLM\' added to bank 10000000gold.','2021-10-23 17:21:42'),(86,1,'\'Chair experience\' level up to \'52\'.','2021-10-23 17:21:43'),(87,1,'\'Chair experience\' level up to \'53\'.','2021-10-23 17:21:43'),(88,1,'\'Chair experience\' level up to \'54\'.','2021-10-23 17:21:44'),(89,1,'\'Chair experience\' level up to \'55\'.','2021-10-23 17:21:44'),(90,1,'\'Chair experience\' level up to \'56\'.','2021-10-23 17:21:45'),(91,1,'\'Chair experience\' level up to \'57\'.','2021-10-23 17:21:45'),(92,1,'\'Chair experience\' level up to \'58\'.','2021-10-23 17:21:46'),(93,1,'\'Chair experience\' level up to \'59\'.','2021-10-23 17:21:46'),(94,1,'\'Chair experience\' level up to \'60\'.','2021-10-23 17:21:47'),(95,1,'\'Chair experience\' level up to \'61\'.','2021-10-23 17:21:47'),(96,1,'\'Chair experience\' level up to \'62\'.','2021-10-23 17:21:48'),(97,1,'\'Chair experience\' level up to \'63\'.','2021-10-23 17:21:49'),(98,1,'\'shengyanLM\' added to bank 10000000gold.','2021-10-23 17:21:49'),(99,1,'\'Chair experience\' level up to \'64\'.','2021-10-23 17:21:49'),(100,1,'\'Chair experience\' level up to \'65\'.','2021-10-23 17:21:50'),(101,1,'\'shengyanLM\' added to bank 10000000gold.','2021-10-23 17:21:51'),(102,1,'\'Chair experience\' level up to \'66\'.','2021-10-23 17:21:51'),(103,1,'\'Chair experience\' level up to \'67\'.','2021-10-23 17:21:52'),(104,1,'\'Chair experience\' level up to \'68\'.','2021-10-23 17:21:53'),(105,1,'\'shengyanLM\' added to bank 10000000gold.','2021-10-23 17:21:53'),(106,1,'\'Chair experience\' level up to \'69\'.','2021-10-23 17:21:54'),(107,1,'\'Chair experience\' level up to \'70\'.','2021-10-23 17:21:55'),(108,1,'\'shengyanLM\' added to bank 10000000gold.','2021-10-23 17:21:55'),(109,1,'\'shengyanLM\' added to bank 10000000gold.','2021-10-23 17:21:57'),(110,1,'\'shengyanLM\' added to bank 10000000gold.','2021-10-23 17:21:58'),(111,1,'\'Chair experience\' level up to \'71\'.','2021-10-23 17:22:03'),(112,1,'\'Chair experience\' level up to \'72\'.','2021-10-23 17:22:04'),(113,1,'\'Chair experience\' level up to \'73\'.','2021-10-23 17:22:05'),(114,1,'\'Chair experience\' level up to \'74\'.','2021-10-23 17:22:06'),(115,1,'\'Chair experience\' level up to \'75\'.','2021-10-23 17:22:07'),(116,1,'\'Chair experience\' level up to \'76\'.','2021-10-23 17:22:08'),(117,1,'\'Chair experience\' level up to \'77\'.','2021-10-23 17:22:09'),(118,1,'\'Chair experience\' level up to \'78\'.','2021-10-23 17:22:10'),(119,1,'\'Chair experience\' level up to \'79\'.','2021-10-23 17:22:10'),(120,1,'\'Chair experience\' level up to \'80\'.','2021-10-23 17:22:11'),(121,1,'\'Chair experience\' level up to \'81\'.','2021-10-23 17:22:12'),(122,1,'\'Chair experience\' level up to \'82\'.','2021-10-23 17:22:13'),(123,1,'\'Chair experience\' level up to \'83\'.','2021-10-23 17:22:14'),(124,1,'\'Chair experience\' level up to \'84\'.','2021-10-23 17:22:15'),(125,1,'\'Chair experience\' level up to \'85\'.','2021-10-23 17:22:16'),(126,1,'\'Chair experience\' level up to \'86\'.','2021-10-23 17:22:17'),(127,1,'\'Chair experience\' level up to \'87\'.','2021-10-23 17:22:18'),(128,1,'\'Chair experience\' level up to \'88\'.','2021-10-23 17:22:19'),(129,1,'\'Chair experience\' level up to \'89\'.','2021-10-23 17:22:20'),(130,1,'\'Chair experience\' level up to \'90\'.','2021-10-23 17:22:21'),(131,1,'\'Chair experience\' level up to \'91\'.','2021-10-23 17:22:22'),(132,1,'\'Chair experience\' level up to \'92\'.','2021-10-23 17:22:23'),(133,1,'\'Chair experience\' level up to \'93\'.','2021-10-23 17:22:24'),(134,1,'\'Chair experience\' level up to \'94\'.','2021-10-23 17:22:25'),(135,1,'\'Chair experience\' level up to \'95\'.','2021-10-23 17:22:26'),(136,1,'\'Chair experience\' level up to \'96\'.','2021-10-23 17:22:27'),(137,1,'\'Chair experience\' level up to \'97\'.','2021-10-23 17:22:28'),(138,1,'\'Chair experience\' level up to \'98\'.','2021-10-23 17:22:29'),(139,1,'\'Chair experience\' level up to \'99\'.','2021-10-23 17:22:30'),(140,1,'\'Chair experience\' level up to \'100\'.','2021-10-23 17:22:31'),(141,1,'\'Chair experience\' level up to \'101\'.','2021-10-23 17:22:32'),(142,1,'\'Chair experience\' level up to \'102\'.','2021-10-23 17:22:33'),(143,1,'\'Chair experience\' level up to \'103\'.','2021-10-23 17:22:34'),(144,1,'\'Chair experience\' level up to \'104\'.','2021-10-23 17:22:35'),(145,1,'\'Chair experience\' level up to \'105\'.','2021-10-23 17:22:36'),(146,1,'\'Chair experience\' level up to \'106\'.','2021-10-23 17:22:37'),(147,1,'\'Chair experience\' level up to \'107\'.','2021-10-23 17:22:38'),(148,1,'\'Chair experience\' level up to \'108\'.','2021-10-23 17:22:39'),(149,1,'\'Chair experience\' level up to \'109\'.','2021-10-23 17:22:40'),(150,1,'\'Chair experience\' level up to \'110\'.','2021-10-23 17:22:41'),(151,1,'\'Chair experience\' level up to \'111\'.','2021-10-23 17:22:42'),(152,1,'\'Chair experience\' level up to \'112\'.','2021-10-23 17:22:42'),(153,1,'\'Chair experience\' level up to \'113\'.','2021-10-23 17:22:43'),(154,1,'\'Chair experience\' level up to \'114\'.','2021-10-23 17:22:44'),(155,1,'\'Chair experience\' level up to \'115\'.','2021-10-23 17:22:45'),(156,1,'\'HX1493\' exit or kicked.','2021-10-23 17:27:29'),(157,1,'New guild leader \'FlowerFell-Sans\'.','2021-10-23 19:02:18'),(158,1,'Guild raised level to \'22\'.','2021-10-24 15:00:55');
/*!40000 ALTER TABLE `tw_guilds_history` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_guilds_houses`
--

DROP TABLE IF EXISTS `tw_guilds_houses`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_guilds_houses` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `GuildID` int DEFAULT NULL,
  `PosX` int NOT NULL,
  `PosY` int NOT NULL,
  `DoorX` int NOT NULL,
  `DoorY` int NOT NULL,
  `TextX` int NOT NULL,
  `TextY` int NOT NULL,
  `Price` int NOT NULL DEFAULT '50000',
  `WorldID` int NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  KEY `OwnerMID` (`GuildID`),
  KEY `WorldID` (`WorldID`),
  CONSTRAINT `tw_guilds_houses_ibfk_1` FOREIGN KEY (`WorldID`) REFERENCES `enum_worlds` (`WorldID`) ON UPDATE CASCADE,
  CONSTRAINT `tw_guilds_houses_ibfk_2` FOREIGN KEY (`GuildID`) REFERENCES `tw_guilds` (`ID`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_guilds_houses`
--

LOCK TABLES `tw_guilds_houses` WRITE;
/*!40000 ALTER TABLE `tw_guilds_houses` DISABLE KEYS */;
INSERT INTO `tw_guilds_houses` VALUES (1,NULL,4250,6352,4496,6461,4206,6224,240000,2),(2,1,9504,5713,9180,5713,9486,5495,280000,2),(3,NULL,3601,1521,3890,1513,3577,1346,320000,11);
/*!40000 ALTER TABLE `tw_guilds_houses` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_guilds_invites`
--

DROP TABLE IF EXISTS `tw_guilds_invites`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_guilds_invites` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `GuildID` int NOT NULL,
  `UserID` int NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  KEY `OwnerID` (`UserID`),
  KEY `MemberID` (`GuildID`),
  CONSTRAINT `tw_guilds_invites_ibfk_1` FOREIGN KEY (`UserID`) REFERENCES `tw_accounts_data` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_guilds_invites_ibfk_2` FOREIGN KEY (`GuildID`) REFERENCES `tw_guilds` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_guilds_invites`
--

LOCK TABLES `tw_guilds_invites` WRITE;
/*!40000 ALTER TABLE `tw_guilds_invites` DISABLE KEYS */;
INSERT INTO `tw_guilds_invites` VALUES (8,1,14),(9,1,23),(11,1,22);
/*!40000 ALTER TABLE `tw_guilds_invites` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_guilds_ranks`
--

DROP TABLE IF EXISTS `tw_guilds_ranks`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_guilds_ranks` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Access` int NOT NULL DEFAULT '3',
  `Name` varchar(32) NOT NULL DEFAULT 'Rank name',
  `GuildID` int NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  KEY `MemberID` (`GuildID`),
  CONSTRAINT `tw_guilds_ranks_ibfk_1` FOREIGN KEY (`GuildID`) REFERENCES `tw_guilds` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_guilds_ranks`
--

LOCK TABLES `tw_guilds_ranks` WRITE;
/*!40000 ALTER TABLE `tw_guilds_ranks` DISABLE KEYS */;
/*!40000 ALTER TABLE `tw_guilds_ranks` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_houses`
--

DROP TABLE IF EXISTS `tw_houses`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_houses` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `UserID` int DEFAULT NULL,
  `PosX` int NOT NULL,
  `PosY` int NOT NULL,
  `DoorX` int NOT NULL,
  `DoorY` int NOT NULL,
  `Class` varchar(32) NOT NULL DEFAULT 'Class name',
  `Price` int NOT NULL,
  `HouseBank` int NOT NULL,
  `PlantID` int NOT NULL,
  `PlantX` int NOT NULL,
  `PlantY` int NOT NULL,
  `WorldID` int NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  KEY `OwnerID` (`UserID`),
  KEY `WorldID` (`WorldID`),
  KEY `PlantID` (`PlantID`),
  CONSTRAINT `tw_houses_ibfk_1` FOREIGN KEY (`PlantID`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_houses_ibfk_2` FOREIGN KEY (`WorldID`) REFERENCES `enum_worlds` (`WorldID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_houses`
--

LOCK TABLES `tw_houses` WRITE;
/*!40000 ALTER TABLE `tw_houses` DISABLE KEYS */;
INSERT INTO `tw_houses` VALUES (1,4,8995,7672,8752,7740,'Elven class',150000,0,18,9456,7766,2),(2,27,7999,5297,8241,5297,'Elven class',150000,886,18,7492,5329,2),(3,NULL,2036,593,1937,585,'Asian-style home',180000,0,18,2559,593,11),(4,NULL,2036,913,1937,905,'Asian-style home',180000,0,18,2559,913,11),(5,NULL,1216,593,1326,585,'Asian-style home',180000,0,18,715,593,11),(6,NULL,1216,913,1326,905,'Asian-style home',180000,0,18,715,913,11);
/*!40000 ALTER TABLE `tw_houses` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_houses_decorations`
--

DROP TABLE IF EXISTS `tw_houses_decorations`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_houses_decorations` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `PosX` int NOT NULL,
  `PosY` int NOT NULL,
  `HouseID` int NOT NULL,
  `DecoID` int NOT NULL,
  `WorldID` int NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  KEY `WorldID` (`WorldID`),
  KEY `HouseID` (`HouseID`),
  KEY `DecoID` (`DecoID`),
  CONSTRAINT `tw_houses_decorations_ibfk_1` FOREIGN KEY (`HouseID`) REFERENCES `tw_houses` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_houses_decorations_ibfk_2` FOREIGN KEY (`DecoID`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_houses_decorations_ibfk_3` FOREIGN KEY (`WorldID`) REFERENCES `enum_worlds` (`WorldID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_houses_decorations`
--

LOCK TABLES `tw_houses_decorations` WRITE;
/*!40000 ALTER TABLE `tw_houses_decorations` DISABLE KEYS */;
/*!40000 ALTER TABLE `tw_houses_decorations` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_items_list`
--

DROP TABLE IF EXISTS `tw_items_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_items_list` (
  `ItemID` int NOT NULL AUTO_INCREMENT,
  `Name` varchar(32) NOT NULL DEFAULT 'Item name',
  `Description` varchar(64) NOT NULL DEFAULT 'Item desc',
  `Icon` varchar(16) NOT NULL DEFAULT 'some1',
  `Type` int DEFAULT '-1',
  `Function` int DEFAULT '-1',
  `Desynthesis` int NOT NULL DEFAULT '100',
  `Selling` int NOT NULL DEFAULT '100',
  `Attribute0` int DEFAULT NULL,
  `Attribute1` int DEFAULT NULL,
  `AttributeValue0` int NOT NULL DEFAULT '0',
  `AttributeValue1` int NOT NULL,
  `ProjectileID` int NOT NULL DEFAULT '-1' COMMENT 'only for weapons',
  PRIMARY KEY (`ItemID`),
  UNIQUE KEY `ItemID` (`ItemID`),
  UNIQUE KEY `Name` (`Name`),
  KEY `ItemBonus` (`Attribute0`),
  KEY `ItemID_2` (`ItemID`),
  KEY `ItemType` (`Type`),
  KEY `tw_items_list_ibfk_3` (`Function`),
  KEY `ItemProjID` (`ProjectileID`),
  KEY `tw_items_list_ibfk_5` (`Attribute1`),
  CONSTRAINT `tw_items_list_ibfk_1` FOREIGN KEY (`Type`) REFERENCES `enum_items_types` (`TypeID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_items_list_ibfk_2` FOREIGN KEY (`Function`) REFERENCES `enum_items_functional` (`FunctionID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=15004 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_items_list`
--

LOCK TABLES `tw_items_list` WRITE;
/*!40000 ALTER TABLE `tw_items_list` DISABLE KEYS */;
INSERT INTO `tw_items_list` VALUES (1,'Gold','Major currency','gold',-1,-1,0,0,16,NULL,0,0,-1),(2,'Hammer','A normal hammer','hammer',6,0,0,0,16,6,10,3,-1),(3,'Gun','Conventional weapon','gun',6,1,0,10,17,NULL,10,0,-1),(4,'Shotgun','Conventional weapon','shotgun',6,2,0,10,18,NULL,5,0,-1),(5,'Grenade','Conventional weapon','grenade',6,3,0,10,19,NULL,10,0,-1),(6,'Rifle','Conventional weapon','rifle',6,4,0,10,20,NULL,10,0,-1),(7,'Material','Required to improve weapons','material',4,-1,0,10,NULL,NULL,0,0,-1),(8,'Ticket guild','Command: /gcreate <name>','ticket',4,-1,0,10,NULL,NULL,0,0,-1),(9,'Skill Point','Skill point','skill_point',-1,-1,0,10,NULL,NULL,0,0,-1),(10,'Decoration Armor','Decoration for house!','deco_house',7,-1,0,10,NULL,NULL,0,0,-1),(11,'Decoration Hearth Elite','Decoration for house!','deco_house',7,-1,0,10,NULL,NULL,0,0,-1),(12,'Decoration Ninja Elite','Decoration for house!','deco_house',7,-1,0,10,NULL,NULL,0,0,-1),(13,'Decoration Hearth','Decoration for house!','deco_house',7,-1,0,10,NULL,NULL,0,0,-1),(14,'Potion mana regen','Regenerate +5%, 15sec every sec.\n','potion_b',8,8,20,10,NULL,NULL,0,0,-1),(15,'Potion health regen','Regenerate +3% health, 15sec every sec.','potion_r',8,8,20,10,NULL,NULL,0,0,-1),(16,'Capsule survival experience','You got 10-50 experience survival','potion_g',1,9,0,10,NULL,NULL,0,0,-1),(17,'Little bag of gold','You got 10-50 gold','pouch',1,9,0,10,NULL,NULL,0,0,-1),(18,'Mirt','Information added later.','some1',4,11,2,10,NULL,NULL,0,0,-1),(20,'Potato','Material need for craft!','potato',4,11,1,10,NULL,NULL,0,0,-1),(21,'Notebook','In it, something is written','paper',4,-1,0,10,NULL,NULL,0,0,-1),(22,'Glue','I wonder what it\'s for?','some4',4,-1,0,10,NULL,NULL,0,0,-1),(23,'Board','Plain Board','board',4,-1,0,10,NULL,NULL,0,0,-1),(24,'Mushroom','Material need for craft!','mushroom',4,11,2,10,NULL,NULL,0,0,-1),(25,'Potion resurrection','Resuscitates in the zone where you died!','potion_p',8,-1,0,10,NULL,NULL,0,0,-1),(26,'Goblin Pickaxe','It happens sometimes','pickaxe',6,5,0,10,14,15,2,3,-1),(27,'Young fighter\'s ring','It happens sometimes','ring',3,10,0,10,8,NULL,125,0,-1),(28,'Small ammo bag','Adds a small amount of ammunition','pouch',3,10,0,10,13,NULL,3,0,-1),(29,'Gel','I wonder what it\'s for?','some4',4,-1,0,10,NULL,NULL,0,0,-1),(30,'Goblin Ingot','Information added later.','ignot_g',4,-1,0,10,NULL,NULL,0,0,-1),(31,'Copper Ingot','Information added later.','ignot_o',4,-1,0,10,NULL,NULL,0,0,-1),(32,'Salantra Blessing','It happens sometimes','ticket',3,10,0,10,8,NULL,250,0,-1),(33,'Explosive module for gun','It happens sometimes','module',3,10,0,10,17,NULL,5,0,-1),(34,'Explosive module for shotgun','It happens sometimes','module',3,10,0,10,18,NULL,5,0,-1),(35,'Kappa meat','Information added later.','meat',4,-1,0,10,NULL,NULL,0,0,-1),(36,'Ring of Arvida','It happens sometimes','ring_light',3,10,0,10,11,10,5,10,-1),(37,'Relic of the Orc Lord','Information added later.','lucky_r',4,-1,0,10,NULL,NULL,0,0,-1),(38,'Ticket reset class stats','Resets only class stats(Dps, Tank, Healer).','ticket',1,8,0,10,NULL,NULL,0,0,-1),(39,'Mode PVP','Settings game.','without',5,10,0,0,NULL,NULL,0,0,-1),(40,'Ticket reset weapon stats','Resets only ammo stats(Ammo).','ticket',1,8,0,10,NULL,NULL,0,0,-1),(41,'Orc\'s Belt','You can feel the light power of mana.','mantle',3,10,0,10,10,5,25,40,-1),(42,'Torn cloth clothes of orcs','Information added later.','some2',4,-1,0,10,NULL,NULL,0,0,-1),(43,'Blessing for discount craft','Need dress it, -20% craft price','book',8,8,0,10,NULL,NULL,0,0,-1),(44,'Noctis fragment','Event Final Fantasy.','dark_crst',4,-1,0,10,NULL,NULL,0,0,-1),(45,'The core of Neptune','Information added later.','lucky_r',4,-1,0,10,NULL,NULL,0,0,-1),(46,'Remains of a dead soul','Information added later.','pouch',4,-1,0,10,NULL,NULL,0,0,-1),(47,'Ring of God of War','It happens sometimes','ring_fire',3,10,0,10,4,0,5,0,-1),(48,'Bracelet of Fire','It happens sometimes','bracelet_fire',3,10,0,10,4,0,7,0,-1),(49,'Big ammo bag','Adds a small amount of ammunition','pouch',3,10,0,10,13,NULL,8,0,-1),(50,'Strength ','Increase your strength','skill',-1,-1,0,0,4,NULL,1,0,-1),(51,'Random home decor','Lucky house decor','lucky_r',1,9,0,0,NULL,NULL,0,0,-1),(10000,'Heavenly hammer','Reinforced kick','h_heaven',6,0,0,10,16,NULL,1,0,-1),(10001,'Heavenly gun','It look doesn\'t bad','g_heaven',6,1,0,10,17,NULL,10,0,3),(10002,'Heavenly shotgun','It look doesn\'t bad','s_heaven',6,2,0,10,18,NULL,10,0,4),(10003,'Heavenly grenade','It look doesn\'t bad','gr_heaven',6,3,0,10,19,NULL,10,0,5),(10004,'Heavenly rifle','It look doesn\'t bad','r_heaven',6,4,0,10,20,NULL,10,0,-1),(10005,'Shadow wings','Dark history','wings',6,6,0,10,7,NULL,300,0,-1),(10006,'Neptune wings','Covered in secrets','wings',6,6,0,10,11,NULL,300,0,-1),(10007,'Angel wings','Covered in secrets','wings',6,6,0,10,10,NULL,300,0,-1),(10008,'Heavenly wings','Covered in secrets','wings',6,6,0,10,8,NULL,300,0,-1),(10009,'Rainbow wings','Covered in secrets','wings',6,6,0,10,8,NULL,300,0,-1),(10010,'Magitech hammer','Reinforced kick','h_magitech',6,0,0,10,16,NULL,1,5,-1),(10011,'Magitech gun','It look doesn\'t bad','g_magitech',6,1,0,10,17,NULL,10,0,0),(10012,'Magitech shotgun','It look doesn\'t bad','s_magitech',6,2,0,10,18,NULL,10,0,1),(10013,'Magitech grenade','It look doesn\'t bad','gr_magitech',6,3,0,10,19,NULL,10,0,2),(10014,'Magitech rifle','It look doesn\'t bad','r_magitech',6,4,0,10,20,NULL,10,0,-1),(10015,'Stars wings','Covered in secrets','wings',6,6,0,10,8,NULL,300,0,-1),(10016,'Bat wings','Covered in secrets','wings',6,6,0,10,8,NULL,300,0,-1),(10017,'Little eagle wings','Covered in secrets','wings',6,6,0,10,8,NULL,300,0,-1),(10018,'Necromante wings','Covered in secrets','wings',6,6,0,10,8,NULL,300,0,-1),(10019,'Goblin hammer','Reinforced kick','h_goblin',6,0,0,10,16,NULL,25,0,-1),(10020,'Goblin gun','It look doesn\'t bad','g_goblin',6,1,0,10,17,NULL,25,0,6),(10021,'Goblin shotgun','It look doesn\'t bad','s_goblin',6,2,0,10,18,NULL,15,0,7),(10022,'Goblin grenade','It look doesn\'t bad','gr_goblin',6,3,0,10,19,NULL,25,0,8),(10023,'Goblin rifle','It look doesn\'t bad','r_goblin',6,4,0,10,20,NULL,25,0,-1),(10024,'Scythe','Reinforced kick','h_scythe',6,0,0,10,16,NULL,1,0,-1),(15000,'Theme Couple','Strictly limited as the theme','ticket',6,7,0,10,5,NULL,100,0,-1),(15001,'Theme Final Fantasy','Strictly limited as the theme','ticket',6,7,0,10,5,NULL,100,0,-1),(15002,'Theme Aion','Strictly limited as the theme','ticket',6,7,0,10,5,NULL,100,0,-1),(15003,'Theme Dragon Nest','Strictly limited as the theme','ticket',6,7,0,10,5,NULL,100,0,-1);
/*!40000 ALTER TABLE `tw_items_list` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_logics_worlds`
--

DROP TABLE IF EXISTS `tw_logics_worlds`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_logics_worlds` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `MobID` int NOT NULL,
  `Mode` int NOT NULL DEFAULT '0' COMMENT '(1,3) 0 up 1 left',
  `ParseInt` int NOT NULL COMMENT '(2) health (3)itemid key',
  `PosX` int NOT NULL,
  `PosY` int NOT NULL,
  `WorldID` int NOT NULL,
  `Comment` varchar(64) NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  KEY `MobID` (`MobID`),
  KEY `WorldID` (`WorldID`),
  KEY `ParseInt` (`ParseInt`),
  CONSTRAINT `tw_logics_worlds_ibfk_1` FOREIGN KEY (`WorldID`) REFERENCES `tw_world_swap` (`WorldID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_logics_worlds_ibfk_2` FOREIGN KEY (`ParseInt`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_logics_worlds`
--

LOCK TABLES `tw_logics_worlds` WRITE;
/*!40000 ALTER TABLE `tw_logics_worlds` DISABLE KEYS */;
/*!40000 ALTER TABLE `tw_logics_worlds` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_positions_farming`
--

DROP TABLE IF EXISTS `tw_positions_farming`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_positions_farming` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `ItemID` int NOT NULL,
  `Level` int NOT NULL DEFAULT '1',
  `PositionX` int NOT NULL,
  `PositionY` int NOT NULL,
  `Distance` int NOT NULL DEFAULT '300',
  `WorldID` int NOT NULL,
  PRIMARY KEY (`ID`),
  KEY `ItemID` (`ItemID`),
  CONSTRAINT `tw_positions_farming_ibfk_1` FOREIGN KEY (`ItemID`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_positions_farming`
--

LOCK TABLES `tw_positions_farming` WRITE;
/*!40000 ALTER TABLE `tw_positions_farming` DISABLE KEYS */;
INSERT INTO `tw_positions_farming` VALUES (1,18,1,320,3585,250,5),(2,20,1,9952,4865,250,2);
/*!40000 ALTER TABLE `tw_positions_farming` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_positions_mining`
--

DROP TABLE IF EXISTS `tw_positions_mining`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_positions_mining` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `ItemID` int NOT NULL,
  `Level` int NOT NULL DEFAULT '1',
  `Health` int NOT NULL DEFAULT '100',
  `PositionX` int NOT NULL,
  `PositionY` int NOT NULL,
  `Distance` int NOT NULL DEFAULT '300',
  `WorldID` int NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  KEY `ItemID` (`ItemID`),
  KEY `WorldID` (`WorldID`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_positions_mining`
--

LOCK TABLES `tw_positions_mining` WRITE;
/*!40000 ALTER TABLE `tw_positions_mining` DISABLE KEYS */;
INSERT INTO `tw_positions_mining` VALUES (1,7,1,150,5742,686,300,0),(2,31,3,240,1485,4100,300,5),(3,31,3,240,3525,4100,2750,5);
/*!40000 ALTER TABLE `tw_positions_mining` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_quests_list`
--

DROP TABLE IF EXISTS `tw_quests_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_quests_list` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Name` varchar(24) NOT NULL DEFAULT 'Quest name',
  `Money` int NOT NULL,
  `Exp` int NOT NULL,
  `StoryLine` varchar(24) NOT NULL DEFAULT 'Hero',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=63 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_quests_list`
--

LOCK TABLES `tw_quests_list` WRITE;
/*!40000 ALTER TABLE `tw_quests_list` DISABLE KEYS */;
INSERT INTO `tw_quests_list` VALUES (1,'Help for workers',15,20,'Main: Arrival'),(2,'Helping a girl',15,20,'Main: Arrival'),(3,'The fight with slime!',25,20,'Trader Sentry'),(4,'Gel? Why do you need it?',25,20,'Trader Sentry'),(5,'Search for a daughter',40,30,'Sailor'),(6,'What is ore mining?',50,30,'Mining'),(7,'My first pickaxe',100,30,'Mining'),(8,'Help destruction of dirt',80,50,'Deputy'),(9,'Raid on the dirt',80,50,'Deputy'),(10,'Acquaintance',15,20,'Main: Apostle Elfia'),(11,'Apostle',20,20,'Main: Apostle Elfia'),(12,'Adventurer',50,40,'Main: Apostle Elfia'),(13,'Something is starting',10,20,'Main: Apostle Elfia'),(14,'History from Apostle',10,20,'Main: Apostle Elfia'),(15,'Diana has a problem',20,30,'Main: Apostle Elfia'),(16,'Mmm delicious',50,30,'Main: Apostle Elfia'),(17,'Time learn something',60,50,'Main: Apostle Elfia'),(18,'Here are the goblins',60,50,'Main: Apostle Elfia'),(19,'Occupation of goblins',60,50,'Main: Apostle Elfia'),(20,'Yasue San',60,50,'Main: Apostle Elfia'),(21,'Abandoned mine',60,50,'Main: Apostle Elfia'),(22,'Goodbye Elfinia',60,50,'Main: Apostle Elfia'),(50,'Officer\'s disputes!',60,50,'Officer Henry'),(55,'Erik\'s way saying help.',110,70,'Gunsmith Eric'),(60,'Why are you here Noctis',100,50,'Final fantasy'),(61,'First assignment',100,50,'Final fantasy'),(62,'Resonance',100,50,'Final fantasy');
/*!40000 ALTER TABLE `tw_quests_list` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_skills_list`
--

DROP TABLE IF EXISTS `tw_skills_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_skills_list` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Name` varchar(64) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `BonusName` varchar(64) NOT NULL DEFAULT '''name''',
  `BonusValue` int NOT NULL DEFAULT '1',
  `ManaPercentageCost` int NOT NULL DEFAULT '10',
  `PriceSP` int NOT NULL,
  `MaxLevel` int NOT NULL,
  `Passive` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_skills_list`
--

LOCK TABLES `tw_skills_list` WRITE;
/*!40000 ALTER TABLE `tw_skills_list` DISABLE KEYS */;
INSERT INTO `tw_skills_list` VALUES (1,'Health turret','Creates turret a recovery health ','life span',3,25,24,8,0),(2,'Sleepy Gravity','Magnet mobs to itself','radius',20,25,28,10,0),(3,'Craft Discount','Will give discount on the price of craft items','% discount gold for craft item',1,0,28,50,1),(4,'Proficiency with weapons','You can perform an automatic fire','can perform an auto fire with all types of weapons',1,0,120,1,1),(5,'Blessing of God of war','The blessing restores ammo','% recovers ammo within a radius of 800',25,50,28,4,0),(6,'Noctis Lucis Attack Teleport','An attacking teleport that deals damage to all mobs radius','% your strength',25,10,100,4,0);
/*!40000 ALTER TABLE `tw_skills_list` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_storages`
--

DROP TABLE IF EXISTS `tw_storages`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_storages` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Name` varchar(32) NOT NULL DEFAULT '''Bussines name''',
  `PosX` int NOT NULL,
  `PosY` int NOT NULL,
  `Currency` int NOT NULL DEFAULT '1',
  `WorldID` int NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  KEY `WorldID` (`WorldID`),
  KEY `Currency` (`Currency`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_storages`
--

LOCK TABLES `tw_storages` WRITE;
/*!40000 ALTER TABLE `tw_storages` DISABLE KEYS */;
INSERT INTO `tw_storages` VALUES (1,'Weapons for young adventurers',9417,6817,1,2),(2,'Elfinia Artifacts',6256,6417,1,2),(3,'Noctis Lucis Caelum',3200,3520,44,5),(5,'Master La',7352,4859,9,2);
/*!40000 ALTER TABLE `tw_storages` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_store_items`
--

DROP TABLE IF EXISTS `tw_store_items`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_store_items` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `ItemID` int NOT NULL,
  `ItemValue` int NOT NULL,
  `RequiredItemID` int NOT NULL DEFAULT '1',
  `Price` int NOT NULL,
  `UserID` int NOT NULL DEFAULT '0',
  `Enchant` int NOT NULL DEFAULT '0',
  `StorageID` int DEFAULT NULL,
  `Time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  KEY `ItemID` (`ItemID`),
  KEY `OwnerID` (`UserID`),
  KEY `StorageID` (`StorageID`),
  KEY `Time` (`Time`),
  KEY `NeedItem` (`RequiredItemID`),
  KEY `Price` (`Price`),
  CONSTRAINT `tw_store_items_ibfk_1` FOREIGN KEY (`ItemID`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `tw_store_items_ibfk_2` FOREIGN KEY (`RequiredItemID`) REFERENCES `tw_items_list` (`ItemID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=268 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_store_items`
--

LOCK TABLES `tw_store_items` WRITE;
/*!40000 ALTER TABLE `tw_store_items` DISABLE KEYS */;
INSERT INTO `tw_store_items` VALUES (1,3,1,1,120,0,0,1,'2020-05-10 18:36:16'),(2,4,1,1,310,0,0,1,'2020-05-10 18:36:16'),(3,5,1,1,320,0,0,1,'2020-05-10 18:36:16'),(4,6,1,1,400,0,0,1,'2020-05-10 18:36:16'),(5,28,1,1,980,0,0,2,'2020-05-13 21:19:28'),(6,36,1,1,690,0,0,2,'2020-05-13 21:19:28'),(7,8,1,1,3800,0,0,2,'2020-05-13 21:19:28'),(8,49,1,1,2100,0,0,2,'2020-05-13 22:19:28'),(12,47,1,1,1200,0,0,2,'2020-05-13 19:19:28'),(13,48,1,1,1500,0,0,2,'2020-05-13 19:19:28'),(19,38,1,1,3200,0,0,2,'2020-05-13 21:19:28'),(22,40,1,1,2500,0,0,2,'2020-05-13 21:19:28'),(42,15001,1,44,300,0,0,3,'2020-05-13 21:19:28'),(43,10017,1,44,300,0,0,3,'2020-05-13 21:19:28'),(45,50,1,9,5,0,0,5,'2020-05-13 22:19:28');
/*!40000 ALTER TABLE `tw_store_items` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_voucher`
--

DROP TABLE IF EXISTS `tw_voucher`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_voucher` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code` varchar(32) NOT NULL,
  `Data` text NOT NULL,
  `Multiple` tinyint(1) NOT NULL DEFAULT '0',
  `ValidUntil` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_voucher`
--

LOCK TABLES `tw_voucher` WRITE;
/*!40000 ALTER TABLE `tw_voucher` DISABLE KEYS */;
INSERT INTO `tw_voucher` VALUES (1,'VALENTINE2021','{\r\n	\"exp\": 10000,\r\n	\"items\": [\r\n		{\r\n			\"id\": 17,\r\n			\"value\": 30\r\n		},\r\n		{\r\n			\"id\": 15000,\r\n			\"value\": 1\r\n		}\r\n	]\r\n}',1,1614517578),(2,'upupdowndownBABA','{\r\n	\"exp\": 2500,\r\n	\"items\": [\r\n		{\r\n			\"id\": 17,\r\n			\"value\": 30\r\n		},\r\n		{\r\n			\"id\": 15003,\r\n			\"value\": 1\r\n		}\r\n	]\r\n}',1,1638316800);
/*!40000 ALTER TABLE `tw_voucher` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_voucher_redeemed`
--

DROP TABLE IF EXISTS `tw_voucher_redeemed`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_voucher_redeemed` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `VoucherID` int NOT NULL,
  `UserID` int NOT NULL,
  `TimeCreated` int NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_voucher_redeemed`
--

LOCK TABLES `tw_voucher_redeemed` WRITE;
/*!40000 ALTER TABLE `tw_voucher_redeemed` DISABLE KEYS */;
INSERT INTO `tw_voucher_redeemed` VALUES (1,2,4,1634897539),(2,2,2,1634898052),(3,2,22,1634898068),(4,2,10,1634898091),(5,3,22,1634948646),(6,3,4,1634949349),(7,4,22,1634949775),(8,2,23,1634951133),(9,2,14,1634960704),(10,2,1,1634989746);
/*!40000 ALTER TABLE `tw_voucher_redeemed` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tw_world_swap`
--

DROP TABLE IF EXISTS `tw_world_swap`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tw_world_swap` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `WorldID` int DEFAULT NULL,
  `PositionX` int DEFAULT NULL,
  `PositionY` int DEFAULT NULL,
  `RequiredQuestID` int DEFAULT NULL,
  `TwoWorldID` int DEFAULT NULL,
  `TwoPositionX` int DEFAULT NULL,
  `TwoPositionY` int DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`),
  KEY `WorldID` (`WorldID`),
  KEY `TwoWorldID` (`TwoWorldID`),
  KEY `tw_world_swap_ibfk_3` (`RequiredQuestID`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tw_world_swap`
--

LOCK TABLES `tw_world_swap` WRITE;
/*!40000 ALTER TABLE `tw_world_swap` DISABLE KEYS */;
INSERT INTO `tw_world_swap` VALUES (1,0,6900,1000,1,1,335,490),(2,1,4605,1067,2,2,3570,7950),(3,2,13760,6680,12,3,400,1260),(4,2,3510,6340,13,4,4740,900),(5,3,4560,1205,19,5,610,4500),(6,2,8328,6020,15,7,4135,840),(7,5,4896,4276,22,9,2905,1227),(8,9,1705,1097,NULL,11,12604,2180);
/*!40000 ALTER TABLE `tw_world_swap` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2021-10-25 19:23:32
