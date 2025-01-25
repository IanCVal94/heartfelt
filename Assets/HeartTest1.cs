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
    private bool previousKinematicState;

    [Header("Audio")]
    public AudioSource heartBeat;

    [Header("Physics")]
    Rigidbody rb;

    void Start()
    {
        // Save the initial scale of the object
        initialScale = transform.localScale;

        rb = GetComponent<Rigidbody>();
        previousKinematicState = rb.isKinematic;
    }

    void Update()
    {
        // Convert to int
        SetHeartRate((int)globalHyperate.GlobalHeartRate);

        // Dynamically update the heart rate in case it changes
        float pulseDuration = 60f / heartRate;

        if (!isPulsing)
        {
            StartCoroutine(Pulse(pulseDuration));
        }

        // Check for changes in the kinematic state
        if (rb.isKinematic != previousKinematicState)
        {
            Debug.Log("Rigidbody kinematic state changed to: " + rb.isKinematic);
            previousKinematicState = rb.isKinematic;
        }
    }

    private IEnumerator Pulse(float duration)
    {
        isPulsing = true;

        // Play heartbeat sound
        if (heartBeat != null)
        {
            heartBeat.Play();
        }

        // Trigger haptics
        TriggerHaptic(duration);

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

    // Trigger haptics for Meta Quest controllers
    private void TriggerHaptic(float duration)
    {
        float pulseStrength = 0.8f; // Strength of vibration (0.0 to 1.0)
        float pulseFrequency = 0.5f; // Frequency of vibration (used for low-level haptics)

        // Trigger haptics on both controllers
        OVRInput.SetControllerVibration(pulseFrequency, pulseStrength, OVRInput.Controller.LTouch); // Left controller
        OVRInput.SetControllerVibration(pulseFrequency, pulseStrength, OVRInput.Controller.RTouch); // Right controller

        // Stop haptics after the duration of one pulse
        StartCoroutine(StopHaptics(duration));
    }

    private IEnumerator StopHaptics(float duration)
    {
        yield return new WaitForSeconds(duration);

        // Stop haptics on both controllers
        OVRInput.SetControllerVibration(0, 0, OVRInput.Controller.LTouch);
        OVRInput.SetControllerVibration(0, 0, OVRInput.Controller.RTouch);
    }
}