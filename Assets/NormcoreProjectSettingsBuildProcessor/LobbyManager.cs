using Normal.Realtime;
using UnityEngine;

public class LobbyManager : MonoBehaviour
{
    public Realtime realtime; // Assign your Realtime component in the Inspector
    public GameObject specialObjectPrefab; // Assign the GameObject to attach for Client 0
    public MonoBehaviour specialScriptForClient1; // Reference to the script to execute for Client 1
    private GameObject attachedObject;

    void Start()
    {
        // Subscribe to the Realtime event for connected clients
        realtime.didConnectToRoom += OnDidConnectToRoom;
    }

    void OnDidConnectToRoom(Realtime realtime)
    {
        int clientID = realtime.clientID; // Get the current client ID

        if (clientID == 0)
        {
            // First client to enter: Attach the special object
            Debug.Log("Client 0 connected. Attaching special object.");
            AttachSpecialObject();
        }
        else if (clientID == 1)
        {
            // Second client to enter: Execute special script code
            Debug.Log("Client 1 connected. Executing special code.");
            ExecuteClient1Code();
        }
    }

    void AttachSpecialObject()
    {
        if (specialObjectPrefab != null)
        {
            // Instantiate the object and attach it to this client
            attachedObject = Instantiate(specialObjectPrefab);
            RealtimeTransform realtimeTransform = attachedObject.GetComponent<RealtimeTransform>();

            if (realtimeTransform != null)
            {
                // Make sure the object follows the client
                realtimeTransform.RequestOwnership();
            }

            Debug.Log("Special object attached to Client 0.");
        }
        else
        {
            Debug.LogWarning("Special Object Prefab is not assigned.");
        }
    }

    void ExecuteClient1Code()
    {
        if (specialScriptForClient1 != null)
        {
            // Enable or trigger the special script
            specialScriptForClient1.enabled = true;

            Debug.Log("Special script for Client 1 executed.");
        }
        else
        {
            Debug.LogWarning("Special script for Client 1 is not assigned.");
        }
    }
}
