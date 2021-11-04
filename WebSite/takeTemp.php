<?php
  include("connection.php");

  if(!empty($_POST)){                  //($_GET)){
    $temperature = $_POST["Temp"];     // $_GET["Temp"];
    $dataSend = substr($_POST["Date"], 0, 10);
    $query = "INSERT INTO temp_tab (temperature, data_send)
            VALUES ('".$temperature."', '".$dataSend."')";
    if ($conn->query($query) === TRUE) {
      echo $temperature  . " " . $dataSend;
    } else {
      echo "Error: " . $sql . "<br>" . $conn->error;
    }
  }

?>