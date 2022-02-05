#coding=utf-8
# GetData_1.py

import requests
from bs4 import BeautifulSoup
import bs4
import re
from time import sleep

def getUrls(url):
    req = requests.get(url)
    req.encoding = 'utf-8'
    if (req == None):
        return
    bf = BeautifulSoup(req.text, 'lxml')
    div_bf = bf.find('div', attrs={'class': 'content_ny'})
    if isinstance(div_bf, bs4.element.Tag):
        div_a = div_bf.find_all('div', attrs={'class': 'list-li'})
        urltxt = open(b'./data/url.txt', 'a')

        for div in div_a:
            title = div.find('a').contents[0]
            link = div.find('a').get("href")
            if('五老' in title):
                print(title + ';' + 'http://www.zgggw.gov.cn/gedixinxi' + link)
                urltxt.write(title + ';' + 'http://www.zgggw.gov.cn/gedixinxi' + link + '\n')

        urltxt.close()


for index in range(2,338):
    sleep(2)
    url = "http://www.zgggw.gov.cn/gedixinxi/index_" + str(index) + '.html'
    print(url)
    getUrls(url)

