<?php 
include('connection.php');
$k = $_POST['potassio'];
$k = !empty($k) ? "'$k'" : "NULL";
$mg = $_POST['magnesio'];
$mg = !empty($mg) ? "'$mg'" : "NULL";
$fe = $_POST['ferro'];
$fe = !empty($fe) ? "'$fe'" : "NULL";
$rinverdente = $_POST['rinverdente'];
$rinverdente = !empty($rinverdente) ? "'$rinverdente'" : "NULL";
$npk = $_POST['npk'];
$npk = !empty($npk) ? "'$npk'" : "NULL";

$sql = "INSERT INTO `my_myfishtank`.`fertilization_tab` (`k`, `mg`, `fe`, `rinverdente`, `npk`) VALUES ($k, $mg, $fe, $rinverdente, $npk)";
//$sql = "INSERT INTO fertilization_tab (k,mg, fe, rinverdente, npk) VALUES ($k, $mg, $fe, $rinverdente, $npk)";
$query= mysqli_query($con, $sql);
$lastId = mysqli_insert_id($con);
if($query == true){
    $data = array('status'=>'true', );
    echo json_encode($data);
}else{
     $data = array('status'=>'false',);
    echo json_encode($data);
} 
   $con->close();  
?>