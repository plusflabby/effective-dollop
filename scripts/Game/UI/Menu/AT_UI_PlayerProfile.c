string m_sAtUiProfileUID = string.Empty;

class AT_UI_PlayerProfile : AT_UI_MENU_BASE
{
	private static const string m_sTextBoxPlayerBiUid = "PlayerBiUid";
	
	private TextWidget playerBiUid;
	
	private override void OnMenuOpen()
	{
		TEXT_TITLE = "UI_PlayerProfile";
		super.OnMenuOpen();
		
		playerBiUid = TextWidget.Cast(rootWidget.FindAnyWidget(m_sTextBoxPlayerBiUid));
		playerBiUid.SetText(m_sAtUiProfileUID);
	}
}