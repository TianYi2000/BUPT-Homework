# -*- coding: utf-8 -*-
import sys

sys.path.append("../")
import jieba
import jieba.analyse
# import MySQLdb
import pymysql
import sys
import codecs
import os
import numpy


def save(word, ser):
    conn = pymysql.connect(host='127.0.0.1'  # 连接名称，默认127.0.0.1
                           , user='root'  # 用户名
                           , passwd='septer'  # 密码
                           , port=3306  # 端口，默认为3306
                           , db='information'  # 数据库名称
                           , charset='utf8'  # 字符编码
    )

    cur = conn.cursor()  # 生成游标对象
    dataa = (word, ser)

    sqll = "INSERT INTO `key_inverted`(`keyword`, `series`) VALUES('" + str(row[1]) + "','" + resstr + "')"
    print(sqll)
    cur.execute(sqll)
    conn.commit()
    cur.close()
    conn.close()


# 连接数据库
try:
    conn = pymysql.connect(host='127.0.0.1'  # 连接名称，默认127.0.0.1
                           , user='root'  # 用户名
                           , passwd='septer'  # 密码
                           , port=3306  # 端口，默认为3306
                           , db='information'  # 数据库名称
                           , charset='utf8'  # 字符编码
                           )
    cur = conn.cursor()  # 生成游标对象
except:
    print("disconnect")

sql_query = "SELECT * FROM `inverted_index` WHERE 1"
cur.execute(sql_query)
searchResult = cur.fetchall()
wordList = []
for row in searchResult:
    #print(row)
    if wordList.count(row[1])==0:
        wordList.append(row[1])
        selquery="SELECT * FROM `inverted_index` WHERE keywords='"+ str(row[1]) +"'"
        cur.execute(selquery)
        res=cur.fetchall()
        resstr=""
        for ins in res:
            resstr+=str(ins[0])+","+str(ins[2])+";"
        sqll = "INSERT INTO `key_inverted`(`keyword`, `series`) VALUES('" + str(row[1]) + "','" + resstr + "')"
        save(str(row[1]), resstr)

cur.close()
conn.close()