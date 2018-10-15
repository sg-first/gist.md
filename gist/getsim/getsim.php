<?php

function getsim($content)
{
	$content = str_replace(' ', '', $content);
    $ch = curl_init();
	curl_setopt($ch,CURLOPT_URL,'http://www.simsimi.com/talk.htm');
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt($ch, CURLOPT_HEADER,1);
	curl_setopt($ch, CURLOPT_NOBODY, false);
	$rs = curl_exec($ch);
	preg_match_all('/Set-Cookie: (.+)=(.+)$/m', $rs, $regs);
	foreach($regs[1] as $i=>$k);
	$cc=str_replace(' Path','' ,$k);
	$cc='simsimi_uid=507454034223;'.$cc;
	$re = HTTPClient('http://www.simsimi.com/func/reqN?lc=ch&ft=1.0&req='.$content.'&fl=http%3A%2F%2Fwww.simsimi.com%2Ftalk.htm',$cc);
	$re = json_decode($re,true);
	return $re['sentence_resp'];
}