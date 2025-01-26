using UnityEngine;
using Normal.Realtime;

public class BoxManager : MonoBehaviour
{
	public static Vector3 client0HeadPosition = Vector3.zero;
	public static Vector3 client1HeadPosition = Vector3.zero;
	
	public HeartBoxWatch heartBoxWatch;
	public HeartBoxStrap heartBoxStrap; 

	private RealtimeAvatarManager avatarManager;

	void Start()
	{
		// Find the RealtimeAvatarManager in the scene
		avatarManager = FindObjectOfType<RealtimeAvatarManager>();

		if (avatarManager == null)
		{
			Debug.LogError("RealtimeAvatarManager not found in the scene.");
		}
	}

	void Update()
	{
		if (avatarManager != null)
		{
			foreach (var avatarEntry in avatarManager.avatars)
			{
				int clientID = avatarEntry.Key; // The clientID of this avatar
				RealtimeAvatar avatar = avatarEntry.Value; // The RealtimeAvatar instance

				if (avatar != null && avatar.gameObject != null)
				{
					// Look for the "Head" GameObject within the avatar
					Transform headTransform = avatar.transform.Find("Head");
					if (headTransform != null)
					{
						Vector3 headPosition = headTransform.position;

						// Update positions based on client ID
						if (clientID == 0)
						{
							client0HeadPosition = headPosition;
							heartBoxWatch.eyeAnchor = headTransform.gameObject;
						}
						else if (clientID == 1)
						{
							client1HeadPosition = headPosition;
							heartBoxStrap.eyeAnchor = headTransform.gameObject;
						}
					}
					else
					{
						Debug.LogWarning($"Head object not found for client {clientID}");
					}
				}
			}
		}
	}

	// Public methods to access the player head positions
	public static Vector3 GetClient0Position()
	{
		return client0HeadPosition;
	}

	public static Vector3 GetClient1Position()
	{
		return client1HeadPosition;
	}
}
