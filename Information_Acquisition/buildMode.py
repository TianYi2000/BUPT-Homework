# -*- coding: utf-8 -*-
import sys
sys.path.append("../")
import jieba
import jieba.analyse
import  pymysql
import sys
import codecs
import os
import numpy

w = open("tfidf_20.txt", "w")
cnt = 0

def save(index_,keywords,tf_idf):
    global cnt,w
    cnt = cnt + 1
    w.write(str(cnt) + ',' + str(index_) + ',' + keywords + ',' + str(tf_idf)+'\n')
    print(cnt, index_)

#连接数据库
try:
	conn=pymysql.connect(host = '122.9.42.19' # 连接名称，默认127.0.0.1
	,user = 'root' # 用户名
	,passwd='Info2021' # 密码
	,port= 3306 # 端口，默认为3306
	,db='information' # 数据库名称
	,charset='utf8' # 字符编码
	)
	cur = conn.cursor() # 生成游标对象
except:
	print("disconnect")


sql_query = "SELECT * FROM `search_result` WHERE 1"
cur.execute(sql_query)
searchResult = cur.fetchall()
for row in searchResult:
    #print(row[0])
    seg_list = jieba.analyse.extract_tags(row[4],topK=20,withWeight=True)

    for line in seg_list:
        # print(row[0])
        # print(line[0])
        # print(line[1])
        save(row[0], line[0], line[1])

w.close()