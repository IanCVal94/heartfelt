using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class DebugLogOutput : MonoBehaviour
{
	public TextMeshPro debugText;

	void OnEnable()
	{
		Application.logMessageReceived += HandleLog;
	}

	void OnDisable()
	{
		Application.logMessageReceived -= HandleLog;
	}

	void HandleLog(string logString, string stackTrace, LogType type)
	{
		if (debugText != null)
		{
			debugText.text = logString + "\n" + debugText.text;

			// Split the text into lines and check the number of lines
			string[] lines = debugText.text.Split('\n');
			if (lines.Length > 50) // Assuming maxLines is 50
			{
				// Remove the last line
				debugText.text = debugText.text.Substring(0, debugText.text.LastIndexOf('\n'));
			}
		}
	}

	// Start is called before the first frame update
	void Start()
	{
		
	}

	// Update is called once per frame
	void Update()
	{
		
	}
}