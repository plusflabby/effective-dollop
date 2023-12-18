class Salts
{
	static string generateSalt()
	{
		// Define characters allowed in the session ID
		string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
		string sessionId = "";
		
		// Define the length of the session ID
		int length = 32;
		
		// Generate a random session ID of the specified length
		for (int i = 0; i < length; i++)
		{
			RandomGenerator randomGenerator = new RandomGenerator();
			int randomIndex = randomGenerator.RandInt(0, length -1);
			sessionId += characters[randomIndex];
		}
		
		return sessionId;
	}
}