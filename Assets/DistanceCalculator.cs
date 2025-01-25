using UnityEngine;

public class DistanceCalculator : MonoBehaviour
{
    public static float distanceBetweenPlayers = 0f;

    void Update()
    {
        // Get the positions of ClientID 0 and ClientID 1
        Vector3 client0Pos = PlayerManager.GetClient0Position();
        // Debug.Log("0: " + client0Pos);
        Vector3 client1Pos = PlayerManager.GetClient1Position();
        // Debug.Log("1: " + client1Pos);

        // Calculate the distance
        distanceBetweenPlayers = Vector3.Distance(client0Pos, client1Pos);
        // Debug.Log("Distance: " + distanceBetweenPlayers);
    }

    public static float GetDistanceBetweenPlayers()
    {
        return distanceBetweenPlayers;
    }
}
