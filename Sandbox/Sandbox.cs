using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WrecklessScript.Core.Input;
using WrecklessScript.Core;

namespace Sandbox
{
    public class Sandbox
    {
        public static void Function()
        {
            string str = "Penis228";
            WrecklessScript.Core.Debug.Log("Hello guys " + " Dickson " + " Kamal " + " Rich boy ");
            WrecklessScript.Core.Debug.Log($"That's very funny {str}");
        }

        public static void UpdateRoutine(float delta)
        {
            //Debug.Log($"Delta: {delta}ms");
            
            if (GamePad.IsPressed(Button.GAMEPAD_X))
                Debug.Log("X button is pressed");
            if (GamePad.IsPressed(Button.GAMEPAD_Y))
                Debug.Log("Y button is pressed");
            if (GamePad.IsPressed(Button.GAMEPAD_B))
                Debug.Log("B button is pressed");
            if (GamePad.IsPressed(Button.GAMEPAD_A))
                Debug.Log("A button is pressed");

            //GamePad.SetVibration(50, 30);

            /*var leftStick = GamePad.LeftStick();

            Debug.Log($"Left stick: {leftStick.X} : {leftStick.Y}");
            Debug.Assert(false);*/

            /*var mouseCoords = Mouse.GetPosition();

            Debug.Log($"Mouse coords: {mouseCoords.X} :: {mouseCoords.Y}");*/

            if (Mouse.IsLeftMouseDown())
                Debug.Log("Suck my dick nigga");
        }
    }
}
