<?php
  include("connection.php");

  if(!empty($_POST)){
    $ph = $_POST["Ph"];
    $dataSend = substr($_POST["Date"], 0, 6);
    $query = "INSERT INTO ph_tab (ph, dataSend)
            VALUES ('".$ph."', '".$dataSend."')";
    if ($conn->query($query) === TRUE) {
      echo "OK";
    } else {
      echo "Error: " . $sql . "<br>" . $conn->error;
    }
  }

?>