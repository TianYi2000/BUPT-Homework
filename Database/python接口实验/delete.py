# delete.py
import mysql.connector
cnx = mysql.connector.connect(user='DB_USER40', password='DB_USER40@123',
host='119.3.164.126',port='3306',
database='user40db')
cmd=cnx.cursor()
cmd.execute("delete from tbcell where CITY='beijing';")
# print(cmd.fetchall())
cnx.commit()
cnx.close()
