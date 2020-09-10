<?php
    $servername = "localhost";
    $username = "root";
    $password = "58565856";
    $database = "project";

      $con = mysqli_connect($servername,$username,$password,$database);
      if (mysqli_connect_errno()){
      echo 'connect faild';}
      else{
        echo 'connect succes';
      }
  global $light;
  if(isset($_GET['light'])){
    $light=$_GET['light'];
  }
  date_default_timezone_set('Asia/Taipei');
  $date= date("Y/m/d H:i:s");
  $sql="INSERT INTO light(light, datetime) VALUES('$light', '$date')";
  mysqli_query($con, $sql);
  mysqli_close($con);
  ?>
