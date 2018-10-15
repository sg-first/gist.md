import json
from tornadohttpclient import TornadoHTTPClient


class SimSimiTalk(object):
    """ 模拟浏览器与SimSimi交流
    :params http: HTTP 客户端实例
    :type http: ~tornadhttpclient.TornadoHTTPClient instance
    """
    def __init__(self, http = None):
        self.http = http or TornadoHTTPClient()

        if not http:
            self.http.set_user_agent("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Ubuntu Chromium/28.0.1500.71 Chrome/28.0.1500.71 Safari/537.36")
            self.http.debug = True
            self.http.validate_cert = False
            self.http.set_global_headers({"Accept-Charset": "UTF-8,*;q=0.5"})

        self.url = "http://www.simsimi.com/func/reqN"
        self.params = {"lc": "zh", "ft": 0.0, "fl": "http://www.simsimi.com/talk.htm"}
        self.ready = False
        self.fetch_kwargs = {}
        self._setup_cookie()

    def _setup_cookie(self):
        self.http.get("http://www.simsimi.com", callback=self._set_profile)

    def _set_profile(self, resp):
        def callback(resp):
            self.ready = True
		params = {"name": "PBot", "uid": "52125598"}
		headers = {"Referer": "http://www.simsimi.com/set_profile_frameview.htm",
				   "Accept": "application/json, text/javascript, */*; q=0.01",
				   "Accept-Language": "zh-cn,en_us;q=0.7,en;q=0.3",
				   "Content-Type": "application/json; charset=utf-8",
				   "X-Requested-With": "XMLHttpRequest",
				   "Cookie": "simsimi_uid=52125598"}
		self.http.post("http://www.simsimi.com/func/setProfile", params,headers=headers, callback=callback)

    def talk(self, msg, callback):
        """ 聊天
        :param msg: 信息
        :param callback: 接收响应的回调
        """
        headers = {"Referer": "http://www.simsimi.com/talk_frameview.htm",
                   "Accept": "application/json, text/javascript, */*; q=0.01",
                   "Accept-Language": "zh-cn,en_us;q=0.7,en;q=0.3",
                   "Content-Type": "application/json; charset=utf-8",
                   "X-Requested-With": "XMLHttpRequest",
                   "Cookie": "simsimi_uid=52125598"}
        if not msg.strip():
            return callback(u"小的在")
        params = {"req": msg.encode("utf-8")}
        params.update(self.params)

        def _talk(resp):
            data = {}
            if resp.body:
                try:
                    data = json.loads(resp.body)
                except ValueError:
                    pass
            print resp.body
            callback(data.get("sentence_resp", "Server respond nothing!"))

        self.http.get(self.url, params, headers=headers,callback=_talk)


import threading, time
simsimi = SimSimiTalk() #定义机器人对象

def callback(response): #完成的时候调用这个把回复显示出来
    print response
    simsimi.http.stop()#关掉http连接

def talk(): #用于多线程获取回复
    while 1:
        if simsimi.ready:
            simsimi.talk(u"你是？", callback) #获取聊天内容回复，获取到就调用callback
            break
        else:
            time.sleep(1)

t = threading.Thread(target=talk) #多线程启动上面定义的talk函数
t.setDaemon(True)
t.start() #开启线程
simsimi.http.start()