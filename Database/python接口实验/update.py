# update.py
import mysql.connector
cnx = mysql.connector.connect(user='DB_USER40', password='DB_USER40@123',
host='119.3.164.126',port='3306',
database='user40db')
cmd=cnx.cursor()
cmd.execute("update tbcell set EARFCN=12345,PCI=32,PSS=1,SSS=10,TAC=10000 WHERE ENODEBID=1")
# print(cmd.fetchall())
cnx.commit()
cnx.close()
