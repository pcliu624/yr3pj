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

      global $celsius,$humidity,$fahrenheit,$date;
    if(isset($_GET['humidity'])){
      $humidity=$_GET['humidity'];
    }
    if(isset($_GET['fahrenheit'])){
      $fahrenheit=$_GET['fahrenheit'];
    }
    if(isset($_GET['celsius'])){
      $celsius=$_GET['celsius'];
    }
    date_default_timezone_set('Asia/Taipei');
    $date= date("Y/m/d H:i:s");


    $sql1="INSERT INTO celsius(celsius,datetime) VALUES('$celsius','$date')";
    $sql2="INSERT INTO humidity(humidity,datetime) VALUES('$humidity','$date')";
    $sql3="INSERT INTO fahrenheit(fahrenheit,datetime) VALUES('$fahrenheit','$date')";
    mysqli_query($con, $sql1);
    mysqli_query($con, $sql2);
    mysqli_query($con, $sql3);
    mysqli_close($con);
    

    ?>
