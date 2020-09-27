<?php
    $servername = "localhost";
    $username = "root";
    $password = "";
    $database = "project";

      $con = mysqli_connect($servername,$username,$password,$database);
      if (mysqli_connect_errno()){
      echo 'connect faild';}
      else{
        echo 'connect succes';
      }
  global $card;
  if(isset($_GET['card'])){
    $card=$_GET['card'];
  }
  date_default_timezone_set('Asia/Taipei');
  $date= date("Y/m/d H:i:s");
  $sql="INSERT INTO rfid(id, datetime) VALUES('$card', '$date')";
  mysqli_query($con, $sql);
  mysqli_close($con);
  ?>
