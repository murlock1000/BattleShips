# WEB-based frontend

Built with Javascript-based React library and uses PHP as a backend for communication with the database.

### Roadmap

- [ ] Login screen
  - [x] Allow registered users to login into the game
  - [ ] Allow registration of new users
- [ ] Player actions
  - [x] Show a dashboard with statistics for a given player
  - [ ] Upload a C++ file that is compiled by the server and used as an AI for a given player
  - [ ] Change the player's username
- [ ] Gameplay
  - [ ] Replay finished games
    - [x] Manual switching through moves
    - [ ] Automatic replay (moves are automatically switched at specified time intervals)

### Installation instructions

These instructions assume that you have the credentials to a working MYSQL-compatible database and history entries for at least one finished game are present in the database.

###### Preparation steps

1. Open the file `db.sample.php` located in `dev/api`.
2. Replace the values in **UPPERCASE** to match the database configuration in your machine.
3. Save the file as `db.local.php`. **DO NOT modify the original file. It is a sample.**

###### Instructions for using PHP's built-in development WEB server

Usage of PHP's built-in development WEB server avoids having to configure a full WEB server installation, however, Windows users may find it easier to use a real server package, such as XAMPP or WAMP, due to various quirks of running standalone PHP in Windows.

- Instructions for Windows users:
  1. Download the **Zip** of the latest *Thread Safe* PHP release from https://windows.php.net/download/
  2. Extract contents of the archive (this will be your PHP installation) into any folder you like (`C:\Users\<yourname>\php\` for example).
  3. Shift + Right-click the folder where you extracted the PHP installation (double check that `php.exe` is directly in this folder) and select `Copy as path`.
  4. Edit your system's `PATH` environment variable to include the path to the PHP installation (optional but tremendously helpful):
     1. Search the Start menu for "environment variables" and choose "Edit the system environment variables".
     2. Click on `Environment variables...` in the bottom-right of the newly opened window.
     3. Edit the `PATH` in `System variables`: add a semi-colon `;` and paste the path to the PHP installation that you copied. **It is extremely important NOT TO OVERWRITE the PATH variable but to append to it.**
     4. Close the Environment Variables and Advanced System Options windows. The step you have completed allows you to use PHP in the command-line just by typing `php` as opposed to typing the full path (`C:\Users\<yourname>\php\php.exe` in our example).
  5. Navigate to `dev` directory in Windows Explorer (`path\to\battleships\frontend-web\dev`, where `php.ini` resides).
  6. Inside the `dev` directory Shift + Right-Click on empty space and select `Open command window here`.
  7. Type `php -S localhost:8000 -c php.ini` and hit Enter (this assumes that you have added PHP installation directory to your PATH in step 4).
  8. The PHP's local development WEB server is now started. Go to `localhost:8000` in your favorite internet browser to access the WEB frontend.
  9. In order to cleanly stop the server, hit <kbd>Ctrl+C</kbd> in the command prompt and close the window.
- Instructions for Linux users
  1. Install `php` (preferably by using your package manager).
  2. Navigate to `dev` directory in the terminal.
  3. Run `php -S localhost:8000` to start PHP's built-in development WEB server.
  4. Go to `localhost:8000` in your favorite internet browser to access the WEB frontend.
  4. Hit <kbd>CTRL+C</kbd> in terminal to cleanly shut-down the PHP developme