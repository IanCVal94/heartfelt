using System.Collections;
using UnityEngine;
using Normal.Realtime;
using Normal.Realtime.Serialization;

public class HeartTest1 : RealtimeComponent
{
	// Dynamic heart rate (beats per minute)
	[SerializeField] private float heartRate = 60f; // Default to 60 BPM

	// Scale parameters
	[SerializeField] private Vector3 minScale = Vector3.one * 0.9f;
	[SerializeField] private Vector3 maxScale = Vector3.one * 1.1f;
	[SerializeField] private float smoothness = 0.5f; // How smooth the transition is

	private Vector3 initialScale;
	private bool isPulsing;

	[Header("Audio")]
	public AudioSource heartBeat;

	[Header("Physics")]
	Rigidbody rb;
	public RealtimeTransform realTimeTransform;

	[Header("Heart Box")]
	public HeartBox heartBox;
	public bool isHeldByBox = false;

	// Distance thresholds for grabbing and releasing
	public float grabDistance = 0.1f;   // Distance to "grab" the heart
	public float releaseDistance = 0.2f; // Distance to "release" the heart

	// Cooldown logic
	private Vector3 lastReleasePosition;
	private bool isInCooldown = false;

	void Start()
	{
		// Save the initial scale of the object
		initialScale = transform.localScale;

		rb = GetComponent<Rigidbody>();

		// Ensure the Heart has a RealtimeTransform component
		realTimeTransform = GetComponent<RealtimeTransform>();

		// Initialize lastReleasePosition
		lastReleasePosition = transform.position;
	}

	void Update()
	{
		// Convert to int and set heart rate
		SetHeartRate((float)globalHyperate.GlobalHeartRate);

		// Dynamically update the heart rate in case it changes
		float pulseDuration = 60f / heartRate;

		if (!isPulsing)
		{
			StartCoroutine(Pulse(pulseDuration));
		}

		// Check distance to heartBox and handle grabbing/releasing
		if (heartBox != null)
		{
			// Continuously update the heart's position and rotation if it is grabbed
			if (isHeldByBox)
			{
				transform.position = heartBox.transform.position;
				transform.rotation = heartBox.transform.rotation;
			}
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
			transform.localScale = Vector3.Lerp(minScale, maxScale, t);
			yield return null;
		}

		// Pulse in (slower shrink)
		elapsedTime = 0f;
		float shrinkDuration = duration * 0.7f; // Slower shrink phase
		while (elapsedTime < shrinkDuration)
		{
			elapsedTime += Time.deltaTime;
			float t = Mathf.SmoothStep(0f, 1f, elapsedTime / shrinkDuration);
			transform.localScale = Vector3.Lerp(maxScale, minScale, t);
			yield return null;
		}

		// Reset the scale to the initial value (just in case)
		transform.localScale = initialScale;
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

	public void HeartBoxGrabbed(HeartBox box)
	{
		// If the heart is in cooldown, return early
		if (isInCooldown)
		{
			Debug.Log("Heart is in cooldown, cannot grab yet.");
			return;
		}

		if (!isHeldByBox) // Ensure it is only called once
		{
			heartBox = box;
			isHeldByBox = true;

			realTimeTransform.RequestOwnership();

			Debug.Log("Heart grabbed by box");
		}
	}

	public void HeartBoxReleased()
	{
		if (isHeldByBox) // Ensure it is only called once
		{
			isHeldByBox = false;
			lastReleasePosition = transform.position; // Save the release position
			isInCooldown = true;

			StartCoroutine(CooldownCoroutine());
			Debug.Log("Heart released by box");
		}
	}

	private IEnumerator CooldownCoroutine()
	{
		// Wait until the heart moves farther than the grab distance from the last release position
		while (Vector3.Distance(transform.position, lastReleasePosition) <= grabDistance)
		{
			yield return null; // Keep checking every frame
		}

		isInCooldown = false; // Cooldown is over
		Debug.Log("Cooldown over, heart can be grabbed again.");
	}
}