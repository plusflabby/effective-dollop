class AT_EventClass
{
	protected ref array<ref AT_Event> events = {};
	
	ref array<ref AT_Event> getAll()
	{
		return events;
	}
	
	void add(AT_Event ev) 
	{
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
	}
	
	AT_Events getName()
	{
		return name;
	}
	
	Managed getData()
	{
		return data;
	}
	
	bool compare(AT_Event ev)
	{
		return ev.comparableName.Compare(comparableName) < 0;
	}
}