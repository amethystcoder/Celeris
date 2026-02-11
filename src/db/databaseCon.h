#ifndef DATABASECON_H
#define	DATABASECON_H

#include <iostream>
//#include <sql.h>
//#include <sqlext.h>

enum MyEnum
{
	Rel,
	NonRel,
	cache
};

class DatabaseCon
{
public:
	DatabaseCon();
	~DatabaseCon();

	DatabaseCon(const DatabaseCon& rhs) = delete;
	DatabaseCon operator= (const DatabaseCon& rhs) = delete;

	//void connectToDatabase(const std::string& dbType, const std::string& connectionString);

	//void runQuery(SQLHDBC dbc, const char* query);

	//database types


	//functions to connect to different databases


private:

};

#endif // !DATABASECON_H
