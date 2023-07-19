1. Download Flyway CLI [here](https://documentation.red-gate.com/fd/command-line-184127404.html?_ga=2.139774259.1044514967.1689776764-1493925904.1689680977)

2. Extract the folder in the project repository.

3. To be able to test flyway on a local SQL DB, you need to do the following:
	- download jTDS driver from [here](https://github.com/milesibastos/jTDS/releases). It is an unofficial fork of the jTDS driver that supports local SQL server.
	- go to folder `drivers`, and replace the `jtds-1.3.1.jar` with the downloaded driver

4. Go to folder `conf`, open file `flyway.conf` and we'll start configure flyway for a local SQL server.
	- put `flyway.url=jdbc:jtds:sqlserver://./TestVersioning;instance=LOCALDB#397ED88A;namedPipe=true` as the name where
		- `TestVersioning` is the name of the database that we want to connect
		- `LOCALDB#397ED88A` is the full server name located in Properties of the database -> Permissions -> Server name
	- add `flyway.user` and `flyway.password` (Note: `flyway.user` is the login of the username). If you want to add a new user, you can use this script

        ```sql
        create login minhlogin with password = '123456789'
        go

        create user minh for login minhlogin
        go

        exec sp_addrolemember 'db_owner', 'minh'
        ```

	- add `flyway.validateMigrationNaming=true` so that validation of the sql script names is done when migration is done

5. Create an sql file that will create out first table
	
	- we'll name it `V1__CreateProductTable.sql`. It's important to keep this naming convention as flyway uses it to properly version the db. More information about naming can be found [here](https://www.red-gate.com/blog/database-devops/flyway-naming-patterns-matter)
	- we'll add following sql

        ```sql
        CREATE TABLE PRODUCT
        (
        ID INT,
        PRODUCT_NAME VARCHAR(100)
        )
        ```
6. Run `flyway migrate`. The db should now be versioned correctly and a new table `flyway_schema_history` should be created
