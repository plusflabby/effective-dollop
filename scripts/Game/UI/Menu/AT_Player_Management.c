class AT_Player_ManagementUI : AT_UI_MENU_BASE
{
	// Static variables
	protected static const string BUTTON_KICK = "KickButton";
	protected static const string BUTTON_BAN = "Button1";
	protected static const string BUTTON_TELEPORT_THERE = "Button2";
	protected static const string BUTTON_TELEPORT_HERE = "Button3";
	protected static const string BUTTON_SPECTATE = "Button4";
	protected static const string LISTBOX_PLAYERS = "playerslistbox";
	protected static const string BUTTON_SEARCH = "Button0";
	protected static const string EDIT_BOX_PLAYERUID = "EditBoxRoot0";
	protected static const string WINDOW_RESULTS = "Window0";
	
	// Variables
	protected TextListboxWidget listbox;
	protected SCR_ButtonBaseComponent spectateButton;
	protected SCR_ButtonBaseComponent kickButton;
	protected SCR_ButtonBaseComponent tpThereButton;
	protected SCR_ButtonBaseComponent tpHereButton;
	SCR_EditBoxComponent uidEditBox;
	TextWidget resultTextWidget;
	ref PDI_Result search;
	WindowWidget windowWidget;
	SCR_ButtonBaseComponent searchButton;

	protected override void OnMenuOpen()
	{
		TEXT_TITLE = "UI_PlayerManagement";
		super.OnMenuOpen();
		
		//! setup spectate button
		spectateButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_SPECTATE, rootWidget);
		spectateButton.m_OnClicked.Insert(SpectateButton);
		
		//! setup kick button
		kickButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_KICK, rootWidget);
		kickButton.m_OnClicked.Insert(KickButton);
		
		//! setup teleport there button
		tpThereButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_TELEPORT_THERE, rootWidget);
		tpThereButton.m_OnClicked.Insert(TeleportThereButton);
		
		//! setup teleport here button
		tpHereButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_TELEPORT_HERE, rootWidget);
		tpHereButton.m_OnClicked.Insert(TeleportHereButton);
		
		//! Here we will setup and populate the players list
		listbox = TextListboxWidget.Cast(rootWidget.FindAnyWidget(LISTBOX_PLAYERS));
		loadPlayerListBox();
		
		//! setup player database 
		searchButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_SEARCH, rootWidget);
		searchButton.m_OnClicked.Insert(SearchButton);
	}
	
	private void loadPlayerListBox()
	{
		array<int> playerIDs = new array<int>;
		PlayerManager pm = GetGame().GetPlayerManager();
		pm.GetPlayers(playerIDs);
		
		// add players to listbox
		for (int i; i < playerIDs.Count(); i++;)
		{
			AT_playerData player = new AT_playerData();
			player.id = playerIDs.Get(i);
			listbox.AddItem(pm.GetPlayerName(playerIDs.Get(i)), player, 0);
		}
		
		// Select first row
		listbox.SelectRow(0);
	}
	
	private void events(AT_Events event_)
	{
		AT_playerData player = AT_playerData.Cast(listbox.GetItemData(listbox.GetSelectedRow()));
		if (AT_EVENT_CLASS)
			AT_EVENT_CLASS.add(new AT_Event(player, event_, SCR_Enum.GetEnumName(AT_Events, event_) + "_" + player.id));
	}
	
	private void KickButton() {
		events(AT_Events.Kick);
	}
	
	private void TeleportThereButton() {
		events(AT_Events.TeleportThere);
	}
	
	private void TeleportHereButton() {
		events(AT_Events.TeleportHere);
	}
	
	private void SpectateButton() {
		events(AT_Events.Spectate);
	}
	
	private void SearchButton() {
		resultTextWidget = TextWidget.Cast(rootWidget.FindAnyWidget("resulttext"));
		uidEditBox = SCR_EditBoxComponent.Cast(SCR_EditBoxComponent.GetEditBoxComponent(EDIT_BOX_PLAYERUID, rootWidget));
		string value = uidEditBox.GetValue();
		//Print(value);
		
		search = PlayerDatabaseIntergration.findPlayerProfile(value);
		//Print(search.ToString());
		
		switch (search.result_code)
		{
			case PDI_Results.SUCCESS:
			{
				if (search.results.Count() > 1)
				{
					string header = "SUCCESSFULLY FOUND | Found "+search.results.Count().ToString();
					
					foreach (DB_PlayerProfile profile : search.results)
					{
						header = header + "\n" + profile.m_sBiUID + " : " + profile.m_sName;
					}
					
					resultTextWidget.SetText(header);
					resultTextWidget.SetVisible(true);
					
					//Text00001273563
					
				}
				else
				{
//					resultTextWidget.SetText("SUCCESSFULLY FOUND | Found UID="+search.player.m_sBiUID);
//					resultTextWidget.SetVisible(true);
					//! Open player's profile
					m_sAtUiProfileUID = search.player.m_sBiUID;
					GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.AT_PlayerProfile);
					Close();
				}
				break;
			}
			case PDI_Results.NOT_FOUND:
			{
				resultTextWidget.SetText("NOT FOUND");
				resultTextWidget.SetVisible(true);
				break;
			}
			case PDI_Results.INVALID_SEARCH:
			{
				resultTextWidget.SetText("INVALID SEARCH VALUE, Enter in player's BI UID");
				resultTextWidget.SetVisible(true);
				break;
			}
		}
		
	}
}

class AT_playerData
{
	int id;
}

