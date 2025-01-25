using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Sender : MonoBehaviour
{

    public Sngty.SingularityManager singularity;

    // Start is called before the first frame update
    void Start()
    {
        // singularity.sendMessage("Hello Arduino!", new Sngty.DeviceSignature());
    }

    // Update is called once per frame
    void Update()
    {
        singularity.sendMessage("Hello Arduino!", new Sngty.DeviceSignature());
    }


}
