#include <iostream>
#include <mysql.h>
#include "SQL.h"

using namespace std;

int main() {
    bool con;
    int op;
    string database;
    string table;
    string column;
    string user;
    string password;
    string server;

    cout << "Enter server (Example Recommended: localhost): ";
    cin >> server;
    cout << "Enter user: ";
    cin >> user;
    cout << "Enter password: ";
    cin >> password;
    SQL sql = SQL(server.c_str(), user.c_str(), password.c_str(), "mysql");

    con = sql.connect();
    if (!con) {
        std::cout << "Failed connecting";
    }
    else {
        std::cout << "Connection with database succesfully\n\n";
        std::cout << "Choose Database to exctract";
        database = sql.listDatabases();
        std::cout << "\nChoose Table to exctract:\n";
        table = sql.listTables(database);
        sql.createDatabase(database, table);
        sql.copyData(database, table);
        column = sql.getFirstColumnName();
        sql.createTrigger(database, table);
    }
}
