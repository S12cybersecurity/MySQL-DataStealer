# MySQL-DataStealer
Post-Exploitation Tool to Steal MySQL Data, and with persistence extract all data from MySQL table every time that Windows are opened with a Server to receive the extracted Files

The tool is designed to target and extract data from MySQL databases, specifically on Windows systems. It is classified as a post-exploitation tool, which means that it is used after an attacker has already gained unauthorized access to a system.

Once the tool is executed on the compromised Windows machine, it establishes persistence, which means that it sets itself up to run automatically each time the system is rebooted or restarted. It then proceeds to extract data from MySQL tables on the system and sends it to a remote server that has been set up to receive the extracted files. This enables the attacker to continually monitor the database and extract any new information that is added to it, even after the initial attack.

This tool works by copying the target MySQL table to a new table and creating a trigger on the original table. This trigger will insert any new values added to the original table into the new table. This ensures that the attacker has a copy of all data added to the table, including any new data that is added after the initial attack.

To establish persistence, the tool creates a registry key for the current user. This registry key will execute the tool every time the user logs in, ensuring that the tool continues to extract data from the MySQL database.

The tool also sends the extracted file from the copied table to a command-and-control (C2) server, which allows the attacker to remotely access the stolen data.

# POC

Let's start.

First of all you need to transfer to victim machine the sqlop.exe and libmysql.dll in the same path to execute it!

![image](https://user-images.githubusercontent.com/79543461/228654366-eb2973aa-c9d9-43d0-9c48-7fb1672690d3.png)

Execute sqlop.exe, and provided the required information.

Choose Database:

![image](https://user-images.githubusercontent.com/79543461/228654480-34ba8a2d-d8d6-42ce-a0f6-cd417c3230f1.png)

In my case the 8 (wplogin).

![image](https://user-images.githubusercontent.com/79543461/228654544-f3f09c91-d1aa-4cdb-9b16-f3314d6e6f37.png)

Choose table…

![image](https://user-images.githubusercontent.com/79543461/228654585-4b0aa04b-62b6-450d-bb4f-9161b6d4dcdc.png)

Now the new database, table and trigger are created.

Trigger:

![image](https://user-images.githubusercontent.com/79543461/228654640-219ffe6b-ae8f-4494-926d-ae7a9a3adb93.png)

New Database and Table:

![image](https://user-images.githubusercontent.com/79543461/228654681-8e096c17-f73b-4d2d-a227-c2ccbc11483c.png)

Now let's do the second part of attack, you need to follow and do this step to get the extracted data and persistence:

![image](https://user-images.githubusercontent.com/79543461/228654715-f6d97d08-686b-4ec0-978f-806b9f3820b8.png)

Let's do it!

![image](https://user-images.githubusercontent.com/79543461/228654756-6f502943-3fd0-4f2a-9e09-9caef68c7905.png)

Perfect, now provide the required information, the program requires the IP from malicious C2 server to receive the file.

And all the attack its done, now you only need to open your C2 server and waiting for the extracted table.

Now i add new values in wpCredentials table (like if this table represents a Web Login) and the program extract the new data inserted every time!

![image](https://user-images.githubusercontent.com/79543461/228654834-d78495da-49e6-4a50-bcbf-163fee758f84.png)

The trigger works good, now i put my server listener with port 1212…ç

![image](https://user-images.githubusercontent.com/79543461/228654868-26768af7-8a7e-4cb6-8e06-9df69c8313f0.png)

The malware sends it in port 1212, you can't use other.
And i only need to restart the machine:

![image](https://user-images.githubusercontent.com/79543461/228654921-0ec1435e-6793-4469-b56c-92b190ed94ec.png)

Let's check the content…

![image](https://user-images.githubusercontent.com/79543461/228654965-cd50cfc4-0f4d-4c67-9332-cee27b5ef0f3.png)
