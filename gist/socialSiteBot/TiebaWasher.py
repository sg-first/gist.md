import requests
import re
import time
import random
import urllib
import sys
from bs4 import BeautifulSoup
from settings import *
import threading

#一般情况下，以下内容不需要修改
baiduUrl = 'http://wapp.baidu.com/mo/q----,sz@320_240-1-3---2'
loginUrl = 'http://wappass.baidu.com/passport/login'
tiebaUrl = 'http://tieba.baidu.com/mo/q-3e541558ae919cfb4eb8882d6548e1e8.3.1409068854.1.jJChlKUjE0MD--41FDC07CB1D1617DD4B1F9044CDD0B4C%3AFG%3D1-sz%40320_240%2C-1-3-0--2--wapp_1409068597987_564'
#一些或许需要修改的
timeInterval = 30 #刷新贴吧的时间间隔（秒）

def login_check(content):
    """
    通过查找页面中是否包含本帐号的百度ID来验证是否登录成功
    :param content: 页面HTML内容
    :return:
    """
	global baiduID
    if content.find(baiduID.encode("utf8")) >= 0:
        return True
    return False


def print_delimiter(): #打印分隔符
    print '-' * 80


def login(): #登录
    global session
    try:
        session = requests.session()
        session.get(baiduUrl)#打开一次百度首页，获取相关cookies
        page = session.post(loginUrl, data=loginData, headers={'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/36.0.1985.125 Safari/537.36'})
    except:
        print "Login Error"
        return False

    if not login_check(page.content):
        print 'Login failed'
        return False
    else:
        print 'Login successful!'
        return True


def get_response(): #构造回复帖子时用到的postData
	global replyWords
    global subject_soup
    data = {'sub1': u'回帖', 'co': replyWords}
    #找出div标签，class为d h的部分，这些都是回帖表单需要的元素
    subjects = subject_soup.find_all(name="div", attrs={"class": "d h"})
    hidden_elems = subjects[0].find_all(name="input", attrs={"type": "hidden"})
    for elem in hidden_elems:
        data[elem['name']] = elem['value']
    reply_request = session.post(tiebaUrl+"/submit", data=data)
    #print reply_request.content
    subject_main_content = subject_soup.find(name="div", attrs={"class": "bc p"}).strong.text
    if reply_request.status_code == 200:
        print "[%s]Reply successful!: name：%s" % (time.asctime(), subject_main_content.encode("utf8"))
    else:
        print "[%s]Reply failed!: name：%s" % (time.asctime(), subject_main_content.encode("utf8"))
                    

def get_subject(subject_url): #获取主题内容并且将一段参数设为全局变量
    sub_page = session.get(subject_url)    
    global subject_soup
    subject_soup = BeautifulSoup(sub_page.content)
    subject_main_content = subject_soup.find(name="div", attrs={"class": "bc p"}).strong.text
    subject_main_content += subject_soup.find(name="div", attrs={"class": "i"}).text
    return subject_main_content


def washer(): #抢二楼
	global tiebaName
    #打开指定贴吧页面，遍历找出回复数为0的主题贴，打开它并回复该主题贴
    tieba_page = session.get(tiebaUrl+("/m?kw=%s&lp=1030" % urllib.quote(tiebaName.encode("utf8"))))
    soup = BeautifulSoup(tieba_page.content)

    #找出div标签，class为i的部分，即为主题贴的信息
    subjects = soup.find_all(name="div", attrs={"class": "i"})
    if not len(subjects):
        print "[%s]There is nothing to do." % time.asctime()
        return

    for sub in subjects:
        if sub.p.find(text=re.compile(u"回0")): #如果找到回复数为0的帖子
            print "[%s]open subject:%s" % (time.asctime(), tiebaUrl+"/"+sub.a['href'])
            _link = tiebaUrl+"/"+sub.a['href']
			get_subject(_link)
            get_response()


def sign_in_all():#签到
    favorite = session.get(tiebaUrl+"/m?tn=bdFBW&tab=favorite")
    soup = BeautifulSoup(favorite.content)
    tieba_list = soup.find_all(name="tr")
    for tieba in tieba_list:
        sign_in(tiebaUrl+"/"+tieba.td.a["href"])
    print u"签到完成！"


def sign_in(url):
    page = session.get(url)
    soup = BeautifulSoup(page.content)
    sign_in_div = soup.find(name="td", attrs={"style": "text-align:right;"})
    if sign_in_div and sign_in_div.a and sign_in_div.a.get("href"):
        sign_in_url = sign_in_div.a["href"]
        session.get("http://tieba.baidu.com"+sign_in_url)
        print "[ok]:%s" % "http://tieba.baidu.com"+sign_in_url

		
def Start():#开始抢二楼
    if login():
        sign_in_all()#登录成功就签到
        print_delimiter()
       
        while True:#然后开始抢二楼
            try:
                washer()
            except Exception as e:
                print "[%s]wash Error!" % time.asctime()
                print str(e)
            print_delimiter()
            time.sleep(random.randint(timeInterval, timeInterval+10))


def SetParameters(username,password,tiebaname,replywords,baiduid):
	loginData = {
		'username': username,
		'password': password,
		'submit': u'登录',
		'quick_user': '0',
		'isphone': '0',
		'sp_login': 'waprate',
		'loginmerge': '1',
		'u': 'http%3A%2F%2Ftieba.baidu.com%2Fmo%2Fq-000000--C05ACC38DA3E0BC7458BA9A6B9E3D705%3AFG%3D1-sz%401321_1003%2C-1-3-0--1--wapp_1407166803555_355%2Fm%3Ftn%3DbdIndex%26lp%3D5014%26pinf%3D1_2_0__%40%25E6%25B9%2596%25E5%258D%2597%25E7%25A7%2591%25E6%258A%2580%25E5%25A4%25A7%25E5%25AD%25A6%26uid%3DC05ACC38DA3E0BC7458BA9A6B9E3D705%253AFG%253D1',
		'skin': 'default_v2',
		'tpl': 'tb',
		'pu': 'sz@1321_1003,',
		'tn': 'bdIndex',
		'bd_page_type': '1',
	}
	global tiebaName
	tiebaName=tiebaname
	global replyWords
	replyWords=replywords
	global baiduID
	baiduID=baiduid
	
#想要开始，先调用SetParameters再调用Start