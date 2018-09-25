640x480 resolution images

Selected images from DsGroundTruth:
hand_scan
- hand_scan -> hand_model_1 -> 641112023095 -> rez_640x480 -> Eauto_Loff -> Left -> *1000.png
- hand_scan -> hand_model_2 -> 641112023095 -> rez_640x480 -> Eauto_Loff -> Left -> *1000.png
- hand_scan -> hand_model_3 -> 641112023095 -> rez_640x480 -> Eauto_Loff -> Left -> *1000.png
- hand_scan -> hand_model_4 -> 641112023095 -> rez_640x480 -> Eauto_Loff -> Left -> *1000.png
- hand_scan -> hand_model_5 -> 641112023095 -> rez_640x480 -> Eauto_Loff -> Left -> *1000.png
- hand_scan -> hand_real_2  -> 641112023095 -> rez_640x480 -> Eauto_Loff -> Left -> *1000.png
heptagon_3
- heptagon_3 -> test_scene_1 -> 641112023129 -> rez_640x480 -> E1_Loff -> Left -> *1000.png
- heptagon_3 -> test_scene_2 -> 641112023129 -> rez_640x480 -> E1_Loff -> Left -> *1000.png

hand_scan
- hand_scan -> hand_model_1 -> 641112023095 -> rez_640x480 -> Eauto_Loff -> Right -> *1000.png
- hand_scan -> hand_model_2 -> 641112023095 -> rez_640x480 -> Eauto_Loff -> Right -> *1000.png
- hand_scan -> hand_model_3 -> 641112023095 -> rez_640x480 -> Eauto_Loff -> Right -> *1000.png
- hand_scan -> hand_model_4 -> 641112023095 -> rez_640x480 -> Eauto_Loff -> Right -> *1000.png
- hand_scan -> hand_model_5 -> 641112023095 -> rez_640x480 -> Eauto_Loff -> Right -> *1000.png
- hand_scan -> hand_real_2  -> 641112023095 -> rez_640x480 -> Eauto_Loff -> Right -> *1000.png
heptagon_3
- heptagon_3 -> test_scene_1 -> 641112023129 -> rez_640x480 -> E1_Loff -> Right -> *1000.png
- heptagon_3 -> test_scene_2 -> 641112023129 -> rez_640x480 -> E1_Loff -> Right -> *1000.png

Description:
## Stereo Image Files
* Input images to a stereo algorithm are located at Root->dataset->scene->camera->resolution->lighting_condition, for example, `S:\DsAlgoGroundTruth\hand_scan\hand_model_1\641112023095\rez_640x480\Eauto_Loff\`. Here, `Eauto_Loff` means 'exposure  is auto and laser (emitter) is off'. 
* In the lighting condition directory, there are 4 subdirectories `Left`, `Right`, `LeftColor`, `RightColor` which contain the left monochrome, right monochrome, left RGB, right RGB images respectively. 
* All images are in png formats and are named like `1000.png`, `1001.png`, etc. They were taken consecutively for the the same scene.
* Monochrome images are 12 bits and RGB images are 10 bits. 
* The lighting condition directory name usually describes the exposure and laser emitter settings. For example, `Eauto_Loff` means 'exposure=auto' and 'laser is off'. 


## Ground Truth Files
* The ground truths files for that scene/camera/resolution are contained under  Root->dataset->scene->camera->resolution->GroundTruth, for example, `S:\DsAlgoGroundTruth\hand_scan\hand_model_1\641112023095\rez_640x480\GroundTruth\`
* Ground truth files include a disparity file (ending in`gtdisp.png`) and a mask file ending in `gtmask.png`. 
* The disparity file is in `.pfm` format and in floats. 
* The mask file is in 8 bit png grayscale format. There are 3 values for the mask: 0 means invalid/no information; 255 mean valid, visible; and 128 means occluded.

