# Template code mod for Sonic Generations (2011)
This is a template for a new code mod project using the BlueBlur API and related helpers. This covers about everything necessary to get started making a code mod. Making code mods requires some knowledge of C++ and some reverse engineering skills, help will not be provided for those! **Please read the readme before using this solution!**

# How to use
To use the template, you must do these things:
1. Install Visual Studio 2017 or above
2. Install MSVC v142 in the Visual Studio Installer
3. Install Sonic Generations (2011) and HedgeModManager
4. Clone or fork this repository
5. Define GensPath (look below for a guide)
If you've completed all these steps, you should be able to press F5 with the solution open to start debugging the mod (make sure to enable it in HedgeModManager). There is also a powershell script to rename the `TemplateMod` references to your own mod name, just make sure to avoid spaces.

This solution automatically takes care of copying the mod files to your mod's folder, by default it will go to a folder with the same name as the mod (e.g `mods\TemplateMod`), you can change this by editing the Post Build Event in the project.

# Defining GensPath
To add GensPath to PATH:
1. Search for "Edit the system environment variables"
2. Go to "Environment Variables..."
3. Add a new User Variable, set its name to `GensPath` and set the value to the folder containing `SonicGenerations.exe`, NOT the path to the executable itself, but it's parent folder (e.g `C:\Program Files (x86)\Steam\steamapps\common\Sonic Generations\`)
4. Log out and log back in or restart Windows
