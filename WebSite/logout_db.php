<?php
// Start the session
session_start();

// Destroy the session.
if(session_destroy()){
  session_commit();
  exit('success');
}else{  
  exit('failed');
}
?>