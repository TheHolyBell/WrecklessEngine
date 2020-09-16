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

            var tr = GetComponent<TransformComponent>().Transform;

            var rotMat = Matrix4.RotationY(Time.TotalTime);
            tr.D11 = rotMat.D11;
            tr.D12 = rotMat.D12;
            tr.D13 = rotMat.D13;

            tr.D21 = rotMat.D21;
            tr.D22 = rotMat.D22;
            tr.D23 = rotMat.D23;

            tr.D31 = rotMat.D31;
            tr.D32 = rotMat.D32;
            tr.D33 = rotMat.D33;

            if (GamePad.IsPressed(Button.GAMEPAD_ARROW_UP))
            {
                tr.D42++;
            }
            else if (GamePad.IsPressed(Button.GAMEPAD_ARROW_DOWN))
            {
                tr.D42--;
            }
            else if (GamePad.IsPressed(Button.GAMEPAD_ARROW_RIGHT))
            {
                tr.D41++;
            }
            else if (GamePad.IsPressed(Button.GAMEPAD_ARROW_LEFT))
            {
                tr.D41--;
            }
            GetComponent<TransformComponent>().Transform = tr;
            //Debug.Log("Privetiki:)");
        }
    }
}
