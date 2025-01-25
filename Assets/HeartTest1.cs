using System.Collections;
using UnityEngine;

public class HeartTest1 : MonoBehaviour
{
    // Dynamic heart rate (beats per minute)
    [SerializeField] private float heartRate = 60f; // Default to 60 BPM

    // Scale parameters
    [SerializeField] private Vector3 minScale = Vector3.one * 0.9f;
    [SerializeField] private Vector3 maxScale = Vector3.one * 1.1f;
    [SerializeField] private float smoothness = 0.5f; // How smooth the transition is

    private Vector3 initialScale;
    private bool isPulsing;

    void Start()
    {
        // Save the initial scale of the object
        initialScale = transform.localScale;
    }

    void Update()
    {
        // Dynamically update the heart rate in case it changes
        float pulseDuration = 60f / heartRate;

        if (!isPulsing)
        {
            StartCoroutine(Pulse(pulseDuration));
        }
    }

    private IEnumerator Pulse(float duration)
    {
        isPulsing = true;

        // Pulse out (quick grow)
        float elapsedTime = 0f;
        float growDuration = duration * 0.3f; // Quicker grow phase
        while (elapsedTime < growDuration)
        {
            elapsedTime += Time.deltaTime;
            float t = Mathf.SmoothStep(0f, 1f, elapsedTime / growDuration);
            transform.localScale = Vector3.Lerp(minScale, maxScale, t);
            yield return null;
        }

        // Pulse in (slower shrink)
        elapsedTime = 0f;
        float shrinkDuration = duration * 0.7f; // Slower shrink phase
        while (elapsedTime < shrinkDuration)
        {
            elapsedTime += Time.deltaTime;
            float t = Mathf.SmoothStep(0f, 1f, elapsedTime / shrinkDuration);
            transform.localScale = Vector3.Lerp(maxScale, minScale, t);
            yield return null;
        }

        // Reset the scale to the initial value (just in case)
        transform.localScale = initialScale;
        isPulsing = false;
    }

    // Public method to update the heart rate dynamically
    public void SetHeartRate(float newHeartRate)
    {
        heartRate = Mathf.Clamp(newHeartRate, 30f, 200f); // Clamp to realistic BPM values
    }
}
