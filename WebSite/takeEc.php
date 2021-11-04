<?php
  include("connection.php");

  if(!empty($_POST)){
    $tds = $_POST["Ec"];
    $dataSend = substr($_POST["Date"], 0, 10);
    $ec = $tds * 1.56;
    $queryEC = "INSERT INTO ec_tab (ec, data_send)
            VALUES ('".$ec."', '".$dataSend."')";
    if ($conn->query($queryEC) === TRUE) {
      echo "OK";
    } else {
      echo "Error: " . $sql . "<br>" . $conn->error;
    }
    $query = "INSERT INTO tds_tab (tds, data_send)
            VALUES ('".$tds."', '".$dataSend."')";
    if ($conn->query($query) === TRUE) {
      echo "OK";
    } else {
      echo "Error: " . $sql . "<br>" . $conn->error;
    }
  }

?>