# MySQL-DataStealer
Post-Exploitation Tool to Steal MySQL Data, and with persistence extract all data from MySQL table every time that Windows are opened with a Server to receive the extracted Files

The tool is designed to target and extract data from MySQL databases, specifically on Windows systems. It is classified as a post-exploitation tool, which means that it is used after an attacker has already gained unauthorized access to a system.

Once the tool is executed on the compromised Windows machine, it establishes persistence, which means that it sets itself up to run automatically each time the system is rebooted or restarted. It then proceeds to extract data from MySQL tables on the system and sends it to a remote server that has been set up to receive the extracted files. This enables the attacker to continually monitor the database and extract any new information that is added to it, even after the initial attack.

This tool works by copying the target MySQL table to a new table and creating a trigger on the original table. This trigger will insert any new values added to the original table into the new table. This ensures that the attacker has a copy of all data added to the table, including any new data that is added after the initial attack.

To establish persistence, the tool creates a registry key for the current user. This registry key will execute the tool every time the user logs in, ensuring that the tool continues to extract data from the MySQL database.

The tool also sends the extracted file from the copied table to a command-and-control (C2) server, which allows the attacker to remotely access the stolen data.
