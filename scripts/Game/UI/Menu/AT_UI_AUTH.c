class AT_UI_AUTH : MenuBase
{
	// Static variables
	protected static const string TEXT_TITLE = "Auth";
	protected static const string BUTTON_REGISTER = "REGISTERBUTTON0";
	protected static const string EDIT_BOX_USERNAME = "EditBoxRoot0";
	protected static const string EDIT_BOX_PASSWORD = "EditBoxRoot01";
	protected static const string BUTTON_LOGIN = "REGISTERBUTTON";
	
	Widget rootWidget;
	
	protected override void OnMenuOpen()
	{
		Print("AT_UI_AUTH->OnMenuOpen: menu opened", LogLevel.SPAM);
		
		
		rootWidget = GetRootWidget();
		if (!rootWidget)
		{
			Print("AT_UI_AUTH->OnMenuOpen: rootWidget layout error", LogLevel.ERROR);
			return;
		}
		
		//setup register button
		SCR_ButtonBaseComponent regButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_REGISTER, rootWidget);
		if (!regButton)
		{
			Debug.Error("AT_UI_AUTH->OnMenuOpen: regButton error");
			return;
		}
		regButton.m_OnClicked.Insert(RegisterButton);
		
		//setup login button
		SCR_ButtonBaseComponent loginButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_LOGIN, rootWidget);
		if (!loginButton)
		{
			Debug.Error("AT_UI_AUTH->OnMenuOpen: loginButton error");
			return;
		}
		loginButton.m_OnClicked.Insert(LoginButton);
		
		
		
		//Set up listeners to close menu
		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
		{
			// this is for the menu/dialog to close when pressing ESC
			// an alternative is to have a button with the SCR_NavigationButtonComponent component
			// and its Action Name field set to MenuBack - this would activate the button on ESC press
			inputManager.AddActionListener("MenuOpen", EActionTrigger.DOWN, Close);
			inputManager.AddActionListener("MenuBack", EActionTrigger.DOWN, Close);
#ifdef WORKBENCH // in Workbench, F10 is used because ESC closes the preview
			inputManager.AddActionListener("MenuOpenWB", EActionTrigger.DOWN, Close);
			inputManager.AddActionListener("MenuBackWB", EActionTrigger.DOWN, Close);
#endif
		}
		
		//Tell AT_Main this menu is IsOpen
		AT_MainStatic.setCurrentMenu(TEXT_TITLE);
	}

	protected override void OnMenuClose()
	{
		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
		{
			inputManager.RemoveActionListener("MenuOpen", EActionTrigger.DOWN, Close);
			inputManager.RemoveActionListener("MenuBack", EActionTrigger.DOWN, Close);
#ifdef WORKBENCH
			inputManager.RemoveActionListener("MenuOpenWB", EActionTrigger.DOWN, Close);
			inputManager.RemoveActionListener("MenuBackWB", EActionTrigger.DOWN, Close);
#endif
		}
	}
	
	protected void RegisterButton() {
		
		SCR_EditBoxComponent usernameEditBox = SCR_EditBoxComponent.GetEditBoxComponent(EDIT_BOX_USERNAME, rootWidget);
		if (!usernameEditBox)
		{ 
			Debug.Error("AT_UI_AUTH->RegisterButton: usernameEditBox error");
			return;
		}
		SCR_EditBoxComponent passwordEditBox = SCR_EditBoxComponent.GetEditBoxComponent(EDIT_BOX_PASSWORD, rootWidget);
		if (!passwordEditBox)
		{
			Debug.Error("AT_UI_AUTH->RegisterButton: passwordEditBox error");
			return;
		}
		
		string the_password = passwordEditBox.GetValue();
		string the_username = usernameEditBox.GetValue();
		
		Account account = new Account();
		
		//Print("the_password "+the_password, LogLevel.NORMAL);
		//Print("the_username "+the_username, LogLevel.NORMAL);
		account.registration(the_username, the_password);
	}
	
	protected void LoginButton() {
		
		SCR_EditBoxComponent usernameEditBox = SCR_EditBoxComponent.GetEditBoxComponent(EDIT_BOX_USERNAME, rootWidget);
		if (!usernameEditBox)
		{ 
			Debug.Error("AT_UI_AUTH->LoginButton: usernameEditBox error");
			return;
		}
		SCR_EditBoxComponent passwordEditBox = SCR_EditBoxComponent.GetEditBoxComponent(EDIT_BOX_PASSWORD, rootWidget);
		if (!passwordEditBox)
		{
			Debug.Error("AT_UI_AUTH->LoginButton: passwordEditBox error");
			return;
		}
		
		string the_password = passwordEditBox.GetValue(); //Greg01234567890
		string the_username = usernameEditBox.GetValue(); //greg
		
		Account account = new Account();
		bool login = account.login(the_username, the_password);
		
		Debug.Error("AT_UI_AUTH->LoginButton: login="+login);
	}
	
}

