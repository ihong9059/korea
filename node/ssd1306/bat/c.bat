
@REM compile
@REM docker run --rm -v /c/wirepas/wm-sdk:/home/wirepas/wm-sdk -w /home/wirepas/wm-sdk -ti wirepas/sdk-builder:v1.2 make app_name=dualmcu_app target_board=pca10040

@REM docker run --rm -v /c/wirepas/wm-sdk:/home/wirepas/wm-sdk -w /home/wirepas/wm-sdk -ti wirepas/sdk-builder:v1.2 make app_name=tokai target_board=pca10040
docker run --rm -v /c/mesh/wm-sdk:/home/wirepas/wm-sdk -w /home/wirepas/wm-sdk -ti wirepas/sdk-builder:v1.2 make app_name=ssd1306 target_board=ssd1306
@REM docker run --rm -v /c/wirepas/wm-sdk:/home/wirepas/wm-sdk -w /home/wirepas/wm-sdk -ti wirepas/sdk-builder:v1.2 make app_name=ssd1306 target_board=pca10040
@REM docker run --rm -v /c/wirepas/wm-sdk:/home/wirepas/wm-sdk -w /home/wirepas/wm-sdk -ti wirepas/sdk-builder:v1.2 make app_name=base target_board=pca10040
@REM docker run --rm -v /c/mesh/wm-sdk:/home/wirepas/wm-sdk -w /home/wirepas/wm-sdk -ti wirepas/sdk-builder:v1.2 make app_name=rack target_board=rack
@REM docker run --rm -v /c/mesh/wm-sdk:/home/wirepas/wm-sdk -w /home/wirepas/wm-sdk -ti wirepas/sdk-builder:v1.2 make app_name=node target_board=pca10040
