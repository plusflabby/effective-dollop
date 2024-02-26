class AT_EventClass
{
	protected ref array<ref AT_Event> events = {};
	
	ref array<ref AT_Event> getAll()
	{
		if (AT_GLOBALS.client.DEBUG)
			Print("AT_EventClass getAll()", LogLevel.WARNING);
		
		return events;
	}
	
	void add(AT_Event ev) 
	{
		if (AT_Global.client.DEBUG)
			Print("AT_EventClass add()", LogLevel.WARNING);
		
		bool stopAdd = false;
		foreach (AT_Event ev_ : events)
		{
			if (ev_.compare(ev))
			{
				stopAdd = true;
				break;
			}
		}
		
		if (!stopAdd)
			events.Insert(ev);
		
	}
	
	void remove(int id)
	{
		if (AT_Global.client.DEBUG)
			Print("AT_EventClass remove()", LogLevel.WARNING);
		
		events.Remove(id);
	}
}

enum AT_Events
{
	Kick,
	TeleportThere,
	TeleportHere,
	Spectate,
	SessionUpdate,
	PlayerDatabaseSearch,
	PlayerProfileGetData
}

class AT_Event
{
	private ref Managed data;
	private AT_Events name;
	private string comparableName;
	
	void AT_Event(Managed data_, AT_Events name_, string comparableName_)
	{
		data = data_;
		name = name_;
		comparableName = comparableName_;
		
		if (AT_Global.client.DEBUG)
			Print("AT_Event constructor()", LogLevel.WARNING);
	}
	
	AT_Events getName()
	{
		if (AT_Global.client.DEBUG)
			Print("AT_Event getName()", LogLevel.WARNING);
		
		return name;
	}
	
	Managed getData()
	{
		if (AT_Global.client.DEBUG)
			Print("AT_Event getData()", LogLevel.WARNING);
		
		return data;
	}
	
	bool compare(AT_Event ev)
	{
		if (AT_Global.client.DEBUG)
			Print("AT_Event compare()", LogLevel.WARNING);
		
		return ev.comparableName.Compare(comparableName) < 0;
	}
}