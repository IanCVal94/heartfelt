using UnityEngine;

public class ColorChanger : MonoBehaviour
{
    [SerializeField] private Renderer targetRenderer; // Renderer of the GameObject to change color
    [SerializeField] private float value = 50f; // Value to evaluate (can be changed dynamically)

    private Color darkRed = new Color(0.5f, 0f, 0f); // Dark red
    private Color brightRed = new Color(1f, 0f, 0f); // Bright red

    // Update is called once per frame
    void Update()
    {
        // Clamp the value to ensure it stays within range
        float clampedValue = Mathf.Clamp(value, 50f, 120f);

        // Calculate the t parameter for Lerp (Linear Interpolation)
        float t = (clampedValue - 50f) / (120f - 50f);

        // Interpolate between darkRed and brightRed based on the value
        Color newColor = Color.Lerp(darkRed, brightRed, t);

        // Apply the color to the material of the target renderer
        if (targetRenderer != null && targetRenderer.material != null)
        {
            targetRenderer.material.color = newColor;
        }
    }

    // Public method to update the value (optional)
    public void SetValue(float newValue)
    {
        value = newValue;
    }
}
