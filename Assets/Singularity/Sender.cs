using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Sender : MonoBehaviour
{

    public Sngty.SingularityManager singularity;

    // Start is called before the first frame update
    void Start()
    {
        InvokeRepeating("SendHeartRate", 0f, 1f);
    }

    void SendHeartRate()
    {
        int heartRate = globalHyperate.GlobalHeartRate;
        string heartRateString = heartRate.ToString();
        singularity.sendMessage(heartRateString, new Sngty.DeviceSignature());
    }

    // Update is called once per frame
    void Update()
    {
 
    }


}
