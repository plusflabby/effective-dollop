/* Global Variables */

string m_sAtUiProfileUID = string.Empty;
ref array<string> profileData = new array<string>();
ref AT_EventClass AT_EVENT_CLASS = new AT_EventClass();


ref array<ref vSetVariable> at_scripts;
ref AT_Database atDB;
ref callbackAT variableOne;

class AT_Global
{
	AT_Server server;
	AT_Client client;
	
	void AT_Global()
	{
		if (Replication.IsRunning() == false)
			return;
		
		if (Replication.IsClient())
			client = AT_Client();
			
		if (Replication.IsServer())
			server = AT_Server();
	}
}
void AT_Global();

class AT_Server
{
	void AT_Server()
	{
		atDB = SetUpServerDatabase();
		variableOne = new callbackAT();
		at_scripts = new array<ref vSetVariable>;
	}
}

class AT_Client
{

}

