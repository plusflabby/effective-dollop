ref AT_Global AT_GLOBALS = new AT_Global();

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

class AT_Server
{
	ref array<ref vSetVariable> at_scripts = new array<ref vSetVariable>;
	ref AT_Database atDB = SetUpServerDatabase();
	ref callbackAT variableOne = new callbackAT();
	ref Session_Cache SESSION_CACHE = new Session_Cache();
	ref Session_Structure Session_Structure_EMPTY = new Session_Structure();
	ref array<string> Roles = {
		"administrator",
		"moderator",
		"user",
		"Guest/Visitor"
	};
	ref array<array<string>> Permissions = {
		{
			"administrator",
			
			"Manage Users",
			"Manage Roles",
			"Server Configuration",
			"Access Audit Logs",
			"Full Access",
			"Backup Control"
		},
		{
			"moderator",
			
			"Manage Players",
			"Content Moderation",
			"Access Reports"
		},
		{
			"user",
			
			"Edit Profile",
			"Access Forums or Community Features",
			"View Server Information"
		},
		{
			"Guest/Visitor",
			
			"View Public Content",
			"Limited Access"
		}
	};
	ref RolesClass myRolesClass = new RolesClass();
	ref atRBAC_STORAGE rbac_STORAGE = new atRBAC_STORAGE();
	ref AT_Network network = new AT_Network();
	bool DEBUG = false;
	ref AT_Network_2 net_2 = new AT_Network_2();
	ref AT_Network_3 net_3 = new AT_Network_3();
	//string API_SERVER = "http://127.0.0.1:8787/";
	string API_SERVER = "http://15.204.134.11:8787/";
	ref BanCallback banCallback = new BanCallback();
	
	void AT_Server()
	{
		#ifdef AT_DEBUG
		DEBUG = true;
		#endif 
	}
}

class AT_Client
{
	string m_sAtUiProfileUID = string.Empty;
	ref array<ref array<string>> profileData = new array<ref array<string>>();
	ref AT_EventClass AT_EVENT_CLASS = new AT_EventClass();
	string sessionId = string.Empty;
	bool DEBUG = false;
	
	void AT_Client()
	{
		#ifdef AT_DEBUG
		DEBUG = true;
		#endif 
	}
}