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
			string.Format("AT_NETWORK_ID_%1", Math.RandomFloat(100.00, 999.999).ToString().Hash())
		);
		saveJsonBi.WriteValue(
			"AT_NETWORK_DATETIME",
			string.Format("AT_NETWORK_DATETIME_%1", SCR_DateTimeHelper.GetDateTimeUTC())
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
		restContext.POST(m_jsonCallback, "api/setup", saveJsonBi.ExportToString());
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
			string.Format("AT_NETWORK_REMOTE_ID_%1", id)
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
	void POST(SCR_JsonSaveContext data, bool respond)
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
			string.Format("%1", )
		);
		
		RestContext restContext = GetGame().GetRestApi().GetContext(AT_GLOBALS.server.API_SERVER);
		
		if (!restContext)
		{
			Print("AT_Network_2 POST, Failed, to get RestAPI context. Unable to send data", LogLevel.ERROR);
			return;
		}
		
		restContext.SetHeaders("Content-Type,application/json");
		restContext.POST(m_jsonCallback_NO_RES, "api/sendingdata", data.ExportToString());
		restContext = null;
	}
	//! function to get data-to-send as string with/without getting response from api 
	bool POST_without_response(string str)
	{
		ref SCR_JsonSaveContext saveJsonBi = new SCR_JsonSaveContext();
		saveJsonBi.EnableTypeDiscriminator();
		saveJsonBi.WriteValue(
			"AT_DATA_TO_GIVE",
			string.Format("%1", str)
		);
		
		if (LOGS) Print("AT_Network_2 post without response, sending, string size =" + str.Length(), LogLevel.WARNING);
		
		AT_GLOBALS.server.net_2.POST(saveJsonBi, false);
		
		return true;
	}
	//! Function that returns server's id hash, that is stored
	private string getServerRemoteId()
	{
		//! Read current values 
		SCR_JsonLoadContext readJsonBi = new SCR_JsonLoadContext();
		readJsonBi.LoadFromFile(FILE);
		readJsonBi.EnableTypeDiscriminator();
		string AT_NETWORK_REMOTE_ID = string.Empty;
		readJsonBi.ReadValue("AT_NETWORK_REMOTE_ID", AT_NETWORK_REMOTE_ID);
		return AT_NETWORK_REMOTE_ID;
	}
	//! Function to exchange data with API
	void POST_with_response(string str);

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
