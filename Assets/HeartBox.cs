using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HeartBox : MonoBehaviour
{
    public GameObject eyeAnchor;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (eyeAnchor != null)
        {
            // Update position
            Vector3 newPosition = new Vector3(eyeAnchor.transform.position.x, eyeAnchor.transform.position.y - 0.25f, eyeAnchor.transform.position.z);
            transform.position = newPosition;

            // Update rotation
            Quaternion newRotation = Quaternion.Euler(transform.rotation.eulerAngles.x, eyeAnchor.transform.rotation.eulerAngles.y, transform.rotation.eulerAngles.z);
            transform.rotation = newRotation;
        }
    }
}