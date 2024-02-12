nrfjprog.exe --recover

@REM nrfjprog.exe -f NRF52 --program \wirepas\wm-sdk\build\pca10040\tokai_ll\final_image_tokai_ll.hex --chiperase
nrfjprog.exe -f NRF52 --program /mesh/wm-sdk/build/korea/korea/final_image_korea.hex --chiperase
@REM nrfjprog.exe -f NRF52 --program \mesh\wm-sdk\build\pca10040\node\final_image_node.hex --chiperase

nrfjprog --reset

