import mysql.connector
cnx = mysql.connector.connect(user='bupt2018dbs50', password='bupt2018dbs50@123',
                              host='121.36.4.61', database='bupt2018dbs50db') 
cnx.cmd_query("set session INTERACTIVE_TIMEOUT=10000") 
cnx.commit() # 数据库有更新必须用到此语句 
cnx.close()