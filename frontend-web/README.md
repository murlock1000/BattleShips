# WEB-based frontend

Built with Javascript-based React library and uses PHP as a backend for communication with the database.

### Roadmap

- [ ] Login screen
  - [x] Allow registered users to login into the game
  - [ ] Allow registration of new users
- [ ] Player actions
  - [x] Show a dashboard with statistics for a given player
  - [ ] Upload an AI which is assigned to a player
  - [ ] Change the username of a player
- [ ] Gameplay
  - [ ] Replay finished games
    - [x] Manual switching through moves
    - [ ] Automatic replay (moves are automatically switched at specified time intervals)

### Installation instructions

These instructions assume that you have the credentials to a working MYSQL-compatible database and at least one finished game is present in the database.

1. Duplicate (copy and paste) the file `db.sample.php` in `dev/api` and rename it to `db.local.php`.
2. Open the newly created `db.local.php` and replace the values in **UPPERCASE** to match the configuration of the database in your machine.
3. Instructions for Windows:
   1. Download the **Zip** of the latest *Thread Safe* PHP release from https://windows.php.net/download/
   2. Extract the contents of the archive (this will be your PHP installation) into any folder you like (`C:\Users\<yourname>\php\` for example)
   3. Shift + Right-click the folder, where you extracted the PHP installation (double check that `php.exe` is directly in this folder) and select `Copy as path`
   4. Edit your system's `PATH` environment variable to include the path to the PHP installation (optional but tremendously helpful)
      1. Search the Start menu for "environment variables" and click the option that says something similar to "Edit the system environment variables"
      2. Click on `Environment variables...` in the bottom-right of the newly opened window
      3. Edit the `PATH` in `System variables` and add a semi-colon `;` followed by the path to the PHP installaton (you copied it earlier, so paste it). **IT IS EXTREMELY IMPORTANT NOT TO OVERWRITE THE PATH VARIABLE BUT TO APPEND TO THE END OF IT**
      4. Close the windows of Environment Variables and Advanced System Options. The step you have completed allows you to use PHP in the command-line just by typing `php` as opposed to typing the full path (`C:\Users\<yourname>\php\php.exe` in our example) when you want to start the PHP server
   5. Navigate to `dev` directory in Windows Explorer (`path\to\battleships\frontend-web\dev`, where `php.ini` resides)
   6. Inside the `dev` directory Shift + Right-Click on empty space and select `Open command window here`
   7. Type `php -S localhost:8000 -c php.ini` (this assumes that you have added PHP installation directory to your path in step iv)
   8. The PHP's local development WEB server is now started. You can now go to `localhost:8000` in your web browser to launch the WEB frontend
   9. In order to cleanly stop the server, press <kbd>Ctrl+C</kbd> in the command prompt.
4. Instructions for Linux coming soon