/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          zf_device_imu660rb
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.10.2
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
* 2023-04-28       pudding            ��������ע��˵��
********************************************************************************************************************/
/*********************************************************************************************************************
* ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�            ��Ƭ���ܽ�
*                   // Ӳ�� SPI ����
*                   SCL/SPC           �鿴 zf_device_imu660rb.h �� IMU660RB_SPC_PIN �궨��
*                   SDA/DSI           �鿴 zf_device_imu660rb.h �� IMU660RB_SDI_PIN �궨��
*                   SA0/SDO           �鿴 zf_device_imu660rb.h �� IMU660RB_SDO_PIN �궨��
*                   CS                �鿴 zf_device_imu660rb.h �� IMU660RB_CS_PIN �궨��
*                   VCC               3.3V��Դ
*                   GND               ��Դ��
*                   ������������
*
*                   // ��� IIC ����
*                   SCL/SPC           �鿴 zf_device_imu660rb.h �� IMU660RB_SCL_PIN �궨��
*                   SDA/DSI           �鿴 zf_device_imu660rb.h �� IMU660RB_SDA_PIN �궨��
*                   VCC               3.3V��Դ
*                   GND               ��Դ��
*                   ������������
*                   ------------------------------------
********************************************************************************************************************/

#include "zf_common_debug.h"
#include "zf_driver_delay.h"
#include "zf_driver_spi.h"
#include "zf_driver_gpio.h"
#include "zf_driver_soft_iic.h"
#include "zf_device_config.h"
#include "zf_device_imu660rb.h"

int16 imu660rb_gyro_x = 0, imu660rb_gyro_y = 0, imu660rb_gyro_z = 0;            // ��������������   gyro (������)
int16 imu660rb_acc_x = 0, imu660rb_acc_y = 0, imu660rb_acc_z = 0;               // ������ٶȼ����� acc  (accelerometer ���ٶȼ�)

#if IMU660RB_USE_SOFT_IIC
static soft_iic_info_struct imu660rb_iic_struct;

#define imu660rb_write_register(reg, data)        (soft_iic_write_8bit_register (&imu660rb_iic_struct, (reg), (data)))
#define imu660rb_write_registers(reg, data, len)  (soft_iic_write_8bit_registers(&imu660rb_iic_struct, (reg), (data), (len)))
#define imu660rb_read_register(reg)               (soft_iic_read_8bit_register  (&imu660rb_iic_struct, (reg)))
#define imu660rb_read_registers(reg, data, len)   (soft_iic_read_8bit_registers (&imu660rb_iic_struct, (reg), (data), (len)))
#else
//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RB д�Ĵ���
// ����˵��     reg             �Ĵ�����ַ
// ����˵��     data            ����
// ���ز���     void
// ʹ��ʾ��     imu660rb_write_register(IMU660RB_PWR_CONF, 0x00);                   // �رո߼�ʡ��ģʽ
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static void imu660rb_write_register(uint8 reg, uint8 data)
{
    IMU660RB_CS(0);
    spi_write_8bit_register(IMU660RB_SPI, reg | IMU660RB_SPI_W, data);
    IMU660RB_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RB д����
// ����˵��     reg             �Ĵ�����ַ
// ����˵��     data            ����
// ���ز���     void
// ʹ��ʾ��     imu660rb_write_registers(IMU660RB_INIT_DATA, imu660rb_config_file, sizeof(imu660rb_config_file));
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
// static void imu660rb_write_registers(uint8 reg, const uint8 *data, uint32 len)
// {
//     IMU660RB_CS(0);
//     spi_write_8bit_registers(IMU660RB_SPI, reg | IMU660RB_SPI_W, data, len);
//     IMU660RB_CS(1);
// }

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RB ���Ĵ���
// ����˵��     reg             �Ĵ�����ַ
// ���ز���     uint8           ����
// ʹ��ʾ��     imu660rb_read_register(IMU660RB_CHIP_ID);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu660rb_read_register(uint8 reg)
{
    uint8 data;
    IMU660RB_CS(0);
    data = spi_read_8bit_register(IMU660RB_SPI, reg | IMU660RB_SPI_R);
    IMU660RB_CS(1);
    return data;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RB ������
// ����˵��     reg             �Ĵ�����ַ
// ����˵��     data            ���ݻ�����
// ����˵��     len             ���ݳ���
// ���ز���     void
// ʹ��ʾ��     imu660rb_read_registers(IMU660RB_ACC_ADDRESS, dat, 6);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static void imu660rb_read_registers(uint8 reg, uint8 *data, uint32 len)
{
    IMU660RB_CS(0);
    spi_read_8bit_registers(IMU660RB_SPI, reg | IMU660RB_SPI_R, data, len);
    IMU660RB_CS(1);

}
#endif

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RB �Լ�
// ����˵��     void
// ���ز���     uint8           1-�Լ�ʧ�� 0-�Լ�ɹ�
// ʹ��ʾ��     imu660rb_self_check();
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu660rb_self_check (void)
{
    uint8 dat = 0, return_state = 0;
    uint16 timeout_count = 0;
    do
    {
        if(timeout_count ++ > IMU660RB_TIMEOUT_COUNT)
        {
            return_state =  1;
            break;
        }
        dat = imu660rb_read_register(IMU660RB_CHIP_ID);
        system_delay_ms(1);
    }while(0x6B != dat);                                                        // ��ȡ�豸ID�Ƿ����0X24���������0X24����Ϊû��⵽�豸
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ IMU660RB ���ٶȼ�����
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     imu660rb_get_acc();                                             // ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
// ��ע��Ϣ     ʹ�� SPI �Ĳɼ�ʱ��Ϊ69us
//             ʹ�� IIC �Ĳɼ�ʱ��Ϊ126us        �ɼ����ٶȼƵ�ʱ����ɼ������ǵ�ʱ��һ�µ�ԭ���Ƕ�ֻ�Ƕ�ȡ�Ĵ�������
//-------------------------------------------------------------------------------------------------------------------
void imu660rb_get_acc (void)
{
    uint8 dat[6];

    imu660rb_read_registers(IMU660RB_ACC_ADDRESS, dat, 6);
    imu660rb_acc_x = (int16)(((uint16)dat[1]<<8 | dat[0]));
    imu660rb_acc_y = (int16)(((uint16)dat[3]<<8 | dat[2]));
    imu660rb_acc_z = (int16)(((uint16)dat[5]<<8 | dat[4]));
}
//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ IMU660RB ����������
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     imu660rb_get_gyro();                                            // ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
// ��ע��Ϣ     ʹ�� SPI �Ĳɼ�ʱ��Ϊ69us
//             ʹ�� IIC �Ĳɼ�ʱ��Ϊ126us
//-------------------------------------------------------------------------------------------------------------------
void imu660rb_get_gyro (void)
{
    uint8 dat[6];

    imu660rb_read_registers(IMU660RB_GYRO_ADDRESS, dat, 6);
    imu660rb_gyro_x = (int16)(((uint16)dat[1]<<8 | dat[0]));
    imu660rb_gyro_y = (int16)(((uint16)dat[3]<<8 | dat[2]));
    imu660rb_gyro_z = (int16)(((uint16)dat[5]<<8 | dat[4]));
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �� IMU660RB ���ٶȼ�����ת��Ϊʵ����������
// ����˵��     gyro_value      ������ļ��ٶȼ�����
// ���ز���     void
// ʹ��ʾ��     float data = imu660rb_acc_transition(imu660rb_acc_x);           // ��λΪ g(m/s^2)
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
float imu660rb_acc_transition (int16 acc_value)
{
    float acc_data = 0;
    switch(IMU660RB_ACC_SAMPLE)
    {
        case 0x30: acc_data = (float)acc_value / 16393; break;                  // 0x30 ���ٶ�����Ϊ:��2G      ��ȡ���ļ��ٶȼ����� ���� 16393 ������ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        case 0x38: acc_data = (float)acc_value / 8197;  break;                  // 0x38 ���ٶ�����Ϊ:��4G      ��ȡ���ļ��ٶȼ����� ���� 8197 �� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        case 0x3C: acc_data = (float)acc_value / 4098;  break;                  // 0x3C ���ٶ�����Ϊ:��8G      ��ȡ���ļ��ٶȼ����� ���� 4098 �� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        case 0x34: acc_data = (float)acc_value / 2049;  break;                  // 0x34 ���ٶ�����Ϊ:��16G     ��ȡ���ļ��ٶȼ����� ���� 2049 �� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        default: break;
    }
    return acc_data;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �� IMU660RB ����������ת��Ϊʵ����������
// ����˵��     gyro_value      �����������������
// ���ز���     void
// ʹ��ʾ��     float data = imu660rb_gyro_transition(imu660rb_gyro_x);         // ��λΪ��/s
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
float imu660rb_gyro_transition (int16 gyro_value)
{
    float gyro_data = 0;
    switch(IMU660RB_GYR_SAMPLE)
    {
        case 0x52: gyro_data = (float)gyro_value / 228.6f;  break;              //  0x52 ����������Ϊ:��125dps  ��ȡ�������������ݳ��� 228.6��   ����ת��Ϊ������λ�����ݣ���λΪ����/s
        case 0x50: gyro_data = (float)gyro_value / 114.3f;  break;              //  0x50 ����������Ϊ:��250dps  ��ȡ�������������ݳ��� 114.3��   ����ת��Ϊ������λ�����ݣ���λΪ����/s
        case 0x54: gyro_data = (float)gyro_value / 57.1f;   break;              //  0x54 ����������Ϊ:��500dps  ��ȡ�������������ݳ��� 57.1��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
        case 0x58: gyro_data = (float)gyro_value / 28.6f;   break;              //  0x58 ����������Ϊ:��1000dps ��ȡ�������������ݳ��� 28.6��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
        case 0x5C: gyro_data = (float)gyro_value / 14.3f;   break;              //  0x5C ����������Ϊ:��2000dps ��ȡ�������������ݳ��� 14.3��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
        case 0x51: gyro_data = (float)gyro_value / 7.1f;    break;              //  0x51 ����������Ϊ:��4000dps ��ȡ�������������ݳ��� 7.1��     ����ת��Ϊ������λ�����ݣ���λΪ����/s
        default: break;
    }
    return gyro_data;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ʼ�� IMU660RB
// ����˵��     void
// ���ز���     uint8           1-��ʼ��ʧ�� 0-��ʼ���ɹ�
// ʹ��ʾ��     imu660rb_init();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint8 imu660rb_init (void)
{
    uint8 return_state = 0;
    system_delay_ms(20);                                                        // �ȴ��豸�ϵ�ɹ�

#if IMU660RB_USE_SOFT_IIC
    soft_iic_init(&imu660rb_iic_struct, IMU660RB_DEV_ADDR, IMU660RB_SOFT_IIC_DELAY, IMU660RB_SCL_PIN, IMU660RB_SDA_PIN);        // ���� IMU660RB �� IIC �˿�
#else
    spi_init(IMU660RB_SPI, SPI_MODE0, IMU660RB_SPI_SPEED, IMU660RB_SPC_PIN, IMU660RB_SDI_PIN, IMU660RB_SDO_PIN, SPI_CS_NULL);   // ���� IMU660RB �� SPI �˿�
    gpio_init(IMU660RB_CS_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);                  // ���� IMU660RB �� CS �˿�
//    imu660rb_read_register(IMU660RB_CHIP_ID);                                   // ��ȡһ���豸ID ���豸����ΪSPIģʽ
#endif

    do{
        if(imu660rb_self_check())                                               // IMU660RB �Լ�
        {
            // �������������˶�����Ϣ ������ʾ����λ��������
            // ��ô���� IMU660RB �Լ������ʱ�˳���
            // ���һ�½�����û������ ���û������ܾ��ǻ���
            zf_log(0, "imu660rb self check error.");
            return_state = 1;
            break;
        }

        imu660rb_write_register(IMU660RB_INT1_CTRL, 0x03);                      // ���������� ���ٶ����ݾ����ж�
        imu660rb_write_register(IMU660RB_CTRL1_XL, IMU660RB_ACC_SAMPLE);        // ���ü��ٶȼ����� ��8G �Լ������������ 52hz �Լ����ٶ���Ϣ�ӵ�һ���˲������
        // IMU660RB_CTRL1_XL �Ĵ���
        // ����Ϊ:0x30 ���ٶ�����Ϊ:��2G      ��ȡ���ļ��ٶȼ����� ����16393������ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        // ����Ϊ:0x38 ���ٶ�����Ϊ:��4G      ��ȡ���ļ��ٶȼ����� ����8197�� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        // ����Ϊ:0x3C ���ٶ�����Ϊ:��8G      ��ȡ���ļ��ٶȼ����� ����4098�� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        // ����Ϊ:0x34 ���ٶ�����Ϊ:��16G     ��ȡ���ļ��ٶȼ����� ����2049�� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)

        imu660rb_write_register(IMU660RB_CTRL2_G, IMU660RB_GYR_SAMPLE);         // ���������Ǽ����� ��2000dps �Լ������������ 208hz
        // IMU660RB_CTRL2_G �Ĵ���
        // ����Ϊ:0x52 ����������Ϊ:��125dps  ��ȡ�������������ݳ���228.6��   ����ת��Ϊ������λ�����ݣ���λΪ����/s
        // ����Ϊ:0x50 ����������Ϊ:��250dps  ��ȡ�������������ݳ���114.3��   ����ת��Ϊ������λ�����ݣ���λΪ����/s
        // ����Ϊ:0x54 ����������Ϊ:��500dps  ��ȡ�������������ݳ���57.1��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
        // ����Ϊ:0x58 ����������Ϊ:��1000dps ��ȡ�������������ݳ���28.6��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
        // ����Ϊ:0x5C ����������Ϊ:��2000dps ��ȡ�������������ݳ���14.3��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
        // ����Ϊ:0x51 ����������Ϊ:��4000dps ��ȡ�������������ݳ���7.1��     ����ת��Ϊ������λ�����ݣ���λΪ����/s

        imu660rb_write_register(IMU660RB_CTRL3_C, 0x44);                        // ʹ�����������ֵ�ͨ�˲���
        imu660rb_write_register(IMU660RB_CTRL4_C, 0x02);                        // ʹ�����ֵ�ͨ�˲���
        imu660rb_write_register(IMU660RB_CTRL5_C, 0x00);                        // ���ٶȼ�����������������
        imu660rb_write_register(IMU660RB_CTRL6_C, 0x00);                        // �������ٶȼƸ�����ģʽ �����ǵ�ͨ�˲� 133hz
        imu660rb_write_register(IMU660RB_CTRL7_G, 0x00);                        // ���������Ǹ�����ģʽ �رո�ͨ�˲�
        imu660rb_write_register(IMU660RB_CTRL9_XL, 0x01);                       // �ر�I3C�ӿ�

    }while(0);
    return return_state;
}


