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

    SQL sql = SQL("localhost", "root", "password", "mysql");

    con = sql.connect();
    if (!con) {
        std::cout << "Failed connecting";
    }
    else {
        std::cout << "Connection succesfully\n\n";
        std::cout << "Database List:\n";
        database = sql.listDatabases();
        table = sql.listTables(database);
        sql.createDatabase(database, table);
        sql.copyData(database, table);
        column = sql.getFirstColumnName();
        cout << "sdad";
        sql.createTrigger(database, table);
        cout << "final";
    }
}
