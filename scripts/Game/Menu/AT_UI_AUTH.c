class AT_UI_AUTH : AT_UI_MENU_BASE
{
	// Static variables
	protected static const string BUTTON_REGISTER = "REGISTERBUTTON0";
	protected static const string EDIT_BOX_USERNAME = "EditBoxRoot0";
	protected static const string EDIT_BOX_PASSWORD = "EditBoxRoot01";
	protected static const string BUTTON_LOGIN = "REGISTERBUTTON";
	protected static const string TEXT_RESPONSE = "responseTXT";
	
	protected SCR_ButtonBaseComponent regButton;
	protected SCR_ButtonBaseComponent loginButton;
	protected SCR_EditBoxComponent usernameEditBox;
	protected SCR_EditBoxComponent passwordEditBox;
	protected TextWidget responseForRegOrLogin;
	
	protected override void OnMenuOpen()
	{
		TEXT_TITLE = "Auth";
		super.OnMenuOpen();
		
		//setup register button
		regButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_REGISTER, rootWidget);
		regButton.m_OnClicked.Insert(RegisterButton);
		
		//setup login button
		loginButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_LOGIN, rootWidget);
		loginButton.m_OnClicked.Insert(LoginButton);
		
		usernameEditBox = SCR_EditBoxComponent.GetEditBoxComponent(EDIT_BOX_USERNAME, rootWidget);
		passwordEditBox = SCR_EditBoxComponent.GetEditBoxComponent(EDIT_BOX_PASSWORD, rootWidget);
		responseForRegOrLogin = TextWidget.Cast(rootWidget.FindAnyWidget(TEXT_RESPONSE));
	}
	
	protected void RegisterButton()
	{
		string the_password = passwordEditBox.GetValue();
		string the_username = usernameEditBox.GetValue();
		array<string> rpc_data = new array<string>();
		rpc_data.Insert(the_username);
		rpc_data.Insert(the_password);
		AT_Global.client.AT_EVENT_CLASS.add(new AT_Event(rpc_data, AT_Events.SessionUpdate, "SessionUpdate"));
		GetGame().GetCallqueue().CallLater(Close, 4000, false);
	}
	
	protected void LoginButton()
	{
		string the_password = passwordEditBox.GetValue();
		string the_username = usernameEditBox.GetValue();
		array<string> rpc_data = new array<string>();
		rpc_data.Insert(the_username); // testyTEST
		rpc_data.Insert(the_password); // testyY1234556789YYY01929384
		AT_Global.client.AT_EVENT_CLASS.add(new AT_Event(rpc_data, AT_Events.SessionUpdate, "SessionUpdate"));
		GetGame().GetCallqueue().CallLater(reload, 3000, false);
	}
	
	void reload()
	{
		Close();
		GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.AT_PM);
	}
}

