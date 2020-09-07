using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using WrecklessScript.Core;
using WrecklessScript.Core.Input;

namespace Sandbox
{
    public class Shkura : Entity
    {
        public override void Start()
        {
            GetComponent<TagComponent>().Tag = "Su4ka";
            //AddComponent<TransformComponent>();
            Debug.Log(GetComponent<TagComponent>().Tag + " has been created");
        }

        public override void Update()
        {
            //Debug.Log("Skuroten'");
            //GamePad.VibrationState state;
            //state.LeftMotor = 40;
            //state.RightMotor = 40;
            //GamePad.Vibration = state;
            Debug.Log("Skura::Update. ID" + ID);
            Debug.Log(GetComponent<TagComponent>().Tag);
        }
    }
}
