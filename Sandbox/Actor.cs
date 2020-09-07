using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using WrecklessScript.Core;
using WrecklessScript.Core.Input;

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
        }
        public override void Update()
        {
            //if(GamePad.IsPressed(Button.GAMEPAD_A))
                //Debug.Log("Here in Update routine. FrameCount: " + Time.FrameCount + " DeltaTime is: " + Time.DeltaTime + " | " + Name);
            Debug.Log(GetComponent<TagComponent>().Tag);
        }
    }
}
