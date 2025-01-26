using System.Collections;
using UnityEngine;
using Normal.Realtime;
using Normal.Realtime.Serialization;
using Oculus.Interaction;
using Oculus.Haptics;

public class HeartTest1 : RealtimeComponent<HeartTest1Model>
{
	// Dynamic heart rate (beats per minute)
	[SerializeField] private float heartRate = 60f; // Default to 60 BPM

	// Scale parameters
	[SerializeField] private float minScale = 0.9f;
	[SerializeField] private float maxScale = 1.1f;
	[SerializeField] private float smoothness = 0.5f; // How smooth the transition is

	public Transform heartVisuals;

	private Vector3 initialScale;
	private bool isPulsing;

	[Header("Audio")]
	public AudioSource heartBeat;
	
	
	[Header("Physics")]
	Rigidbody rb;
	public RealtimeTransform realTimeTransform;

	[Header("Heart Box")]
	public HeartBoxWatch heartBoxWatch;
	public HeartBoxStrap heartBoxStrap;
	public bool isHeldByWatch = false;
	public bool isHeldByStrap = false;


	// Distance thresholds for grabbing and releasing
	public float grabDistance = 0.1f;   // Distance to "grab" the heart
	public float releaseDistance = 0.2f; // Distance to "release" the heart

	private bool isGrabbed = false;

	void Start()
	{
		// Save the initial scale of the object
		initialScale = heartVisuals.localScale;

		rb = GetComponent<Rigidbody>();

		// Ensure the Heart has a RealtimeTransform component
		realTimeTransform = GetComponent<RealtimeTransform>();
		
	}

	void Update()
	{
		// if space bar is pressed, release the heart
		if (Input.GetKeyDown(KeyCode.Space))
		{
			HeartBoxReleased();
		}
		
		// Convert to int and set heart rate
		SetHeartRate((float)globalHyperate.GlobalHeartRate);

		// Dynamically update the heart rate in case it changes
		float pulseDuration = 60f / heartRate;

		if (!isPulsing)
		{
			StartCoroutine(Pulse(pulseDuration));
		}

		// Continuously update the heart's position and rotation if it is grabbed
		if (isHeldByWatch)
		{
			transform.position = heartBoxWatch.transform.position;
			transform.rotation = heartBoxWatch.transform.rotation;
		}
		else if (isHeldByStrap)
		{
			transform.position = heartBoxStrap.transform.position;
			transform.rotation = heartBoxStrap.transform.rotation;
		}
	}

	private IEnumerator Pulse(float duration)
	{
		isPulsing = true;

		// Play heartbeat sound
		if (heartBeat != null)
		{
			heartBeat.Play();
		}

		// Trigger haptics
		TriggerHaptic(duration);

		// Pulse out (quick grow)
		float elapsedTime = 0f;
		float growDuration = duration * 0.3f; // Quicker grow phase
		while (elapsedTime < growDuration)
		{
			elapsedTime += Time.deltaTime;
			float t = Mathf.SmoothStep(0f, 1f, elapsedTime / growDuration);
			heartVisuals.localScale = initialScale * Mathf.Lerp(minScale, maxScale, t);
			yield return null;
		}

		// Pulse in (slower shrink)
		elapsedTime = 0f;
		float shrinkDuration = duration * 0.7f; // Slower shrink phase
		while (elapsedTime < shrinkDuration)
		{
			elapsedTime += Time.deltaTime;
			float t = Mathf.SmoothStep(0f, 1f, elapsedTime / shrinkDuration);
			heartVisuals.localScale = initialScale * Mathf.Lerp(maxScale, minScale, t);
			yield return null;
		}

		// Reset the scale to the initial value (just in case)
		heartVisuals.localScale = initialScale;
		isPulsing = false;
	}

	// Public method to update the heart rate dynamically
	public void SetHeartRate(float newHeartRate)
	{
		heartRate = Mathf.Clamp(newHeartRate, 30f, 200f); // Clamp to realistic BPM values
	}

	// Trigger haptics for Meta Quest controllers
	private void TriggerHaptic(float duration)
	{
		float pulseStrength = 0.8f; // Strength of vibration (0.0 to 1.0)
		float pulseFrequency = 0.5f; // Frequency of vibration (used for low-level haptics)

		// Trigger haptics on both controllers
		OVRInput.SetControllerVibration(pulseFrequency, pulseStrength, OVRInput.Controller.LTouch); // Left controller
		OVRInput.SetControllerVibration(pulseFrequency, pulseStrength, OVRInput.Controller.RTouch); // Right controller

		// Stop haptics after the duration of one pulse
		StartCoroutine(StopHaptics(duration));
	}

	private IEnumerator StopHaptics(float duration)
	{
		yield return new WaitForSeconds(duration);

		// Stop haptics on both controllers
		OVRInput.SetControllerVibration(0, 0, OVRInput.Controller.LTouch);
		OVRInput.SetControllerVibration(0, 0, OVRInput.Controller.RTouch);
	}

	public void HeartBoxGrabbedWatch()
	{
		if (!isHeldByWatch && !isHeldByStrap)
		{
			if (isGrabbed)
			{
				Debug.Log("Heart is in already grabbed by watch, cannot grab yet.");
				return;
			}
			
			isHeldByWatch = true;
			UpdateHeldByWatchBoolOnServer();
			
			isHeldByStrap = false;
			UpdateHeldByStrapBoolOnServer();

			realTimeTransform.RequestOwnership();

			Debug.Log("Heart grabbed by watch");
		}
	}
	
	public void HeartBoxGrabbedStrap()
	{
		if (!isHeldByWatch && !isHeldByStrap)
		{
			if (isGrabbed)
			{
				Debug.Log("Heart is in already grabbed by strap, cannot grab yet.");
				return;
			}
			
			isHeldByStrap = true;
			UpdateHeldByStrapBoolOnServer();
			
			isHeldByWatch = false;
			UpdateHeldByWatchBoolOnServer();

			realTimeTransform.RequestOwnership();

			Debug.Log("Heart grabbed by strap");
		}
	}

	public void HeartBoxReleased()
	{
		if (isHeldByWatch)
		{
			isHeldByWatch = false;
			UpdateHeldByWatchBoolOnServer();
			Debug.Log("Heart released by watch");
		}
		else if (isHeldByStrap)
		{
			isHeldByStrap = false;
			UpdateHeldByStrapBoolOnServer();
			Debug.Log("Heart released by strap");
		}
		
			
		isGrabbed = false;
		UpdateServerIsGrabbedBoolean();


		Debug.Log("Heart released by box");
		
	}


	
	
	public void OnGrab()
	{
		isGrabbed = true;
		UpdateServerIsGrabbedBoolean();
	}
	
	public void OnRelease()
	{
		isGrabbed = false;
		UpdateServerIsGrabbedBoolean();
	}
	
	
	
	
	
	
	
	public void updateLocalWatchBoolean(HeartTest1Model model, bool value)
	{
		isHeldByWatch = model.isHeldByWatch;
	}
	
	void UpdateLocalIsGrabbedBoolean(HeartTest1Model model, bool value)
	{
		isGrabbed = model.isGrabbed;
	}
	
	public void updateLocalStrapBoolean(HeartTest1Model model, bool value)
	{
		isHeldByStrap = model.isHeldByStrap;
	}
	
	protected override void OnRealtimeModelReplaced(HeartTest1Model previousModel, HeartTest1Model currentModel)
	{
		if (previousModel != null)
		{
			// Unregister from events
			previousModel.isHeldByWatchDidChange -= updateLocalWatchBoolean;
			previousModel.isHeldByStrapDidChange -= updateLocalStrapBoolean;
			previousModel.isGrabbedDidChange -= UpdateLocalIsGrabbedBoolean; 
			
		}
		
		if (currentModel != null)
		{
			if(currentModel.isFreshModel)
			{
				model.isHeldByWatch = isHeldByWatch;
				model.isHeldByStrap = isHeldByStrap;
				model.isGrabbed = isGrabbed;
			}
			
			// Update data from model
			//updateLocalBoolean();
			
			// Register for events so we'll know if the value changes later
			currentModel.isHeldByWatchDidChange += updateLocalWatchBoolean;
			currentModel.isHeldByStrapDidChange += updateLocalStrapBoolean;
			currentModel.isGrabbedDidChange += UpdateLocalIsGrabbedBoolean;
		}
	}
	
	void UpdateHeldByWatchBoolOnServer()
	{
		model.isHeldByWatch = isHeldByWatch;
	}
	
	void UpdateHeldByStrapBoolOnServer()
	{
		model.isHeldByStrap = isHeldByStrap;
	}
	
	void UpdateServerIsGrabbedBoolean()
	{
		model.isGrabbed = isGrabbed;
	}
	
	
	
	
	
	
}