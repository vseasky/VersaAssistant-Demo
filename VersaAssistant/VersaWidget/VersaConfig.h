/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:06
 * @FilePath     : \VersaAssistant\VersaWidget\VersaConfig.h
 * @Description  : 
 */


#ifndef VERSA_ASSISTANT_VERSATHEME_H
#define VERSA_ASSISTANT_VERSATHEME_H

#include "VersaAppVersionConfig.h"
#include <QObject>
#include <QMap>
#include <QVector>
#include <QByteArray>
#include <QMetaType>
#include <QJsonArray>


#define RegExpIpAddr "^(?:\\d{1,3}\\.){3}\\d{1,3}$"
#define RegExpIpPort "^[0-9]*[1-9][0-9]*$"

struct VersaThemeStruct
{
	QMap<QString, QString> mIconPath;
	QMap<QString, QColor> mIconColor;
	QMap<QString, QColor> mBackGroundColor;
	QMap<QString, QColor> mFontColor;
	QMap<QString, QFont> mFontStyle;
};

enum VersaRegisterEnum
{
	eValueEnableOpenGl  // 使能OpenGl
	, eValueEnableReceiveHex  // 接收的16进制格式
	, eValueEnableReceiveTimestamp // 接收的时间戳
	, eValueEnableReceiveShowIpAddr// 接收的ip地址
	, eValueEnableReceiveShowSendText // 接收的显示 Tx 数据
	, eValueEnableSendMult   // 多条发送更改
	, eValueEnableSendHex   // 发送Hex格式
	, eValueEnableSendNewLine // 追加新行
	, eValueEnableSendEnter// enter键发送
};

enum VersaConfigEnum
{
	eByteNormalWidgetSend
	, eByteMultWidgetSend
	, eByteProtocolSend
	, eByteProtocolRead
	, eVersaAppModeFile
	, eVersaTextCodec
	, eVersaRegisterValue
	, eVersaSideBarInfo
	, eVersaAppConfigNum
};

struct VersaUserConfig
{
public:
	QString mVersaAppName = VERSA_ASSISTANT_NAME;
	QString mVersaAppVersion = VERSA_ASSISTANT_VERSION;
	QString mLibNameAds = LIB_NAME_ADS;
	QString mLibNameFreegult = LIB_NAME_FREEGULT;
	QString mLibNameAssimp = LIB_NAME_ASSIMP;
	QString mLibNameLibusb = LIB_NAME_LIBUSB;
	QVector<QByteArray> listByteArray;
};

// 寄存器
class VersaConfig : QObject
{
public:
	uint32_t configRegister = 0;
	QString mVseraThemeQssStyle = "";
	VersaThemeStruct mVseraTheme;
	VersaUserConfig userConfig;
	
	const QString UserSettingPath = "setting.ini";
	
	QString UserThemeFile = "";
	QString UserConfigFile = "";
	QString UserLicenseKey = "";
	
	QByteArray UserGeometry;
	QByteArray UserDockState;
	
	QStringList configFileList;
	QStringList themeFileList;
	
	QJsonArray jsonConfigColor;
	VersaConfig();
	~VersaConfig();
	
	QIcon ImageSvgLoad(const QString &File, QColor color, int size);
	
	void setRegisterValue(VersaRegisterEnum registerType, bool registerValue);
	bool getRegisterValue(VersaRegisterEnum registerType);
	
	QStringList getConfigList();
	QStringList getThemeList();
	
	void setVersaUserConfig(VersaConfigEnum index, const QByteArray &value);
	QByteArray getVersaUserConfig(VersaConfigEnum index);
	
	QByteArray saveVersaUserConfig();
	VersaUserConfig loadVersaUserConfig(const QByteArray &buffer);
	// 立即
public slots:
	void saveConfig();
	void restoreConfig();
	void restoreStyle();
	void defaultColorJson(const QString &style);
	void changeUserThemeFile(const QString &file);
	void changeUserConfigFile(const QString &file);
	void changeUserLicenseKey(const QString &license);
	QByteArray getSaveQByteArry(void);
	void loadQByteArry(QByteArray &byteArray);
	void setVersaUserConfigUint32(VersaConfigEnum index, const uint32_t &value);
	uint32_t getVersaUserConfigUint32(VersaConfigEnum index);
};

VersaConfig *VersaConfigInit(void);
VersaConfig *VersaConfigFree(void);

#define ThemeClass                        (VersaConfigInit())
#define ThemeIconFun(file, color, size)   VersaConfigInit()->ImageSvgLoad(file, color, size)

#define ThemeIcon(file, color, size)        VersaConfigInit()->ImageSvgLoad( \
                                                        VersaConfigInit()->mVseraTheme.mIconPath[file],\
                                                        VersaConfigInit()->mVseraTheme.mIconColor[color],\
                                                        size)
#define ThemeColor(color)               (VersaConfigInit()->mVseraTheme.mIconColor[color])
#define ThemeQFont(font)                (VersaConfigInit()->mVseraTheme.mFontStyle[font])
#define ThemeStyleQss()                 (VersaConfigInit()->mVseraThemeQssStyle)
// #define VersaConfigAutoSave()       VersaConfigInit()->autoSaveConfig()
// #define VersaConfigAutoLoad(data)   VersaConfigInit()->autoLoadConfig(data)

#define VersaRegisterSet(cmd, value) VersaConfigInit()->setRegisterValue(cmd,value)
#define VersaRegisterGet(cmd) VersaConfigInit()->getRegisterValue(cmd)

#define VersaUserConfigByteSet(cmd, value)    VersaConfigInit()->setVersaUserConfig(cmd,value)
#define VersaUserConfigByteGet(cmd)           VersaConfigInit()->getVersaUserConfig(cmd)

#define VersaSaveInfo()  VersaConfigInit()->saveConfig()
#define VersaRestoreInfo()  VersaConfigInit()->restoreConfig()
#define VersaUserGeometry() VersaConfigInit()->UserGeometry
#define VersaUserDockState() VersaConfigInit()->UserDockState
#define VersaUserLicense() VersaConfigInit()->UserLicenseKey
#define VersaUser() VersaConfigInit()

#endif //VERSA_ASSISTANT_VERSATHEME_H
