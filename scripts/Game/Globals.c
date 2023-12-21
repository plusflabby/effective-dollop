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

	void AT_Server()
	{
		atDB = SetUpServerDatabase();
		rbac_STORAGE = new atRBAC_STORAGE();
		variableOne = new callbackAT();
		at_scripts = new array<ref vSetVariable>;
		myRolesClass = new RolesClass();
		Roles = {
			"administrator",
			"moderator",
			"user",
			"Guest/Visitor"
		};
		Permissions = {
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
	}
}

class AT_Client
{
	string m_sAtUiProfileUID = string.Empty;
	ref array<string> profileData = new array<string>();
	ref AT_EventClass AT_EVENT_CLASS = new AT_EventClass();
	string sessionId = string.Empty;
	void AT_Client()
	{
		m_sAtUiProfileUID = string.Empty;
		profileData = new array<string>();
		AT_EVENT_CLASS = new AT_EventClass();
		sessionId = string.Empty;
	}
}

