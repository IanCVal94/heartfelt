using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Normal.Realtime;

public class HeartBox : RealtimeComponent
{
	public GameObject eyeAnchor;
	public HeartTest1 heart;
	public bool hasHeart = false;

	public RealtimeTransform realTimeTransform;
	public Rigidbody heartRb;

	// Distance thresholds for grabbing and releasing
	public float grabDistance = 1.0f;   // Distance to "grab" the heart
	public float releaseDistance = 2.0f; // Distance to "release" the heart

	void Start()
	{
		// Ensure the HeartBox has a RealtimeTransform component
		realTimeTransform = heart.GetComponent<RealtimeTransform>();

		// Ensure the HeartBox has a Rigidbody component
		heartRb = heart.GetComponent<Rigidbody>();

		if (hasHeart)
		{
			Debug.Log("Heartbox starts with the heart.");
			heart.transform.position = transform.position;
		}
	}

	void Update()
	{
		if (eyeAnchor != null)
		{
			// Update the position and rotation of the HeartBox to follow the eyeAnchor
			Vector3 newPosition = new Vector3(
				eyeAnchor.transform.position.x,
				eyeAnchor.transform.position.y - 0.5f,
				eyeAnchor.transform.position.z
			);
			transform.position = newPosition;

			Quaternion newRotation = Quaternion.Euler(
				transform.rotation.eulerAngles.x,
				eyeAnchor.transform.rotation.eulerAngles.y,
				transform.rotation.eulerAngles.z
			);
			transform.rotation = newRotation;
		}

		
		// Check distance to heart and handle grabbing/releasing
		if (heart != null)
		{
			float distanceToHeart = Vector3.Distance(transform.position, heart.transform.position);

			//Debug.Log("Distance to heart: " + distanceToHeart);

			if (!hasHeart && distanceToHeart <= grabDistance)
			{
				heart.HeartBoxGrabbed(this);
			}
			

		}
	}

	private void GrabHeart()
	{
		hasHeart = true;
		Debug.Log("Grabbed the heart!");

		// Request ownership
		realTimeTransform.RequestOwnership();

		// Make the heart kinematic
		heartRb.isKinematic = true;
	}

	private void ReleaseHeart()
	{
		hasHeart = false;
		Debug.Log("Released the heart!");

		// Make the heart non-kinematic
		heartRb.isKinematic = false;
	}
}