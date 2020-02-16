<?php function hfile($f) { printf("$f?%s", filemtime($f)); } ?>
<!DOCTYPE html>
<html>
<head>
  <title>IT Battleships</title>
  <link rel="stylesheet" href="lib/normalize.css">
  <link rel="stylesheet" href="<?php hfile('css/game.css'); ?>">
  <link href="https://fonts.googleapis.com/css?family=Changa:400,700&display=swap" rel="stylesheet">
  <link rel="stylesheet" href="lib/fontawesome/css/all.min.css">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
</head>
<body>
  <div id="root">
  </div>
  <script src="https://unpkg.com/react@16/umd/react.development.js" crossorigin></script>
  <script src="https://unpkg.com/react-dom@16/umd/react-dom.development.js" crossorigin></script>
  <script src="lib/classnames.js"></script>
  <script src="<?php hfile('js/game.js'); ?>"></script>
  <!--<script src="https://kit.fontawesome.com/bf153ef58b.js" crossorigin="anonymous"></script>-->
</body>
</html>