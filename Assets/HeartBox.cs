using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR.Interaction.Toolkit;
using UnityEngine.XR.Interaction.Toolkit.Attachment;
using UnityEngine.XR.Interaction.Toolkit.Gaze;
using UnityEngine.XR.Interaction.Toolkit.Interactors;
using UnityEngine.XR.Interaction.Toolkit.Transformers;
using UnityEngine.XR.Interaction.Toolkit.Utilities;
using UnityEngine.XR.Interaction.Toolkit.Utilities.Pooling;

public class HeartBox : MonoBehaviour
{
    public GameObject eyeAnchor;
    public HeartTest1 heart;
    public UnityEngine.XR.Interaction.Toolkit.Interactables.XRGrabInteractable heartInteractable;
    public bool hasHeart = false;

    // Start is called before the first frame update
    void Start()
    {
        // Set heart interactable
        heartInteractable = heart.GetComponent<UnityEngine.XR.Interaction.Toolkit.Interactables.XRGrabInteractable>();
        
        // Ensure the HeartBox has a collider and is set to be a trigger
        Collider collider = GetComponent<Collider>();
        if (collider == null)
        {
            collider = gameObject.AddComponent<BoxCollider>();
        }
        collider.isTrigger = true;

        // Ensure the HeartBox is kinematic
        Rigidbody rb = GetComponent<Rigidbody>();
        if (rb == null)
        {
            rb = gameObject.AddComponent<Rigidbody>();
        }
        rb.isKinematic = true;
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

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.tag == "Heart")
        {
            hasHeart = true;
            
    
        }
        
        Debug.Log("HeartBox has heart: " + hasHeart);
    }
    
    private void OnTriggerStay(Collider other)
    {
        // keep checking if the heart is still grabbed
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.gameObject.tag == "Heart")
        {
            hasHeart = false;
            
        
        }
        
        Debug.Log("HeartBox has heart: " + hasHeart);
    }
}