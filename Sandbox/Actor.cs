using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using WrecklessScript.Core;

namespace Sandbox
{
    public class Actor : Entity
    {
        static int Number = 0;
        int m_Number = 0;
        public override void Start()
        {
            m_Number = Number;
            Number++;
            Debug.Log("I've been created");
        }
        public override void Update()
        {
            Debug.Log("Here in Update routine. DeltaTime is: " + Time.DeltaTime + " | Number: " + m_Number);
        }
    }
}
