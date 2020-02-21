function LoginField(props){
  return (
    <div className='loginFieldWrapper'>
      <input
          className='loginField'
          name={props.name}
          placeholder={props.placeholder}
          onChange={props.onChange}
          value={props.value}
        />
    </div>
  );
}

class LoginView extends React.Component {
  errors = {
    loginError: 'Specified username was not found',
    dbError: 'Could not connect to the database. Check F12 console for details.',
    jsonError: 'Invalid response from the server',
  };
  constructor(props){
    super(props);
    this.state = {
      username: '',
      error: '',
    }
    this.handleInputChange = this.handleInputChange.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);
    this.handleLogin = this.handleLogin.bind(this);
  }
  handleInputChange(e){
    this.setState({
      [e.target.name]: e.target.value,
    });
  }
  handleSubmit(e){
    e.preventDefault();
    if (this.state.username.length > 0){
      this.handleLogin(this.state.username);
    }
  }
  handleLogin(username){
    let comp = this;
    let query = {
      username: username,
    };
    fetch('api/login.php', {
      method: 'POST',
      body: JSON.stringify(query),
      cache: 'no-store',
      credentials: 'same-origin',
    }).then(response => response.json().then(function(json){
      if (response.ok) {
        if (!json.errorType) {
          comp.props.onLogin(json.userData);
        } else {
          if (json.errorType === 'nouser') {
            comp.setState({
              error: comp.errors.loginError,
            });
          }
          else if (json.errorType === 'db') {
            console.log(json.errorText);
            comp.setState({
              error: comp.errors.dbError,
            });
          }
        }
      } else {
        comp.setState({
          error: `Server error ${response.status} occurred`,
        });
      }
    }).catch(function(){
      comp.setState({
        error: comp.errors.jsonError,
      });
    }));
  }
  render(){
    return (
      <div className='floatingBox'>
        <div className='loginView'>
          <h1 className='gameName'>Battleships</h1>
          <div className='loginHeader'>Enter your username to login</div>
          {this.state.error &&
          <div className='loginErrorWrapper'>
            <div className='loginError'>{this.state.error}</div>
          </div>}
          <form className='loginForm'>
            <div className='loginContainer'>
              <div className='loginFields'>
                <LoginField
                  value={this.state.username}
                  onChange={this.handleInputChange}
                  name='username'
                  placeholder='username'
                />
              </div>
              <div className='loginButtonContainer'>
                <button type='submit' className='loginButton' onClick={this.handleSubmit}>LOGIN</button>
              </div>
            </div>
          </form>
        </div>
      </div>
    );
  }
}

function Navbar(props) {
  return (
    <div className='navbarWrapper'>
      <div className='mwContainer'>
        <div className='navbar'>
          <div className='navColLeft'>
            <div className='gameName'>Battleships</div>
          </div>
          <div className='navColRight'>
            <button type='button' className='logoutBox' onClick={props.onLogout} >
              <div className='userIcon'>
                <i className='fas fa-user-circle fa-fw' />
              </div>
              <div className='username'>{props.username}</div>
              <div className='logoutIcon'>
                <i className='fas fa-sign-out-alt fa-fw' />
              </div>
            </button>
          </div>
        </div>
      </div>
    </div>
  );
}

function StatsTile(props) {
  return (
    <div className='statsTileWrapper'>
      <div className={`statsTile  ${props.type}`} >
        <div className='tileCounter'>{props.count}</div>
        <div className='tileText'>{props.text}</div>
      </div>
    </div>
  );
}

function PlayerStats(props) {
  return (
    <div className='playerStats'>
      <h1 className='sectionHeader'>Dashboard</h1>
      <div className='statsTiles'>
        <StatsTile type='gameCount' count={props.wins + props.losses} text='Games played' />
        <StatsTile type='wins' count={props.wins} text='Wins' />
        <StatsTile type='losses' count={props.losses} text='Losses' />
      </div>
    </div>
  );
}

class FinishedGamesTable extends React.Component {
  constructor(props) {
    super(props);
  }
  forwardGameReplay(gameData) {
    this.props.onGameReplay(gameData);
  }
  render() {
    const rows = this.props.games.map(function(game){
      const gameData = game;
      return (
        <tr key={game.id} onClick={this.forwardGameReplay.bind(this, gameData)} >
          <td className='historyPlay'>
            <i className='far fa-play-circle fa-fw playInactive' />
            <i className='fas fa-play-circle fa-fw playActive' />
          </td>
          <td>{game.id}</td>
          <td>{game.name}</td>
          <td>{game.player1}</td>
          <td>{game.player2}</td>
          <td>{game.winner}</td>
        </tr>
      );
    }, this);
    return (
      <div className='finishedGamesTableWrapper'>
        <table className='finishedGamesTable'>
          <thead>
            <tr>
              <th className='historyPlay'><i className='fas fa-play-circle' /></th>
              <th>#</th>
              <th>Name</th>
              <th>Player 1</th>
              <th>Player 2</th>
              <th>Winner</th>
            </tr>
          </thead>
          <tbody>{rows}</tbody>
        </table>
      </div>
    );
  }
}

class FinishedGames extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      games: null,
    };
    this.getFinishedGames = this.getFinishedGames.bind(this);
    this.getFinishedGames();
  }
  getFinishedGames(){
    const comp = this;
    fetch('api/get_finished.php', {
      method: 'GET',
      credentials: 'same-origin',
      cache: 'no-cache',
    }).then(function(response){
      if (response.ok) {
        response.json().then(function(json){
          comp.setState({
            games: json,
          });
        });
      }
    });
  }
  render() {
    return (
      <div className='finishedGames'>
        <div className='tableHeader'>
          <div className='tableTitle'>Finished Games</div>
          <div className='refreshBox'>
            <button type='button' className='refreshBtn' onClick={this.getFinishedGames} >
              <div className='refreshIcon'>
                <i className='fas fa-sync' />
              </div>
              <div className='refreshText'>Refresh</div>
            </button>
          </div>
        </div>
        {this.state.games && <FinishedGamesTable games={this.state.games} onGameReplay={this.props.onGameReplay} />}
      </div>
    );
  }
}

function DashboardView(props) {
  return (
    <div className='dashboardView'>
      <Navbar username={props.userData.username} onLogout={props.onLogout} />
      <div className='dashboard'>
        <PlayerStats wins={props.userData.wins} losses={props.userData.losses} />
        <FinishedGames onGameReplay={props.onGameReplay} />
      </div>
    </div>
  );
}

function GameHeader(props) {
  return (
    <div className='gameHeader'>
      <div className='player1Header'>
        <div className='playerName'>{props.gameData.player1}</div>
        {props.gameData.is_ai_1 && <div className='aiBox'>AI</div>}
      </div>
      <div className='gameTitle'>
        <div className='gameName'>{props.gameData.name}</div>
        <div className='showType'>
          <div className='showTypeIcon'>
            <i className='fas fa-history' />
          </div>
          <div className='showTypeText'>Replay</div>
        </div>
      </div>
      <div className='player2Header'>
        {props.gameData.is_ai_2 && <div className='aiBox'>AI</div>}
        <div className='playerName'>{props.gameData.player2}</div>
      </div>
    </div>
  );
}

function BoardSquare(props) {
  let type;
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
  let icon;
  let isDestroyed = false;
  switch (props.value) {
    case '0':
      icon = false;
      break;
    case '1':
      icon = 'fas fa-poop';
      break;
    case '2':
      icon = 'fas fa-bomb'
      break;
    case '3':
      icon = 'fas fa-times'
      isDestroyed = true;
      break;
  }
  let classes = classNames({
    'boardSquare': true,
    [type]: true,
    'active': props.isActive,
    'destroyed': isDestroyed,
  });
  return (
    <div className='boardSquareOuter'>
      <div className='boardSquareInner'>
        <div className={classes}>
          {icon && <i className={`${icon} fa-fw`} />}
        </div>
      </div>
    </div>
  );
}

function BoardRow(props) {
  let squares = [];
  for (let x = 0; x < 10; ++x) {
    let isActive = (props.current === x) ? true : false;
    squares[x] = <BoardSquare key={x} type={props.types[x]} value={props.values[x]} isActive={isActive} />;
  }
  return (
    <div className='boardRow'>{squares}</div>
  );
}

function SpectatorBoard(props) {
  let boardRows = [];
  for (let y = 0; y < 10; ++y) {
    let currentx = (props.activeCell[1] === y) ? props.activeCell[0] : -1;
    boardRows[y] = <BoardRow key={y} types={props.map[y]} values={props.board[y]} current={currentx} />;
  }
  return (
    <div className='spectatorBoard'>{boardRows}</div>
  );
}

function MoveItem(props){
  const shipTypes = {
    '1': 'Carrier',
    '2': 'Battleship',
    '3': 'Cruiser',
    '4': 'Submarine',
    '5': 'Destroyer',
  };
  const res = props.res;
  let itemType;
  let resText;
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
      resText = `SUNK a ${shipTypes[res[2]]}`
      itemType = 'sink';
      break;
  }
  const classes = classNames({
    'moveItem': true,
    [itemType]: true,
    'done': props.isDone,
    'active': props.isActive,
  });
  return (
    <div className={classes} onClick={props.onClick}>
      <span className='movePlayer'>{props.username}</span>
      <span className='movePos'>{` (${props.pos[0]}; ${props.pos[1]}) `}</span>
      <span className='moveResult'>{resText}</span>
    </div>
  );
}

class MoveList extends React.Component {
  constructor(props) {
    super(props);
  }
  changeMove(moveid) {
    this.props.onMoveChange(moveid);
  }
  render() {
    if (!this.props.moves) return null;
    const moveItems = this.props.moves.map(function(move, i){
      return (
        <MoveItem key={i}
        onClick={this.changeMove.bind(this, i)}
        isActive={this.props.currentMove == i ? true : false}
        isDone={i < this.props.currentMove ? true : false}
        username={move.pid == this.props.gameData.pid1 ? this.props.gameData.player1 : this.props.gameData.player2}
        pos={move.pos.split('-')}
        res={move.res}
        />
      );
    }, this);
    return (
      <div className='moveList' ref={this.props.refFunc}>{moveItems}</div>
    );
  }
}

function GameButton(props) {
  return (
    <div className='gameButtonWrapper'>
      <button type='button' className={`gameButton ${props.type}`} onClick={props.onClick} >
        <div className='buttonText'>{props.text}</div>
      </button>
    </div>
  );
}

function GameFooter(props) {
  return (
    <div className='gameFooter'>
      <div className='gameFooterColLeft'>
        <GameButton type='return' text='EXIT' onClick={props.returnAction} /> 
      </div>
      <div className='gameFooterColRight'>
        <GameButton type='next' text='NEXT MOVE' onClick={props.onNextMoveClick} />
      </div>
    </div>
  );
}

class ReplayView extends React.Component {
  constructor(props) {
    super(props);
    this.inactiveCell = [-1,-1];
    this.state = {
      moveListWidth: 0,
      isLoaded: false,
      map1: this.fillEmptyMap(),
      map2: this.fillEmptyMap(),
      board1: this.fillEmptyMap(),
      board2: this.fillEmptyMap(),
      moves: null,
      currentMove: -1,
      activeBoard: -1,
      activeCell: [-1,-1],
    };
    this.loadGameHistory = this.loadGameHistory.bind(this);
    this.calculateMove = this.calculateMove.bind(this);
    this.nextMove = this.nextMove.bind(this);
    this.parseMapFromString = this.parseMapFromString.bind(this);
    this.getMoveListWidth = this.getMoveListWidth.bind(this);
    this.loadGameHistory();
  }
  getMoveListWidth(el) {
    if (el) {
      this.setState({
        moveListWidth: el.offsetWidth,
      });
    }
  }
  fillEmptyMap() {
    let map = [];
    for (let y = 0; y < 10; ++y) {
      map[y] = [];
      for (let x = 0; x < 10; ++x) {
        map[y][x] = '0';
      }
    }
    return map;
  }
  loadGameHistory(){
    const comp = this;
    const query = {
      id: comp.props.gameData.id,
    };
    fetch('api/get_game_history.php', {
      method: 'POST',
      credentials: 'same-origin',
      cache: 'no-cache',
      body: JSON.stringify(query),
    }).then(function(response){
      if (response.ok) {
        response.json().then(function(json){
          const map1 = comp.parseMapFromString(json.map1);
          const map2 = comp.parseMapFromString(json.map2);
          comp.setState({
            isLoaded: true,
            map1: map1,
            map2: map2,
            moves: json.moves,
          });
        });
      }
    });
  }
  parseMapFromString(mapStr) {
    let map = [];
    let pos = 0;
    for (let y = 0; y < 10; ++y) {
      map[y] = [];
      for (let x = 0; x < 10; ++x) {
        map[y][x] = mapStr[pos++];
      }
    }
    return map;
  }
  nextMove() {
    if (this.state.currentMove + 1 != this.state.moves.length)
      this.calculateMove(this.state.currentMove + 1);
  }
  calculateMove(moveid) {
    const p1 = this.props.gameData.pid1;
    const moves = this.state.moves;
    const maps = [this.state.map1, this.state.map2];
    let boards = [this.fillEmptyMap(), this.fillEmptyMap()];
    let bi;
    let activeCell = [-1,-1];
    for (let i = 0; i <= moveid; ++i) {
      let move = moves[i];
      bi = move.pid == p1 ? 1 : 0;
      let pos = move.pos.split('-');
      let x = parseInt(pos[0]);
      let y = parseInt(pos[1]);
      activeCell[0] = x;
      activeCell[1] = y;
      switch (move.res[0]) {
        case '0':
          boards[bi][y][x] = '1';
          break;
        case '1':
          boards[bi][y][x] = '2'
          break;
        case '2':
          let shipType = move.res[2];
          if (x >= 0 && maps[bi][y][x-1] == shipType) {
            let currx = x;
            while (currx >= 0 && maps[bi][y][currx] == shipType) {
              boards[bi][y][currx--] = '3';
            }
          }
          if (x != 9 && maps[bi][y][x+1] == shipType) {
            let currx = x;
            while (currx <= 9 && maps[bi][y][currx] == shipType) {
              boards[bi][y][currx++] = '3';
            }
          }
          if (y != 0 && maps[bi][y-1][x] == shipType) {
            let curry = y;
            while (curry >= 0 && maps[bi][curry][x] == shipType) {
              boards[bi][curry--][x] = '3';
            }
          }
          if (y != 9 && maps[bi][y+1][x] == shipType) {
            let curry = y;
            while (curry <= 9 && maps[bi][curry][x] == shipType) {
              boards[bi][curry++][x] = '3';
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
      activeCell: activeCell,
    });
  }
  render() {
    let mainWrapper = null;
    if (this.state.isLoaded) {
      mainWrapper = (
        <div className='mainWrapper'>
          <div className='moveListContainer'>
            <MoveList
              refFunc={this.getMoveListWidth}
              moves={this.state.moves}
              currentMove={this.state.currentMove}
              gameData={this.props.gameData}
              onMoveChange={this.calculateMove} >
            </MoveList>
          </div>
          <div className='replayBoards'>
            <SpectatorBoard
              map={this.state.map1}
              board={this.state.board1}
              activeCell={this.state.activeBoard === 0 ? this.state.activeCell : this.inactiveCell} />
            <div className='boardsSeparator' style={{ width: this.state.moveListWidth }} />
            <SpectatorBoard
              map={this.state.map2}
              board={this.state.board2} 
              activeCell={this.state.activeBoard === 1 ? this.state.activeCell : this.inactiveCell} />
          </div>
        </div>
      );
    }
    return (
      <div className='replayView'>
        <div className='gameContainerFixed'>
          <div className='gameContainer'>
            <GameHeader gameData={this.props.gameData} />
            {mainWrapper}
            <GameFooter returnAction={this.props.returnToDash} onNextMoveClick={this.nextMove} />
          </div>
        </div>
      </div>
    );
  }
}

class PlayerView extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      view: 'dash',
      gameData: null,
    };
    this.replayGame = this.replayGame.bind(this);
    this.returnToDashboard = this.returnToDashboard.bind(this);
  }
  replayGame(gameData) {
    this.setState({
      view: 'replay',
      gameData: gameData,
    });
  }
  returnToDashboard() {
    this.setState({
      view: 'dash',
      gameData: null,
    });
  }
  render() {
    let view;
    switch(this.state.view) {
      case 'dash':
        view = <DashboardView onGameReplay={this.replayGame} userData={this.props.userData} onLogout={this.props.onLogout} />;
        break;
      case 'replay':
        view = <ReplayView gameData={this.state.gameData} returnToDash={this.returnToDashboard} />
        break;
    }
    return view;
  }
}

function InitErrorView(props) {
  const errors = {
    dbError: 'Could not connect to the database:',
    jsonError: 'Invalid response from the PHP script:',
    serverError: 'Server error occured:',
  };
  return (
    <div className='floatingBox'>
      <div className='initErrorView'>
        <div className='initErrorIcon'>
          <i className='fas fa-exclamation-triangle' />
        </div>
        <div className='initErrorHeader'>
          Game initialization failure.
        </div>
        <div className='initErrorText'>{errors[props.error.errorType]}</div>
        <div className='initErrorDetails'>{props.error.errorDetails}</div>
      </div>
    </div>
  );
}

class Game extends React.Component {
  constructor(props){
    super(props);
    this.state = {
      init: false,
      initFail: false,
      initError: '',
      isLoggedIn: false,
      userData: null,
    };
    this.checkSession = this.checkSession.bind(this);
    this.handleLogin = this.handleLogin.bind(this);
    this.handleLogout = this.handleLogout.bind(this);
    this.checkSession();
  }
  checkSession(){
    const comp = this;
    fetch('api/session.php', {
      method: 'GET',
      credentials: 'same-origin',
      cache: 'no-store',
    }).then(function(response){
      if (response.ok) {
        response.json().then(function(json){
          if (!json.errorType) {
            if (json.isLoggedIn) {
              comp.setState({
                init: true,
                isLoggedIn: true,
                userData: json.userData,
              });
            } else {
              comp.setState({
                init: true,
              });
            }
          } else {
            if (json.errorType == 'db') {
              comp.setState({
                initFail: true,
                initError: {
                  errorType: 'dbError',
                  errorDetails: json.errorText,
                },
              });
            }
          }
        }).catch(function(error){
          console.log(error);
          comp.setState({
            initFail: true,
            initError: {
              errorType: 'jsonError',
              errorDetails: error.toString(),
            },
          });
        })
      } else {
        comp.setState({
          initFail: true,
          initError: {
            errorType: 'serverError',
            errorDetails: `Error ${response.status} ${response.statusText}`,
          },
        });
      }
    });
  }
  handleLogin(userData) {
    this.setState({
      isLoggedIn: true,
      userData: userData,
    });
  }
  handleLogout() {
    const comp = this;
    fetch('api/logout.php', {
      method: 'GET',
      credentials: 'same-origin',
      cache: 'no-store',
    }).then(function(response){
      comp.setState({
        isLoggedIn: false,
        userData: null,
      });
    });
  }
  render(){
    let view;
    if (!this.state.init) {
      if (this.state.initError) {
        view = <InitErrorView error={this.state.initError} />
      } else {
        view = null;
      }
    }
    else {
      if (this.state.isLoggedIn) view = (
        <PlayerView
          userData={this.state.userData}
          onLogout={this.handleLogout} />
      );
      else view = <LoginView onLogin={this.handleLogin} />
    }
    return view;
  }
}

ReactDOM.render(<Game />, document.querySelector('#root'));