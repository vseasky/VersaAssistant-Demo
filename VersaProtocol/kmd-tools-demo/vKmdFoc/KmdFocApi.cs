using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;



using vSeaskyProtocol;


namespace vKmdFocApi
{
    public partial class KmdFocApi :vSerialPort
    {
        public UInt16 vKmdNodeId = 0x01;
        unsafe public static UInt32* pTxData = (UInt32*)Marshal.AllocHGlobal((24));
        public KmdFocApi()
        {

        }
        public void vSetCheckKmdNodeId(UInt16 id)
        {
            vKmdNodeId = (UInt16)id;
        }

        public UInt16 vGetCheckKmdNodeId()
        {
            return vKmdNodeId;
        }

        /// <summary>
        /// 按需求初始化协议内存和相关回调函数,
        /// </summary>
        public unsafe void vSeaskyProtocolInit()
        {
            vSeaskyPortInit(24,24,new pReceivePointer(vSerialReceiveCallback), new pDebugPointer(vSerialDebugCallback),
                ((byte)COM_LOG_LEVEL.COM_LOG_LEVEL_DEBUG) | 
                ((byte)COM_LOG_LEVEL.COM_LOG_LEVEL_INFO_) | 
                ((byte)COM_LOG_LEVEL.COM_LOG_LEVEL_WARN_) | 
                ((byte)COM_LOG_LEVEL.COM_LOG_LEVEL_ERROR) | 
                ((byte)COM_LOG_LEVEL.COM_LOG_LEVEL_FATAL));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="tKmdMotorCmd"></param>
        public void KmdMotorControl(UInt32 tKmdMotorCmd)
        {
            UInt32 CanFrame;
            if (vSerialState == true)
            {
                unsafe
                {
                    hal_make_frame_id((UInt32)vKmdNodeId, (UInt32)(eHalCanKmdCmd.HAL_KMD_MOTOR_CTR), &CanFrame);
                    pTxData[0] = tKmdMotorCmd;
                    vKmdTransmit(CanFrame, pTxData);
                }
            }
        }


        /// <summary>
        /// 设置位置
        /// </summary>
        /// <param name="pData"></param>
        public void KmdSetPosition(float pData)
        {
            UInt32 CanFrame;
            KmdData pKmdData = new KmdData();
            pKmdData.data_float = pData;
            if (vSerialState == true)
            {
                unsafe
                {
                    hal_make_frame_id((UInt32)vKmdNodeId, (UInt32)(eHalCanKmdCmd.HAL_KMD_READY_SET_POSITION), &CanFrame);
                    pTxData[0] = pKmdData.data_u32;
                    vKmdTransmit(CanFrame, pTxData);
                }
            }
        }

        /// <summary>
        /// 设置转速
        /// </summary>
        /// <param name="pData"></param>
        public void KmdSetVelocity(float pData)
        {
            UInt32 CanFrame;
            KmdData pKmdData = new KmdData();
            pKmdData.data_float = pData;
            if (vSerialState == true)
            {
                unsafe
                {
                    hal_make_frame_id((UInt32)vKmdNodeId, (UInt32)(eHalCanKmdCmd.HAL_KMD_READY_SET_VELOCITY), &CanFrame);
                    pTxData[0] = pKmdData.data_u32;
                    vKmdTransmit(CanFrame, pTxData);
                }
            }
        }

        /// <summary>
        /// 设置电流
        /// </summary>
        /// <param name="pData"></param>
        public void KmdSetCurrent(float pData)
        {
            UInt32 CanFrame;
            KmdData pKmdData = new KmdData();
            pKmdData.data_float = pData;
            if (vSerialState == true)
            {
                unsafe
                {
                    hal_make_frame_id((UInt32)vKmdNodeId, (UInt32)(eHalCanKmdCmd.HAL_KMD_READY_SET_CURRENT), &CanFrame);
                    pTxData[0] = pKmdData.data_u32;
                    vKmdTransmit(CanFrame, pTxData);
                }
            }
        }

        /// <summary>
        /// 获取目标、实际位置
        /// </summary>
        public void KmdGetVersion()
        {
            UInt32 CanFrame;
            if (vSerialState == true)
            {
                unsafe
                {
                    hal_make_frame_id((UInt32)vKmdNodeId, (UInt32)(eHalCanKmdCmd.HAL_KMD_GET_FW_VERSION), &CanFrame);
                    vKmdTransmit(CanFrame, pTxData);
                }
            }
        }

        /// <summary>
        /// 获取目标、实际位置
        /// </summary>
        public void KmdGetMotorMode()
        {
            UInt32 CanFrame;
            if (vSerialState == true)
            {
                unsafe
                {
                    hal_make_frame_id((UInt32)vKmdNodeId, (UInt32)(eHalCanKmdCmd.HAL_KMD_GET_CONFIG), &CanFrame);
                    pTxData[0] = ((UInt32)(eHalUsrConfigsCmd.HAL_USER_CONFIG_CONTROL_MODE)) & 0XFFFF;
                    vKmdTransmit(CanFrame, pTxData);
                }
            }
        }

        /// <summary>
        /// 获取目标、实际位置
        /// </summary>
        public void KmdGetPosition()
        {
            UInt32 CanFrame;
            if (vSerialState == true)
            {
                unsafe
                {
                    hal_make_frame_id((UInt32)vKmdNodeId, (UInt32)(eHalCanKmdCmd.HAL_KMD_GET_POSITION), &CanFrame);
                    vKmdTransmit(CanFrame, pTxData);
                }
            }
        }

        /// <summary>
        /// 获取目标、实际转速
        /// </summary>
        public void KmdGetVelocity()
        {
            UInt32 CanFrame;
            if (vSerialState == true)
            {
                unsafe
                {
                    hal_make_frame_id((UInt32)vKmdNodeId, (UInt32)(eHalCanKmdCmd.HAL_KMD_GET_VELOCITY), &CanFrame);
                    vKmdTransmit(CanFrame, pTxData);
                }
            }
        }

        /// <summary>
        /// 获取目标、实际电流
        /// </summary>
        public void KmdGetCurrent()
        {
            UInt32 CanFrame;
            if (vSerialState == true)
            {
                unsafe
                {
                    hal_make_frame_id((UInt32)vKmdNodeId, (UInt32)(eHalCanKmdCmd.HAL_KMD_GET_CURRENT), &CanFrame);
                    vKmdTransmit(CanFrame, pTxData);
                }
            }
        }

        /// <summary>
        /// 获取总线电压、电流
        /// </summary>
        public void KmdGetFocBus()
        {
            UInt32 CanFrame;
            if (vSerialState == true)
            {
                unsafe
                {
                    hal_make_frame_id((UInt32)vKmdNodeId, (UInt32)(eHalCanKmdCmd.HAL_KMD_GET_FOCBUS), &CanFrame);
                    vKmdTransmit(CanFrame, pTxData);
                }
            }
        }


        /// <summary>
        /// 获取总线电压、电流
        /// </summary>
        public void KmdGetDeviceId_0_7()
        {
            UInt32 CanFrame;
            if (vSerialState == true)
            {
                unsafe
                {
                    hal_make_frame_id((UInt32)vKmdNodeId, (UInt32)(eHalCanKmdCmd.HAL_KMD_GET_DEVIEC_ID_00_63), &CanFrame);
                    vKmdTransmit(CanFrame, pTxData);
                }
            }
        }

        /// <summary>
        /// 获取总线电压、电流
        /// </summary>
        public void KmdGetDeviceId_8_15()
        {
            UInt32 CanFrame;
            if (vSerialState == true)
            {
                unsafe
                {
                    hal_make_frame_id((UInt32)vKmdNodeId, (UInt32)(eHalCanKmdCmd.HAL_KMD_GET_DEVIEC_ID_64_95), &CanFrame);
                    vKmdTransmit(CanFrame, pTxData);
                }
            }
        }

        /// <summary>
        /// 复位参数
        /// </summary>
        public void KmdUsrConfigReset()
        {
            UInt32 CanFrame;
            if (vSerialState == true)
            {
                unsafe
                {
                    KmdMotorControl((UInt32)eHalMotorCmd.HAL_MOTOR_CONFIGS_RESET_ALL);
                }
            }
        }

        /// <summary>
        /// 保存参数
        /// </summary>
        public void KmdUsrConfigSave()
        {
            UInt32 CanFrame;
            if (vSerialState == true)
            {
                unsafe
                {
                    KmdMotorControl((UInt32)eHalMotorCmd.HAL_MOTOR_CONFIGS_UPDATA);
                }
            }
        }

        /// <summary>
        /// 保存参数
        /// </summary>
        public void KmdSystemReset()
        {
            UInt32 CanFrame;
            if (vSerialState == true)
            {
                unsafe
                {
                    hal_make_frame_id((UInt32)vKmdNodeId, (UInt32)(eHalCanKmdCmd.HAL_KMD_SYSTEM_RESET), &CanFrame);
                    vKmdTransmit(CanFrame, pTxData);
                }
            }
        }

        /// <summary>
        /// 获取配置信息命令
        /// </summary>
        /// <param name="HalUsrConfigsCmd"></param>
        public void KmdGetUsrConfig(UInt32 HalUsrConfigsCmd)
        {
            UInt32 CanFrame;
            if (vSerialState == true)
            {
                unsafe
                {
                    hal_make_frame_id((UInt32)vKmdNodeId, (UInt32)(eHalCanKmdCmd.HAL_KMD_GET_CONFIG), &CanFrame);
                    pTxData[0] = HalUsrConfigsCmd & 0XFFFF;
                    vKmdTransmit(CanFrame, pTxData);
                }
            }
        }

        /// <summary>
        /// 设置配置信息命令
        /// </summary>
        /// <param name="HalUsrConfigsCmd"></param>
        /// <param name="pData"></param>
        public void KmdSetUsrConfig(UInt32 HalUsrConfigsCmd, UInt32 pData)
        {
            UInt32 CanFrame;
            if (vSerialState == true)
            {
                unsafe
                {
                    hal_make_frame_id((UInt32)vKmdNodeId, (UInt32)(eHalCanKmdCmd.HAL_KMD_SET_CONFIG), &CanFrame);
                    pTxData[0] = HalUsrConfigsCmd & 0XFFFF;
                    pTxData[1] = pData;
                    vKmdTransmit(CanFrame, pTxData);
                }
            }
        }

        /// <summary>
        /// CLR 接收数据处理完成回调函数
        /// </summary>
        /// <param name="equipment_type"></param>
        /// <param name="equipment_id"></param>
        /// <param name="data_id"></param>
        /// <param name="pData"></param>
        /// <param name="data_len"></param>
        public unsafe void vSerialReceiveCallback(UInt16 equipment_type, UInt16 equipment_id, UInt16 data_id, UInt32* pData, UInt16 data_len)
        {
            //Console.WriteLine("vSerialReceiveCallback");
        }

        /// <summary>
        /// CLR debug 回调函数
        /// </summary>
        /// <param name="pStr"></param>
        public unsafe void vSerialDebugCallback(sbyte* pStr)
        {
            //Console.WriteLine("vSerialDebugCallback");
        }

        /// <summary>
        /// KmdFoc发送函数
        /// </summary>
        /// <param name="CanId"></param>
        /// <param name="pData"></param>
        /// <returns></returns>
        public unsafe bool vKmdTransmit(UInt32 CanId, UInt32* pData)
        {
            vcSeaskyPort.ProtocolTransmit((UInt16)eSkyEquipmentEnum.BSP_EQ_TYPE_KMD_FOC,(UInt16)CanId, 0, pData, 2);
            return true;
        }

        /// <summary>
        /// 生成pFrameID
        /// </summary>
        /// <param name="pNodeID">nodeID</param>
        /// <param name="pCmdId">pCmdId</param>
        /// <param name="pFrameID"></param>
        public unsafe static void hal_make_frame_id(UInt32 pNodeID, UInt32 pCmdId, UInt32* pFrameID)
        {
            // Frame
            // CMD    | nodeID
            // 7 bits | 4 bits
            *pFrameID = (pCmdId << 4) | (pNodeID & 0X0F);
        }

        /// <summary>
        /// 解析pFrameID
        /// </summary>
        /// <param name="pNodeID">nodeID</param>
        /// <param name="pCmdId">pCmdId</param>
        /// <param name="pFrameID"></param>
        public unsafe static void hal_parse_frame_id(UInt32* pNodeID, UInt32* pCmdId, UInt32 pFrameID)
        {
            // Frame
            // CMD    | nodeID
            // 7 bits | 4 bits
            *pNodeID = (pFrameID & 0x0F);
            *pCmdId = pFrameID >> 4;
        }

        /// <summary>
        /// 生成orderId
        /// </summary>
        /// <param name="pCmdId"></param>
        /// <param name="pResult"></param>
        /// <param name="pOrderId"></param>
        public unsafe static void hal_make_order_id(UInt32 pCmdId, UInt32 pResult, UInt32* pOrderId)
        {
            // pOrderId
            // pResult    | pCmdId
            // 16 bits    | 16bits
            *pOrderId = (pResult << 16) | (pCmdId & 0XFFFF);
        }

        /// <summary>
        /// 解析orderId
        /// </summary>
        /// <param name="pCmdId"></param>
        /// <param name="pResult"></param>
        /// <param name="pOrderId"></param>
        public unsafe static void hal_parse_order_id(UInt32* pCmdId, UInt32* pResult, UInt32 pOrderId)
        {
            // pOrderId
            // pResult    | pCmdId
            // 16 bits    | 16bits
            *pCmdId = pOrderId & 0xFFFF;
            *pResult = (pOrderId >> 16);
        }

        /// <summary>
        /// 解析电机状态
        /// </summary>
        /// <param name="pFsm"></param>
        /// <param name="pErr"></param>
        /// <param name="tData"></param>
        public unsafe static void parse_motor_state(UInt32* pFsm, UInt32* pErr, UInt32 tData)
        {
            *pFsm = tData >> 16;
            *pErr = tData & 0XFFFF;
        }

    }
}
