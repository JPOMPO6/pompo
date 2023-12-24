
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <zephyr/types.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/settings/settings.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/kernel.h>
#include <zephyr/mgmt/mcumgr/smp_bt.h>
#include <zephyr/logging/log.h>

#include <nrf.h>
#include <nrf52832_peripherals.h>
#include <nrfx_gpiote.h>
#include <nrfx.h>
#include <nrfx_uart.h>
#include <nrfx_timer.h>

#include "./user/H3LIS331DL.h"
#include "./user/nrf_delay.h"
#include "./user/LSM6DSO.h"
#include "./user/uart.h"

#define LED1 6 // 6

// --------    %%    Bluetooth Define Block    %%    ---------- //
#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)
#define BT_UUID_CUSTOM_SERVICE_VAL \
	BT_UUID_128_ENCODE(0x1bc50001, 0x0200, 0xb8be, 0xe611, 0xe60c60b7c467)
static struct bt_uuid_128 primary_service_uuid = BT_UUID_INIT_128(BT_UUID_CUSTOM_SERVICE_VAL);

static bt_addr_le_t bond_addr;

// ---------------------- Last shot stats process Block ------------------------ //
#define SHOT_COUNT 2
#define VAL_ACC 4
#define VAL_ACC_X 6
#define VAL_ACC_Y 8
#define VAL_ACC_Z 10
#define VAL_GYRO 12
#define VAL_GYRO_X 14
#define VAL_GYRO_Y 16
#define VAL_GYRO_Z 18
#define VAL_GYRO_D 20
#define LAST_SHOT_STATUS_VALUE_LEN 22
static uint8_t advertising_buff[LAST_SHOT_STATUS_VALUE_LEN] = {0};

BT_GATT_SERVICE_DEFINE(primary_service,
					   BT_GATT_PRIMARY_SERVICE(&primary_service_uuid), );

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_CUSTOM_SERVICE_VAL),
};

struct bt_le_adv_param adv_param = {
	.id = BT_ID_DEFAULT,
	.interval_min = BT_GAP_ADV_FAST_INT_MIN_2,
	.interval_max = BT_GAP_ADV_FAST_INT_MAX_2,
	.options = BT_LE_ADV_OPT_CONNECTABLE | BT_LE_ADV_OPT_USE_NAME, //
};

static void connected(struct bt_conn *conn, uint8_t err)
{
	nrf_uart_disable(UART_INSTANCE);
	// k_sleep(K_SECONDS(1));
	if (err)
	{
		// uart_puts("\r\nConnection failed (err 0x"); decimal_to_char(err); uart_puts(")\r\n");
	}
	else
	{
		// uart_puts("\r\nConnected\r\n");
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	// uart_puts("\r\nDisconnected (reason 0x"); decimal_to_char(reason); uart_puts(")\r\n");
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected};

static void copy_last_bonded_addr(const struct bt_bond_info *info, void *data)
{
	bt_addr_le_copy(&bond_addr, &info->addr);
}

static void bt_ready(void)
{
	int err;
	//char addr[BT_ADDR_LE_STR_LEN];

	// uart_puts("\r\nBluetooth initialized\r\n");

	if (IS_ENABLED(CONFIG_SETTINGS))
	{
		settings_load();
	}

	static const struct bt_data sd[] = {
		BT_DATA(BT_DATA_MANUFACTURER_DATA, advertising_buff, 22),
	};

	err = bt_le_adv_start(&adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd)); // sd, ARRAY_SIZE(sd)

	if (err)
	{
		// uart_puts("\r\nAdvertising failed to start (err"); decimal_to_char(err); uart_puts(")\r\n");
	}
	else
	{
		// uart_puts("\r\nAdvertising successfully started\r\n");
	}
}

void pairing_complete(struct bt_conn *conn, bool bonded)
{
	// uart_puts("\r\nPairing completed. Rebooting in 5 seconds...\r\n");
	k_sleep(K_SECONDS(1));
	sys_reboot(SYS_REBOOT_WARM);
}

static struct bt_conn_auth_info_cb bt_conn_auth_info = {
	.pairing_complete = pairing_complete};
// -------------------------------------------//

// --------    %%    Timer Define Block    %%    ---------- //
#define TIMER_INST_IDX 3
#define TIME_WAIT_MS 10UL
uint8_t user_timer_flg;
uint32_t timer_val = 0;
// -------------------------------------------//

// --------    %%    User Variable Define Block    %%    ---------- //
//#define GYRO_THRESHOLD 2500
///#define ACC_THRESHOLD 1500
//#define ACCELEROMETER_PEAK_FACTOR 0.5
#define ACC_THRESHOLD 3 * 1000 // Assuming acceleration values are in milli-g's
#define CONSECUTIVE_READINGS 5
#define MIN_SHOTS_DETECTED 100
#define DATA_BUFFER_SIZE 300
int16_t acc_x_data[DATA_BUFFER_SIZE] = {0};
int16_t acc_y_data[DATA_BUFFER_SIZE] = {0};
int16_t acc_z_data[DATA_BUFFER_SIZE] = {0};
int32_t acc_val[DATA_BUFFER_SIZE] = {0};
int16_t gyro_x_data[DATA_BUFFER_SIZE] = {0};
int16_t gyro_y_data[DATA_BUFFER_SIZE] = {0};
int16_t gyro_z_data[DATA_BUFFER_SIZE] = {0};
int32_t gyro_val[DATA_BUFFER_SIZE] = {0};
int32_t gyro_dev[DATA_BUFFER_SIZE] = {0};
uint16_t shot_time_cnt = 0, flight_time_cnt = 0, m_time_cnt = 0, shot_cnt = 0;
uint16_t max_speed = 0;
uint32_t max_acc = 0;
bool shot_started = false;
bool shot_ended = false;
int shots_stated_cnt = 0, shots_ended_cnt = 0;
//int peak_acceleration = 0;

void led_init();
void user_timer_init();
void external_oscillator_start();
void calc_shot_status();
uint32_t findSQRT(uint64_t number);
int16_t calculate_shot_speed(int *accelerometer_data, int start_index, int end_index, uint16_t time_interval);
void shot_detect();
// -------------------------------------------//

// --------    %%   Main Function Define Block   %%    ---------- //
int main(void)
{
	external_oscillator_start();

	// uart init
	//  uart_init();
	//  uart_puts("\r\nThis is NRF52Ball test code!!\r\n");

	// Timer init
	user_timer_init();

	// led init
	led_init();

	// bluetooth init
	os_mgmt_register_group();
	img_mgmt_register_group();
	smp_bt_register();
	int err = bt_enable(NULL);
	if (err)
	{
		// uart_puts("\r\nBluetooth init failed (err"); decimal_to_char(err); uart_puts(")\r\n");
		return 0;
	}
	bt_ready();
	bt_conn_auth_info_cb_register(&bt_conn_auth_info);

	// H3LIS331DL Init
	init_h3i3(H3LIS331DL_ODR_1000Hz, H3LIS331DL_NORMAL, H3LIS331DL_FULLSCALE_2);

	// LSM6dSO Init
	init_lsm6dso();

	shot_ended = true;
	shot_started = false;
	// uart_puts("\r\n--- MEASURE START --->> ");
	while (1)
	{
		// k_sleep(K_SECONDS(1));
		timer_val = 0.1;
		k_msleep(.01);
		calc_shot_status();
		shot_detect();
	}
	return 0;
}

void led_init()
{
	for (uint8_t i = 0; i < 3; i++)
	{
		// uart_puts("\r\n----- LED ON -----\r\n");
		nrf_gpio_pin_set(LED1);
		nrf_delay_ms(2000);
		// uart_puts("\r\n----- LED OFF -----\r\n");
		nrf_gpio_pin_clear(LED1);
		nrf_delay_ms(2000);
	}
	// uart_puts("\r\n----- LED ON -----\r\n");
	nrf_gpio_pin_set(LED1);
}

void external_oscillator_start()
{
	// Use the external crystal oscillator as the system clock source
	NRF_CLOCK->TASKS_HFCLKSTOP = 1;
	NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
	NRF_CLOCK->TASKS_HFCLKSTART = 1;
	while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
	{
		// Wait for the system clock to switch to the external crystal oscillator
	}
}

void user_timer_handler(nrf_timer_event_t event_type, void *p_context)
{
	nrfx_timer_t *timer_inst = p_context;
	timer_val++;
}

void user_timer_init()
{
	// uart_puts("\r\n----- 																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																										Timer Init Start -----\r\n");
	nrfx_timer_t timer_inst = NRFX_TIMER_INSTANCE(TIMER_INST_IDX);
	nrfx_timer_config_t config = NRFX_TIMER_DEFAULT_CONFIG;
	config.bit_width = NRF_TIMER_BIT_WIDTH_32;
	config.p_context = &timer_inst;
	nrfx_err_t status = nrfx_timer_init(&timer_inst, &config, user_timer_handler);
	// NRFX_ASSERT(status == NRFX_SUCCESS);

#define TIMER_INST NRFX_CONCAT_2(NRF_TIMER, TIMER_INST_IDX)
#define TIMER_INST_HANDLER NRFX_CONCAT_3(nrfx_timer_, TIMER_INST_IDX, _irq_handler)
	IRQ_DIRECT_CONNECT(NRFX_IRQ_NUMBER_GET(TIMER_INST), IRQ_PRIO_LOWEST, TIMER_INST_HANDLER, 0);

	uint32_t desired_ticks = nrfx_timer_ms_to_ticks(&timer_inst, TIME_WAIT_MS);
	nrfx_timer_extended_compare(&timer_inst, NRF_TIMER_CC_CHANNEL0, desired_ticks,
								NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
	nrfx_timer_enable(&timer_inst);
	// nrfx_timer_is_enabled(&timer_inst); //? uart_puts("\r\n----- Timer enabled -----\r\n") : uart_puts("\r\n----- Timer disabled -----\r\n");
}

void saadc_init(void)
{
}

void calc_shot_status()
{
	AxesRaw_t data;
	int32_t sum[6] = {0};
	for (int i = 0; i < DATA_BUFFER_SIZE - 1; i++)
	{
		acc_x_data[i] = acc_x_data[i + 1];
		acc_y_data[i] = acc_y_data[i + 1];
		acc_z_data[i] = acc_z_data[i + 1];
		acc_val[i] = acc_val[i + 1];
		gyro_x_data[i] = gyro_x_data[i + 1];
		gyro_y_data[i] = gyro_y_data[i + 1];
		gyro_z_data[i] = gyro_z_data[i + 1];
		gyro_val[i] = gyro_val[i + 1];
		sum[0] += acc_x_data[i];
		sum[1] += acc_y_data[i];
		sum[2] += acc_z_data[i];
		sum[3] += gyro_x_data[i];
		sum[4] += gyro_y_data[i];
		sum[5] += gyro_z_data[i];
		if (max_acc < acc_val[i])
			max_acc = acc_val[i];
	}

	status_t response = get_acc_axes_h3i3(&data);
	if (MEMS_SUCCESS == response)
	{
		acc_x_data[DATA_BUFFER_SIZE - 1] = data.AXIS_X;
		acc_y_data[DATA_BUFFER_SIZE - 1] = data.AXIS_Y;
		acc_z_data[DATA_BUFFER_SIZE - 1] = data.AXIS_Z;
		gyro_x_data[DATA_BUFFER_SIZE - 1] = read_raw_gyro_x_lsm6dso();
		gyro_y_data[DATA_BUFFER_SIZE - 1] = read_raw_gyro_y_lsm6dso();
		gyro_z_data[DATA_BUFFER_SIZE - 1] = read_raw_gyro_z_lsm6dso();
	}

	sum[0] += acc_x_data[DATA_BUFFER_SIZE - 1];
	sum[1] += acc_y_data[DATA_BUFFER_SIZE - 1];
	sum[2] += acc_z_data[DATA_BUFFER_SIZE - 1];
	sum[3] += gyro_x_data[DATA_BUFFER_SIZE - 1];
	sum[4] += gyro_y_data[DATA_BUFFER_SIZE - 1];
	sum[5] += gyro_z_data[DATA_BUFFER_SIZE - 1];

	int32_t axis_x = sum[0] * 3 / DATA_BUFFER_SIZE;
	int32_t axis_y = sum[1] * 3 / DATA_BUFFER_SIZE;
	int32_t axis_z = sum[2] * 3 / DATA_BUFFER_SIZE;
	int16_t gyro_x = sum[3] / DATA_BUFFER_SIZE;
	int16_t gyro_y = sum[4] / DATA_BUFFER_SIZE;
	int16_t gyro_z = sum[5] / DATA_BUFFER_SIZE;

	// Assuming gyro_val is an array of magnitudes and timer_val is the time difference
void calculate_gyro_magnitude_derivative(int32_t gyro_val[], uint32_t timer_val, int32_t gyro_derivative[], int size) {
    for (int i = 1; i < size; i++) {
        // Ensure no division by zero
        if (timer_val == 0) continue;

        gyro_derivative[i] = (gyro_val[i] - gyro_val[i - 1]) / timer_val;
    }
}

	if (gyro_val[DATA_BUFFER_SIZE - 1] < 10)
		import_para_h3i3(axis_x, axis_y, axis_z);
	acc_val[DATA_BUFFER_SIZE - 1] = findSQRT(axis_x * axis_x + axis_y * axis_y + axis_z * axis_z);
	gyro_val[DATA_BUFFER_SIZE - 1] = findSQRT(gyro_x * gyro_x + gyro_y * gyro_y + gyro_z * gyro_z);
	advertising_buff[VAL_ACC] = acc_val[DATA_BUFFER_SIZE - 1] % 256;
	advertising_buff[VAL_ACC + 1] = acc_val[DATA_BUFFER_SIZE - 1] / 256;
	advertising_buff[VAL_ACC_X] = axis_x % 256;
	advertising_buff[VAL_ACC_X + 1] = axis_x / 256;
	advertising_buff[VAL_ACC_Y] = axis_y % 256;
	advertising_buff[VAL_ACC_Y + 1] = axis_y / 256;
	advertising_buff[VAL_ACC_Z] = axis_z % 256;
	advertising_buff[VAL_ACC_Z + 1] = axis_z / 256;
	advertising_buff[VAL_GYRO] = gyro_val[DATA_BUFFER_SIZE - 1] % 256;
	advertising_buff[VAL_GYRO + 1] = gyro_val[DATA_BUFFER_SIZE - 1] / 256;
	advertising_buff[VAL_GYRO_X] = gyro_x % 256;
	advertising_buff[VAL_GYRO_X + 1] = gyro_x / 256;
	advertising_buff[VAL_GYRO_Y] = gyro_y % 256;
	advertising_buff[VAL_GYRO_Y + 1] = gyro_y / 256;
	advertising_buff[VAL_GYRO_Z] = gyro_z % 256;
	advertising_buff[VAL_GYRO_Z + 1] = gyro_z / 256;
	//int16_t gyro_dev = (gyro_val[DATA_BUFFER_SIZE - 1] - gyro_val[DATA_BUFFER_SIZE - 11])*10 / timer_val;
	advertising_buff[max_speed] = gyro_z % 256;
	advertising_buff[max_speed + 1] = gyro_z / 256;

	// In your main function or wherever appropriate
int32_t gyro_derivative[DATA_BUFFER_SIZE];  // Array to store the derivative
calculate_gyro_magnitude_derivative(gyro_val, timer_val, gyro_derivative, DATA_BUFFER_SIZE);

    // Check if the current gyroscope value indicates the start of a shot
	gyro_dev[DATA_BUFFER_SIZE - 1]=(gyro_val[DATA_BUFFER_SIZE - 1] - gyro_val[DATA_BUFFER_SIZE - 11]) /.01375; 
	if (gyro_dev[DATA_BUFFER_SIZE - 1] > 100000 && !shot_started)
	//if(((gyro_val[DATA_BUFFER_SIZE - 1] - gyro_val[DATA_BUFFER_SIZE - 10]) / timer_val )/10> 1000000 && !shot_started)
    {
        shot_started = true;																																																										
        shot_ended = false;
     //   peak_acceleration = gyro_val[DATA_BUFFER_SIZE - 1];
    }

    // Check if the current gyroscope value indicates the end of a shot
    if (shot_started && gyro_val[DATA_BUFFER_SIZE - 1] < 200)
    {
        shot_ended = true;
        shot_started = false;
        shot_cnt++;
      //  peak_acceleration = 0;
        advertising_buff[SHOT_COUNT] = shot_cnt % 256;
    	advertising_buff[SHOT_COUNT + 1] = shot_cnt / 256;

        // Calculate and store the top speed of the shot
        // [Add code here to calculate and store the top speed]
    }
}
uint32_t findSQRT(uint64_t number)
{
    uint64_t start = 0, end = number;
    uint64_t mid;
    uint32_t ans = 0; // Initialize ans to 0

    while (start <= end)
    {
        mid = (start + end) / 2;
        if (mid * mid == number)
        {
            ans = mid;
            break;
        }
        if (mid * mid < number)
        {
            start = mid + 1;
            ans = mid; // Update ans in this branch as well
        }
        else
        {
            end = mid - 1;
        }
    }

    if (number > 0 && ans == 0)
        return end;

    return ans;
}

int16_t calculate_shot_velocity(int *accelerometer_data, int start_index, int end_index, uint16_t time_interval)
{
    int16_t velocity = 0.0;
    int32_t acceleration_sum = 0.0;
    int32_t time_sum = 0;

    for (int i = start_index; i < end_index; i++)
    {
        acceleration_sum += accelerometer_data[i];
        time_sum += time_interval;
        velocity += (acceleration_sum * time_interval) / 1000; // Assuming time_interval is in milliseconds
    }

    return velocity / time_sum;
}
void shot_detect()
{
	advertising_buff[0] = 0xfe;
	advertising_buff[1] = 0xff;

	// bt_le_adv_stop();

	static const struct bt_data sd[] = {
		BT_DATA(BT_DATA_MANUFACTURER_DATA, advertising_buff, 22),
	};
	bt_le_adv_update_data(ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
}
