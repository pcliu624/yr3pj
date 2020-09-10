<?php
/* Include the `../src/fusioncharts.php` file that contains functions to embed the charts.*/
include("wrappers 2/php-wrapper/fusioncharts.php");
/* The following 4 code lines contains the database connection information. Alternatively, you can move these code lines to a separate file and include the file here. You can also modify this code based on your database connection.   */
$hostdb = "localhost";  // MySQl host
$userdb = "root";  // MySQL username
$passdb = "58565856";  // MySQL password
$namedb = "project";  // MySQL database name
// Establish a connection to the database
$dbhandle = new mysqli($hostdb, $userdb, $passdb, $namedb);
/*Render an error message, to avoid abrupt failure, if the database connection parameters are incorrect */
if ($dbhandle->connect_error) {
  exit("There was an error with your connection: ".$dbhandle->connect_error);
}

$sql = "SELECT light FROM(SELECT * FROM light ORDER BY datetime DESC LIMIT 1) AS c ORDER BY datetime ASC;";


  $data = $dbhandle->query($sql);
if($data->num_rows>0){
  while($row = $data->fetch_assoc()) {
    $output=$row["light"];
  print(json_encode($output));
}
}

mysqli_close($dbhandle);
?>
