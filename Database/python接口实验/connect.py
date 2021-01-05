import mysql.connector
cnx = mysql.connector.connect(user='DB_USER40', password='DB_USER40@123',
host='119.3.164.126',port='3306',
database='user40db')
cmd=cnx.cursor()
cmd.execute("show session variables where Variable_name in ('interactive_timeout', 'wait_timeout');")
print(cmd.fetchall())
cnx.commit()
cnx.close()
