<?
/* 数据表：
CREATE TABLE acghub_msg(
    `id` int(11) NOT NULL AUTO_INCREMENT,
    `_from` int(11) NOT NULL,
    `_to` int(11) NOT NULL,
    `content` TEXT NOT NULL,
    `datetime` TEXT NOT NULL,
    `isread` int(11) NOT NULL,
    PRIMARY KEY (id)
    )DEFAULT CHARSET=utf8;
*/

class MsgController{ //私信类
 
  function __construct(){
    date_default_timezone_set('Etc/GMT-8');
    mysql_select_db("acghub_msg");
  }
 
  function SubmitToSever($method,$sql){
    //method=1->insert/update method=2->select
    $msginfo=array();
    $msginfo_cnt=0;
    switch ($method) {
      case 1:
        $res=mysql_query($sql);
        echo mysql_error();
        if($res!=false){
          if(mysql_affected_rows()!=-1){
            return true;
          }else{return false;}
        }else{return false;}
        break;
      case 2:
        $res=mysql_query($sql);
        if($res!=false){
          while($row=mysql_fetch_row($res)){
            $msginfo[$msginfo_cnt]=$row;
            $msginfo_cnt++;
          }
          return $msginfo;
        }else{return false;}
        break;
      default:
        return false;
        break;
    }
 
  }
 
  function SendTo($from,$to,$content){
    $sql="INSERT INTO `acghub_msg`
    (`_from`, `_to`, `content`, `datetime`, `isread`) 
    VALUES ($from,$to,'$content','".date("Y-m-d H:i:s")."',0)";
    return $this->SubmitToSever(1,$sql);
  }
 
  function ReceiveFrom($from,$to){
    $sql="SELECT  `content`, `datetime`, `isread` FROM `acghub_msg`
     WHERE `_from`=$from and `_to`=$to";
     return $this->SubmitToSever(2,$sql);
  }
 
  function ReceiveTo($from,$to){
    $sql="SELECT  `content`, `datetime`, `isread` FROM `acghub_msg`
     WHERE `_from`=$to and `_to`=$from";
     return $this->SubmitToSever(2,$sql);    
  }
 
  function isRead($from,$to,$content){
    //1->已读 0->未读
    $sql="SELECT `isread` FROM `acghub_msg` 
    WHERE `_from`=$from  and `_to`=$to and  `content`='$content'";
    $looked=$this->SubmitToSever(2,$sql);
    if($looked==1){return true;}else{return false;}
  }
 
  function ReadMarking($from,$to,$content){
    $sql="UPDATE `acghub_msg` SET `isread`=1 
    WHERE `_from`=$from and `_to`=$to and `content`='$content'";
    return $this->SubmitToSever(1,$sql);
  }
 
  function GetFrom($to){
    $fromID=array();
    $sql="SELECT DISTINCT  `_from` FROM `acghub_msg` WHERE `_to`=$to";
    $fromQueue=$this->SubmitToSever(2,$sql);
    foreach ($fromQueue as $key => $value) {
      $fromID[$key]=$value[0];
    }
    return $fromID;
  }
 
  function GetLastMsg($from,$to){
    $msgqueue=$this->ReceiveFrom($from,$to);
    $msgcount=count($msgqueue);
    $lastmsg=$msgqueue[$msgcount-1];
    return $lastmsg;
  }
 
  function GetTo($from){
    //通过from查找from和谁私信过
    $ToID=array();
    $sql="SELECT DISTINCT  `_to` FROM `acghub_msg` WHERE `_from`=$from";
    $ToQueue=$this->SubmitToSever(2,$sql);
    foreach ($ToQueue as $key => $value){
      $ToID[$key]=$value[0];
    }
    return $ToID;
  }
 
  function GetContent($method=0,$from,$to){
    //method=0->Sender //method=1->Receiver
    switch ($method){
      case 1:
        $MessageQueue=$this->ReceiveTo($from,$to);
        break;
      case 0:
        $MessageQueue=$this->ReceiveFrom($from,$to);
        break;
      default:
        return false;
        break;
    }
    $MsgContent=array();
    foreach ($MessageQueue as $key => $value) {
      $MsgContent[$key]=$value[0];
    }
    return $MsgContent;
  }
 
  function GetDateTime($method=0,$from,$to){
    //method=0->Sender //method=1->Receiver
    switch ($method){
      case 1:
        $MessageQueue=$this->ReceiveTo($from,$to);
        break;
      case 0:
        $MessageQueue=$this->ReceiveFrom($from,$to);
        break;
      default:
        return false;
        break;
    }    
    $MsgContent=array();
    foreach ($MessageQueue as $key => $value) {
      $MsgContent[$key]=$value[1];
    }
    return $MsgContent;
  }
 
}