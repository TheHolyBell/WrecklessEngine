using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
            //AddComponent<TagComponent>();
            //RemoveComponent<TagComponent>();
            //AddComponent<TransformComponent>().Transform = Matrix4.Translate(new Vector3(5, 20, 100));
            
        }
        public override void Update()
        {
            /*Matrix4 mat = new Matrix4();
            mat.D00 = 5;
            mat.D10 = 10;
            mat.D20 = 15;*/
            //if (HasComponent<TransformComponent>())
            //    GetComponent<TransformComponent>().Transform = mat;
            //if(GamePad.IsPressed(Button.GAMEPAD_A))
            //Debug.Log("Here in Update routine. FrameCount: " + Time.FrameCount + " DeltaTime is: " + Time.DeltaTime + " | " + Name);

            /*if(Keyboard.IsKeyDown(KeyCode.Escape))
                Debug.Log("Update function. Tag: " + GetComponent<TagComponent>().Tag + ". Total time: " + Time.TotalTime + ". Delta Time: " + Time.DeltaTime);
            if (HasComponent<TransformComponent>())
            {
                //Debug.Log(GetComponent<TagComponent>().Tag);
                GetComponent<TransformComponent>().Transform.DebugPrint();
                Debug.Log("\n\n");
            }
                //Debug.Log("Hey guys!!! I have transform component");
            */

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
            Debug.Log("Dickson " + Time.TotalTime + " Scale: " + Scale);
        }
    }
}
