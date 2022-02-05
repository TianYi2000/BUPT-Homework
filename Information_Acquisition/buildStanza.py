# coding=gbk
import stanza
import os
import pymysql
import operator

def Exist(word):
    global index
    for keyword in keywords[index]:
        if (word == keyword):
            return True
    return False
id = 0
index = 0

def fetch_keyword():
    # 连接数据库
    try:
        conn = pymysql.connect(host='xxxxx'  # 连接名称，默认127.0.0.1
                               , user='root'  # 用户名
                               , passwd='xxxxx'  # 密码
                               , port=3306  # 端口，默认为3306
                               , db='information'  # 数据库名称
                               , charset='utf8'  # 字符编码
                               )
        cur = conn.cursor()  # 生成游标对象
    except:
        print("disconnect")

    sql_query = "SELECT * FROM `inverted_index_20`"
    cur.execute(sql_query)
    searchResult = cur.fetchall()
    wordList = []
    for i in range(2010):
        wordList.append([])
    for row in searchResult:
        print("building keywords:",row[1])
        wordList[row[1]].append(row[2])
    cur.close()
    conn.close()
    return wordList

print("Building a Chinese pipeline...")
zh_nlp = stanza.Pipeline('zh', processors='tokenize,pos,lemma,depparse,ner', verbose=True, use_gpu=True)
w = open("entity_result_all.csv", "w")
keywords = fetch_keyword()
dirpath="data/news/mil"
for root, dirs, files in os.walk(dirpath):
    for f in files:
        index = index + 1
        print(index, keywords[index])
        path = os.path.join(root, f)
        f = open(path, 'r')
        text = f.read()
        zh_doc = zh_nlp(text)
        ent_set =set()
        for sent in zh_doc.sentences:
            for ent in sent.ents:
                ent_set.add(ent.text + ',' + ent.type)

        for ent in ent_set:
            id = id + 1
            w.write(str(id) + ',' + str(index) + ',' + ent + '\n')
        # rela_set=set()
        # for sent in zh_doc.sentences:
        #     for word in sent.words:
        #         if (Exist(word.text) and word.head > 0 and Exist(sent.words[word.head-1].text) and word.text != sent.words[word.head-1].text):
        #             rela_set.add(word.text + ',' +sent.words[word.head-1].text + ',' + word.deprel) #from,to,type
        # for ent in ent_set:
        #     print(ent)
        # for rela in rela_set:
        #     id = id + 1
        #     w.write(str(id) + ',' + str(index) + ',' + rela + '\n')
        f.close()
w.close()
# print(*[f'id: {word.id}\tword: {word.text}\thead id: {word.head}\thead: {sent.words[word.head-1].text if word.head > 0 else "root"}\tdeprel: {word.deprel}' for sent in zh_doc.sentences for word in sent.words], sep='\n')



# print(zh_doc.entities)
#
# all_entity = []
#
# for entity in zh_doc.entities:
#     all_entity[entity["type"]].append(entity["text"])

# zh_doc = zh_nlp('该剧讲述了渝州城永安当的小伙计景天和唐门大小姐唐雪见受到两人随身玉佩的彼此吸引，他们二人“热闹而又尴尬”地相识了，成了一对欢喜冤家。其实雪见和景天正是彼此的有缘人，而他们都来历不凡，身藏几世的秘密，随着雪见家族的剧变，二人阴差阳错地步入了江湖的血雨腥风之中，并结识了徐长卿、紫萱和龙葵等，等待他们的是更加惊险的前程和重大的责任。')
# print("{:12s}\t{:12s}\t{:6s}\t{:6s}\t{:12s}".format(\
#               'text', 'lemma', 'pos', 'head', 'deprel'))
# print(*[f'id: {word.id}\tword: {word.text}\thead id: {word.head}\thead: {sent.words[word.head-1].text if word.head > 0 else "root"}\tdeprel: {word.deprel}' for sent in zh_doc.sentences for word in sent.words], sep='\n')
