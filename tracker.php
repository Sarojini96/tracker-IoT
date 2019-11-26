<?php
class gpsdata{
 public $link='';
 function __construct($userIP, $latitude, $longitude){
  $this->connect();
  $this->storeInDB($userIP, $latitude, $longitude);
 }
 
 function connect(){
  $this->link = mysqli_connect('localhost','admin','PASSWORD*','DBNAME') or die('Cannot connect to the DB');
  //mysqli_select_db($this->link,'<temphumidnew>') or die('Cannot select the DB');
 }
 
 function storeInDB($userIP, $latitude, $longitude){
  $query = "insert into gpsdata set userIP='".$userIP."', latitude='".$latitude."',longitude='".$longitude."'";
  $result = mysqli_query($this->link,$query) or die('Errant query:  '.$query);
 }
 
}
if($_GET['userIP'] != ''and $_GET['latitude'] != '' and $_GET['longitude'] != ''){
 $gpsdata=new gpsdata($_GET['userIP'],$_GET['latitude'],$_GET['longitude']);
}
?>
