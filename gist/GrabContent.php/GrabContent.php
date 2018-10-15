<?
function curl_get_contents($url,$timeout=1) 
{ 
	$curlHandle = curl_init(); 
	curl_setopt( $curlHandle , CURLOPT_URL, $url ); 
	curl_setopt( $curlHandle , CURLOPT_RETURNTRANSFER, 1 ); 
	curl_setopt( $curlHandle , CURLOPT_TIMEOUT, $timeout ); 
	$result = curl_exec( $curlHandle ); 
	curl_close( $curlHandle ); 
	return $result; 
} 

function reg_cut($content,$front,$back)
{
	$content=preg_replace ('([\\s\\S]*)'.$front,"",$content);
	return preg_replace ($back.'([\\s\\S]*)',"",$content);
}

$content = curl_get_contents('http://');
$areicle=reg_cut($content,"","");