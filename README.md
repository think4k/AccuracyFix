<h1 align="center">Counter-Strike 1.6 Accuracy Fix</h1>
<p align="center">Counter-Strike 1.6 Accuracy Fix Plugin for MetaMod and ReGameDLL_CS</p>

<p align="center">
    <a href="https://github.com/SmileYzn/AccuracyFix/issues"><img alt="GitHub Issues" src="https://img.shields.io/github/issues-raw/smileyzn/AccuracyFix?style=flat-square"></a>
    <a href="https://github.com/SmileYzn/AccuracyFix/actions"><img alt="GitHub Workflow Status" src="https://img.shields.io/github/actions/workflow/status/SmileYzn/AccuracyFix/msbuild.yml?branch=main&label=Windows&style=flat-square"></a>
    <a href="https://github.com/SmileYzn/AccuracyFix/actions"><img alt="GitHub Workflow Status" src="https://img.shields.io/github/actions/workflow/status/smileyzn/AccuracyFix/makefile.yml?branch=main&label=Linux&style=flat-square"></a>
</p>

<h3>About Plugin</h3>
<p>This works with TraceLine function from Half-Life Engine, and make the default CS aim better.</p>
<p>It also can control weapons recoil, but that is not the real objective of plugin!</p>

------

<h3>Settings and variables</h3>

<details>
    <summary>Click to open</summary>
    
    // Accuracy of all weapons
    // If is set, it will replace all weapon variables
    // The default distance to fix trace line is 8192.0
    // Set to -1.0 to disable and use individual weapon values
    //
    // Default "-1.0"
    //
    af_accuracy_all     "-1.0"
    
    // Accuracy of each weapon
    // The default distance to fix trace line is 8192.0
    //
    // Default "8192.0"
    //
    af_accuracy_weapon_ak47 	"8192.0"
    af_accuracy_weapon_aug 		"8192.0"
    af_accuracy_weapon_awp 		"8192.0"
    af_accuracy_weapon_deagle 	"8192.0"
    af_accuracy_weapon_elite 	"8192.0"
    af_accuracy_weapon_famas	"8192.0"
    af_accuracy_weapon_fiveseven	"8192.0"
    af_accuracy_weapon_g3sg1 	"8192.0"
    af_accuracy_weapon_galil 	"8192.0"
    af_accuracy_weapon_glock 	"8192.0"
    af_accuracy_weapon_glock18 	"8192.0"
    af_accuracy_weapon_m249 	"8192.0"
    af_accuracy_weapon_m3 		"2020.0"
    af_accuracy_weapon_m4a1 	"8192.0"
    af_accuracy_weapon_mac10 	"8192.0"
    af_accuracy_weapon_mp5navy 	"8192.0"
    af_accuracy_weapon_p228 	"8192.0"
    af_accuracy_weapon_p90 		"8192.0"
    af_accuracy_weapon_scout 	"8192.0"
    af_accuracy_weapon_sg550 	"8192.0"
    af_accuracy_weapon_sg552 	"8192.0"
    af_accuracy_weapon_tmp 		"8192.0"
    af_accuracy_weapon_ump45 	"8192.0"
    af_accuracy_weapon_usp 		"8192.0"
    af_accuracy_weapon_xm1014 	"2020.0"

    // Aim distance check for all weapons
    // If is set, it will replace all weapon variables
    // The default aim distance check of an weapon is 2000.0
    // Set to -1.0 to disable and use individual weapon values
    //
    // Default "-1.0"
    //
    af_distance_all 		"-1.0"
    
    // Aim distance check of each weapon
    // The default distance check aim is 8192.0
    //
    // Default "8192.0"
    //
    af_distance_weapon_ak47 	"8192.0"
    af_distance_weapon_aug 		"8192.0"
    af_distance_weapon_awp 		"8192.0"
    af_distance_weapon_deagle 	"8192.0"
    af_distance_weapon_elite 	"8192.0"
    af_distance_weapon_famas	"8192.0"
    af_distance_weapon_fiveseven	"8192.0"
    af_distance_weapon_g3sg1 	"8192.0"
    af_distance_weapon_galil 	"8192.0"
    af_distance_weapon_glock 	"8192.0"
    af_distance_weapon_glock18 	"8192.0"
    af_distance_weapon_m249 	"8192.0"
    af_distance_weapon_m3 		"8192.0"
    af_distance_weapon_m4a1 	"8192.0"
    af_distance_weapon_mac10 	"8192.0"
    af_distance_weapon_mp5navy 	"8192.0"
    af_distance_weapon_p228 	"8192.0"
    af_distance_weapon_p90 		"8192.0"
    af_distance_weapon_scout 	"8192.0"
    af_distance_weapon_sg550 	"8192.0"
    af_distance_weapon_sg552 	"8192.0"
    af_distance_weapon_tmp 		"8192.0"
    af_distance_weapon_ump45 	"8192.0"
    af_distance_weapon_usp 		"8192.0"
    af_distance_weapon_xm1014 	"8192.0"
    
</details>
