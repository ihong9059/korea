# You can add more sources here and libraries if needed
SRCS += $(SRCS_PATH)/uttec/uttec.c
SRCS += $(SRCS_PATH)/uttec/mesh.c
SRCS += $(SRCS_PATH)/uttec/Uart.c
SRCS += $(SRCS_PATH)/uttec/wirepas.c
SRCS += $(SRCS_PATH)/uttec/ssd1306.c
SRCS += $(SRCS_PATH)/uttec/ssd1309_fonts.c
SRCS += $(SRCS_PATH)/uttec/myJob.c
SRCS += $(SRCS_PATH)/uttec/twi.c
SRCS += $(SRCS_PATH)/uttec/bh1745.c
SRCS += $(SRCS_PATH)/uttec/eeprom.c
SRCS += $(SRCS_PATH)/uttec/lps22hb.c
SRCS += $(SRCS_PATH)/uttec/ccs811.c
SRCS += $(SRCS_PATH)/uttec/aht20.c
SRCS += $(SRCS_PATH)/uttec/bmp280.c
SRCS += $(SRCS_PATH)/uttec/bh1750.c
SRCS += $(SRCS_PATH)/uttec/sx1509.c
SRCS += $(SRCS_PATH)/uttec/mpu9250.c
INCLUDES += -I$(SRCS_PATH)/uttec

# SRCS +=
# INCLUDES +=
# LIBS +=
# LDFLAGS +=

# Example:
# SRCS += $(SRCS_PATH)my_driver.c
# LIBS += my_lib.a
# include math.h lib
LIBS +=
LDFLAGS += -lm

# Define default network settings
CFLAGS += -DNETWORK_ADDRESS=$(default_network_address)
CFLAGS += -DNETWORK_CHANNEL=$(default_network_channel)

#Set node operating mode (i.e low-energy or low-latency)
ifneq ($(default_operating_mode),0)
    CFLAGS+= -DENABLE_LOW_LATENCY_MODE
endif

# Use Shared Data
SHARED_DATA=yes

# Use App Scheduler and configure it
APP_SCHEDULER=yes

APP_PRINTING=yes
CFLAGS += -DAPP_SCHEDULER_MAX_TASKS=2

SHARED_APP_CONFIG=yes
CFLAGS += -DSHARED_APP_CONFIG_MAX_FILTER=2

# This application use HAL for buttons and leds
HAL_BUTTON=yes
HAL_LED=yes
HAL_VOLTAGE=yes
HAL_PERSISTENT_MEMORY=yes
HAL_I2C=yes 
HAL_HW_DELAY=yes

SRCS += $(NRF52_HAL_PREFIX)i2c.c

# SRCS += $(SRCS_PATH)/uttec/uttec.c
# SRCS += $(SRCS_PATH)/uttec/mesh.c
# SRCS += $(SRCS_PATH)/uttec/Uart.c
# SRCS += $(SRCS_PATH)/uttec/wirepas.c
# INCLUDES += -I$(SRCS_PATH)/uttec
