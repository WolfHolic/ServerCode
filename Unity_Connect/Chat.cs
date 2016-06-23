using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class Chat : MonoBehaviour {

    Vector2 currentScrollPos = new Vector2();
    string received_texts;
    string input_text;
    // Use this for initialization
    void Awake()
    {
        TestNet testNet = GameObject.Find("test").GetComponent<TestNet>();
        this.input_text = "";
    }

    public void on_receive_chat_msg(string text)
    {

        this.currentScrollPos.y = float.PositiveInfinity;
    }
    void OnGUI()
    {
        // Received text.
        GUILayout.BeginVertical();
        currentScrollPos = GUILayout.BeginScrollView(currentScrollPos,
            GUILayout.MaxWidth(Screen.width), GUILayout.MinWidth(Screen.width),
            GUILayout.MaxHeight(Screen.height - 100), GUILayout.MinHeight(Screen.height - 100));

        GUILayout.BeginHorizontal();
        GUI.skin.label.wordWrap = true;
        GUILayout.Label(received_texts);
        GUILayout.EndHorizontal();

        GUILayout.EndScrollView();
        GUILayout.EndVertical();


        // Input.
        GUILayout.BeginHorizontal();
        this.input_text = GUILayout.TextField(this.input_text, GUILayout.MaxWidth(Screen.width - 100), GUILayout.MinWidth(Screen.width - 100),
            GUILayout.MaxHeight(50), GUILayout.MinHeight(50));

        if (GUILayout.Button("Send", GUILayout.MaxWidth(100), GUILayout.MinWidth(100), GUILayout.MaxHeight(50), GUILayout.MinHeight(50)))
        {
            TestNet testNet = GameObject.Find("test").GetComponent<TestNet>();
            testNet.beginSend(this.input_text);
            this.input_text = "";            
        }
        GUILayout.EndHorizontal();
    }
}
