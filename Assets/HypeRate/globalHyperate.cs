using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Newtonsoft.Json.Linq;
using NativeWebSocket;

public class globalHyperate : MonoBehaviour
{
    public string websocketToken = "<Request your Websocket Token>"; // Your WebSocket Token
    public string hyperateID = "internal-testing";

    // Global variable to store the heart rate value
    public static int GlobalHeartRate { get; private set; }

    private WebSocket websocket;

    async void Start()
    {
        websocket = new WebSocket("wss://app.hyperate.io/socket/websocket?token=" + websocketToken);
        Debug.Log("Connect!");

        websocket.OnOpen += () =>
        {
            Debug.Log("Connection open!");
            SendWebSocketMessage();
        };

        websocket.OnError += (e) =>
        {
            Debug.Log("Error! " + e);
        };

        websocket.OnClose += (e) =>
        {
            Debug.Log("Connection closed!");
        };

        websocket.OnMessage += (bytes) =>
        {
            // Get the message as a string
            var message = System.Text.Encoding.UTF8.GetString(bytes);
            var msg = JObject.Parse(message);

            if (msg["event"].ToString() == "hr_update")
            {
                // Update the global heart rate variable
                GlobalHeartRate = int.Parse(msg["payload"]["hr"].ToString());
                Debug.Log($"Heart Rate Updated: {GlobalHeartRate}");
                // should be able to pull from most other c# scripts by using globalHyperate.GlobalHeartRate.ToString();
            }
        };

        // Send heartbeat message every 25 seconds
        InvokeRepeating("SendHeartbeat", 1.0f, 25.0f);

        await websocket.Connect();
    }

    void Update()
    {
#if !UNITY_WEBGL || UNITY_EDITOR
        websocket.DispatchMessageQueue();
#endif
    }

    async void SendWebSocketMessage()
    {
        if (websocket.State == WebSocketState.Open)
        {
            // Log into the "internal-testing" channel
            await websocket.SendText("{\"topic\": \"hr:" + hyperateID + "\", \"event\": \"phx_join\", \"payload\": {}, \"ref\": 0}");
        }
    }

    async void SendHeartbeat()
    {
        if (websocket.State == WebSocketState.Open)
        {
            // Send heartbeat message
            await websocket.SendText("{\"topic\": \"phoenix\", \"event\": \"heartbeat\", \"payload\": {}, \"ref\": 0}");
        }
    }

    private async void OnApplicationQuit()
    {
        await websocket.Close();
    }
}
