/*
Upon successful authentication, generate a session identifier (session token or session ID) for the user. Store this session identifier securely.
*/

class Session_Init
{
	string mySessionId;
	//string myAssignedRole;
	
	void Session_Init()
	{
		mySessionId = generateSessionId();
	}
	
	void ~Session_Init()
	{
		mySessionId = "";
	}
	
	private string generateSessionId()
	{
		// Define characters allowed in the session ID
		string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
		string sessionId = "";
		
		// Define the length of the session ID
		int length = 20;
		
		// Generate a random session ID of the specified length
		for (int i = 0; i < length; i++)
		{
			RandomGenerator randomGenerator = new RandomGenerator();
			int randomIndex = randomGenerator.RandInt(0, length -1);
			sessionId += characters[randomIndex];
		}
		
		return sessionId;
	}
	
	//! return will be the role saved to username's profile 
	string getRoleFromUsername(string username)
	{
		return AT_GLOBALS.server.rbac_STORAGE.get(username);
	}
}