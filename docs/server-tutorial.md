## 开服教程

### 准备的环境
1. Debian 10/Ubuntu 20.04 或者同时期的 Linux 发行版
2. 编译好的 mmotee 服务端
3. MySQL 5.x/8.x 服务端&客户端

### 步骤1:配置 MySQL
1. 新建一个数据库,命名为 `mmohard`,存放数据
2. 新建一个用户,命名为 `mmotee`,赋予 `mmohard.*` 的所有权限
3. 向 `mmohard` 导入 `template-cn.sql`

参考命令:
```
#apt update && apt upgrade
...
#apt install mysql-server git
...
#mysql_secure_installation

Securing the MySQL server deployment.

Connecting to MySQL using a blank password.

VALIDATE PASSWORD COMPONENT can be used to test passwords
and improve security. It checks the strength of password
and allows the users to set only those passwords which are
secure enough. Would you like to setup VALIDATE PASSWORD component?

Press y|Y for Yes, any other key for No: n
Please set the password for root here.

New password: 

Re-enter new password: 
By default, a MySQL installation has an anonymous user,
allowing anyone to log into MySQL without having to have
a user account created for them. This is intended only for
testing, and to make the installation go a bit smoother.
You should remove them before moving into a production
environment.

Remove anonymous users? (Press y|Y for Yes, any other key for No) : y
Success.


Normally, root should only be allowed to connect from
'localhost'. This ensures that someone cannot guess at
the root password from the network.

Disallow root login remotely? (Press y|Y for Yes, any other key for No) : y
Success.

By default, MySQL comes with a database named 'test' that
anyone can access. This is also intended only for testing,
and should be removed before moving into a production
environment.


Remove test database and access to it? (Press y|Y for Yes, any other key for No) : y
 - Dropping test database...
Success.

 - Removing privileges on test database...
Success.

Reloading the privilege tables will ensure that all changes
made so far will take effect immediately.

Reload privilege tables now? (Press y|Y for Yes, any other key for No) : y
Success.

All done!

#mysql

Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 52
Server version: 8.0.26-0ubuntu0.20.04.2 (Ubuntu)

Copyright (c) 2000, 2021, Oracle and/or its affiliates.

Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.


mysql> create database mmohard;
Query OK, 1 row affected (0.01 sec)

mysql> create user 'mmotee'@'localhost' identified by '114514';
Query OK, 0 rows affected (0.02 sec)

mysql> grant all on mmohard.* to 'mmotee'@'localhost';
Query OK, 0 rows affected (0.01 sec)

mysql> use mmohard
Database changed
mysql> source /home/lighthouse/template-cn.sql
Query OK, 0 rows affected (0.00 sec)
...
mysql> \q
Bye
```

### 步骤2:写配置文件
按照 `autoexec.cfg.example` 的内容写一个配置文件,各选项请参考 [`config_variables.h`](src/engine/shared/config_variables.h).

下面列举几个重要选项:
1. *`inf_add_sqlserver` (r/w) 数据库名称 数据表前缀 用户名 密码 服务器地址 服务器端口*
    
    添加数据库(r=读,w=写).
    
    **注意:** 数据表前缀请勿设置除 `tw` 以外的值.
