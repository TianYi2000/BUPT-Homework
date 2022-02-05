# -*- coding: utf-8 -*-
import sys

import time

sys.path.append("../")
import jieba
import jieba.analyse
# import MySQLdb
import pymysql
import datetime
import sys
import codecs
import os
import numpy
from decimal import Decimal

def match(matchStr):
    #连接数据库
    try:
        conn=pymysql.connect(host = '122.9.42.19'
# 连接名称，默认127.0.0.1
        ,user = 'root' # 用户名
        ,passwd='Info2021' # 密码
        ,port= 3306 # 端口，默认为3306
        ,db='information' # 数据库名称
        ,charset='utf8' # 字符编码
        )
        cur = conn.cursor() # 生成游标对象
    except:
        print("disconnect")


    #将每个关键词在检索语句中的tfidf视为权值，每一个关键词去倒排索引中检索，比较每一篇文章的总tfidf

    seg_list = jieba.analyse.extract_tags(matchStr,topK=10,withWeight=True)
    # print(seg_list)

    textDict = {'0': 0}
    #textDict['1']=5
    #print(textDict['1'])

    # 不对句子进行分词 直接模糊匹配
    selquery2 = "SELECT * FROM `key_inverted` WHERE keyword LIKE'%" + matchStr + "%'"
    cur.execute(selquery2)
    res2 = cur.fetchall()
    for jw in res2:
        x2 = jw[1].split(";")
        for ww in x2:
            # print(x)
            if ww.strip() == '':
                continue
            else:
                yy = ww.split(",")
                # print(y[0])
                # print(line[0])
                # print(line[1])
                # print(y[1])
                # qstr="文章id为："+y[0]+"，该关键词为："+str(line[0])+"，该关键词在检索中权重为："+str(line[1])+"，该关键词在该文章权值为："+str(y[1])
                # print(qstr)
                try:
                    textDict[yy[0]] = textDict[yy[0]] + 0.1 * float(yy[1])
                except:
                    textDict[yy[0]] = 0.1 * float(yy[1])

    # 直接用句子对文章内容去模糊匹配
    selquery3 = "SELECT iindex FROM `search_result` WHERE content LIKE'%" + matchStr + "%'"
    cur.execute(selquery3)
    res3 = cur.fetchall()
    #print(res3)
    for pp in res3:
        #print(pp[0])
        try:
            textDict[pp[0]] = textDict[pp[0]] + 0.1 * 0.1
        except:
            textDict[pp[0]] = 0.1 * 0.1


    # 分词后关键字相等
    val=[0.0]*3000
    for line in seg_list:
        # print(line[0])
        # print(line[1])
        selquery = "SELECT * FROM `key_inverted` WHERE keyword='" + str(line[0]) + "'"
        cur.execute(selquery)
        res = cur.fetchall()
        for j in res:
            x=j[1].split(";")
            for w in x:
                #print(x)
                if w.strip() == '':
                    continue
                else:
                    y=w.split(",")
                    # print(y[0])
                    # print(line[0])
                    # print(line[1])
                    # print(y[1])
                    # qstr="文章id为："+y[0]+"，该关键词为："+str(line[0])+"，该关键词在检索中权重为："+str(line[1])+"，该关键词在该文章权值为："+str(y[1])
                    # print(qstr)
                    try:
                        textDict[y[0]] = textDict[y[0]] + float(line[1]) * float(y[1])
                    except:
                        textDict[y[0]] = float(line[1]) * float(y[1])



    #print(textDict)
    #按照value降序排序，得到一个list
    #gjc的优化：考虑时间因素,即用上述的相似度再加上5/(当前时间-文章时间)
    for element in textDict.items():
        sql_query1 = "SELECT * FROM `search_result` WHERE iindex=" + str(element[0])
        cur.execute(sql_query1)
        Result = cur.fetchall()
        if len(Result) > 0:
            dd = datetime.datetime.strptime(Result[0][3], "%Y-%m-%d")
            today = datetime.datetime.now()
            delta=today-dd;
            extra= 5/delta.days;
            textDict[element[0]]=textDict[element[0]]+extra;


    #.........
    new_textDict = sorted(textDict.items(),  key=lambda d: d[1], reverse=True)
    #ww[0]为序号
    #print(new_textDict)
    #print(new_textDict)
    #下面依次打印
    relist = []
    for ww in new_textDict:
        if int(ww[0])!=0:
            # print(ww[0])
            # print(ww[1])
            sql_query = "SELECT * FROM `search_result` WHERE iindex="+str(ww[0])
            #print(sql_query)
            cur.execute(sql_query)
            searchResult = cur.fetchall()
            temp=[]
            temp.append(searchResult)
            temp.append(ww[1])
            id=temp[0][0][0]
            sql_query0="select keywords from `inverted_index_20` where index_="+str(id);
            cur.execute(sql_query0)
            searchResult0=cur.fetchall()
            sql_query1="select `from`,`to`,type from `keywords_relation` where index_="+str(id);
            cur.execute(sql_query1)
            searchResult1 =cur.fetchall()
            sql_query2="select entity,type from `entities` where index_="+str(id);
            cur.execute(sql_query2)
            searchResult2=cur.fetchall()
            temp.append(searchResult0)
            temp.append(searchResult1)
            temp.append(searchResult2)
            relist.append(temp)


    return relist

matchStr="国家主席"
res = match(matchStr)


