<?php

require_once 'db.local.php';

$conn = new mysqli('localhost', $DB['user'], $DB['password'], $DB['dbname']);

if ($conn->connect_error) {
  die($conn->connect_error);
}

$games = [];
$response = [];
$gameCount = 0;
$i = 0;

$res = $conn->query("SELECT gameID,game_name,player1_ID,player2_ID,winnerID FROM history");

if ($gameCount = $res->num_rows) {
  while ($row = $res->fetch_assoc()) {
    $games[$i] = [
      'id' => $row['gameID'],
      'name' => $row['game_name'],
      'ID1' => $row['player1_ID'],
      'ID2' => $row['player2_ID'],
      'wID' => $row['winnerID'],
    ];
    $response[$i] = [];
    ++$i;
  }
}

$prep = $conn->prepare("SELECT username,is_ai FROM users WHERE userID = ?");
$prep->bind_param('i', $userID);
$i = $gameCount - 1;

foreach ($games as $game) {
  $userID = $game['ID1'];
  $prep->execute();
  $res = $prep->get_result();
  $row = $res->fetch_assoc();
  $p1 = $row['username'];
  $is_ai_1 = boolval($row['is_ai']);
  $userID = $game['ID2'];
  $prep->execute();
  $res = $prep->get_result();
  $row = $res->fetch_assoc();
  $p2 = $row['username'];
  $is_ai_2 = boolval($row['is_ai']);
  $winner = $game['wID'] == $game['ID1'] ? $p1 : $p2;
  $response[$i--] = [
    'id' => $game['id'],
    'name' => $game['name'],
    'pid1' => $game['ID1'],
    'player1' => $p1,
    'is_ai_1' => $is_ai_1,
    'pid2' => $game['ID2'],
    'player2' => $p2,
    'is_ai_2' => $is_ai_2,
    'winner' => $winner,
  ];
}

$prep->close();
$conn->close();

echo json_encode($response);

// echo '<pre>';
// var_dump($response);
// echo '</pre>';

?>