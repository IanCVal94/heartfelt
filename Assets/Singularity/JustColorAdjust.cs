using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

public class JustColorAdjust : MonoBehaviour
{   
    public Volume volume; // Assign your Volume GameObject here
    private ColorAdjustments colorAdjustments;
    private int gh = globalHyperate.GlobalHeartRate;
    // public GameObject parentObject;
    public int hh = 105;

    private int number = 0; // Initial number

    private void Start()
    {
        //parentObject.SetActive(false);
        // Check if the Volume has a ColorAdjustments component
        if (volume.profile.TryGet(out colorAdjustments))
        {
            Debug.Log("Color Adjustments found in the volume profile.");
        }
        else
        {
            Debug.LogError("Color Adjustments not found in the volume profile. Please add it.");
        }
    }

    private void Update()
    {
        //ParticleSystem[] particleSystems = parentObject.GetComponentsInChildren<ParticleSystem>();
        gh = globalHyperate.GlobalHeartRate;
        if (colorAdjustments != null)
        {
            // Increment logic
            if (number > -30 && gh>hh)
            {
                number--;
                // foreach (ParticleSystem ps in particleSystems)
                // {
                //     var emission = ps.emission;
                //     emission.enabled = true;
                // }
                //parentObject.SetActive(true);
            }
            else
            {
                // Decrement logic
                if (number < 0)
                {
                    number++;
                    // foreach (ParticleSystem ps in particleSystems)
                    // {
                    //     var emission = ps.emission;
                    //     emission.enabled = false;
                    // }
                    //parentObject.SetActive(false);
                }
            }

            // Apply the number to the Color Adjustments property
            colorAdjustments.contrast.value = number; // Example using saturation, modify as needed
        }
    }
}
