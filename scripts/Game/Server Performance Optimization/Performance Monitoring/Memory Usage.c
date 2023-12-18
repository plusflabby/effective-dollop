class vMemoryUsage : vSetMod
{
	private int memoryUsage = 0;
	
	void setValue()
	{
		ServerPerformanceMonitor.memoryUsageUpdate(memoryUsage);
	}
	
	void vMemoryUsage()
	{
		vSetModSTART();
		memoryUsage = System.MemoryAllocationKB();
		setValue();
	}
	void ~vMemoryUsage()
	{
		memoryUsage = 0;
		vSetModSTOP();
	}
}