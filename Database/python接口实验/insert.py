# insert.py
import mysql.connector
cnx = mysql.connector.connect(user='DB_USER40', password='DB_USER40@123',
host='119.3.164.126',port='3306',
database='user40db')
cmd=cnx.cursor()
cmd.execute("insert into tbcell (CITY, SECTOR_ID, SECTOR_NAME, ENODEBID, ENODEB_NAME) values('beijing','000001', 'haidian-HLHF-1',000001,'haidian-HLHF');")
# print(cmd.fetchall())
cnx.commit()
cnx.close()
