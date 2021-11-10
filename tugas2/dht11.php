<?php
class dht11{
public $link='';
function __construct($temperature, $humidity){
    $this ->connect();
    $this ->storeInDB($temperature,$humidity);
}

function connect(){
    $this-> link = mysql_connect('localhost','root','') or die ('cannot connect to the DB');
    mysqli_select_db($this->link,'dht') or die ('cannot select the DB');
}

function storeInDB($temperature, $humidity){
    $query = "insert into dht11 set humidity='".$humidity."', temperature='".$temperature."',
    $result = mysqli_query($this->link,$query) or die('errant query: '.$query);

}

if($_GET['temperature']!='' and $_GET['humidity'] != ''){
    $dht11=new dht11($_GET['temperature'],$_GET['humidity']);
}
?>