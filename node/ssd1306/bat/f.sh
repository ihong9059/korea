nrfjprog --recover

# nrfjprog -f NRF52 --program ./build/pca10040/test/final_image_test.hex --chiperase
# nrfjprog -f NRF52 --program ./build/pca10040/bike/final_image_bike.hex --chiperase
nrfjprog -f NRF52 --program /Users/honggwangseon/wm-sdk/build/ssd1306/ssd1306/final_image_ssd1306.hex --chiperase
# nrfjprog -f NRF52 --program ./build/pca10040/ssd1306/final_image_ssd1306.hex --chiperase
# nrfjprog -f NRF52 --program /Users/honggwangseon/wm-sdk/build/rack/rack/final_image_rack.hex --chiperase

nrfjprog --reset

