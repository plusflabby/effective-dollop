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
		return ScrServerCmdResult("Opened remote admin menu!", EServerCmdResultType.OK);
	}

	protected ScrServerCmdResult menu_open()
	{
		MenuManager mm = GetGame().GetMenuManager();
		if (!mm)
			return ScrServerCmdResult(string.Empty, EServerCmdResultType.ERR);
		mm.OpenMenu(ChimeraMenuPreset.AT_DASHBOARD);
		
		return ScrServerCmdResult("Opened admin menu!", EServerCmdResultType.OK);
	}
	
	protected ScrServerCmdResult HandleCommand(array<string> argv, int playerId = 0)
	{
		
		if (argv.Count() > 1)
		{
			if (argv[1] == "help")
			{
				if (argv[2] == "menu") return ScrServerCmdResult(
					"Help for admin menu command." +
					"\n#admin menu open (Opens the admin tooling menu)" +
					"\n#admin menu logout (Logout of the admin tooling menu)" +
					"\n#admin menu remote (Opens the administration website)",
					EServerCmdResultType.OK
				);
				else return ScrServerCmdResult(string.Empty, EServerCmdResultType.ERR);
			}
			else if (argv[1] == "menu")
			{
				if (argv[2] == "remote")
					return menu_remote();
			}
		}
		
		
		return ScrServerCmdResult(
			"Help for admin command." +
			"\n#admin help <menu/remote>" +
			"\n#admin menu <logout/open/remote>",
			EServerCmdResultType.OK
		);
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
		return true;
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
		return HandleCommand(argv, playerId);
	}
	
	// Handle Chat command on client
	//-----------------------------------------------------------------------------
	override ref ScrServerCmdResult OnChatClientExecution(array<string> argv, int playerId)
	{
		if (argv.Count() > 2)
		{
			if (argv[1] == "menu")
			{
				if (argv[2] == "open")
					return menu_open();
			}
		}
		
		return ScrServerCmdResult(string.Empty, EServerCmdResultType.OK);
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
		switch(m_eSubcommandArg)
		{
			// Handle Create Result
			case SCR_EBanSubcommandArg.EBSA_CREATE:
//				if (m_iBanPlayerId > 0)
//					GetGame().GetPlayerManager().KickPlayer(m_iBanPlayerId, PlayerManagerKickReason.BAN);
				return ScrServerCmdResult(string.Format("Player '%1' banned!", "tests"), EServerCmdResultType.OK);
			
			// Handle Remove Result
			case SCR_EBanSubcommandArg.EBSA_REMOVE:
				return ScrServerCmdResult("Ban removed!", EServerCmdResultType.OK);
		
//			// Handle List Result
//			case SCR_EBanSubcommandArg.EBSA_LIST:
//				return ListBansResult();
		}
		return ScrServerCmdResult(string.Empty, EServerCmdResultType.OK);
	}
	
	// Handle Pending command
	//-----------------------------------------------------------------------------
	override ref ScrServerCmdResult OnUpdate()
	{
		switch(m_Callback.m_eState)
		{
			case EBackendCallbackState.EBCS_SUCCESS: return HandleSuccessfulResult();
			case EBackendCallbackState.EBCS_PENDING: return ScrServerCmdResult(string.Empty, EServerCmdResultType.PENDING);
			case EBackendCallbackState.EBCS_TIMEOUT: return ScrServerCmdResult("Timeout", EServerCmdResultType.ERR);
		}
		return ScrServerCmdResult(string.Empty, EServerCmdResultType.ERR);	
	}
}