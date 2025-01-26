using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Normal.Realtime;

public class HeartBoxStrap : MonoBehaviour
{
	public GameObject eyeAnchor;
	public HeartTest1 heart;
	public bool hasHeart = false;

	// Distance thresholds for grabbing and releasing
	public float grabDistance = 0.10f;   // Distance to "grab" the heart
	public float releaseDistance = 0.20f; // Distance to "release" the heart
	
	public float headToHeartOffset = 0.25f;

	void Start()
	{
		
		/*
		if (hasHeart)
		{
			Debug.Log("Heartbox starts with the heart.");
			heart.transform.position = transform.position;
		}
		*/
	}

	void Update()
	{
		if (eyeAnchor != null)
		{
			// Update the position and rotation of the HeartBox to follow the eyeAnchor
			Vector3 newPosition = new Vector3(
				eyeAnchor.transform.position.x,
				eyeAnchor.transform.position.y - headToHeartOffset,
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

			if (distanceToHeart <= grabDistance)
			{
				heart.HeartBoxGrabbedStrap();
			}
			

		}
	}

	/*
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
	*/
}