# Boards compatible with this app 
TARGET_BOARDS := ssd1306 pca10040 korea
#for temporary 2023.02.07
#korea 2024.02.09 0x04 = company, channel = 0x04
default_network_address ?= 4
default_network_channel ?= 4
default_network_cipher_key ?= 0x04,0x04,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15
default_network_authen_key ?= 0x04,0x04,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15
#  								   00010203040506070809101112131415

# Define a specific application area_id
app_specific_area_id=0x80597B

#define node operating mode (i.e low-energy or low-latency : 0 => low-energy / 1 => low-latency)
default_operating_mode ?= 0

# App version
app_major=1
app_minor=1
app_maintenance=1
app_development=0
