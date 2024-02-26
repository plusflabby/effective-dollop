class AT_Network : Managed
{
	// !
	// ! Network Variables (static/non-static)
	// !
	private const string FILE = "$profile:AT_NETWORK.json";
	private const bool LOGS = AT_GLOBALS.server.DEBUG;
	private ref AT_NETWORK_1_RestCallback m_jsonCallback = new AT_NETWORK_1_RestCallback();
	// !
	// ! Network Constructor(s)
	// !
	void AT_Network()
	{
	}
	void ~AT_Network()
	{
	}
	// !
	// ! Network Functions
	// !
	// ! Public function to see if net has been init... for the first time
	bool isSetUp()
	{
		bool check = FileIO.FileExists(FILE);
		if (LOGS) Print("AT_Network network isSetUp=" + check.ToString(), LogLevel.WARNING);
		return check;
	}
	// ! Public function to init... for the first time
	void SetUp()
	{
		SCR_JsonSaveContext saveJsonBi = new SCR_JsonSaveContext();
		saveJsonBi.EnableTypeDiscriminator();
		saveJsonBi.WriteValue(
			"AT_NETWORK_ID",
			string.Format("%1", Math.RandomFloat(100.00, 999.999).ToString().Hash())
		);
		saveJsonBi.WriteValue(
			"AT_NETWORK_DATETIME",
			string.Format("%1", SCR_DateTimeHelper.GetDateTimeUTC())
		);
		saveJsonBi.SaveToFile(FILE);
		
		if (LOGS) Print("AT_Network network SetUp file made/created", LogLevel.WARNING);
		
		RestContext restContext = GetGame().GetRestApi().GetContext(AT_GLOBALS.server.API_SERVER);
		
		if (!restContext)
		{
			Print("AT_Network SetUp, Failed, to get RestAPI context. Unable to send data", LogLevel.ERROR);
			return;
		}
		
		restContext.SetHeaders("Content-Type,application/json");
		restContext.POST(m_jsonCallback, "api/v1/setup", saveJsonBi.ExportToString());
		restContext = null;
	}
	
	//! Identifying game
	void SaveRemoteId(string id)
	{
		//! Read current values 
		SCR_JsonLoadContext readJsonBi = new SCR_JsonLoadContext();
		readJsonBi.LoadFromFile(FILE);
		readJsonBi.EnableTypeDiscriminator();
		string AT_NETWORK_DATETIME = string.Empty;
		readJsonBi.ReadValue("AT_NETWORK_DATETIME", AT_NETWORK_DATETIME);
		string AT_NETWORK_ID = string.Empty;
		readJsonBi.ReadValue("AT_NETWORK_ID", AT_NETWORK_ID);
		
		// Write values 
		SCR_JsonSaveContext saveJsonBi = new SCR_JsonSaveContext();
		saveJsonBi.EnableTypeDiscriminator();
		saveJsonBi.WriteValue(
			"AT_NETWORK_REMOTE_ID",
			string.Format("%1", id)
		);
		saveJsonBi.WriteValue(
			"AT_NETWORK_ID",
			AT_NETWORK_ID
		);
		saveJsonBi.WriteValue(
			"AT_NETWORK_DATETIME",
			AT_NETWORK_DATETIME
		);
		saveJsonBi.SaveToFile(FILE);
	}
}

class AT_NETWORK_1_RestCallback: RestCallback
{
	//------------------------------------------------------------------------------------------------
    override void OnError(int errorCode)
    {
        if (AT_GLOBALS.server.DEBUG) Print("AT_NETWORK_1_RestCallback Sending data has failed", LogLevel.ERROR);
    };
 
	//------------------------------------------------------------------------------------------------
    override void OnTimeout()
    {
        if (AT_GLOBALS.server.DEBUG) Print("AT_NETWORK_1_RestCallback Sending data has timed out", LogLevel.ERROR);
    };
 
	//------------------------------------------------------------------------------------------------
    override void OnSuccess(string data, int dataSize)
    {
		if (AT_GLOBALS.server.DEBUG) Print("AT_NETWORK_1_RestCallback request_size=" + dataSize);
		
		SCR_JsonLoadContext dataReader = new SCR_JsonLoadContext(true);
		dataReader.ImportFromString(data);
		
		//! Read success variable
		bool API_Response_Success = false;
		dataReader.ReadValue("success", API_Response_Success);
		if (AT_GLOBALS.server.DEBUG) Print("AT_NETWORK_1_RestCallback response_success=" + API_Response_Success.ToString(), LogLevel.WARNING);
        
		//! Read AT_REMOTE_ID variable 
		string API_Response_AT_REMOTE_ID = string.Empty;
		dataReader.ReadValue("AT_REMOTE_ID", API_Response_AT_REMOTE_ID);
		if (AT_GLOBALS.server.DEBUG) Print("AT_NETWORK_1_RestCallback response_AT_REMOTE_ID=" + API_Response_AT_REMOTE_ID, LogLevel.WARNING);
        
		//! Save AT_REMOTE_ID
		AT_GLOBALS.server.network.SaveRemoteId(API_Response_AT_REMOTE_ID);
    };
};



//! Network part 2 
/*


//! 
//! returns response uuid or "success"

... if resppnse is needed a callque will be made to get the data?
*/

//! Class overall will send/exchange data to the api
class AT_Network_2 : Managed
{
	// !
	// ! Network Variables (static/non-static)
	// !
	private const string FILE = "$profile:AT_NETWORK.json";
	private const bool LOGS = AT_GLOBALS.server.DEBUG;
	private ref AT_NETWORK_2_RestCallback_NR m_jsonCallback_NO_RES = new AT_NETWORK_2_RestCallback_NR();
	// !
	// ! Network Constructor(s)
	// !
	void AT_Network_2()
	{
	}
	void ~AT_Network_2()
	{
	}
	// !
	// ! Network Functions
	// !
	//! function will send a post to /api/sendingdata
	void POST(SCR_JsonSaveContext data, API_Post_Types type = API_Post_Types.DEBUG)
	{
		//! Add server network id to request
		string Net_ID = AT_GLOBALS.server.net_2.getServerRemoteId();
		if (Net_ID.IsEmpty())
		{
			Print("AT_Network_2 POST, Failed, to get Remote Network Id. Unable to send data", LogLevel.ERROR);
			return;
		}
		data.WriteValue(
			"AT_NETWORK_REMOTE_ID",
			string.Format("%1", Net_ID)
		);
		
		//! Add the type of request
		data.WriteValue(
			"AT_DATA_TYPE",
			string.Format("%1", type)
		);
		RestContext restContext = GetGame().GetRestApi().GetContext(AT_GLOBALS.server.API_SERVER);
		
		if (!restContext)
		{
			Print("AT_Network_2 POST, Failed, to get RestAPI context. Unable to send data", LogLevel.ERROR);
			return;
		}
		
		restContext.SetHeaders("Content-Type,application/json");
		restContext.POST(m_jsonCallback_NO_RES, "api/v1/sendingdata", data.ExportToString());
		restContext = null;
	}
	
	//! function to get data-to-send as string with/without getting response from api 
	bool POST_without_response(string str, API_Post_Types type = API_Post_Types.DEBUG)
	{
		ref SCR_JsonSaveContext saveJsonBi = new SCR_JsonSaveContext();
		saveJsonBi.EnableTypeDiscriminator();
		saveJsonBi.WriteValue(
			"AT_DATA_TO_GIVE",
			string.Format("%1", str)
		);
		
		if (LOGS) Print("AT_Network_2 post without response, sending, string size =" + str.Length(), LogLevel.WARNING);
		
		AT_GLOBALS.server.net_2.POST(saveJsonBi, type);
		
		return true;
	}
	//! Function that returns server's id hash, that is stored
	string getServerRemoteId()
	{
		//! Read current values 
		SCR_JsonLoadContext readJsonBi = new SCR_JsonLoadContext();
		readJsonBi.LoadFromFile(FILE);
		readJsonBi.EnableTypeDiscriminator();
		string AT_NETWORK_REMOTE_ID = string.Empty;
		readJsonBi.ReadValue("AT_NETWORK_REMOTE_ID", AT_NETWORK_REMOTE_ID);
		return AT_NETWORK_REMOTE_ID;
	}
	
	//! post req with response
	string POST_SYNC(SCR_JsonSaveContext data, API_Post_Response_Types type = API_Post_Response_Types.DEBUG)
	{
		//! Add server network id to request
		string Net_ID = AT_GLOBALS.server.net_2.getServerRemoteId();
		if (Net_ID.IsEmpty())
		{
			Print("AT_Network_2 POST, Failed, to get Remote Network Id. Unable to send data", LogLevel.ERROR);
			return string.Empty;
		}
		data.WriteValue(
			"AT_NETWORK_REMOTE_ID",
			string.Format("%1", Net_ID)
		);
		
		//! Add the type of request
		data.WriteValue(
			"AT_DATA_TYPE",
			string.Format("%1", type)
		);
		RestContext restContext = GetGame().GetRestApi().GetContext(AT_GLOBALS.server.API_SERVER);
		
		if (!restContext)
		{
			Print("AT_Network_2 POST, Failed, to get RestAPI context. Unable to send data", LogLevel.ERROR);
			return string.Empty;
		}
		
		
		restContext.SetHeaders("Content-Type,application/json");
		return restContext.POST_now("api/v1/sendingdata", data.ExportToString());
	}
	
	//! Function to exchange data with API
	void POST_with_response(string str, API_Post_Response_Types type = API_Post_Response_Types.DEBUG)
	{
		ref SCR_JsonSaveContext saveJsonBi = new SCR_JsonSaveContext();
		saveJsonBi.EnableTypeDiscriminator();
		saveJsonBi.WriteValue(
			"AT_DATA_TO_GIVE",
			string.Format("%1", str)
		);
		
		if (LOGS) Print("AT_Network_2 post response, sending, string size =" + str.Length(), LogLevel.WARNING);
		
		string resposne = AT_GLOBALS.server.net_2.POST_SYNC(saveJsonBi, type);
		
		SCR_JsonLoadContext dataReader = new SCR_JsonLoadContext(true);
		dataReader.ImportFromString(resposne);
		
		//! Read success variable
		bool API_Response_Success = false;
		dataReader.ReadValue("success", API_Response_Success);
		if (AT_GLOBALS.server.DEBUG) Print("AT_NETWORK_2_RestCallback response_success=" + API_Response_Success.ToString(), LogLevel.WARNING);
		
		// Read str variable
		string API_Response_Str = "";
		dataReader.ReadValue("str", API_Response_Str);
		Print(string.Format("API RESPONSE=%1", API_Response_Str));
	}

}

//! Class with response data not used
class AT_NETWORK_2_RestCallback_NR: RestCallback
{
	//------------------------------------------------------------------------------------------------
    override void OnError(int errorCode)
    {
        if (AT_GLOBALS.server.DEBUG) Print("AT_NETWORK_2_RestCallback_NR Sending data has failed", LogLevel.ERROR);
    };
 
	//------------------------------------------------------------------------------------------------
    override void OnTimeout()
    {
        if (AT_GLOBALS.server.DEBUG) Print("AT_NETWORK_2_RestCallback_NR Sending data has timed out", LogLevel.ERROR);
    };
 
	//------------------------------------------------------------------------------------------------
    override void OnSuccess(string data, int dataSize)
    {
		if (AT_GLOBALS.server.DEBUG) Print("AT_NETWORK_2_RestCallback_NR request_size=" + dataSize);
		
		SCR_JsonLoadContext dataReader = new SCR_JsonLoadContext(true);
		dataReader.ImportFromString(data);
		
		//! Read success variable
		bool API_Response_Success = false;
		dataReader.ReadValue("success", API_Response_Success);
		if (AT_GLOBALS.server.DEBUG) Print("AT_NETWORK_2_RestCallback_NR response_success=" + API_Response_Success.ToString(), LogLevel.WARNING);
		
    };
};

enum API_Post_Types
{
	DEBUG,
	PlayerList
}

enum API_Post_Response_Types
{
	DEBUG
}

// ! Network part 3 - Getting task(s) from API and executing them 
class AT_Network_3 : Managed
{
	// !
	// ! Network Variables (static/non-static)
	// !
	private const string FILE = "$profile:AT_NETWORK.json";
	private const bool LOGS = AT_GLOBALS.server.DEBUG;
	// !
	// ! Network Constructor(s)
	// !
	void AT_Network_3()
	{
	}
	void ~AT_Network_3()
	{
	}
	// !
	// ! Network Functions
	// !
	// ! Get task(s) from API 
	void getTaskList()
	{
		//! Get net remote id 
		string network_id = AT_GLOBALS.server.net_2.getServerRemoteId();
		
		RestContext restContext = GetGame().GetRestApi().GetContext(AT_GLOBALS.server.API_SERVER);
		
		if (!restContext)
		{
			Print("AT_Network_3 SetUp, Failed, to get RestAPI context. Unable to send data", LogLevel.ERROR);
			return;
		}
		
		string resposne = restContext.GET_now("api/v1/task?s=" + network_id);
		//Print(resposne, LogLevel.ERROR);
		
		SCR_JsonLoadContext dataReader = new SCR_JsonLoadContext(true);
		dataReader.ImportFromString(resposne);
		
		//! Read tasks variable
		array<string> API_Response_Tasks = new array<string>();
		dataReader.ReadValue("tasks", API_Response_Tasks);
		
		foreach (string task : API_Response_Tasks)
		{
			//Print(task, LogLevel.ERROR);
			SCR_JsonLoadContext taskReader = new SCR_JsonLoadContext(true);
			taskReader.ImportFromString(task);
			//! Read username variable
			string username = string.Empty;
			taskReader.ReadValue("username", username);
			//! Read password variable
			string password = string.Empty;
			taskReader.ReadValue("password", password);
			//! Read task execute variable
			string taskExec = string.Empty;
			taskReader.ReadValue("task", taskExec);
			
			// verify username & password 
			if (!Account.validate(password))
			{
				if (LOGS) Print("AT_Network_3 failed to validate password", LogLevel.ERROR);
				continue;
			}
			if (!Password_Storage.usernameExists(username))
			{
				if (LOGS) Print("AT_Network_3 failed to find username", LogLevel.ERROR);
				continue;
			}		
			if (!Password_Storage.compareForLogin(username, password))
			{
				if (LOGS) Print("AT_Network_3 failed to compare user & pass", LogLevel.ERROR);
				continue;
			}
			
			// Execute a task
			AT_GLOBALS.server.net_3.executeTask(taskExec);
		}
	}
	
	// ! Execute a single task (ex. param = "KICK;1;")
	void executeTask(string task_str)
	{
		array<string> variables = new array<string>();
		task_str.Split(";", variables, true);
		string action = variables.Get(0);
		string playerId = variables.Get(1);
		string playerNetId = variables.Get(2);
		Print(variables, LogLevel.ERROR);
		Print(action, LogLevel.ERROR);
		Print(playerId, LogLevel.ERROR);
		Print(playerNetId, LogLevel.ERROR);
		
//		int playerIdToUse = playerId;
//		
//		if (!playerNetId.IsEmpty())
//		{
//			GetGame().GetPlayerManager().GetPlayer
//		}
		
		switch(action)
		{
			case "KICK":
					GetGame().GetPlayerManager().KickPlayer(playerId.ToInt(), PlayerManagerKickReason.KICK);
				break;
	
			default:
				Print(string.Format("AT_Network_3 action(%1) is not valid", action), LogLevel.ERROR);
				break;
		}
		return;
	}
}
