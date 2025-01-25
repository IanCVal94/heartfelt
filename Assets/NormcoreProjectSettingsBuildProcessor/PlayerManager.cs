using UnityEngine;
using Normal.Realtime;

public class PlayerManager : MonoBehaviour
{
    public static Vector3 client0Position = Vector3.zero;
    public static Vector3 client1Position = Vector3.zero;

    private Realtime realtime;

    void Start()
    {
        // Assign the Realtime component
        realtime = FindObjectOfType<Realtime>();

        if (realtime == null)
        {
            Debug.LogError("Realtime component not found in the scene.");
        }
    }

    void Update()
    {
        // Get all players with RealtimeTransform in the scene
        RealtimeTransform[] players = FindObjectsOfType<RealtimeTransform>();

        foreach (RealtimeTransform player in players)
        {
            if (player.ownerID == 0)
            {
                client0Position = player.transform.position;
            }
            else if (player.ownerID == 1)
            {
                client1Position = player.transform.position;
            }
        }
    }

    public static Vector3 GetClient0Position()
    {
        return client0Position;
    }

    public static Vector3 GetClient1Position()
    {
        return client1Position;
    }
}
