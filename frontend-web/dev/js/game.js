var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _defineProperty(obj, key, value) { if (key in obj) { Object.defineProperty(obj, key, { value: value, enumerable: true, configurable: true, writable: true }); } else { obj[key] = value; } return obj; }

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function _possibleConstructorReturn(self, call) { if (!self) { throw new ReferenceError("this hasn't been initialised - super() hasn't been called"); } return call && (typeof call === "object" || typeof call === "function") ? call : self; }

function _inherits(subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function, not " + typeof superClass); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, enumerable: false, writable: true, configurable: true } }); if (superClass) Object.setPrototypeOf ? Object.setPrototypeOf(subClass, superClass) : subClass.__proto__ = superClass; }

function LoginField(props) {
  return React.createElement(
    'div',
    { className: 'loginFieldWrapper' },
    React.createElement('input', {
      className: 'loginField',
      name: props.name,
      placeholder: props.placeholder,
      onChange: props.onChange,
      value: props.value
    })
  );
}

var LoginView = function (_React$Component) {
  _inherits(LoginView, _React$Component);

  function LoginView(props) {
    _classCallCheck(this, LoginView);

    var _this = _possibleConstructorReturn(this, (LoginView.__proto__ || Object.getPrototypeOf(LoginView)).call(this, props));

    _this.errors = {
      loginError: 'Specified username was not found',
      dbError: 'Could not connect to the database. Check F12 console for details.',
      jsonError: 'Invalid response from the server'
    };

    _this.state = {
      username: '',
      error: ''
    };
    _this.handleInputChange = _this.handleInputChange.bind(_this);
    _this.handleSubmit = _this.handleSubmit.bind(_this);
    _this.handleLogin = _this.handleLogin.bind(_this);
    return _this;
  }

  _createClass(LoginView, [{
    key: 'handleInputChange',
    value: function handleInputChange(e) {
      this.setState(_defineProperty({}, e.target.name, e.target.value));
    }
  }, {
    key: 'handleSubmit',
    value: function handleSubmit(e) {
      e.preventDefault();
      if (this.state.username.length > 0) {
        this.handleLogin(this.state.username);
      }
    }
  }, {
    key: 'handleLogin',
    value: function handleLogin(username) {
      var comp = this;
      var query = {
        username: username
      };
      fetch('api/login.php', {
        method: 'POST',
        body: JSON.stringify(query),
        cache: 'no-store',
        credentials: 'same-origin'
      }).then(function (response) {
        return response.json().then(function (json) {
          if (response.ok) {
            if (!json.errorType) {
              comp.props.onLogin(json.userData);
            } else {
              if (json.errorType === 'nouser') {
                comp.setState({
                  error: comp.errors.loginError
                });
              } else if (json.errorType === 'db') {
                console.log(json.errorText);
                comp.setState({
                  error: comp.errors.dbError
                });
              }
            }
          } else {
            comp.setState({
              error: 'Server error ' + response.status + ' occurred'
            });
          }
        }).catch(function () {
          comp.setState({
            error: comp.errors.jsonError
          });
        });
      });
    }
  }, {
    key: 'render',
    value: function render() {
      return React.createElement(
        'div',
        { className: 'floatingBox' },
        React.createElement(
          'div',
          { className: 'loginView' },
          React.createElement(
            'h1',
            { className: 'gameName' },
            'Battleships'
          ),
          React.createElement(
            'div',
            { className: 'loginHeader' },
            'Enter your username to login'
          ),
          this.state.error && React.createElement(
            'div',
            { className: 'loginErrorWrapper' },
            React.createElement(
              'div',
              { className: 'loginError' },
              this.state.error
            )
          ),
          React.createElement(
            'form',
            { className: 'loginForm' },
            React.createElement(
              'div',
              { className: 'loginContainer' },
              React.createElement(
                'div',
                { className: 'loginFields' },
                React.createElement(LoginField, {
                  value: this.state.username,
                  onChange: this.handleInputChange,
                  name: 'username',
                  placeholder: 'username'
                })
              ),
              React.createElement(
                'div',
                { className: 'loginButtonContainer' },
                React.createElement(
                  'button',
                  { type: 'submit', className: 'loginButton', onClick: this.handleSubmit },
                  'LOGIN'
                )
              )
            )
          )
        )
      );
    }
  }]);

  return LoginView;
}(React.Component);

function Navbar(props) {
  return React.createElement(
    'div',
    { className: 'navbarWrapper' },
    React.createElement(
      'div',
      { className: 'mwContainer' },
      React.createElement(
        'div',
        { className: 'navbar' },
        React.createElement(
          'div',
          { className: 'navColLeft' },
          React.createElement(
            'div',
            { className: 'gameName' },
            'Battleships'
          )
        ),
        React.createElement(
          'div',
          { className: 'navColRight' },
          React.createElement(
            'button',
            { type: 'button', className: 'logoutBox', onClick: props.onLogout },
            React.createElement(
              'div',
              { className: 'userIcon' },
              React.createElement('i', { className: 'fas fa-user-circle fa-fw' })
            ),
            React.createElement(
              'div',
              { className: 'username' },
              props.username
            ),
            React.createElement(
              'div',
              { className: 'logoutIcon' },
              React.createElement('i', { className: 'fas fa-sign-out-alt fa-fw' })
            )
          )
        )
      )
    )
  );
}

function StatsTile(props) {
  return React.createElement(
    'div',
    { className: 'statsTileWrapper' },
    React.createElement(
      'div',
      { className: 'statsTile  ' + props.type },
      React.createElement(
        'div',
        { className: 'tileCounter' },
        props.count
      ),
      React.createElement(
        'div',
        { className: 'tileText' },
        props.text
      )
    )
  );
}

function PlayerStats(props) {
  return React.createElement(
    'div',
    { className: 'playerStats' },
    React.createElement(
      'h1',
      { className: 'sectionHeader' },
      'Dashboard'
    ),
    React.createElement(
      'div',
      { className: 'statsTiles' },
      React.createElement(StatsTile, { type: 'gameCount', count: props.wins + props.losses, text: 'Games played' }),
      React.createElement(StatsTile, { type: 'wins', count: props.wins, text: 'Wins' }),
      React.createElement(StatsTile, { type: 'losses', count: props.losses, text: 'Losses' })
    )
  );
}

var FinishedGamesTable = function (_React$Component2) {
  _inherits(FinishedGamesTable, _React$Component2);

  function FinishedGamesTable(props) {
    _classCallCheck(this, FinishedGamesTable);

    return _possibleConstructorReturn(this, (FinishedGamesTable.__proto__ || Object.getPrototypeOf(FinishedGamesTable)).call(this, props));
  }

  _createClass(FinishedGamesTable, [{
    key: 'forwardGameReplay',
    value: function forwardGameReplay(gameData) {
      this.props.onGameReplay(gameData);
    }
  }, {
    key: 'render',
    value: function render() {
      var rows = this.props.games.map(function (game) {
        var gameData = game;
        return React.createElement(
          'tr',
          { key: game.id, onClick: this.forwardGameReplay.bind(this, gameData) },
          React.createElement(
            'td',
            { className: 'historyPlay' },
            React.createElement('i', { className: 'far fa-play-circle fa-fw playInactive' }),
            React.createElement('i', { className: 'fas fa-play-circle fa-fw playActive' })
          ),
          React.createElement(
            'td',
            null,
            game.id
          ),
          React.createElement(
            'td',
            null,
            game.name
          ),
          React.createElement(
            'td',
            null,
            game.player1
          ),
          React.createElement(
            'td',
            null,
            game.player2
          ),
          React.createElement(
            'td',
            null,
            game.winner
          )
        );
      }, this);
      return React.createElement(
        'div',
        { className: 'finishedGamesTableWrapper' },
        React.createElement(
          'table',
          { className: 'finishedGamesTable' },
          React.createElement(
            'thead',
            null,
            React.createElement(
              'tr',
              null,
              React.createElement(
                'th',
                { className: 'historyPlay' },
                React.createElement('i', { className: 'fas fa-play-circle' })
              ),
              React.createElement(
                'th',
                null,
                '#'
              ),
              React.createElement(
                'th',
                null,
                'Name'
              ),
              React.createElement(
                'th',
                null,
                'Player 1'
              ),
              React.createElement(
                'th',
                null,
                'Player 2'
              ),
              React.createElement(
                'th',
                null,
                'Winner'
              )
            )
          ),
          React.createElement(
            'tbody',
            null,
            rows
          )
        )
      );
    }
  }]);

  return FinishedGamesTable;
}(React.Component);

var FinishedGames = function (_React$Component3) {
  _inherits(FinishedGames, _React$Component3);

  function FinishedGames(props) {
    _classCallCheck(this, FinishedGames);

    var _this3 = _possibleConstructorReturn(this, (FinishedGames.__proto__ || Object.getPrototypeOf(FinishedGames)).call(this, props));

    _this3.state = {
      games: null
    };
    _this3.getFinishedGames = _this3.getFinishedGames.bind(_this3);
    _this3.getFinishedGames();
    return _this3;
  }

  _createClass(FinishedGames, [{
    key: 'getFinishedGames',
    value: function getFinishedGames() {
      var comp = this;
      fetch('api/get_finished.php', {
        method: 'GET',
        credentials: 'same-origin',
        cache: 'no-cache'
      }).then(function (response) {
        if (response.ok) {
          response.json().then(function (json) {
            comp.setState({
              games: json
            });
          });
        }
      });
    }
  }, {
    key: 'render',
    value: function render() {
      return React.createElement(
        'div',
        { className: 'finishedGames' },
        React.createElement(
          'div',
          { className: 'tableHeader' },
          React.createElement(
            'div',
            { className: 'tableTitle' },
            'Finished Games'
          ),
          React.createElement(
            'div',
            { className: 'refreshBox' },
            React.createElement(
              'button',
              { type: 'button', className: 'refreshBtn', onClick: this.getFinishedGames },
              React.createElement(
                'div',
                { className: 'refreshIcon' },
                React.createElement('i', { className: 'fas fa-sync' })
              ),
              React.createElement(
                'div',
                { className: 'refreshText' },
                'Refresh'
              )
            )
          )
        ),
        this.state.games && React.createElement(FinishedGamesTable, { games: this.state.games, onGameReplay: this.props.onGameReplay })
      );
    }
  }]);

  return FinishedGames;
}(React.Component);

function DashboardView(props) {
  return React.createElement(
    'div',
    { className: 'dashboardView' },
    React.createElement(Navbar, { username: props.userData.username, onLogout: props.onLogout }),
    React.createElement(
      'div',
      { className: 'dashboard' },
      React.createElement(PlayerStats, { wins: props.userData.wins, losses: props.userData.losses }),
      React.createElement(FinishedGames, { onGameReplay: props.onGameReplay })
    )
  );
}

function GameHeader(props) {
  return React.createElement(
    'div',
    { className: 'gameHeader' },
    React.createElement(
      'div',
      { className: 'player1Header' },
      React.createElement(
        'div',
        { className: 'playerName' },
        props.gameData.player1
      ),
      props.gameData.is_ai_1 && React.createElement(
        'div',
        { className: 'aiBox' },
        'AI'
      )
    ),
    React.createElement(
      'div',
      { className: 'gameTitle' },
      React.createElement(
        'div',
        { className: 'gameName' },
        props.gameData.name
      ),
      React.createElement(
        'div',
        { className: 'showType' },
        React.createElement(
          'div',
          { className: 'showTypeIcon' },
          React.createElement('i', { className: 'fas fa-history' })
        ),
        React.createElement(
          'div',
          { className: 'showTypeText' },
          'Replay'
        )
      )
    ),
    React.createElement(
      'div',
      { className: 'player2Header' },
      props.gameData.is_ai_2 && React.createElement(
        'div',
        { className: 'aiBox' },
        'AI'
      ),
      React.createElement(
        'div',
        { className: 'playerName' },
        props.gameData.player2
      )
    )
  );
}

function BoardSquare(props) {
  var _classNames;

  var type = void 0;
  switch (props.type) {
    case '0':
      type = 'empty';
      break;
    case '1':
      type = 'carrier';
      break;
    case '2':
      type = 'battleship';
      break;
    case '3':
      type = 'cruiser';
      break;
    case '4':
      type = 'submarine';
      break;
    case '5':
      type = 'destroyer';
      break;
  }
  var icon = void 0;
  var isDestroyed = false;
  switch (props.value) {
    case '0':
      icon = false;
      break;
    case '1':
      icon = 'fas fa-poop';
      break;
    case '2':
      icon = 'fas fa-bomb';
      break;
    case '3':
      icon = 'fas fa-times';
      isDestroyed = true;
      break;
  }
  var classes = classNames((_classNames = {
    'boardSquare': true
  }, _defineProperty(_classNames, type, true), _defineProperty(_classNames, 'active', props.isActive), _defineProperty(_classNames, 'destroyed', isDestroyed), _classNames));
  return React.createElement(
    'div',
    { className: 'boardSquareOuter' },
    React.createElement(
      'div',
      { className: 'boardSquareInner' },
      React.createElement(
        'div',
        { className: classes },
        icon && React.createElement('i', { className: icon + ' fa-fw' })
      )
    )
  );
}

function BoardRow(props) {
  var squares = [];
  for (var x = 0; x < 10; ++x) {
    var isActive = props.current === x ? true : false;
    squares[x] = React.createElement(BoardSquare, { key: x, type: props.types[x], value: props.values[x], isActive: isActive });
  }
  return React.createElement(
    'div',
    { className: 'boardRow' },
    squares
  );
}

function SpectatorBoard(props) {
  var boardRows = [];
  for (var y = 0; y < 10; ++y) {
    var currentx = props.activeCell[1] === y ? props.activeCell[0] : -1;
    boardRows[y] = React.createElement(BoardRow, { key: y, types: props.map[y], values: props.board[y], current: currentx });
  }
  return React.createElement(
    'div',
    { className: 'spectatorBoard' },
    boardRows
  );
}

function MoveItem(props) {
  var _classNames2;

  var shipTypes = {
    '1': 'Carrier',
    '2': 'Battleship',
    '3': 'Cruiser',
    '4': 'Submarine',
    '5': 'Destroyer'
  };
  var res = props.res;
  var itemType = void 0;
  var resText = void 0;
  switch (res[0]) {
    case '0':
      resText = 'MISS';
      itemType = 'miss';
      break;
    case '1':
      resText = 'HIT';
      itemType = 'hit';
      break;
    case '2':
      resText = 'SUNK a ' + shipTypes[res[2]];
      itemType = 'sink';
      break;
  }
  var classes = classNames((_classNames2 = {
    'moveItem': true
  }, _defineProperty(_classNames2, itemType, true), _defineProperty(_classNames2, 'done', props.isDone), _defineProperty(_classNames2, 'active', props.isActive), _classNames2));
  var posx = parseInt(props.pos[0]) + 1;
  var posy = parseInt(props.pos[1]) + 1;
  return React.createElement(
    'div',
    { className: classes, onClick: props.onClick },
    React.createElement(
      'span',
      { className: 'movePlayer' },
      props.username
    ),
    React.createElement(
      'span',
      { className: 'movePos' },
      ' (' + posx + '; ' + posy + ') '
    ),
    React.createElement(
      'span',
      { className: 'moveResult' },
      resText
    )
  );
}

var MoveList = function (_React$Component4) {
  _inherits(MoveList, _React$Component4);

  function MoveList(props) {
    _classCallCheck(this, MoveList);

    return _possibleConstructorReturn(this, (MoveList.__proto__ || Object.getPrototypeOf(MoveList)).call(this, props));
  }

  _createClass(MoveList, [{
    key: 'changeMove',
    value: function changeMove(moveid) {
      this.props.onMoveChange(moveid);
    }
  }, {
    key: 'render',
    value: function render() {
      if (!this.props.moves) return null;
      var moveItems = this.props.moves.map(function (move, i) {
        return React.createElement(MoveItem, { key: i,
          onClick: this.changeMove.bind(this, i),
          isActive: this.props.currentMove == i ? true : false,
          isDone: i < this.props.currentMove ? true : false,
          username: move.pid == this.props.gameData.pid1 ? this.props.gameData.player1 : this.props.gameData.player2,
          pos: move.pos.split('-'),
          res: move.res
        });
      }, this);
      return React.createElement(
        'div',
        { className: 'moveList', ref: this.props.refFunc },
        moveItems
      );
    }
  }]);

  return MoveList;
}(React.Component);

function GameButton(props) {
  return React.createElement(
    'div',
    { className: 'gameButtonWrapper' },
    React.createElement(
      'button',
      { type: 'button', className: 'gameButton ' + props.type, onClick: props.onClick },
      React.createElement(
        'div',
        { className: 'buttonText' },
        props.text
      )
    )
  );
}

function GameFooter(props) {
  return React.createElement(
    'div',
    { className: 'gameFooter' },
    React.createElement(
      'div',
      { className: 'gameFooterColLeft' },
      React.createElement(GameButton, { type: 'return', text: 'EXIT', onClick: props.returnAction })
    ),
    React.createElement(
      'div',
      { className: 'gameFooterColRight' },
      React.createElement(GameButton, { type: 'prev', text: 'PREV', onClick: props.prevMove }),
      React.createElement(GameButton, { type: 'next', text: 'NEXT', onClick: props.nextMove })
    )
  );
}

var ReplayView = function (_React$Component5) {
  _inherits(ReplayView, _React$Component5);

  function ReplayView(props) {
    _classCallCheck(this, ReplayView);

    var _this5 = _possibleConstructorReturn(this, (ReplayView.__proto__ || Object.getPrototypeOf(ReplayView)).call(this, props));

    _this5.inactiveCell = [-1, -1];
    _this5.state = {
      moveListWidth: 0,
      isLoaded: false,
      map1: _this5.fillEmptyMap(),
      map2: _this5.fillEmptyMap(),
      board1: _this5.fillEmptyMap(),
      board2: _this5.fillEmptyMap(),
      moves: null,
      currentMove: -1,
      activeBoard: -1,
      activeCell: [-1, -1]
    };
    _this5.getMoveListWidth = _this5.getMoveListWidth.bind(_this5);
    _this5.parseMapFromString = _this5.parseMapFromString.bind(_this5);
    _this5.loadGameHistory = _this5.loadGameHistory.bind(_this5);
    _this5.calculateMove = _this5.calculateMove.bind(_this5);
    _this5.nextMove = _this5.nextMove.bind(_this5);
    _this5.prevMove = _this5.prevMove.bind(_this5);
    _this5.loadGameHistory();
    return _this5;
  }

  _createClass(ReplayView, [{
    key: 'getMoveListWidth',
    value: function getMoveListWidth(el) {
      if (el) {
        this.setState({
          moveListWidth: el.offsetWidth
        });
      }
    }
  }, {
    key: 'parseMapFromString',
    value: function parseMapFromString(mapStr) {
      var map = [];
      var pos = 0;
      for (var y = 0; y < 10; ++y) {
        map[y] = [];
        for (var x = 0; x < 10; ++x) {
          map[y][x] = mapStr[pos++];
        }
      }
      return map;
    }
  }, {
    key: 'fillEmptyMap',
    value: function fillEmptyMap() {
      var map = [];
      for (var y = 0; y < 10; ++y) {
        map[y] = [];
        for (var x = 0; x < 10; ++x) {
          map[y][x] = '0';
        }
      }
      return map;
    }
  }, {
    key: 'loadGameHistory',
    value: function loadGameHistory() {
      var comp = this;
      var query = {
        id: comp.props.gameData.id
      };
      fetch('api/get_game_history.php', {
        method: 'POST',
        credentials: 'same-origin',
        cache: 'no-cache',
        body: JSON.stringify(query)
      }).then(function (response) {
        if (response.ok) {
          response.json().then(function (json) {
            var map1 = comp.parseMapFromString(json.map1);
            var map2 = comp.parseMapFromString(json.map2);
            comp.setState({
              isLoaded: true,
              map1: map1,
              map2: map2,
              moves: json.moves
            });
          });
        }
      });
    }
  }, {
    key: 'calculateMove',
    value: function calculateMove(moveid) {
      var p1 = this.props.gameData.pid1;
      var moves = this.state.moves;
      var maps = [this.state.map1, this.state.map2];
      var boards = [this.fillEmptyMap(), this.fillEmptyMap()];
      var bi = void 0;
      var x = void 0;
      var y = void 0;
      for (var i = 0; i <= moveid; ++i) {
        var move = moves[i];
        bi = move.pid == p1 ? 1 : 0;
        var pos = move.pos.split('-');
        x = parseInt(pos[0]);
        y = parseInt(pos[1]);
        switch (move.res[0]) {
          case '0':
            boards[bi][y][x] = '1';
            break;
          case '1':
            boards[bi][y][x] = '2';
            break;
          case '2':
            var shipType = move.res[2];
            if (x >= 0 && maps[bi][y][x - 1] == shipType) {
              var currx = x;
              while (currx >= 0 && maps[bi][y][currx] == shipType) {
                boards[bi][y][currx--] = '3';
              }
            }
            if (x != 9 && maps[bi][y][x + 1] == shipType) {
              var _currx = x;
              while (_currx <= 9 && maps[bi][y][_currx] == shipType) {
                boards[bi][y][_currx++] = '3';
              }
            }
            if (y != 0 && maps[bi][y - 1][x] == shipType) {
              var curry = y;
              while (curry >= 0 && maps[bi][curry][x] == shipType) {
                boards[bi][curry--][x] = '3';
              }
            }
            if (y != 9 && maps[bi][y + 1][x] == shipType) {
              var _curry = y;
              while (_curry <= 9 && maps[bi][_curry][x] == shipType) {
                boards[bi][_curry++][x] = '3';
              }
            }
            break;
        }
      }
      this.setState({
        board1: boards[0],
        board2: boards[1],
        currentMove: moveid,
        activeBoard: bi,
        activeCell: [x, y]
      });
    }
  }, {
    key: 'nextMove',
    value: function nextMove() {
      if (this.state.currentMove + 1 != this.state.moves.length) this.calculateMove(this.state.currentMove + 1);
    }
  }, {
    key: 'prevMove',
    value: function prevMove() {
      if (this.state.currentMove != -1) this.calculateMove(this.state.currentMove - 1);
    }
  }, {
    key: 'render',
    value: function render() {
      var mainWrapper = null;
      if (this.state.isLoaded) {
        mainWrapper = React.createElement(
          'div',
          { className: 'mainWrapper' },
          React.createElement(
            'div',
            { className: 'moveListContainer' },
            React.createElement(MoveList, {
              refFunc: this.getMoveListWidth,
              moves: this.state.moves,
              currentMove: this.state.currentMove,
              gameData: this.props.gameData,
              onMoveChange: this.calculateMove })
          ),
          React.createElement(
            'div',
            { className: 'replayBoards' },
            React.createElement(SpectatorBoard, {
              map: this.state.map1,
              board: this.state.board1,
              activeCell: this.state.activeBoard === 0 ? this.state.activeCell : this.inactiveCell }),
            React.createElement('div', { className: 'boardsSeparator', style: { width: this.state.moveListWidth } }),
            React.createElement(SpectatorBoard, {
              map: this.state.map2,
              board: this.state.board2,
              activeCell: this.state.activeBoard === 1 ? this.state.activeCell : this.inactiveCell })
          )
        );
      }
      return React.createElement(
        'div',
        { className: 'replayView' },
        React.createElement(
          'div',
          { className: 'gameContainerFixed' },
          React.createElement(
            'div',
            { className: 'gameContainer' },
            React.createElement(GameHeader, { gameData: this.props.gameData }),
            mainWrapper,
            React.createElement(GameFooter, { returnAction: this.props.returnToDash, nextMove: this.nextMove, prevMove: this.prevMove })
          )
        )
      );
    }
  }]);

  return ReplayView;
}(React.Component);

var PlayerView = function (_React$Component6) {
  _inherits(PlayerView, _React$Component6);

  function PlayerView(props) {
    _classCallCheck(this, PlayerView);

    var _this6 = _possibleConstructorReturn(this, (PlayerView.__proto__ || Object.getPrototypeOf(PlayerView)).call(this, props));

    _this6.state = {
      view: 'dash',
      gameData: null
    };
    _this6.replayGame = _this6.replayGame.bind(_this6);
    _this6.returnToDashboard = _this6.returnToDashboard.bind(_this6);
    return _this6;
  }

  _createClass(PlayerView, [{
    key: 'replayGame',
    value: function replayGame(gameData) {
      this.setState({
        view: 'replay',
        gameData: gameData
      });
    }
  }, {
    key: 'returnToDashboard',
    value: function returnToDashboard() {
      this.setState({
        view: 'dash',
        gameData: null
      });
    }
  }, {
    key: 'render',
    value: function render() {
      var view = void 0;
      switch (this.state.view) {
        case 'dash':
          view = React.createElement(DashboardView, { onGameReplay: this.replayGame, userData: this.props.userData, onLogout: this.props.onLogout });
          break;
        case 'replay':
          view = React.createElement(ReplayView, { gameData: this.state.gameData, returnToDash: this.returnToDashboard });
          break;
      }
      return view;
    }
  }]);

  return PlayerView;
}(React.Component);

function InitErrorView(props) {
  var errors = {
    dbError: 'Could not connect to the database:',
    jsonError: 'Invalid response from the PHP script:',
    serverError: 'Server error occured:'
  };
  return React.createElement(
    'div',
    { className: 'floatingBox' },
    React.createElement(
      'div',
      { className: 'initErrorView' },
      React.createElement(
        'div',
        { className: 'initErrorIcon' },
        React.createElement('i', { className: 'fas fa-exclamation-triangle' })
      ),
      React.createElement(
        'div',
        { className: 'initErrorHeader' },
        'Game initialization failure.'
      ),
      React.createElement(
        'div',
        { className: 'initErrorText' },
        errors[props.error.errorType]
      ),
      React.createElement(
        'div',
        { className: 'initErrorDetails' },
        props.error.errorDetails
      )
    )
  );
}

var Game = function (_React$Component7) {
  _inherits(Game, _React$Component7);

  function Game(props) {
    _classCallCheck(this, Game);

    var _this7 = _possibleConstructorReturn(this, (Game.__proto__ || Object.getPrototypeOf(Game)).call(this, props));

    _this7.state = {
      init: false,
      initFail: false,
      initError: '',
      isLoggedIn: false,
      userData: null
    };
    _this7.checkSession = _this7.checkSession.bind(_this7);
    _this7.handleLogin = _this7.handleLogin.bind(_this7);
    _this7.handleLogout = _this7.handleLogout.bind(_this7);
    _this7.checkSession();
    return _this7;
  }

  _createClass(Game, [{
    key: 'checkSession',
    value: function checkSession() {
      var comp = this;
      fetch('api/session.php', {
        method: 'GET',
        credentials: 'same-origin',
        cache: 'no-store'
      }).then(function (response) {
        if (response.ok) {
          response.json().then(function (json) {
            if (!json.errorType) {
              if (json.isLoggedIn) {
                comp.setState({
                  init: true,
                  isLoggedIn: true,
                  userData: json.userData
                });
              } else {
                comp.setState({
                  init: true
                });
              }
            } else {
              if (json.errorType == 'db') {
                comp.setState({
                  initFail: true,
                  initError: {
                    errorType: 'dbError',
                    errorDetails: json.errorText
                  }
                });
              }
            }
          }).catch(function (error) {
            console.log(error);
            comp.setState({
              initFail: true,
              initError: {
                errorType: 'jsonError',
                errorDetails: error.toString()
              }
            });
          });
        } else {
          comp.setState({
            initFail: true,
            initError: {
              errorType: 'serverError',
              errorDetails: 'Error ' + response.status + ' ' + response.statusText
            }
          });
        }
      });
    }
  }, {
    key: 'handleLogin',
    value: function handleLogin(userData) {
      this.setState({
        isLoggedIn: true,
        userData: userData
      });
    }
  }, {
    key: 'handleLogout',
    value: function handleLogout() {
      var comp = this;
      fetch('api/logout.php', {
        method: 'GET',
        credentials: 'same-origin',
        cache: 'no-store'
      }).then(function (response) {
        comp.setState({
          isLoggedIn: false,
          userData: null
        });
      });
    }
  }, {
    key: 'render',
    value: function render() {
      var view = void 0;
      if (!this.state.init) {
        if (this.state.initError) {
          view = React.createElement(InitErrorView, { error: this.state.initError });
        } else {
          view = null;
        }
      } else {
        if (this.state.isLoggedIn) view = React.createElement(PlayerView, {
          userData: this.state.userData,
          onLogout: this.handleLogout });else view = React.createElement(LoginView, { onLogin: this.handleLogin });
      }
      return view;
    }
  }]);

  return Game;
}(React.Component);

ReactDOM.render(React.createElement(Game, null), document.querySelector('#root'));