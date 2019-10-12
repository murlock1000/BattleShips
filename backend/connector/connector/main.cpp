#include <iostream>
#include <windows.h>
#include <mysql.h>
#include <sstream>
#include <typeinfo>

using namespace std;

int PassQuery(MYSQL * conn, string query) {
    int qstate = 0;
    const char * q = query.c_str();
    qstate = mysql_query(conn, q);

    return qstate;
}
int Login(MYSQL * conn, string name) {

    stringstream ss; //setting up a query to the database
    MYSQL_RES * res;

    ss << "SELECT connected FROM users WHERE username ='" << name << "' ";

    string query = ss.str();
    ss.str("");
    if (PassQuery(conn, query) == 0) { //proccessing query
        res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);

        if (row == 0) {
            cout << "No such user" << endl;
            return 0;
        }

        if ( * row[0] == '0') {
            mysql_free_result(res);

            ss << "UPDATE users SET connected=1 WHERE username ='" << name << "' "; //connecting
            query = ss.str();
            ss.str("");
            if (PassQuery(conn, query) == 0) {

                cout << "CONNECTED" << endl;

                ss << "SELECT idusers FROM users WHERE username='" << name << "'";
                cout << "Login query: " << ss.str() << endl;
                query = ss.str();
                if (PassQuery(conn, query) != 0) {
                    cout << "Exception: " << mysql_error(conn) << endl;
                    return 0;
                }
                res = mysql_store_result(conn);
                row = mysql_fetch_row(res);
                cout << "Data: " << row[0] << " " << atoi(row[0]) << endl;
                return atoi(row[0]);

            } else {
                cout << "Exception: " << mysql_error(conn) << endl;
            }
        } else {
            cout << "There is already a connection" << endl;
        }

        mysql_free_result(res); //clearing the query result

    } else {
        cout << "Exception: " << mysql_error(conn) << endl;
    }

    return 0;

}

int Register(MYSQL * conn, string name) {

    int qstate = 0;

    stringstream ss;

    ss << "INSERT INTO users(username) VALUES('" << name << "') ";
    string query = ss.str();
    ss.str("");
    if (PassQuery(conn, query) == 0) {

        cout << "Registration complete." << endl;

        return Login(conn, name);
    } else {
        if (mysql_errno(conn) == 1062) {
            cout << "Username already taken." << endl;
        } else {
            cout << "Exception: " << mysql_error(conn) << endl;
        }
    }
    return 0;
}

/*void SelectExample(MYSQL* conn, string name){

    stringstream ss;

    ss<< "SELECT userID FROM users WHERE username ='" <<name<<"' ";
    string query = ss.str();

    if(PassQuery(conn,query)==0){
        MYSQL_RES *a = mysql_store_result(conn);
        cout<<"Login complete. "<<mysql_field_count(conn)<<" "<<a->current_field<<endl;
        mysql_free_result(a);

    }else{
        cout<<"Exception: "<<mysql_error(conn)<<endl;
    }

}
*/

void Disconnect(MYSQL * conn, string name) {

    stringstream ss;
    string query;
    MYSQL_RES * res;

    ss << "UPDATE users SET connected=0 WHERE username ='" << name << "' ";
    query = ss.str();
    PassQuery(conn, query);
    if (PassQuery(conn, query) == 0) {
        cout << "DISCONNECTED" << endl;
    } else {
        cout << "Exception: " << mysql_error(conn) << endl;
    }

}

void ShowLobby(MYSQL * conn) {
    stringstream ss;
    ss << "SELECT idlobbies, lobby_name, description, admin_id FROM lobbies WHERE lobby_status ='w'";
    string query = ss.str();
    if (PassQuery(conn, query) == 0) {
        MYSQL_RES * res = mysql_store_result(conn);
        MYSQL_ROW row; // = mysql_fetch_row(res);

        for (int i = 0; i < res -> row_count; i++) {
            row = mysql_fetch_row(res);
            cout << row[0] << " " << row[1] << " " << row[2] << " " << row[3] << endl;
        }
    } else {
        cout << "Exception: " << mysql_error(conn) << endl;
    }

}

int CreateLobby(MYSQL * conn, string name, string description) {
    stringstream ss;
    ss << "INSERT INTO lobbies (lobby_name,description,admin_id,lobby_status) VALUES ('" << name << " lobby','" << description << "', (SELECT idusers FROM users WHERE username='" << name << "'),'w')";

    string query = ss.str();
    ss.str("");
    if (PassQuery(conn, query) == 0) {

        cout << "Lobby created" << endl;
        ss << "SELECT LAST_INSERT_ID()";
        query = ss.str();
        if (PassQuery(conn, query) != 0) {
            cout << "EXCEPTION: " << mysql_error(conn) << endl;
            return -1;
        }
        MYSQL_RES * res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);

        return atoi(row[0]);
    } else {
        cout << "Exception: " << mysql_error(conn) << endl;
    }
    return -1;
}

int LeaveLobby(MYSQL * conn, int id) {

    stringstream ss;
    ss << "DELETE FROM lobbies WHERE idlobbies=" << id;

    string query = ss.str();
    ss.str("");
    if (PassQuery(conn, query) == 0) {

        cout << "Lobby erased" << endl;
        return -1;
    } else {
        cout << "Exception: " << mysql_error(conn) << endl;
    }
    return id;

}

int JoinLobby(MYSQL * conn, string lobbyid, int userid) {

    stringstream ss;
    ss << "UPDATE lobbies SET user_id=" << userid << ",lobby_status='r' WHERE idlobbies =" << lobbyid; //connecting

    string query = ss.str();
    ss.str("");
    if (PassQuery(conn, query) == 0) {

        cout << "Entered The lobby and waiting for start" << endl;

        ///START READING INPUTS, OUTPUTS IN USER TABLE!
        ss.str(lobbyid);
        int ret = 0;
        ss >> ret;
        //cout<<"RETURNINAM "<<ret<<endl;
        return ret;
    } else {
        cout << "Exception: " << mysql_error(conn) << endl;
    }
    return -1;

}

string GameLogic(string consoleOutput, string gameStatus){
///YOUR STRATEGY
return "0 0";
}

void MainGame(MYSQL * conn, int lobbyid, int userid) {

    bool inGame = true;
    stringstream ss;
    string consoleOutput = "";
    string gameStatus = "";
    ss << "SELECT console_output, game_status FROM users WHERE idusers =" << userid;

    string query = ss.str();
    ss.str("");
    string response;
    while (inGame) {
        if (PassQuery(conn, query) == 0) {

            MYSQL_RES * res = mysql_store_result(conn);
            MYSQL_ROW row;
            for (int i = 0; i < res -> row_count; i++) {
                consoleOutput = "";
                gameStatus = "";
                row = mysql_fetch_row(res);

                if (row[0] == NULL) {
                    //  cout<<"NULL ";
                } else {
                    consoleOutput = row[0];
                    // cout<<consoleOutput<<" ";
                }
                if (row[1] == NULL) {
                    //  cout<<"NULL"<<endl;
                } else {
                    gameStatus = row[1];
                    //  cout<<gameStatus<<endl;
                }

                if(gameStatus == "userid"){
                response = GameLogic(consoleOutput, gameStatus);   ///READ OUTPUTS AND GIVE INPUT

                ss.str("");

                ss<<"UPDATE users SET user_input=" << response << ",game_status='C' WHERE idusers =" << userid; //connecting

                query = ss.str();
                if (PassQuery(conn, query) == 0) {
                        cout<<"accepted answer"<<endl;
                } else {
                cout << "Exception: " << mysql_error(conn) << endl;
                }

                }
                if (gameStatus == "F") {
                    inGame = false;
                    cout<<"GAME OVER YOU WON or not"<<endl;
                }
            }
        } else {
            cout << "Exception: " << mysql_error(conn) << endl;
        }

    }
}
/*void test(MYSQL* conn, int lobbyid, int userid){

    stringstream ss;
    string consoleOutput="";
    string gameStatus="";
    ss<<"SELECT console_output FROM users WHERE idusers =89";

    string query = ss.str();
    ss.str("");

        if(PassQuery(conn,query)==0){

        MYSQL_RES *res = mysql_store_result(conn);
        cout<<"veika"<<endl;
        MYSQL_ROW row = mysql_fetch_row(res);
        cout<<"veik0"<<endl;

            cout<<row<<" AHA "<<mysql_num_fields(res)<< endl;
            if(row[0]==NULL){
                cout<<"GOT IT"<<endl;
            }
           // cout<<row[0]<<endl;
            cout<<"wut"<<endl;

    }
    else{
    cout<<"Exception: "<<mysql_error(conn)<<endl;
    }
}
*/
int main() {
    bool running = true;
    string input;
    string username; // = "test";
    int userID = -1;
    int inLobby = -1;

    MYSQL * conn;
    conn = mysql_init(0);
    conn = mysql_real_connect(conn, "192.168.0.142", "admin", "admin", "battleships", 0, NULL, 0);

    if (conn) {
        cout << "Connected." << endl;
    } else {
        cout << "Failed To connect, " << mysql_error(conn) << endl;
    }

    while (running) {
        cin >> input;

        if (input == "quit") {
            running = 0;
            if (userID) {

                if (inLobby != -1) {
                    LeaveLobby(conn, inLobby);
                }
                Disconnect(conn, username);
            }
        }
        if (input == "register") {
            if (userID == -1) {
                cin >> input;
                userID = Register(conn, input);
                if (userID != -1) {
                    username = input;
                }
            } else {
                cout << "Already Connected" << endl;
            }
        }

        if (input == "login") {
            if (userID == -1) {
                cin >> input;
                userID = Login(conn, input);
                if (userID != -1) {
                    username = input;
                }
            } else {
                cout << "Already Connected" << endl;
            }
        }

        if (input == "list") {
            cout << "showing lobbies" << endl;
            ShowLobby(conn);
        }

        if (input == "create") {
            if (userID) {
                if (inLobby != -1) {
                    cout << "Already in a lobby" << endl;
                } else {
                    cout << "creating lobby, fill in description: " << endl;
                    cin >> input;
                    inLobby = CreateLobby(conn, username, input);
                }
            } else {
                cout << "Connect first" << endl;
            }

        }

        if (input == "leave") {
            if (inLobby == -1) {
                cout << "Not in a lobby" << endl;
            } else {
                inLobby = LeaveLobby(conn, inLobby);
            }
        }

        if (input == "info") {
            cout << username << " " << userID << " " << inLobby << endl;
        }

        if (input == "join") {
            //  cout<<conn<<" "<<inLobby<<" "<<userID<<endl;
            //   test(conn,inLobby,userID);
            if (userID == 0) {
                cout << "Connect first" << endl;

            } else {
                if (inLobby != -1) {
                    cout << "Already in a lobby" << endl;
                    break;
                } else {

                    cin >> input;

                    inLobby = JoinLobby(conn, input, userID);
                    if (inLobby != -1) {
                        MainGame(conn, inLobby, userID);
                    }
                }
            }

        }
    }

    mysql_close(conn);
    return 0;
}
