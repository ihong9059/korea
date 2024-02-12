nrfjprog --recover

# nrfjprog -f NRF52 --program ./build/pca10040/test/final_image_test.hex --chiperase
# nrfjprog -f NRF52 --program ./build/pca10040/bike/final_image_bike.hex --chiperase
# nrfjprog -f NRF52 --program ./build/thingy/thingy/final_image_thingy.hex --chiperase
# nrfjprog -f NRF52 --program ./build/pca10040/ssd1306/final_image_ssd1306.hex --chiperase
# nrfjprog -f NRF52 --program ./build/pca10040/lamp/final_image_lamp.hex --chiperase
nrfjprog -f NRF52 --eraseall 

# nrfjprog --reset

