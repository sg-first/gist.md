<?php

$qq = '';//QQ号
$sid = '';//填写sid的值
$con = '';//自定义内容
$qzone = new qzone($qq,$sid);

class qzone
{
	private $sid ='';
	
	public function __construct($qq,$sid)
	{
		$this->sid = $sid;
		$url = "http://ish.z.qq.com/infocenter_v2.jsp?B_UID={$qq}&sid={$sid}&g_ut=2";
		$re = $this->fetch($url);
		$this->getsaying($re);
	}
	
	private function fetch($url,$postdata=null)
	{
		$ch = curl_init();
        curl_setopt($ch, CURLOPT_URL,$url);
        curl_setopt($ch, CURLOPT_USERAGENT, "Mozilla/5.0 (compatible; MSIE 10.0; Windows Phone 8.0; Trident/6.0; IEMobile/10.0; ARM; Touch; NOKIA; Lumia 820)");
        if($postdata!=null) curl_setopt($ch, CURLOPT_POSTFIELDS,$postdata);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($ch, CURLOPT_TIMEOUT, 20);
        $re = curl_exec($ch);
        curl_close($ch);
        return $re;
	}
	
	private function getsaying($html)
	{
		preg_match_all('/<\/a>:(.*?)评论\(\d\)<\/a>/',$html,$match);
		foreach($match[0] as $k)
		{
			if(strstr($k,'评论(0)'))//没有评论就评论
			{
                $k = str_replace(PHP_EOL, '', $k);
                $k = str_replace('&#10;', '', $k);
                $k = html_entity_decode($k);
				preg_match('/<\/a>:(.*?)<span class="txt-fade">/',$k,$content);
				preg_match('/myfeed_mood.jsp\?sid=.*&B_(.*?)&t1_source/',$k,$data);
				$content = preg_replace('/<img[^>]+>/', '', $content[1]);
                echo '找到一条说说：'.$content.' 机器人的回复是：';
				$data = 'B_'.$data[1];
				$content = $con;//这块可以自己改成别的，比如结合这个https://code.csdn.net/snippets/1285440
                echo $content.'<br />';
                $this->postcomment($content,$data);//post上去
				sleep(3);
			}
		}
	}
	
	private function postcomment($content,$data)
	{
        $postdata = "content={$content}&{$data}&t1_source=1&feedcenter_pn=1&flag=1&type=all&channel=0&back=false&offset=0&ic=false&dl=null&to_tweet=0&submit=%E8%AF%84%E8%AE%BA";
        $this->fetch("http://blog30.z.qq.com/mood/mood_reply.jsp?sid={$this->sid}&g_ut=2",$postdata);	
	}
}