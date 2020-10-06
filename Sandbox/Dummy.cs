using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WrecklessScript.Core;

namespace Sandbox
{
    public class Dummy : Entity
    {
        public int Scale = 0;
        public override void Start()
        {
           
        }

        public override void Update()
        {
            Debug.Log("Hello guys, this is the Wreckless Game Engine " + Scale);
        }
    }
}
