<?php
session_start();
if (empty($_SESSION)) exit();
session_destroy();
?>