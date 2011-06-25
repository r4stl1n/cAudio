using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using cAudio;

namespace CSharpTutorial1_2DSound
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            //Some fancy text
            Console.WriteLine("cAudio 2.2.0 Tutorial 1: Basic 2D Audio. C# \n \n");

            Console.WriteLine("\nAvailable Playback Devices: \n");

            //Create an uninitialized Audio Manager
            IAudioManager mAudioMgr = cAudioCSharpWrapper.createAudioManager(false);

            //Grab a list of avalible devices
            IAudioDeviceList pDeviceList = cAudioCSharpWrapper.createAudioDeviceList();

            //Get the number of avalible devices.
            uint deviceCount = pDeviceList.getDeviceCount();

            //Get the defualt device name.
            string defaultDeviceName = pDeviceList.getDefaultDeviceName().ToString();

            for(uint i=0; i< deviceCount; ++i)
		    {
		    	string deviceName = pDeviceList.getDeviceName(i).ToString();

			    if(deviceName.Equals(defaultDeviceName))
				    Console.WriteLine(" "+ i + "): " + deviceName.ToString() + " [DEFAULT] \n");
			    else
				    Console.WriteLine(" "+ i + "): " + deviceName.ToString() + " \n");
		    }

            Console.WriteLine("\n");
            Console.WriteLine("Choose a device by number: ");
            string deviceSelection = Console.ReadLine();
            uint deviceSelect = Convert.ToUInt32(deviceSelection);
            Console.WriteLine("\n");
            mAudioMgr.initialize(pDeviceList.getDeviceName(deviceSelect).ToString());

            if (mAudioMgr != null)
            {

                IAudioSource mCurrentSource;

                mCurrentSource = mAudioMgr.create("bing", "../../../../Media/cAudioTheme1.ogg");

                if (mCurrentSource != null)
                {

                    mCurrentSource.setVolume(0.5f);
                    mCurrentSource.play2d(false);


                    while (mCurrentSource.isPlaying())
                    {
                        cAudioCSharpWrapper.cAudioSleep(10);
                    }
                }

                mAudioMgr.shutDown();
                cAudioCSharpWrapper.destroyAudioManager(mAudioMgr);
            }
            else
	        {
		        Console.WriteLine("Failed to create audio playback manager. \n");
	        }

            Console.WriteLine("Press any key to quit \n");
            Console.ReadLine();
        }
    }
}
