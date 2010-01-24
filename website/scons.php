<?php
ob_start();
echo "Redirect...";
header("Location: compile.php");
ob_flush();
?> 
