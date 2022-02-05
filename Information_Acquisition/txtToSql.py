# -*- coding: utf-8 -*-
import sys
import codecs
# import MySQLdb
import  pymysql
import os
import numpy

#爬取文章数
global numOfText
numOfText=1974

#连接数据库
try:
	conn=pymysql.connect(host='127.0.0.1'  # 连接名称，默认127.0.0.1
                           , user='root'  # 用户名
                           , passwd='septer'  # 密码
                           , port=3306  # 端口，默认为3306
                           , db='information'  # 数据库名称
                           , charset='utf8'  # 字符编码
	)
	cur = conn.cursor() # 生成游标对象
except:
	print("disconnect")


#将爬取下来的文章序号、题目、URL、日期、内容存储到information的search_result表中
dirpath="C:\\Users\\septer\\Documents\\GitHub\\InfoSecondWork\\data\\news\\mil"
for root, dirs, files in os.walk(dirpath):
	# root 表示当前正在访问的文件夹路径
	# dirs 表示该文件夹下的子目录名list
	# files 表示该文件夹下的文件list
	# 遍历文件
	for f in files:
		path = os.path.join(root, f)
		#print(path) 
		f = open(path, 'r')
		# 获取文件名
		filename = os.path.basename(path)
		#print(filename)
		x = filename.split("-")
		y = x[3].split(".")
		i=int(y[0])
		time=x[0]+"-"+x[1]+"-"+x[2]
		#print(i)
		#print(time)
		#path="C:\\Users\\septer\\Documents\\GitHub\\InfoSecondWork\\search-app\data\\news\\mil\\news"+str(i)+".txt"
		#f = codecs.open(path, 'r', 'utf-8')   #设置文件对象
		url="http://www.chinanews.com"
		with open('C:\\Users\\septer\\Documents\\GitHub\\InfoSecondWork\\search-app\\data\\url.txt', 'r') as furl:
			for num, line in enumerate(furl):
				if num == i-1:
					url+=line
					furl.close()
					break
		passage = f.read()     #将txt文件的所有内容读入到字符串str中
		with open(path, 'r') as ftit:
			for num, line in enumerate(ftit):
				if num == 0:
					title=line
					ftit.close()
					break


		#print(i)
		title="'"+title+"'"
		#print(title)
		url="'"+url+"'"
		#print(url)
		time="'"+time+"'"
		#print(time)
		passage="'"+passage+"'"
		#print(passage)
		f.close()   #将文件关闭

		Insertquery = "INSERT INTO search_result(iindex, title, url, date, content) VALUES("+str(i)+", "+title+", "+url+", "+time+", "+passage+")"
		#sql = "INSERT INTO information (index, title, url, date, content) VALUES (i, title, url, time, passage)"  # SQL语句
		#print(Insertquery)
		#print(Insertquery)
		try:
			cur.execute(Insertquery)  # 执行SQL语句
			conn.commit()
		except:
			print("execute error")
			conn.rollback()

cur.close() # 关闭游标
conn.close() # 关闭连接
