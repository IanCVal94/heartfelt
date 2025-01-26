using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HueChanger : MonoBehaviour
{
    [Range(50, 120)]
    public float value = globalHyperate.GlobalHeartRate; // Value to determine the hue

    private Renderer objectRenderer;

    // Colors for the range
    private Color darkRed = new Color(0.5f, 0f, 0f); // Dark red
    private Color brightRed = new Color(1f, 0f, 0f); // Bright red

    void Start()
    {
        // Get the Renderer component of the game object
        objectRenderer = GetComponent<Renderer>();

        if (objectRenderer == null)
        {
            Debug.LogError("Renderer component not found on the object. Please attach this script to an object with a material.");
        }
    }

    void Update()
    {
        // Update the material color based on the value
        UpdateColor();
    }

    private void UpdateColor()
    {
        if (objectRenderer != null && objectRenderer.material != null)
        {
            // Interpolate the color based on the value
            float t = Mathf.InverseLerp(50f, 120f, value); // Normalize the value to a range of 0 to 1
            Color currentColor = Color.Lerp(darkRed, brightRed, t);

            // Apply the color to the object's material
            objectRenderer.material.color = currentColor;
        }
    }
}
