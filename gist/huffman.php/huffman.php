<?php
 
/**
* huffmanTreeNode类
*/
class huffmanTreeNode{
     
    private $weight;
    private $flag;
    private $parent,$lchild,$rchild;
 
    function __construct($weight=NULL){
        $this->weight=$weight;
        $this->flag=0;
        $this->parent=$this->lchild=$this->rchild=NULL;
    }
 
    function getFlag(){
        return $this->flag;
    }
 
    function getParent(){
        return $this->parent;
    }
 
    function getLchild(){
        return $this->lchild;
    }
 
    function getRchild(){
        return $this->rchild;
    }
 
    function getWeight(){
        return $this->weight;
    }
 
    function setFlag($flag){
        $this->flag=$flag;
    }
 
    function setWeight($weight){
        $this->weight=$weight;
    }
 
    function setParent($parent){
        $this->parent=$parent;
    }
 
    function setLchild($lchild){
        $this->lchild=$lchild;
    }
 
    function setRchild($rchild){
        $this->rchild=$rchild;
    }
 
}
 
/**
* huffman tree
*/
class huffmanTree{
     
    //求哈夫曼编码算法,$W存放N个字符的权值
    function huffmanCoding($w){
        $n=count($w); //字符个数
        $m=2*$n; //哈夫曼树的结点数
        $HN[]= new huffmanTreeNode($m);
         
        //构造N个具有权值的结点
        for($i=0;$i<$n;$i++){
            $HN[$i]=new huffmanTreeNode($w[$i]);
        }
 
        //构建哈夫曼树
        for($i=$n;$i<$m;$i++){
            $min1=$this->selectMin($HN,$i-1);
            $min1->setFlag(1);
            $min2=$this->selectMin($HN,$i-1);
            $min2->setFlag(1);
 
            //构造min1个min2的父结点,并修改父结点的权值
            $HN[$i]=new huffmanTreeNode();
            $min1->setParent($HN[$i]);
            $min2->setParent($HN[$i]);
            $HN[$i]->setLchild($min1);
            $HN[$i]->setRchild($min2);
            $HN[$i]->setWeight($min1->getWeight()+$min2->getWeight());
        }
 
        //由叶子到根逆向求每个字符的哈夫曼编码
        $huffCode=array();
        for($j=0;$j<$n;$j++){
            $start=$n-1; //编码的开始位置,初始化为数组的结尾
            for($c=$HN[$j],$p=$c->getParent();$p!=NULL;$c=$p,$p=$p->getParent()){
                //从叶子到根逆向求编码
                if($p->getLchild()===$c){ //左孩子为0
                    $huffCode[$j][$start--]=0;
                }else{
                    //右孩子编码为1
                    $huffCode[$j][$start--]=1;
                }
            }
            $huffCode[$j][$start]=-1; //编码的开始标志为-1,编码为-1之后的0,1序列
        }
        return $huffCode;
    }
 
    //在HN[0..i-1]中选择不在哈夫曼树中且weight最小的结点
    function selectMin($HN,$end){
        $min=$HN[$end];
        for($i=0;$i<=$end;$i++){
            $h=$HN[$i];
            if($h->getFlag==0 && $h->getWeight()<$min->getWeight()){
                $min=$h;
            }
        }
        return $min;
    }
}
 
$w=[23,11,5,3,29,14,7,8];
$ht=new huffmanTree();
$HN=$ht->huffmanCoding($w);
 
print_r($HN);
 
for($i=0;$i<count($HN);$i++){
    print($w[$i]."    ");
    for($j=0;$j<$HN[$j];$j++){
        if($HN[$i][$j]==-1){
            for($k=$j+1;$k<count($HN[$j]);$k++){
                print($HN[$i][$k]);
            }
            break;
        }
    }
    print("<br>");
}