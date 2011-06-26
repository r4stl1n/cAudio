using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using cAudio;

namespace CSharpTutorial6_CustomEventHandler
{
    public class EventHandler : ISourceEventHandler
    {

        public override void onUpdate()
        {
            Console.WriteLine("onPlay");
        }

        public override void onRelease()
        {
            Console.WriteLine("onRelease");
        }

        public override void onPlay()
        {
            Console.WriteLine("onPlay");
        }

        public override void onStop()
        {
            Console.WriteLine("onStop");
        }

        public override void onPause()
        {
            Console.WriteLine("onPause");
        }
    }
}
