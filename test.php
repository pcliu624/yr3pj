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
?>

    <html>

    <head>
        <title>FusionCharts | Multi-Series Chart using PHP and MySQL</title>
        <script src="http://static.fusioncharts.com/code/latest/fusioncharts.js"></script>
        <script src="http://static.fusioncharts.com/code/latest/fusioncharts.charts.js"></script>
        <script src="http://static.fusioncharts.com/code/latest/themes/fusioncharts.theme.ocean.js"></script>
    </head>

    <body>

        <?php
  $strQuery = "SELECT * FROM(SELECT * FROM celsius ORDER BY datetime DESC LIMIT 10) AS c ORDER BY datetime ASC; ";
  $result = $dbhandle->query($strQuery) or exit("Error code ({$dbhandle->errno}): {$dbhandle->error}");
  if ($result) {

  $arrData = array(
        "chart" => array(
            "caption"=> "TEMP",

            "xAxisname"=> "time",
            "yAxisName"=> "celsius",
            "numberPosfix"=> "*c",
            "theme"=> "ocean"
            )
          );
          // creating array for categories object
          $categoryArray=array();
          $dataseries1=array();

          // pushing category array values
          while($row = mysqli_fetch_array($result)) {
            array_push($categoryArray, array(
            "label" => $row["datetime"]
          )
        );
        array_push($dataseries1, array(
          "value" => $row["celsius"]
          )
      );
      }
      $arrData["categories"]=array(array("category"=>$categoryArray));
      // creating dataset object
      $arrData["dataset"] = array(array("seriesName"=> "datetime", "data"=>$dataseries1, "renderas"=>"line","showvalues"=>"0"));
      /*JSON Encode the data to retrieve the string containing the JSON representation of the data in the array. */
      $jsonEncodedData = json_encode($arrData);
      // chart object
      $msChart = new FusionCharts("mscombi2d", "chart1" , "600", "350", "chart-container", "json", $jsonEncodedData);
      // Render the chart
      $msChart->render();
      // closing db connection
      $dbhandle->close();
   }
?>

            <center>
                <div id="chart-container">Chart will render here!</div>
            </center>
    </body>

    </html>
