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
    # �������ݿ�
    try:
        conn = pymysql.connect(host='xxxxx'  # �������ƣ�Ĭ��127.0.0.1
                               , user='root'  # �û���
                               , passwd='xxxxx'  # ����
                               , port=3306  # �˿ڣ�Ĭ��Ϊ3306
                               , db='information'  # ���ݿ�����
                               , charset='utf8'  # �ַ�����
                               )
        cur = conn.cursor()  # �����α����
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

# zh_doc = zh_nlp('�þ署�������ݳ���������С��ƾ�������Ŵ�С����ѩ���ܵ�������������ı˴����������Ƕ��ˡ����ֶ������Ρ�����ʶ�ˣ�����һ�Ի�ϲԩ�ҡ���ʵѩ���;������Ǳ˴˵���Ե�ˣ������Ƕ�������������ؼ��������ܣ�����ѩ������ľ�䣬������������ز����˽�����Ѫ���ȷ�֮�У�����ʶ���쳤�䡢����������ȣ��ȴ����ǵ��Ǹ��Ӿ��յ�ǰ�̺��ش�����Ρ�')
# print("{:12s}\t{:12s}\t{:6s}\t{:6s}\t{:12s}".format(\
#               'text', 'lemma', 'pos', 'head', 'deprel'))
# print(*[f'id: {word.id}\tword: {word.text}\thead id: {word.head}\thead: {sent.words[word.head-1].text if word.head > 0 else "root"}\tdeprel: {word.deprel}' for sent in zh_doc.sentences for word in sent.words], sep='\n')
