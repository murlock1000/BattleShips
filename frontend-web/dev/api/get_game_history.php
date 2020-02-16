<?php
function predump($var) {
  echo '<pre>';
  var_dump($var);
  echo '</pre>';
}

$request = json_decode(file_get_contents('php://input'));

$gameID = $request->id;

require_once 'db.local.php';

$conn = new mysqli('localhost', $DB['user'], $DB['password'], $DB['dbname']);

if ($conn->connect_error) {
  die($conn->connect_error);
}

$res = $conn->query("SELECT map1,map2 FROM history WHERE gameID = $gameID");
$row = $res->fetch_assoc();

$map1 = $row['map1'];
$map2 = $row['map2'];
$moves = [];
$i = 0;

$res = $conn->query("SELECT move_pos,move_res,playerID FROM moves WHERE gameID = $gameID");
while ($row = $res->fetch_assoc()) {
  $moves[$i++] = [
    'pos' => $row['move_pos'],
    'res' => $row['move_res'],
    'pid' => $row['playerID'],
  ];
}

$conn->close();

$response = [
  'map1' => $map1,
  'map2' => $map2,
  'moves' => $moves,
];

echo json_encode($response);
?>