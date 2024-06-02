using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Controls.Primitives;
using System.Threading;
using System.ComponentModel;

using vSeaskyProtocol;
using vKmdFocApi;
using CustomControls;
namespace KmdFoc
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {

        public int[] kmdSerialBaudRate =
        {
            256000,
        };

        /// <summary>
        /// usrConfigStrList参数可视化配置
        /// </summary>
        string[,] usrConfigStrList =
        {
            //Name,单位,控件类型,CMD，提示
            {"motor_pole_pairs","Auto","Ei",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_MOTOR_POLE_PAIRS).ToString(),"电磁极对数，在校准过程中自动测量，目前支持2~30极对数，无需设置"},
            {"motor_phase_resistance","Auto","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_MOTOR_PHASE_RESISTANCE).ToString(),"电机定子绕组线圈电阻值，在校准过程中自动测量,无需设置。"},
            {"motor_phase_inductance","Auto","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_MOTOR_PHASE_INDUCTANCE).ToString(),"电机定子线圈相电感值，在校准过程中自动测量，无需设置。"},
            {"inertia","A/(turn/s^2)","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_INERTIA).ToString(),"设置转动惯量，电机轴维持以1转每秒的加速度运行时所需要提供的电流值(能量)，需要根据电机轴重量和所带负载来进行调试,HAL_CONTROL_MODE_VELOCITY_RAMP和HAL_CONTROL_MODE_POSITION_TRAP控制模式下有效。"},
            //// Encoder
            {"encoder_type"," ","Ee",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_ENCODER_TYPE).ToString(),"设置编码器类型，更改编码器之后必须复位和重启，有效更改复位和重启前还应该先保存设置。"},
            {"encoder_pll_bandwidth"," ","Ei",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_ENCODER_PLL_BW).ToString(),"设置编码器 PLL 带宽，一般对于高分辨率编码器 (> 4000个计数/转) 此值应该越高，这样有助于减少电机振动。对于HALL这种分辨率很低的编码器，不宜设置过高的参数。"},
            {"encoder_cpr"," ","Ei",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_ENCODER_CPR).ToString(),"设置编码器分辨率，绝对值编码器无需设置，对于HALL编码器必须在校准前设置为(6*极对数)。"},
            // Calib
            {"calib_valid","Auto","Ei",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_CALIB_VALID).ToString(),"当前校准数据是否有效，校准完成后设置为True。"},
            {"calib_current","A","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_CALIB_CURRENT).ToString(),"设置校准时的电流值，设置太小校准时无法顺畅转动，设置过大则可能导致电机发热严重。"},
            {"calib_max_voltage","V","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_CALIB_MAX_VOLTAGE).ToString(),"设置校准时的最大电压。"},
            // Control
            {"control_mode","","Em",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_CONTROL_MODE).ToString(),"设置电机控制模式,一共有六种模式。"},
            {"current_ramp_rate","A/sec","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_CURRENT_RAMP_RATE).ToString(),"设置电流爬升控制模式下电流爬升速度，电机控制模式为CURRENT_RAMP时有效。"},
            {"ramp_rate","(turn/s)/s","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_VEL_RAMP_RATE).ToString(),"设置转速爬升模式下转速爬升速度，电机控制模式为VELOCITY_RAMP时有效。"},
            {"traj_vel","turn/s","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_TRAJ_VEL).ToString(),"设置梯形轨迹控制模式下最大转速，电机控制模式为POSITION_TRAP时有效。"},
            {"traj_accel","(turn/s)/s","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_TRAJ_ACCEL).ToString(),"设置梯形轨迹控制模式下加速度，电机控制模式为POSITION_TRAP时有效。"},
            {"traj_decel","(turn/s)/s","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_TRAJ_DECEL).ToString(),"设置梯形轨迹控制模式下减速度，电机控制模式为POSITION_TRAP时有效。"},
            {"pos_gain","(turn/s)/turn","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_POS_GAIN).ToString(),"设置位置环增益。"},
            {"vel_gain","A/(turn/s)","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_VEL_GAIN).ToString(),"设置转速环增益。"},
            {"vel_integrator_gain","A/((turn/s)*s)","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_VEL_INTEGRATOR_GAIN).ToString(),"设置转速环积分增益。"},
            {"vel_limit","turn/s","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_VEL_LIMIT).ToString(),"设置转速环限制。"},
            {"current_limit","A","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_CURRENT_LIMIT).ToString(),"设置电流环限制。"},
            {"current_ctrl_p_gain","Auto","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_CURRENT_CTRL_P_GAIN).ToString(),"设置电流环增益，在校准过程中自动计算得到，也可自行设置。"},
            {"current_ctrl_i_gain","Auto","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_CURRENT_CTRL_I_GAIN).ToString(),"设置电流环积分增益，在校准过程中自动计算得到，也可自行设置。"},
            {"current_ctrl_bandwidth","rad/s","Ei",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_CURRENT_CTRL_BW).ToString(),"设置电流环滤波带宽，更新带宽时也会同时更新电流环增益和电流环积分增益。[100~2000]"},	                                    
            // Protect
            {"protect_under_voltage","V","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_PROTECT_UNDER_VOLTAGE).ToString(),"设置低压保护阈值。"},
            {"protect_over_voltage","V","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_PROTECT_OVER_VOLTAGE).ToString(),"设置过压保护阈值。"},
            {"protect_over_current","A","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_PROTECT_OVER_CURRENT).ToString(),"设置过流保护阈值。"},

            {"protect_over_speed","turn/s","Ed",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_PROTECT_OVER_SPEED).ToString(),"设置电机超速保护阈值。"},	                                          
            // CAN
            {"can_id","","Ei",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_CAN_ID).ToString(),"设置设备ID，范围1~16。"},					        // CAN bus ID
            {"can_timeout_ms","","Ei",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_CAN_TIMEOUT_MS).ToString(),"设置通信超时时间，超过此时间未进行通信，电机将停止运行，设置为0则不启用。如果启用，设置值需满足大于500ms，默认0。"},
            {"can_report_heart_ms","","Ei",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_CAN_HEARTBEAT_MS).ToString(),"设置设备心跳，设备将周期性上报状态和位置，设置为0则不启用。如果启用，设置值需满足大于2ms，特别的串口方式需要大于20ms,默认0。"},
            {"cen_report_heart_choose","","Ei",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_CAN_HEARTBEAT_CH).ToString(),"选择can心跳周期性更新的数据，默认[(1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)]。"},
            //Debug
            {"uart_debug_ms","","Ei",((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_UART_DEBUG_MS).ToString(),"设置串口开启调试，按设定周期打印数据波形。设置为0则不启用。如果启用，设置值需满足大于10ms,默认0。"}
        };


        /// <summary>
        /// 存储索引值，避免使用时再去遍历，避免重复运算，加快正式运行时的运行速度
        /// </summary>
        UInt16[] KmdUsrConfigCmdMap = new UInt16[((int)eHalUsrConfigsCmd.HAL_USER_CONFIG_NUM)];

        /// <summary>
        /// False/True String
        /// </summary>
        string[] KmdBoolComboxStr =
{
            "False",
            "True",
        };

        /// <summary>
        /// 控制模式显示
        /// </summary>
        string[] KmdControlModeStr =
        {
            "MODE_CURRENT",
            "MODE_CURRENT_RAMP",
            "MODE_VELOCITY",
            "MODE_VELOCITY_RAMP",
            "MODE_POSITION",
            "MODE_POSITION_TRAP",
        };

        /// <summary>
        /// 控制模式显示
        /// </summary>
        string[] KmdEncoderTypeStr =
        {
            "HAL_ENCODER_MT6825",
            "HAL_ENCODER_MA730",
            "HAL_ENCODER_AS5047P",
            "HAL_ENCODER_HALL",
            "HAL_ENCODER_ABI(暂不支持)",
        };

        /// <summary>
        /// 容器类型
        /// </summary>
        public enum ekmdDataTemplateMenu
        {
            kmdDataTemplateMenuDouble = 0,
            kmdDataTemplateMenuInt,
            kmdDataTemplateMenuBool,
            kmdDataTemplateControlMode,
            kmdDataTemplateEncoderType,
        };

        /// <summary>
        /// DataTemplateSelector 不同容器数据绑定
        /// </summary>
        public class GradeTemplateSelector : DataTemplateSelector
        {
            public DataTemplate vDataTemplateInt { get; set; }
            public DataTemplate vDataTemplateDouble { get; set; }
            public DataTemplate vDataTemplateBool { get; set; }
            public DataTemplate vKmdControlMode { get; set; }
            public DataTemplate vKmdEncoderType { get; set; }

            public override DataTemplate SelectTemplate(object item, DependencyObject container)
            {
                try
                {
                    if (item != null)
                    {
                        KmdUser kmdUserData = item as KmdUser;
                        if (kmdUserData.kmdValueSelect != null)
                        {
                            switch (kmdUserData.kmdValueSelect)
                            {
                                case ekmdDataTemplateMenu.kmdDataTemplateMenuDouble:
                                    { return vDataTemplateDouble; }
                                    break;
                                case ekmdDataTemplateMenu.kmdDataTemplateMenuInt:
                                    { return vDataTemplateInt; }
                                    break;
                                case ekmdDataTemplateMenu.kmdDataTemplateMenuBool:
                                    { return vDataTemplateBool; }
                                    break;
                                case ekmdDataTemplateMenu.kmdDataTemplateControlMode:
                                    { return vKmdControlMode; }
                                    break;
                                case ekmdDataTemplateMenu.kmdDataTemplateEncoderType:
                                    {
                                        return vKmdEncoderType;
                                    }
                                    break;
                                default:; break;
                            }
                        }
                        return base.SelectTemplate(item, container);
                    }
                }
                catch (Exception ex)
                {

                }
                return base.SelectTemplate(item, container);
            }
        }

        /// <summary>
        /// KmdInfo 数据结构，归一化数据结构
        /// </summary>
        public class KmdUser : INotifyPropertyChanged
        {
            public event PropertyChangedEventHandler PropertyChanged;
            public double kmdDoubleValue_;
            public int kmdIntValue_;
            public bool kmdBoolValue_;
            public int kmdMotorControlMode_;
            public int kmdEncoderType_;
            public int kmdId { get; set; }      //编号
            public string kmdName { get; set; } //名称
            public ekmdDataTemplateMenu kmdValueSelect { get; set; }//选择
            public eHalUsrConfigsCmd KmdUsrCmdId { get; set; }
            public string kmdUnit { get; set; }//单位
            public double kmdDoubleValue
            {
                get
                {
                    return (double)kmdDoubleValue_;
                }
                set
                {
                    kmdDoubleValue_ = value;
                    NotifyPropertyChanged("kmdDoubleValue");
                }
            }
            public int kmdIntValue
            {
                get
                {
                    return (int)kmdIntValue_;
                }
                set
                {
                    kmdIntValue_ = value;
                    NotifyPropertyChanged("kmdIntValue");
                }
            }
            public bool kmdBoolValue
            {
                get
                {
                    return (bool)kmdBoolValue_;
                }
                set
                {
                    kmdBoolValue_ = value;
                    NotifyPropertyChanged("kmdBoolValue");
                }
            }
            public int kmdMotorControlMode
            {
                get
                {
                    return (int)kmdMotorControlMode_;
                }
                set
                {
                    kmdMotorControlMode_ = value;
                    NotifyPropertyChanged("kmdMotorControlMode");
                }
            }

            public int kmdEncoderType
            {
                get
                {
                    return (int)kmdEncoderType_;
                }
                set
                {
                    kmdEncoderType_ = value;
                    NotifyPropertyChanged("kmdEncoderType");
                }
            }

            private void NotifyPropertyChanged(string v)
            {
                if (PropertyChanged != null)
                {
                    PropertyChanged(this, new PropertyChangedEventArgs(v));
                }
            }

        };

        /// <summary>
        /// 参数配置界面
        /// </summary>
        const UInt16 KmdUsrRowNum = 33;     //行数
        const UInt16 KmdUsrColNum = 7;      //列数
        List<KmdUser> KmdUserList = new List<KmdUser>();


        public KmdFocApi vcKmdFoc = new KmdFocApi();
        public MainWindow()
        {
            InitializeComponent();
            kmdBaudRateComboBox.ItemsSource = kmdSerialBaudRate;
            kmdBaudRateComboBox.SelectedIndex = 0;
            kmdBaudRateComboBox.IsEnabled = false;
            Thread.Sleep(1000);

            //vSeaskyProtocolInit()
            //回调函数在这里会更方便
            unsafe
            {
                vcKmdFoc.vSeaskyPortInit(24, 24, new pReceivePointer(vKmdReceiveCallback), new pDebugPointer(vKmdDebugCallback),(byte) COM_LOG_LEVEL.COM_LOG_LEVEL_INFO_);
            }

            KmdInitUsrConfigShow();
        }


        /// <summary>
        /// 初始化 KmdInitUsrConfig
        /// </summary>
        private void KmdInitUsrConfigShow()
        {
            DataGridTextColumn tIdDataGridTextColumn = new DataGridTextColumn() { Header = "Id", Binding = new Binding("kmdId") };
            DataGridTextColumn tNameDataGridTextColumn = new DataGridTextColumn() { Header = "Name", Binding = new Binding("kmdName") };
            //public static DataGridTextColumn tValueDataGridTextColumn = new DataGridTextColumn() { Header = "Value", Binding = new Binding("kmdDoubleValue") { StringFormat = "N6" } };
            DataGridTextColumn tUnitDataGridTextColumn = new DataGridTextColumn() { Header = "Unit", Binding = new Binding("kmdUnit") };
            DataGridTemplateColumn tReadDataGridTemplateColumn = new DataGridTemplateColumn();
            DataGridTemplateColumn tWriteDataGridTemplateColumn = new DataGridTemplateColumn();
            DataGridTemplateColumn tHelpDataGridTemplateColumn = new DataGridTemplateColumn();
            DataGridTemplateColumn tValueDataGridTemplateColumn = new DataGridTemplateColumn();
            GradeTemplateSelector tSelectDataGridTemplateColumn = new GradeTemplateSelector();
            menuDataGrid4.AutoGenerateColumns = false;
            menuDataGrid4.CanUserReorderColumns = false;
            menuDataGrid4.CanUserResizeColumns = true;
            menuDataGrid4.CanUserSortColumns = false;
            //menuDataGrid4.GridLinesVisibility = /*"None"*/
            menuDataGrid4.CanUserAddRows = false;
            menuDataGrid4.CanUserDeleteRows = false;
            menuDataGrid4.CanUserResizeRows = false;
            menuDataGrid4.VerticalContentAlignment = VerticalAlignment.Stretch;
            menuDataGrid4.VerticalAlignment = VerticalAlignment.Stretch;
            menuDataGrid4.MinColumnWidth = 64D;
            Style styleRight = new Style(typeof(TextBlock));
            Setter setHorLeft = new Setter(TextBlock.HorizontalAlignmentProperty, HorizontalAlignment.Left);
            Setter setVerCenter = new Setter(TextBlock.VerticalAlignmentProperty, VerticalAlignment.Center);
            styleRight.Setters.Add(setHorLeft);
            styleRight.Setters.Add(setVerCenter);

            tIdDataGridTextColumn.MinWidth = 48D;
            tIdDataGridTextColumn.MaxWidth = 48D;
            tIdDataGridTextColumn.ElementStyle = styleRight;
            tIdDataGridTextColumn.IsReadOnly = true;
            menuDataGrid4.Columns.Add(tIdDataGridTextColumn);

            tNameDataGridTextColumn.MinWidth = 128D;
            tNameDataGridTextColumn.ElementStyle = styleRight;
            tNameDataGridTextColumn.IsReadOnly = true;
            menuDataGrid4.Columns.Add(tNameDataGridTextColumn);

            //tValueDataGridTextColumn.ElementStyle = styleRight;
            //menuDataGrid4.Columns.Add(tValueDataGridTextColumn);

            FrameworkElementFactory tIntTextBox = new FrameworkElementFactory(typeof(NumericUpDownTextBox), "IntTextBox");
            tIntTextBox.SetValue(NumericUpDownTextBox.MinValueProperty, int.MinValue);
            tIntTextBox.SetValue(NumericUpDownTextBox.MaxValueProperty, int.MaxValue);
            tIntTextBox.SetValue(NumericUpDownTextBox.TextProperty, new Binding("kmdIntValue") { UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });
            tIntTextBox.SetValue(NumericUpDownTextBox.MarginProperty, new Thickness(0, 0, 0, 0));
            tIntTextBox.SetValue(NumericUpDownTextBox.PaddingProperty, new Thickness(0, 0, 0, 0));
            tIntTextBox.SetValue(NumericUpDownTextBox.HorizontalAlignmentProperty, HorizontalAlignment.Stretch);
            tIntTextBox.SetValue(NumericUpDownTextBox.VerticalAlignmentProperty, VerticalAlignment.Center);
            tIntTextBox.SetValue(NumericUpDownTextBox.TextAlignmentProperty, TextAlignment.Left);
            tIntTextBox.SetValue(NumericUpDownTextBox.FontSizeProperty, 12D);
            tIntTextBox.SetValue(NumericUpDownTextBox.HeightProperty, 24D);
            tIntTextBox.SetBinding(NumericUpDownTextBox.TagProperty, new Binding("kmdValueSelect"));
            DataTemplate tIntDataTemplate = new DataTemplate();
            tIntDataTemplate.VisualTree = tIntTextBox;

            FrameworkElementFactory tDoubleTextBox = new FrameworkElementFactory(typeof(TextBox), "DoubleTextBox");
            tDoubleTextBox.SetValue(TextBox.TextProperty, new Binding("kmdDoubleValue") { StringFormat = "{0:F6}", UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });
            tDoubleTextBox.SetValue(TextBox.MarginProperty, new Thickness(0, 0, 0, 0));
            tDoubleTextBox.SetValue(TextBox.PaddingProperty, new Thickness(0, 0, 0, 0));
            tDoubleTextBox.SetValue(TextBox.HorizontalAlignmentProperty, HorizontalAlignment.Stretch);
            tDoubleTextBox.SetValue(TextBox.VerticalAlignmentProperty, VerticalAlignment.Center);
            tDoubleTextBox.SetValue(TextBox.TextAlignmentProperty, TextAlignment.Left);
            tDoubleTextBox.SetValue(TextBox.FontSizeProperty, 12D);
            tDoubleTextBox.SetValue(TextBox.HeightProperty, 24D);
            tDoubleTextBox.SetBinding(TextBox.TagProperty, new Binding("kmdValueSelect"));
            DataTemplate tDoubleDataTemplate = new DataTemplate();
            tDoubleDataTemplate.VisualTree = tDoubleTextBox;

            FrameworkElementFactory tBoolComboBox = new FrameworkElementFactory(typeof(ComboBox), "BoolButton");
            tBoolComboBox.SetValue(ComboBox.ItemsSourceProperty, KmdBoolComboxStr);
            tBoolComboBox.SetValue(ComboBox.SelectedIndexProperty, new Binding("kmdBoolValue") { UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });
            tBoolComboBox.SetValue(ComboBox.FontSizeProperty, 12D);
            tBoolComboBox.SetValue(ComboBox.HeightProperty, 24D);
            tBoolComboBox.SetBinding(TagProperty, new Binding("kmdValueSelect"));
            DataTemplate tBoolDataTemplate = new DataTemplate();
            tBoolDataTemplate.VisualTree = tBoolComboBox;

            FrameworkElementFactory tKmModeComboBox = new FrameworkElementFactory(typeof(ComboBox), "KmdModeButton");
            tKmModeComboBox.SetValue(ComboBox.ItemsSourceProperty, KmdControlModeStr);
            //tKmModeComboBox.SetValue(ComboBox.IsDropDownOpenProperty, FA);
            tKmModeComboBox.SetValue(ComboBox.SelectedIndexProperty, new Binding("kmdMotorControlMode") { UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });
            tKmModeComboBox.SetValue(ComboBox.FontSizeProperty, 12D);
            tKmModeComboBox.SetValue(ComboBox.HeightProperty, 24D);
            tKmModeComboBox.SetBinding(TagProperty, new Binding("kmdValueSelect"));
            DataTemplate tKmModeDataTemplate = new DataTemplate();
            tKmModeDataTemplate.VisualTree = tKmModeComboBox;

            FrameworkElementFactory tKmEncoderTypeComboBox = new FrameworkElementFactory(typeof(ComboBox), "kmdEncoderTypeButton");
            tKmEncoderTypeComboBox.SetValue(ComboBox.ItemsSourceProperty, KmdEncoderTypeStr);
            //tKmModeComboBox.SetValue(ComboBox.IsDropDownOpenProperty, FA);
            tKmEncoderTypeComboBox.SetValue(ComboBox.SelectedIndexProperty, new Binding("kmdEncoderType") { UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged });
            tKmEncoderTypeComboBox.SetValue(ComboBox.FontSizeProperty, 12D);
            tKmEncoderTypeComboBox.SetValue(ComboBox.HeightProperty, 24D);
            tKmEncoderTypeComboBox.SetBinding(TagProperty, new Binding("kmdValueSelect"));
            DataTemplate tKmEncoderTypeTemplate = new DataTemplate();
            tKmEncoderTypeTemplate.VisualTree = tKmEncoderTypeComboBox;



            tSelectDataGridTemplateColumn.vDataTemplateInt = tIntDataTemplate;
            tSelectDataGridTemplateColumn.vDataTemplateDouble = tDoubleDataTemplate;
            tSelectDataGridTemplateColumn.vDataTemplateBool = tBoolDataTemplate;
            tSelectDataGridTemplateColumn.vKmdControlMode = tKmModeDataTemplate;
            tSelectDataGridTemplateColumn.vKmdEncoderType = tKmEncoderTypeTemplate;

            tValueDataGridTemplateColumn.MinWidth = 256D;
            tValueDataGridTemplateColumn.MaxWidth = 256D;
            tValueDataGridTemplateColumn.Header = "Value";
            tValueDataGridTemplateColumn.CellTemplateSelector = tSelectDataGridTemplateColumn;

            menuDataGrid4.Columns.Add(tValueDataGridTemplateColumn);



            tUnitDataGridTextColumn.MinWidth = 128D;
            tUnitDataGridTextColumn.MaxWidth = 128D;
            tUnitDataGridTextColumn.ElementStyle = styleRight;
            tUnitDataGridTextColumn.IsReadOnly = true;
            menuDataGrid4.Columns.Add(tUnitDataGridTextColumn);

            //Read栏
            tReadDataGridTemplateColumn.MinWidth = 96D;
            tReadDataGridTemplateColumn.MaxWidth = 96D;
            tReadDataGridTemplateColumn.Header = "Read";
            FrameworkElementFactory tReadButton = new FrameworkElementFactory(typeof(Button), "ReadButton");
            tReadButton.SetValue(Button.ContentProperty, "Read");
            tReadButton.SetValue(Button.FontSizeProperty, 12D);
            tReadButton.SetValue(Button.HeightProperty, 24D);
            tReadButton.SetBinding(Button.TagProperty, new Binding("kmdId"));
            tReadButton.AddHandler(Button.ClickEvent, new RoutedEventHandler(KmdReadButtonClick));
            DataTemplate tReadDataTemplate = new DataTemplate();
            tReadDataTemplate.VisualTree = tReadButton;
            tReadDataGridTemplateColumn.CellTemplate = tReadDataTemplate;
            menuDataGrid4.Columns.Add(tReadDataGridTemplateColumn);

            //Write栏
            tWriteDataGridTemplateColumn.MinWidth = 96D;
            tWriteDataGridTemplateColumn.MaxWidth = 96D;
            tWriteDataGridTemplateColumn.Header = "Write";
            FrameworkElementFactory tWriteButton = new FrameworkElementFactory(typeof(Button), "WriteButton");
            tWriteButton.SetValue(Button.ContentProperty, "Write");
            tWriteButton.SetValue(Button.FontSizeProperty, 12D);
            tWriteButton.SetValue(Button.HeightProperty, 24D);
            tWriteButton.SetBinding(Button.TagProperty, new Binding("kmdId"));
            tWriteButton.AddHandler(Button.ClickEvent, new RoutedEventHandler(KmdWriteButtonClick));
            DataTemplate tWriteDataTemplate = new DataTemplate();
            tWriteDataTemplate.VisualTree = tWriteButton;
            tWriteDataGridTemplateColumn.CellTemplate = tWriteDataTemplate;
            menuDataGrid4.Columns.Add(tWriteDataGridTemplateColumn);

            //Help栏
            tHelpDataGridTemplateColumn.MinWidth = 96D;
            tHelpDataGridTemplateColumn.MaxWidth = 96D;
            tHelpDataGridTemplateColumn.Header = "Help";
            FrameworkElementFactory tHelpButton = new FrameworkElementFactory(typeof(Button), "HelpButton");
            tHelpButton.SetValue(Button.ContentProperty, "Help");
            tHelpButton.SetValue(Button.FontSizeProperty, 12D);
            tHelpButton.SetValue(Button.HeightProperty, 24D);
            tHelpButton.SetBinding(Button.TagProperty, new Binding("kmdId"));
            tHelpButton.AddHandler(Button.ClickEvent, new RoutedEventHandler(KmdHelpButtonClick));
            DataTemplate tHelpDataTemplate = new DataTemplate();
            tHelpDataTemplate.VisualTree = tHelpButton;
            tHelpDataGridTemplateColumn.CellTemplate = tHelpDataTemplate;
            menuDataGrid4.Columns.Add(tHelpDataGridTemplateColumn);
            menuDataGrid4.ItemsSource = KmdUserList;


            for (int i = 0; i < KmdUsrRowNum; i++)
            {
                KmdUser tKmdUser = new KmdUser() { kmdId = i + 1, kmdName = usrConfigStrList[i, 0], kmdDoubleValue = i, kmdUnit = usrConfigStrList[i, 1], KmdUsrCmdId = (eHalUsrConfigsCmd)Convert.ToInt32(usrConfigStrList[i, 3]) };
                switch (usrConfigStrList[i, 2])
                {
                    case "Ei": tKmdUser.kmdValueSelect = ekmdDataTemplateMenu.kmdDataTemplateMenuInt; break;
                    case "Eb": tKmdUser.kmdValueSelect = ekmdDataTemplateMenu.kmdDataTemplateMenuBool; break;
                    case "Ed": tKmdUser.kmdValueSelect = ekmdDataTemplateMenu.kmdDataTemplateMenuDouble; break;
                    case "Em":
                        {
                            tKmdUser.kmdValueSelect = ekmdDataTemplateMenu.kmdDataTemplateControlMode;
                        }
                        break;
                    case "Ee": tKmdUser.kmdValueSelect = ekmdDataTemplateMenu.kmdDataTemplateEncoderType; break;
                }
                KmdUsrConfigCmdMap[(UInt16)tKmdUser.KmdUsrCmdId] = (UInt16)i;
                KmdUserList.Add(tKmdUser);
            }
        }

        /// <summary>
        /// 参数读取按钮事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public void KmdReadButtonClick(object sender, RoutedEventArgs e)
        {
            Button btn = sender as Button;
            int UserConfigGetIndex = -1;
            if (btn != null)
            {
                UserConfigGetIndex = Convert.ToInt32(btn.Tag) - 1;
                if ((UserConfigGetIndex) < KmdUsrRowNum)
                {
                    if (UserConfigGetIndex >= 0)
                    {
                        KmdUserConfigGet(UserConfigGetIndex);
                    }
                }
            }
        }

        /// <summary>
        /// 根据索引值发送获取命令
        /// </summary>
        /// <param name="GetIndex"></param>
        public void KmdUserConfigGet(int GetIndex)
        {
            if (GetIndex < KmdUsrRowNum)
            {
                vcKmdFoc.KmdGetUsrConfig((UInt32)KmdUserList[GetIndex].KmdUsrCmdId);
            }
        }

        /// <summary>
        /// 参数设置按钮事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public void KmdWriteButtonClick(object sender, RoutedEventArgs e)
        {
            Button btn = sender as Button;
            int UserConfigGetIndex = -1;
            if (btn != null)
            {
                UserConfigGetIndex = Convert.ToInt32(btn.Tag) - 1;
                if ((UserConfigGetIndex) < KmdUsrRowNum)
                {
                    if (UserConfigGetIndex >= 0)
                    {
                        KmdUserConfigSet(UserConfigGetIndex);
                    }
                }
            }
        }

        /// <summary>
        /// 根据索引值设置配置
        /// </summary>
        /// <param name="GetIndex"></param>
        public void KmdUserConfigSet(int GetIndex)
        {
            KmdData pKmdData = new KmdData();
            if (GetIndex < KmdUsrRowNum)
            {
                switch (KmdUserList[GetIndex].kmdValueSelect)
                {
                    case ekmdDataTemplateMenu.kmdDataTemplateMenuDouble:
                        {
                            pKmdData.data_float = (float)KmdUserList[GetIndex].kmdDoubleValue;
                        }
                        break;
                    case ekmdDataTemplateMenu.kmdDataTemplateMenuInt:
                        {
                            pKmdData.data_int32 = KmdUserList[GetIndex].kmdIntValue;
                        }
                        break;
                    case ekmdDataTemplateMenu.kmdDataTemplateMenuBool:
                        {
                            if (KmdUserList[GetIndex].kmdBoolValue == false)
                            {
                                pKmdData.data_u32 = 0;
                            }
                            else
                            {
                                pKmdData.data_u32 = 1;
                            }
                        }
                        break;
                    case ekmdDataTemplateMenu.kmdDataTemplateControlMode:
                        {
                            pKmdData.data_int32 = (int)KmdUserList[GetIndex].kmdMotorControlMode;
                        }
                        break;
                    case ekmdDataTemplateMenu.kmdDataTemplateEncoderType:
                        {
                            pKmdData.data_int32 = (int)KmdUserList[GetIndex].kmdEncoderType;
                        }
                        break;
                    default:; break;
                }
                vcKmdFoc.KmdSetUsrConfig(((UInt32)KmdUserList[GetIndex].KmdUsrCmdId) & 0XFFFF, pKmdData.data_u32);
            }
        }

        /// <summary>
        /// 帮助按钮事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public void KmdHelpButtonClick(object sender, RoutedEventArgs e)
        {
            Button btn = sender as Button;
            int HelpIndex = -1;
            if (btn != null)
            {
                HelpIndex = Convert.ToInt32(btn.Tag) - 1;

                if ((HelpIndex) < KmdUsrRowNum)
                {
                    if (HelpIndex >= 0)
                    {
                        MessageBox.Show(usrConfigStrList[HelpIndex, 4], "提示");
                    }
                }
            }
        }

        /// <summary>
        /// 参数设置、获取返回 
        /// </summary>
        /// <param name="pDataCmd"></param>
        /// <param name="pData"></param>
        public void KmdReceiveConfig(UInt32 pDataCmd, UInt32 pData)
        {
            UInt16 ConfigIndex = 0;
            UInt32 tResultState = 0;
            UInt32 tConfigCmdId = 0;
            unsafe
            {
                KmdFocApi.hal_parse_order_id(&tConfigCmdId, &tResultState, pDataCmd);
            }
            if ((eHalOrderResult)tResultState == eHalOrderResult.HAL_RESULT_OK)
            {
                if ((UInt16)tConfigCmdId < KmdUsrConfigCmdMap.Length)
                {
                    ConfigIndex = KmdUsrConfigCmdMap[(UInt16)tConfigCmdId];
                    KmdConfigDataShow(ConfigIndex, pData);
                }
            }
        }

        /// <summary>
        /// 更新设置配置数据，不能太频繁的更新
        /// </summary>
        /// <param name="ReadIndex"></param>
        /// <param name="pData"></param>
        public void KmdConfigDataShow(int ReadIndex, UInt32 pData)
        {
            KmdData pKmdData = new KmdData();
            if (ReadIndex < KmdUsrRowNum)
            {
                pKmdData.data_u32 = pData;
                try
                {
                    switch (KmdUserList[ReadIndex].kmdValueSelect)
                    {
                        case ekmdDataTemplateMenu.kmdDataTemplateMenuDouble:
                            { KmdUserList[ReadIndex].kmdDoubleValue = pKmdData.data_float; }
                            break;
                        case ekmdDataTemplateMenu.kmdDataTemplateMenuInt:
                            { KmdUserList[ReadIndex].kmdIntValue = pKmdData.data_int32; }
                            break;
                        case ekmdDataTemplateMenu.kmdDataTemplateMenuBool:
                            {
                                if (pKmdData.data_int32 == 0)
                                {
                                    KmdUserList[ReadIndex].kmdBoolValue = false;
                                }
                                else
                                {
                                    KmdUserList[ReadIndex].kmdBoolValue = true;
                                }
                            }
                            break;
                        case ekmdDataTemplateMenu.kmdDataTemplateControlMode:
                            {
                                KmdUserList[ReadIndex].kmdMotorControlMode = (int)(eHalControlMode)(pKmdData.data_int32);
                            }
                            break;
                        case ekmdDataTemplateMenu.kmdDataTemplateEncoderType:
                            {
                                KmdUserList[ReadIndex].kmdEncoderType = (int)(eHalEncoderType)(pKmdData.data_int32);
                            }
                            break;
                        default:; break;
                    }
                }
                catch (Exception ex)
                {

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
        public unsafe void vKmdReceiveCallback(UInt16 equipment_type, UInt16 equipment_id, UInt16 data_id, UInt32* pData, UInt16 data_len)
        {

            UInt32 tKmdReceiveCanId = 0;
            UInt32 tKmdNodeId = 0;
            UInt32 tKmdCmdId = 0;
            KmdData[] pKmdData = new KmdData[2];
            tKmdReceiveCanId = equipment_id;
            KmdFocApi.hal_parse_frame_id(&tKmdNodeId, &tKmdCmdId,(UInt32)tKmdReceiveCanId);
            //if (tKmdNodeId != kmdDataInfo.kmdCanID_)
            //{
            //    return;
            //}
            pKmdData[0].data_u32 = pData[0];
            pKmdData[1].data_u32 = pData[1];
            switch ((eHalCanKmdCmd)tKmdCmdId)
            {
                case eHalCanKmdCmd.HAL_KMD_MOTOR_DISABLE:
                    {

                    }
                    break;
                case eHalCanKmdCmd.HAL_KMD_MOTOR_CTR:
                    {

                    }
                    break;
                case eHalCanKmdCmd.HAL_KMD_CALIBRATION_REPORT:
                    {

                    }
                    break;

                //设置位置、速度、电流返回
                case eHalCanKmdCmd.HAL_KMD_READY_SET_POSITION:
                    {


                    }; break;
                case eHalCanKmdCmd.HAL_KMD_READY_SET_VELOCITY:
                    {


                    }; break;
                case eHalCanKmdCmd.HAL_KMD_READY_SET_CURRENT:
                    {

                    }; break;

                //获取位置、速度、电流、电压返回
                case eHalCanKmdCmd.HAL_KMD_GET_POSITION:
                    {

                    }; break;
                case eHalCanKmdCmd.HAL_KMD_GET_VELOCITY:
                    {

                    }; break;
                case eHalCanKmdCmd.HAL_KMD_GET_CURRENT:
                    {

                    }; break;
                case eHalCanKmdCmd.HAL_KMD_GET_FOCBUS:
                    {

                    }; break;

                //获取设置参数返回
                case eHalCanKmdCmd.HAL_KMD_SET_CONFIG: { KmdReceiveConfig(pData[0], pData[1]); }; break;
                case eHalCanKmdCmd.HAL_KMD_GET_CONFIG: { KmdReceiveConfig(pData[0], pData[1]); }; break;
                //电机版本号返回
                case eHalCanKmdCmd.HAL_KMD_GET_FW_VERSION:
                    {
                            //("kmdVersion[" +
                            // (pKmdData[0].data_u32 & 0XFFFF).ToString() + "."
                            // + (pKmdData[0].data_u32 >> 16).ToString() + "."
                            // + (pKmdData[1].data_u32 & 0XFFFF).ToString() + "."
                            // + (pKmdData[1].data_u32 >> 16).ToString() + "]\n");
                    }
                    break;
                case eHalCanKmdCmd.HAL_KMD_FSM_HEARTBEAT0:
                    {

                    }
                    break;
                //获取位置、速度、电流、电压返回
                case eHalCanKmdCmd.HAL_KMD_POS_HEARTBEAT1:
                    {

                    }; break;
                case eHalCanKmdCmd.HAL_KMD_VEL_HEARTBEAT2:
                    {

                    }; break;
                case eHalCanKmdCmd.HAL_KMD_CUR_HEARTBEAT3:
                    {

                    }; break;
                case eHalCanKmdCmd.HAL_KMD_BUS_HEARTBEAT4:
                    {

                    }; break;
                default: { }; break;
            }

        }


        /// <summary>
        /// 复位参数
        /// </summary>
        public void KmdUserConfigReset()
        {
            vcKmdFoc.KmdUsrConfigReset();
        }

        /// <summary>
        /// 保存参数
        /// </summary>
        public void KmdUserConfigSave()
        {
            vcKmdFoc.KmdUsrConfigSave();
        }


        /// <summary>
        /// CLR debug 回调函数
        /// </summary>
        /// <param name="pStr"></param>
        public unsafe void vKmdDebugCallback(sbyte* pStr)
        {
            //Console.WriteLine("vSerialDebugCallback");
        }

        private void KmdConfigClick(object sender, RoutedEventArgs e)
        {
            Button btn = sender as Button;
            switch (btn.Name)
            {
                case "KmdConfigReadAll":
                    {
                        KmdButtonConfig(false);
                        for (int i = 0; i < KmdUsrRowNum; i++)
                        {
                            KmdUserConfigGet(i);
                            Thread.Sleep(10);
                        }
                        KmdButtonConfig(true);
                    }; break;
                case "KmdConfigWriteAll":
                    {
                        KmdButtonConfig(false);
                        for (int i = 0; i < KmdUsrRowNum; i++)
                        {
                            KmdUserConfigSet(i);
                            Thread.Sleep(10);
                        }
                        KmdButtonConfig(true);
                    }; break;
                case "KmdConfigResetAll":
                    {
                        KmdButtonConfig(false);
                        //复位参数
                        KmdUserConfigReset();
                        Thread.Sleep(10);
                        //读取参数
                        for (int i = 0; i < KmdUsrRowNum; i++)
                        {
                            KmdUserConfigGet(i);
                            Thread.Sleep(10);
                        }
                        KmdButtonConfig(true);
                    }; break;
                case "KmdConfigSaveAll":
                    {
                        KmdButtonConfig(false);
                        KmdUserConfigSave();
                        KmdButtonConfig(true);
                    }; break;
            }
        }

        public void KmdButtonConfig(bool enable)
        {
            KmdConfigReadAll.IsEnabled = enable;
            KmdConfigWriteAll.IsEnabled = enable;
            KmdConfigResetAll.IsEnabled = enable;
            KmdConfigSaveAll.IsEnabled = enable;
        }


        private void KmdButtonClick(object sender, RoutedEventArgs e)
        {
            Button btn = sender as Button;

            switch (btn.Name)
            {
                case "kmdComSerialScanfButton":
                    {
                        try
                        {
                           kmdPortNameComboBox.ItemsSource =  (vcKmdFoc.vSerialScanf());
                        }
                        catch
                        {

                        }

                    }; break;
                case "KmdOpenSerialButton":
                    {
                        //KmdOpenSerialButton.IsPressed = false;
                    }; break;
            }
        }

        private void KmdToggleButtonClick(object sender, RoutedEventArgs e)
        {
            ToggleButton btn = sender as ToggleButton;

            switch (btn.Name)
            {
                case "KmdOpenSerialButton":
                    {
                        if(KmdOpenSerialButton.IsChecked==true)
                        {
                            {
                                // 配置参数
                                if (kmdPortNameComboBox.SelectedIndex >= 0)
                                {
                                    vcKmdFoc.vSerialPortName = kmdPortNameComboBox.SelectedItem.ToString();
                                    vcKmdFoc.vSerialPortNameNum = Convert.ToUInt32(vcKmdFoc.vSerialPortName.Replace("COM", ""));
                                }
                                vcKmdFoc.vSerialBaudRate = Convert.ToUInt32(kmdBaudRateComboBox.SelectedItem.ToString());
                                vcKmdFoc.vSerialDataBits = 8;
                                vcKmdFoc.vSerialParity = 0;
                                vcKmdFoc.vSerialStopBits = 1;
                            }
                            vcKmdFoc.vSerialOpen();
                            Thread.Sleep(100);
                            //获取版本号
                            vcKmdFoc.KmdGetVersion();
                            Thread.Sleep(100);
                            vcKmdFoc.KmdGetUsrConfig(1);
                        }
                        else
                        {
                            vcKmdFoc.vSerialClose();
                        }
                    }; break;
            }
        }
    }
}
