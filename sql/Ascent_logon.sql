/*
MySQL Data Transfer
Source Host: localhost
Source Database: ascent_logon
Target Host: localhost
Target Database: ascent_logon
Date: 7/22/2012 2:08:44 PM
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for accounts
-- ----------------------------
DROP TABLE IF EXISTS `accounts`;
CREATE TABLE `accounts` (
  `acct` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Unique ID',
  `login` varchar(32) COLLATE utf8_unicode_ci NOT NULL COMMENT 'Login username',
  `password` varchar(32) COLLATE utf8_unicode_ci NOT NULL COMMENT 'Login password',
  `encrypted_password` varchar(42) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `gm` varchar(32) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'Game permissions',
  `banned` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Account Standing',
  `lastlogin` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT 'Last login timestamp',
  `lastip` varchar(16) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'Last remote address',
  `email` varchar(64) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'Contact e-mail address',
  `flags` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Client flags',
  `forceLanguage` varchar(5) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'enUS',
  `muted` int(30) NOT NULL DEFAULT '0',
  PRIMARY KEY (`acct`),
  UNIQUE KEY `login` (`login`)
) ENGINE=MyISAM AUTO_INCREMENT=72 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Account Information';

-- ----------------------------
-- Table structure for ipbans
-- ----------------------------
DROP TABLE IF EXISTS `ipbans`;
CREATE TABLE `ipbans` (
  `ip` varchar(16) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'Remote host',
  `expire` int(30) NOT NULL DEFAULT '0' COMMENT 'Expiry time (s)',
  PRIMARY KEY (`ip`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='IPBanner';

-- ----------------------------
-- Table structure for votewatch
-- ----------------------------
DROP TABLE IF EXISTS `votewatch`;
CREATE TABLE `votewatch` (
  `acct` int(10) NOT NULL,
  `ctime` int(11) NOT NULL,
  PRIMARY KEY (`acct`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

-- ----------------------------
-- Records 
-- ----------------------------
