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
  global $dust;
  if(isset($_GET['reading'])){
    $dust=$_GET['reading'];
  }
  date_default_timezone_set('Asia/Taipei');
  $date= date("Y/m/d H:i:s");
  $sql="INSERT INTO air(dust, datetime) VALUES('$dust', '$date')";
  mysqli_query($con, $sql);
  mysqli_close($con);
  ?>
