class AT_Global
{
	ref AT_Server server;
	ref static AT_Client client;
	
	void AT_Global()
	{
		server = new AT_Server();
		client = new AT_Client();
	}
}
ref AT_Global AT_GLOBALS = new AT_Global();

class AT_Server
{
	ref array<ref vSetVariable> at_scripts = new array<ref vSetVariable>;
	ref AT_Database atDB = SetUpServerDatabase();
	ref callbackAT variableOne = new callbackAT();
	void AT_Server()
	{
		atDB = SetUpServerDatabase();
		variableOne = new callbackAT();
		at_scripts = new array<ref vSetVariable>;
	}
}

class AT_Client
{
	string m_sAtUiProfileUID = string.Empty;
	ref array<string> profileData = new array<string>();
	ref AT_EventClass AT_EVENT_CLASS = new AT_EventClass();
	void AT_Client()
	{
		m_sAtUiProfileUID = string.Empty;
		profileData = new array<string>();
		AT_EVENT_CLASS = new AT_EventClass();
	}
}

