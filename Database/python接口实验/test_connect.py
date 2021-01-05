import mysql.connector
cnx = mysql.connector.connect(user='bupt2018dbs50', password='bupt2018dbs50@123',
                              host='121.36.4.61', database='bupt2018dbs50db')
print(cnx)
cnx.close()
