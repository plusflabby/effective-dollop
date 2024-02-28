//System.ExportToClipboard(link);

// Admin Server Command
class AdminToolingCommand: ScrServerCommand
{
	protected ref StateBackendCallback m_Callback;
	protected SCR_EBanSubcommandArg m_eSubcommandArg;
	
	protected ScrServerCmdResult menu_remote()
	{
		PlatformService ps = GetGame().GetPlatformService();
		if (!ps)
			return ScrServerCmdResult(string.Empty, EServerCmdResultType.ERR);
		ps.OpenBrowser("https://admin-tooling.flabby.dev/");
		return ScrServerCmdResult("Opened admin website", EServerCmdResultType.OK);
	}

	protected ScrServerCmdResult menu_open()
	{
		MenuManager mm = GetGame().GetMenuManager();
		if (!mm)
			return ScrServerCmdResult(string.Empty, EServerCmdResultType.ERR);
		mm.OpenMenu(ChimeraMenuPreset.AT_DASHBOARD);
		return ScrServerCmdResult("Opened admin menu", EServerCmdResultType.OK);
	}
	
	protected ScrServerCmdResult help()
	{
		string str = string.Empty;
		str += "Help for admin command.";
		str += "\n#admin open (Opens the admin tooling menu)";
		str += "\n#admin remote (Opens the administration website)";
		
		return ScrServerCmdResult(str, EServerCmdResultType.OK);
	}
	
	protected ScrServerCmdResult HandleCommand(array<string> argv, int playerId = 0)
	{
		if (argv.Count() > 1)
		{
			if (argv[1] == "open")
				return menu_open();
			if (argv[1] == "remote")
				return menu_remote();
		}
		return help();
	}
	
	// Specify keyword of command
	//-----------------------------------------------------------------------------
	override string GetKeyword()
	{
		return "admin";
	}
	
	// Run command server-side
	//-----------------------------------------------------------------------------
	override bool IsServerSide()
	{
		return false;
	}
	
	// Set requirement to admin permission via RCON
	//-----------------------------------------------------------------------------
	override int RequiredRCONPermission()
	{
		return ERCONPermissions.PERMISSIONS_ADMIN;
	}
	
	// Set requirement to be logged in administrator for chat command
	//-----------------------------------------------------------------------------
	override int RequiredChatPermission()
	{
		return EPlayerRole.NONE;
	}
	
	// Handle Chat command on server
	//-----------------------------------------------------------------------------
	override ref ScrServerCmdResult OnChatServerExecution(array<string> argv, int playerId)
	{
		return ScrServerCmdResult(string.Empty, EServerCmdResultType.OK);
	}
	
	// Handle Chat command on client
	//-----------------------------------------------------------------------------
	override ref ScrServerCmdResult OnChatClientExecution(array<string> argv, int playerId)
	{
		return HandleCommand(argv, playerId);
	}
	
	// Handle RCON command on server
	//-----------------------------------------------------------------------------
	override ref ScrServerCmdResult OnRCONExecution(array<string> argv)
	{
		return HandleCommand(argv);
	}
	
	// Handle successful result in OnUpdate()
	//-----------------------------------------------------------------------------
	protected ScrServerCmdResult HandleSuccessfulResult()
	{
		return ScrServerCmdResult(string.Empty, EServerCmdResultType.OK);
	}
	
	// Handle Pending command
	//-----------------------------------------------------------------------------
	override ref ScrServerCmdResult OnUpdate()
	{
		return ScrServerCmdResult(string.Empty, EServerCmdResultType.OK);
	}
}