class vMemoryUsage
{
	private int memoryUsage = 0;
	
	void setValue()
	{
		ServerPerformanceMonitor.memoryUsageUpdate(memoryUsage);
	}
	
	void vMemoryUsage()
	{
		memoryUsage = System.MemoryAllocationKB();
		setValue();
	}
	void ~vMemoryUsage()
	{
		memoryUsage = 0;
	}
}