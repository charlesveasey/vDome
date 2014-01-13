vDome
=====

Multi-channel projection software designed for dome projection. Provides real-time warping and slicing for domemaster input.<br /><br />
Developed by Charles Veasey for the Institute of Indian American Arts (IAIA). <br />
Special thanks for prior technical research by: ARTS Labs at the University of New Mexico and Paul Bourke at the University of Western Australia.

##Overview:

vDome is designed to run as background process and accepts any domemaster formatted input as follows: <br />
    - capture (camera or capture card) <br />
    - syphon <br />
    - image <br />
    - video <br />  
        
The capture technique is new and is now favored over the tcp socket input type. 
            - It is much faster and more flexible. 
            - It allows you to put anything on the dome by hooking up another computer to vDome through a capture card.               - vDome essentially becomes the 2nd monitor on your production machine. 
            - For example, I can drag my After Effects preview window onto the dome, and edit in real-time. 
            - Playing videos is done through your favorite media player: Quicktime, VLC, etc. 
            - Simplifies everything, as there are no restrictions on what can be placed onto the dome.
            - Eliminates middle-ware such as Syphon.


        Syphon is useful for a single Mac computer system where your production application supports the Syphon technology.
        
        The image and video file input types I only use for testing and calibrating at the moment, so the only way to change the file name is in code.


    Setup and Calibration
        Initial setup is done in XML ( setting resolution, number of projectors, and input type )
        Calibration is done directly on the dome. This eliminates the need for a control monitor and saves the headache caused by looking back and forth from control monitor to dome.
        There are also some new calibration options (camera perspective shearing) that should simplify calibration compared to the last version.


    Future Development(s)
        - Advanced color correction
            - mandatory goal
            - color channel look up table
        - Edge-blending
            not needed for our dome or yours, but important for others who don't have hardware masking
        - Auto-calibration via camera
            for us, auto-color correction has priority over auto-spatial b/c non-led projectors constantly shift spectrum
        - Direct support for Nvidia warp and intensity API
            stretch goal, we are using AMD Firepro, but have Nvidia cards to test
            some Nvidia cards now support warping and blending directly (meaning you can conform the actual windows desktop to fit onto the dome)
            allows anything you can do on Windows to be on the dome without additional hardware or software (possible $$ saver), but less flexible than capture technique
            vDome would become the calibration tool that creates warp / blend data, but once saved the graphics card would actually do the warp/blend, not vDome.

