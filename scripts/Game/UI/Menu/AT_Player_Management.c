class AT_Player_ManagementUI : AT_UI_MENU_BASE
{
	// Static variables
	protected static const string BUTTON_KICK = "KickButton";
	protected static const string BUTTON_BAN = "Button1";
	protected static const string BUTTON_TELEPORT_THERE = "Button2";
	protected static const string BUTTON_TELEPORT_HERE = "Button3";
	protected static const string BUTTON_SPECTATE = "Button4";
	protected static const string LISTBOX_PLAYERS = "playerslistbox";
	
	// Variables
	protected TextListboxWidget listbox;
	protected SCR_ButtonBaseComponent spectateButton;
	protected SCR_ButtonBaseComponent kickButton;
	protected SCR_ButtonBaseComponent tpThereButton;
	protected SCR_ButtonBaseComponent tpHereButton;

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
	
	private void KickButton() {
		AT_playerData player = AT_playerData.Cast(listbox.GetItemData(listbox.GetSelectedRow()));
		if (AT_EVENT_CLASS)
			AT_EVENT_CLASS.add(new AT_Event(player, AT_Events.Kick, "Kick_Id_" + player.id));
	}
	
	private void TeleportThereButton() {
		AT_playerData player = AT_playerData.Cast(listbox.GetItemData(listbox.GetSelectedRow()));
		if (AT_EVENT_CLASS)
			AT_EVENT_CLASS.add(new AT_Event(player, AT_Events.TeleportThere, "TP_There_Id_" + player.id));
	}
	
	private void TeleportHereButton() {
		AT_playerData player = AT_playerData.Cast(listbox.GetItemData(listbox.GetSelectedRow()));
		if (AT_EVENT_CLASS)
			AT_EVENT_CLASS.add(new AT_Event(player, AT_Events.TeleportHere, "TP_Here_Id_" + player.id));
	}
	
	private void SpectateButton() {
		AT_playerData player = AT_playerData.Cast(listbox.GetItemData(listbox.GetSelectedRow()));
		if (AT_EVENT_CLASS)
			AT_EVENT_CLASS.add(new AT_Event(player, AT_Events.Spectate, "Spectate_Id_" + player.id));
	}
}

class AT_playerData
{
	int id;
}
