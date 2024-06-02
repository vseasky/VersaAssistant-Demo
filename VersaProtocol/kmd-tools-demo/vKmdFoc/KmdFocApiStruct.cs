﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace vKmdFocApi
{
    public enum eSkyEquipmentEnum
    {
        BSP_EQ_TYPE_NULL = 0,
        BSP_EQ_TYPE_KMD_FOC = 1,
    };

    [StructLayout(LayoutKind.Explicit)]
    public struct KmdData
    {
        // FieldOffset控制字段所在的物理位置偏移为0  
        [FieldOffset(0)]
        public float data_float;
        // 同样偏移为0，开始位置与a重叠了。  
        [FieldOffset(0)]
        public Int32 data_int32;
        [FieldOffset(0)]
        public UInt32 data_u32;
    }
    public enum eHalCalibrationReport
    {
        HAL_CALIBRATION_ERROR = 0,
        HAL_CALIBRATION_RES = 1,        //电阻
        HAL_CALIBRATION_IND = 2,        //电感
        HAL_CALIBRATION_PAIRS = 3,      //极对数
        HAL_CALIBRATION_DIR = 4,        //极性
        HAL_CALIBRATION_OFFSET = 5,     //偏移
        HAL_CALIBRATION_POSITION = 6,   //电机位置
        HAL_CALIBRATION_END = 7,        //校准结束

        HAL_CALIBRATION_OFFSET_LUT = 10,//校准数据头
    };

    public enum eCalibrateStep
    {
        HAL_CS_NULL = 0,

        HAL_CS_MOTOT_CALI_READY_START,
        HAL_CS_MOTOT_CALI_READY_RESET,
        HAL_CS_MOTOT_CALI_READY_END,

        HAL_CS_MOTOR_R_START,
        HAL_CS_MOTOR_R_LOOP,
        HAL_CS_MOTOR_R_END,

        HAL_CS_MOTOR_L_START,
        HAL_CS_MOTOR_L_LOOP,
        HAL_CS_MOTOR_L_END,

        HAL_CS_ENCODER_DIR_PP_START,
        HAL_CS_ENCODER_DIR_PP_LOCK,
        HAL_CS_ENCODER_DIR_PP_LOOP,
        HAL_CS_ENCODER_DIR_PP_END,

        HAL_CS_ENCODER_OFFSET_START,
        HAL_CS_ENCODER_OFFSET_LOCK,
        HAL_CS_ENCODER_OFFSET_CW_LOOP,
        HAL_CS_ENCODER_OFFSET_CCW_LOOP,
        HAL_CS_ENCODER_OFFSET_END,

        HAL_CS_ERROR,
    };

    public enum eCalibrationError
    {
        HAL_CE_NULL = 0,
        HAL_CE_PHASE_RESISTANCE_OUT_OF_RANGE,
        HAL_CE_MOTOR_POLE_PAIRS_OUT_OF_RANGE
    };

    public enum eHalFocFsmCmd
    {
        HAL_CMD_FSM_MENU = 0,
        HAL_CMD_FSM_MOTOR,
        HAL_CMD_FSM_MOTOR_ADD,
        HAL_CMD_FSM_MOTOR_DEL,
        HAL_CMD_FSM_CALIBRATION,
        HAL_CMD_FSM_UPDATE_CONFIGS,
        HAL_CMD_FSM_RESET_ERROR,
        HAL_CMD_FSM_UART_SETUP
    };

    public enum eHalFsmStat
    {
        HAL_FSM_STARTUP = 0,
        HAL_FSM_MENU_MODE,
        HAL_FSM_MOTOR_MODE,
        HAL_FSM_CALIBRATION_MODE,

        HAL_FSM_UART_SETUP,
    };

    public enum eHalMotorError
    {
        MOTOR_ERR_OVER_VOLTAGE = 0,
        MOTOR_ERR_UNDER_VOLTAGE = 1,
        MOTOR_ERR_OVER_SPEED = 2,
        MOTOR_ERR_OVER_CURRENT = 3,
        MOTOR_ERR_ENCODER = 4,
        MOTOR_ERR_CAN_CONFLICT = 5,

        MOTOR_ERR_HANLDER = 15,
        MOTOR_ERR_NUM = 16,//此位无效
    };

    public enum eHalCanKmdCmd
    {
        HAL_KMD_MOTOR_DISABLE = 0,//失能电机
        HAL_KMD_MOTOR_CTR,         //电机有限状态机控制	    ->返回值 [(result<<16)|(tHalMotorCmd),float]

        HAL_KMD_CALIBRATION_REPORT, //报告校准

        HAL_KMD_READY_SET_POSITION,//设置目标位置			->返回值 [float,0]
        HAL_KMD_READY_SET_VELOCITY,//设置目标转速			->返回值 [float,0]
        HAL_KMD_READY_SET_CURRENT, //设置目标电流			->返回值 [float,0]

        HAL_KMD_GET_POSITION,       //目标位置，实际位置	->返回值 [float,float]
        HAL_KMD_GET_VELOCITY,       //目标转速，实际转速	->返回值 [float,float]
        HAL_KMD_GET_CURRENT,        //目标电流，实际电流	->返回值 [float,float]
        HAL_KMD_GET_FOCBUS,         //总线电压，实际电压	->返回值 [float,float]

        HAL_KMD_SET_CONFIG,         //设置参数				->返回值 [(result<<16)|(tHalUsrConfigsCmd),实际值(int/float)]
        HAL_KMD_GET_CONFIG,         //获取参数				->返回值 [(result<<16)|(tHalUsrConfigsCmd),实际值(int/float)]

        HAL_KMD_GET_FW_VERSION,     //获取电机版本号

        HAL_KMD_FSM_HEARTBEAT0,     //报告电机状态 0
        HAL_KMD_POS_HEARTBEAT1,     //报告位置信息 1
        HAL_KMD_VEL_HEARTBEAT2,     //报告速度信息 2
        HAL_KMD_CUR_HEARTBEAT3,     //报告电流信息 3
        HAL_KMD_BUS_HEARTBEAT4,     //报告总线信息 4

        HAL_KMD_GET_DEVIEC_ID_00_63, //获取芯片ID
        HAL_KMD_GET_DEVIEC_ID_64_95, //获取芯片ID

        HAL_KMD_SYSTEM_RESET,       //复位系统

        HAL_KMD_MAX_NUM,
    };

    public enum eHalMotorCmd
    {
        HAL_MOTOR_DISABLE = 0,          //失能电机              pData[1] = 当前位置
        HAL_MOTOR_ENABLE,               //使能电机              pData[1] = 当前位置

        HAL_MOTOR_ERROR_REPORT,         //报告错误              pData[1] = 错误信息，自动上报，获取无效
        HAL_MOTOR_ERROR_RESET,          //复位错误              pData[1] = 0

        HAL_MOTOR_GET_STAT,             //获取电机状态          pData[1] = 错误状态

        HAL_MOTOR_CALIBRATION_START,    //开始校准              pData[1] = 0
        HAL_MOTOR_CALIBRATION_ABORT,    //复位校准              pData[1] = 0

        HAL_MOTOR_CONFIGS_UPDATA,        //保存参数             pData[1] = 0
        HAL_MOTOR_CONFIGS_RESET_ALL,	 //复位所有参数         pData[1] = 0
    };


    //128
    public enum eHalUsrConfigsCmd
    {
        HAL_USER_CONFIG_NULL = 0,
        // Motor
        HAL_USER_CONFIG_MOTOR_POLE_PAIRS,
        HAL_USER_CONFIG_MOTOR_PHASE_RESISTANCE,
        HAL_USER_CONFIG_MOTOR_PHASE_INDUCTANCE,
        HAL_USER_CONFIG_INERTIA,
        // Encoder
        HAL_USER_CONFIG_ENCODER_TYPE,
        HAL_USER_CONFIG_ENCODER_PLL_BW,
        HAL_USER_CONFIG_ENCODER_CPR,

        // Calib
        HAL_USER_CONFIG_CALIB_VALID,
        HAL_USER_CONFIG_CALIB_CURRENT,
        HAL_USER_CONFIG_CALIB_MAX_VOLTAGE,
        // Control
        HAL_USER_CONFIG_CONTROL_MODE,
        HAL_USER_CONFIG_CURRENT_RAMP_RATE,
        HAL_USER_CONFIG_VEL_RAMP_RATE,
        HAL_USER_CONFIG_TRAJ_VEL,
        HAL_USER_CONFIG_TRAJ_ACCEL,
        HAL_USER_CONFIG_TRAJ_DECEL,
        HAL_USER_CONFIG_POS_GAIN,
        HAL_USER_CONFIG_VEL_GAIN,
        HAL_USER_CONFIG_VEL_INTEGRATOR_GAIN,
        HAL_USER_CONFIG_VEL_LIMIT,
        HAL_USER_CONFIG_CURRENT_LIMIT,
        HAL_USER_CONFIG_CURRENT_CTRL_P_GAIN,
        HAL_USER_CONFIG_CURRENT_CTRL_I_GAIN,
        HAL_USER_CONFIG_CURRENT_CTRL_BW,
        // Protect
        HAL_USER_CONFIG_PROTECT_UNDER_VOLTAGE,
        HAL_USER_CONFIG_PROTECT_OVER_VOLTAGE,
        HAL_USER_CONFIG_PROTECT_OVER_CURRENT,
        HAL_USER_CONFIG_PROTECT_OVER_SPEED,
        // CAN
        HAL_USER_CONFIG_CAN_ID,
        HAL_USER_CONFIG_CAN_TIMEOUT_MS,
        HAL_USER_CONFIG_CAN_HEARTBEAT_MS,
        HAL_USER_CONFIG_CAN_HEARTBEAT_CH,

        HAL_USER_CONFIG_UART_DEBUG_MS,
        HAL_USER_CONFIG_NUM,//MAX=127
    };

    public enum eHalControlMode
    {
        HAL_CONTROL_MODE_CURRENT = 0,//电流模式
        HAL_CONTROL_MODE_CURRENT_RAMP = 1,//电流爬升模式
        HAL_CONTROL_MODE_VELOCITY = 2,//转速模式
        HAL_CONTROL_MODE_VELOCITY_RAMP = 3,//转速爬升模式
        HAL_CONTROL_MODE_POSITION = 4,//位置模式
        HAL_CONTROL_MODE_POSITION_TRAP = 5,//位置爬升模式
    };

    public enum eHalEncoderType
    {
        HAL_ENCODER_MT6825 = 0, //已经支持
        HAL_ENCODER_MA730,      //已经支持
        HAL_ENCODER_AS5047P,    //已经支持
        HAL_ENCODER_HALL,
        HAL_ENCODER_ABI,		//暂不支持
    };

    public enum eHalOrderResult
    {
        HAL_RESULT_OK,
        HAL_RESULT_FAIL,
        HAL_RESULT_ERROR,
    };

}
