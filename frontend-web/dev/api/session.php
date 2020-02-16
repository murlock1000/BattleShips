<?php
session_start();

$response = [
  'errorType' => '',
  'errorText' => '',
  'isLoggedIn' => '',
  'userData' => '',
];

require_once 'db.local.php';

ob_start();
$conn = new mysqli('localhost', $DB['user'], $DB['password'], $DB['dbname']);
ob_end_clean();
if ($conn->connect_error) {
  $response['errorType'] = 'db';
  $response['errorText'] = $conn->connect_error;
  die(json_encode($response));
}

if (empty($_SESSION['userID'])) {
  $response['isLoggedIn'] = false;
  exit(json_encode($response));
}

$userID = $_SESSION['userID'];

$res = $conn->query("SELECT username,wins,losses FROM users WHERE userID = $userID");

if ($res->num_rows) {
  $row = $res->fetch_assoc();
  $response['userData'] = [
    'userID' => $userID,
    'username' => $row['username'],
    'wins' => intval($row['wins']),
    'losses' => intval($row['losses']),
  ];
}

$conn->close();

$response['isLoggedIn'] = true;

echo json_encode($response);
?>