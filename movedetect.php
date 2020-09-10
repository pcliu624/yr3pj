<?php
      $servername = "localhost";
      $username = "root";
      $password = "58565856";
      $database = "project";
    try{
      $conn = new PDO("mysql:host=$servername;dbname=$database", $username, $password);
      $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
      echo "connect success";
      }catch(PDOException $e){
        echo "connection failed:" .$e->getMessage();
      }
      global $ismotion;
      if(isset($_GET["ismotion"])){
      $ismotion=$_GET["ismotion"];
      }
      $sql="INSERT INTO 'sensor'(detectmove)
      VALUES($ismotion)";?>
