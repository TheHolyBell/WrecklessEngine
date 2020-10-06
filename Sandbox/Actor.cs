using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using WrecklessScript.Core;
using WrecklessScript.Core.Input;
using WrecklessScript.Core.Math;

namespace Sandbox
{
    public class Actor : Entity
    {
        static int Number = 0;
        int m_Number = 0;
        
        public string Name { get; set; }

        public int Scale = 0;
        public Vector3 Translation = default;

        public override void Start()
        {
            m_Number = Number;
            Number++;
            Debug.Log("I've been created");
            
        }
        public override void Update()
        {
            var translation = GetComponent<TransformComponent>().Transform.Translation;
  
            var rotMat = Matrix4.RotationY(Time.TotalTime);
            

            if (GamePad.IsPressed(Button.ArrowUp))
            {
                translation.Y++;
            }
            else if (GamePad.IsPressed(Button.ArrowDown))
            {
                translation.Y--;
            }
            else if (GamePad.IsPressed(Button.ArrowRight))
            {
                translation.X++;
            }
            else if (GamePad.IsPressed(Button.ArrowLeft))
            {
                translation.X--;
            }

            rotMat.D41 = Translation.X;
            rotMat.D42 = Translation.Y;
            rotMat.D43 = Translation.Z;
            GetComponent<TransformComponent>().Transform = rotMat;

            if (GamePad.IsPressed(Button.LShoulder))
                Debug.Log("LS pressed");
            Debug.Log("Dummy message " + Time.TotalTime + " Scale: " + Scale);
        }
    }
}
