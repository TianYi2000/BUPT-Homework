import json
from flask import Flask, request
import retrievalMatch
import re
app = Flask(__name__)

@app.after_request
def cors(environ):
    environ.headers['Access-Control-Allow-Origin']='*'
    environ.headers['Access-Control-Allow-Method']='*'
    environ.headers['Access-Control-Allow-Headers']='x-requested-with,content-type'
    return environ

def equipments(content):#军事装备名称文件包括搜狗语料库，维基百科，新东方军事方面英语词汇翻译
    f=open("mil_names.txt","r",encoding="utf-8")
    data=f.readlines()
    f.close();
    ans=[]
    for i in data:
        i = i.rstrip("\n")
        if i in content and i not in ans:
            ans.append(i)
    return ans;


def countries(content):#国家名称
    f=open("countries.txt","r",encoding="utf-8")
    data=f.readlines()
    f.close();
    ans=[]
    for i in data:
        i = i.rstrip("\n")
        if i in content and i not in ans:
            ans.append(i)
    return ans;


def army_kinds(content):#军队类型
    f=open("army_kinds.txt","r",encoding="utf-8")
    data=f.readlines()
    f.close();
    ans=[]
    for i in data:
        i = i.rstrip("\n")
        if i in content and i not in ans:
            ans.append(i)
    return ans;


#正则表达式匹配，设置一个含有正则表达式字符串列表进行循环匹配

writer=['【*】','电 (*)'];
time0=['[0-9]+-[0-9]+-[0-9]+'];

def writer(content):
    ans=re.findall(r'【编辑:(.*)】',content);
    if len(ans) > 0:
        return ans;
    else:
        ans=re.findall(r'电 \((.+)\)',content);
        return ans;

def time0(content):
    ans=re.findall(r'([0-9]+-[0-9]+-[0-9]+)',content);
    return ans;


@app.route('/search',methods=['POST'])
def index():
    str0=request.form.get('str');
    cnt0=int(request.form.get('limit'));
    tmp=retrievalMatch.match(str0);
    ans=[];cnt=0;
    for i in tmp:
        cnt=cnt+1;
        temp=[]
        temp.append({"similarity": i[1]})
        j=i[0][0]
        temp.append({"id": j[0]})
        temp.append({"title": j[1]})
        temp.append({"url": j[2]})
        time1 = time0(j[4]);
        temp.append({"date": time1[0]})
        temp.append({"content": j[4]})
        equip=equipments(j[4]);
        temp.append({"equipments": equip})
        country = countries(j[4]);
        temp.append({"countries": country})
        army = army_kinds(j[4]);
        temp.append({"army": army})
        writer0=writer(j[4]);
        temp.append({"writer": writer0})
        temp.append({"keyword":i[2]})
        temp.append({"relationship":i[3]})
        temp.append({"entity":i[4]})
        ans.append(temp)
        if (cnt==cnt0):
            break;
    return json.dumps(ans,ensure_ascii=False);




if __name__ == '__main__':
    app.run(host="localhost",debug=True)