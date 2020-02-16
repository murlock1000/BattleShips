<?php
if ($_SERVER['REQUEST_METHOD'] != 'POST') die('Invalid request.');

$request = json_decode(file_get_contents('php://input'));

if (json_last_error() != JSON_ERROR_NONE) die('Invalid request body.');

$response = [
  'userData' => [],
  'errorType' => '',
  'errorText' => '',
];

$username = $request->username;

if (empty($username)) die('Invalid data.');

require_once 'db.local.php';

ob_start();
$conn = new mysqli('localhost', $DB['user'], $DB['password'], $DB['dbname']);
ob_end_clean();
if ($conn->connect_error) {
  $response['errorType'] = 'db';
  $response['errorText'] = $conn->connect_error;
  die(json_encode($response));
}

$query = $conn->prepare("SELECT userID,wins,losses FROM users WHERE username = ?");
$query->bind_param('s', $username);
$query->execute();
$res = $query->get_result();

$query->close();
$conn->close();

if ($res->num_rows) {
  $row = $res->fetch_assoc();
  $response['userData'] = [
    'userID' => $row['userID'],
    'username' => $username,
    'wins' => intval($row['wins']),
    'losses' => intval($row['losses']),
  ];
  session_start();
  $_SESSION['userID'] = $response['userData']['userID'];
  echo json_encode($response);
} else {
  $response['errorType'] = 'nouser';
  echo json_encode($response);
}
?>