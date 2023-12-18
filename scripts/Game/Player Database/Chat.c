modded class SCR_ChatPanel
{
	override void SendMessage()
	{
		super.SendMessage();
		
		string message;
		if (m_Widgets.m_MessageEditBox)
			message = m_Widgets.m_MessageEditBox.GetText();
		else
			return;
		
		string biUid = AT_MainStatic.getUid(GetGame().GetPlayerController().GetPlayerId());
		DB_Chat playerChat = AT_Chat.getChat(biUid);
		Print(playerChat.message);
		AT_Chat.updateChat(message, playerChat);
		if (playerChat.profileDbId.IsEmpty())
		{
			AT_Chat.linkChatToProfile(playerChat, biUid_ : biUid);
		}
	}
}

class DB_Chat : EDF_DbEntity
{
	string biUid = string.Empty;
	string firstDateTime = string.Empty;
	ref array<string> message = new array<string>();
	string profileDbId = string.Empty;
	
	void generate(string biUid_)
	{
		SetId(EDF_DbEntityIdGenerator.Generate());
		biUid = biUid_;
		firstDateTime = SCR_DateTimeHelper.GetDateTimeLocal();
	}
}

//class DB_Chat_Message
//{
//	string txt = string.Empty;
//	string dateTime = string.Empty;
//}

class AT_Chat
{
	private static typename type = String("DB_Chat").ToType();
	
	//! Get chat for player
	static DB_Chat getChat(string biUid)
	{
		Print("123");
		DB_Chat returnValue = new DB_Chat();
		Print("1234"+returnValue.ToString());
		returnValue.generate(biUid);
		Print("1235"+returnValue.ToString());
		PDI_Result searchProfile = PlayerDatabaseIntergration.findPlayerProfile(biUid, 1);
		Print("1236"+returnValue.ToString());
		switch (searchProfile.result_code)
		{
			case PDI_Results.SUCCESS:
			{
				Print("1237"+returnValue.ToString());
				string chatDbId = searchProfile.player.m_sPlayerChatsDbId;
				
				if (chatDbId.IsEmpty())
					break;
				
				Print("123888"+returnValue.ToString());
				Print(chatDbId.IsEmpty().ToString());
				Print(AT_GLOBALS.server.atDB.FindAll(type, EDF_DbFind.Id().Equals(chatDbId)).ToString());
				Print(AT_GLOBALS.server.atDB.FindAll(type, EDF_DbFind.Id().Equals(chatDbId)).GetEntities().ToString());
				Print(AT_GLOBALS.server.atDB.FindAll(type, EDF_DbFind.Id().Equals(chatDbId)).GetEntities().Get(0));
				return DB_Chat.Cast(AT_GLOBALS.server.atDB.FindAll(type, EDF_DbFind.Id().Equals(chatDbId)).GetEntities().Get(0));
					
				break;
			}
		}
				Print("1239"+returnValue.ToString());
		return returnValue;
	};
	
	//! Update player's chat with a new message
	static void updateChat(string msg, DB_Chat chat)
	{
		//DB_Chat_Message chatMsg = new DB_Chat_Message();
		//chatMsg.txt = msg;
		//chatMsg.dateTime = SCR_DateTimeHelper.GetDateTimeLocal();
		chat.message.Insert(msg);
		AT_DB.AddOrUpdateChat(chat);
	}
	
	//! Link player's profile to player's chat 
	static void linkChatToProfile(DB_Chat chat = null/*, DB_PlayerProfile profile = null*/, string biUid_ = string.Empty)
	{
//		if (profile)
//			return;
		
		PDI_Result searchProfile = PlayerDatabaseIntergration.findPlayerProfile(biUid_, 1);
		if (searchProfile.result_code != PDI_Results.SUCCESS)
			return;
		
		DB_PlayerProfile profileCurrent = searchProfile.player;
		//! Update profile with chat's dbid
		profileCurrent.m_sPlayerChatsDbId = chat.GetId();
		AT_DB.AddOrUpdateProfile(profileCurrent);
		//! Update chat with profile's dbid 
		chat.profileDbId = profileCurrent.GetId();
		AT_DB.AddOrUpdateChat(chat);
	}
	
}