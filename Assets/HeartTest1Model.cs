using Normal.Realtime.Serialization;

[RealtimeModel]
public partial class HeartTest1Model {
	[RealtimeProperty(1, true, true)] private bool _isHeldByWatch;
	[RealtimeProperty(2, true, true)] private bool _isHeldByStrap;
	[RealtimeProperty(3, true, true)] private bool _isGrabbed;
}