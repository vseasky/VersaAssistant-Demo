#include "VersaConfig.h"
#include <QColor>
#include <QIcon>
#include <QPainter>
#include <QVariant>
#include <QFile>
#include <QSettings>
#include <QDir>
#include <QApplication>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTextCodec>
#include <QDebug>

VersaConfig *pVersaConfigClass = nullptr;

VersaConfig *VersaConfigInit(void)
{
	if (pVersaConfigClass == nullptr)
	{
		pVersaConfigClass = new VersaConfig;
	}
	return pVersaConfigClass;
}

VersaConfig *VersaConfigFree(void)
{
	if (pVersaConfigClass != nullptr)
	{
		delete pVersaConfigClass;
		pVersaConfigClass = nullptr;
	}
	return pVersaConfigClass;
}

VersaConfig::VersaConfig()
{
	// icon :/icons/icons/content/svg/production/ic_remove_24px.svg
	mVseraTheme.mIconPath.insert("IconAppMain", "://MingCute/contact/send_plane_line.svg");
	mVseraTheme.mIconPath.insert("IconAppMinCl", "://icons/icons/content/svg/production/ic_remove_24px.svg");
	mVseraTheme.mIconPath.insert("IconAppMaxMa",
	                             "://icons/icons/toggle/svg/production/ic_check_box_outline_blank_24px.svg");
	mVseraTheme.mIconPath.insert("IconAppMaxNo", "://icons/icons/image/svg/production/ic_filter_none_24px.svg");
	mVseraTheme.mIconPath.insert("IconAppClose", "://icons/icons/navigation/svg/production/ic_close_24px.svg");
	mVseraTheme.mIconPath.insert("IconConnect", "://MingCute/shape/round_fill.svg");
	mVseraTheme.mIconPath.insert("IconDisConnect", "://MingCute/shape/round_line.svg");
	mVseraTheme.mIconPath.insert("IconPhysicalSerial", "://MingCute/arrow/transfer_fill.svg");
	mVseraTheme.mIconPath.insert("IconPhysicalUSB", "://MingCute/device/usb_flash_disk_line.svg");
	mVseraTheme.mIconPath.insert("IconPhysicalInternet", "://MingCute/logo/edge_fill.svg");
	mVseraTheme.mIconPath.insert("IconSend1", "://MingCute/contact/send_fill.svg");
	mVseraTheme.mIconPath.insert("IconSend2", "://MingCute/contact/send_line.svg");
	mVseraTheme.mIconPath.insert("IconSend3", "://MingCute/contact/send_plane_fill.svg");
	mVseraTheme.mIconPath.insert("IconSend4", "://MingCute/contact/send_plane_line.svg");
	mVseraTheme.mIconPath.insert("IconClean", "://MingCute/system/broom_line.svg");
	mVseraTheme.mIconPath.insert("IconCheck1", "://MingCute/system/check_fill.svg");
	mVseraTheme.mIconPath.insert("IconCheck2", "://MingCute/system/check_line.svg");
	mVseraTheme.mIconPath.insert("IconAvalanche1", "://MingCute/crypto/avalanche_AVAX_fill.svg");
	mVseraTheme.mIconPath.insert("IconAvalanche2", "://MingCute/crypto/avalanche_AVAX_line.svg");
	mVseraTheme.mIconPath.insert("IconDockWidget", "://MingCute/system/restore_line.svg");
	// MingCute/system/restore_line.svg
	// icon 颜色样式
	mVseraTheme.mIconColor.insert("AppBackgroundColor", QColor("#FFFFFF"));
	mVseraTheme.mIconColor.insert("AppSelectColor", QColor("#00C4FF"));
	
	
	mVseraTheme.mFontStyle.insert("FontNormal", QFont("JetBrains Mono", 16, QFont::Normal));
	mVseraTheme.mFontStyle.insert("FontAppName", QFont("Roboto", 16, QFont::Normal));
	mVseraTheme.mFontStyle.insert("FontAppAbout", QFont("JetBrains Mono", 10, QFont::Normal));
	
	userConfig.listByteArray.clear();
	userConfig.listByteArray.resize((int) eVersaAppConfigNum);
}

VersaConfig::~VersaConfig()
{
	userConfig.listByteArray.clear();
}

void VersaConfig::setRegisterValue(VersaRegisterEnum registerType, bool registerValue)
{
	if (registerValue == true)
	{
		configRegister |= (1 << registerType);
	}
	else
	{
		configRegister &= (~(1 << registerType));
	}
	setVersaUserConfigUint32(eVersaRegisterValue, configRegister);
}

bool VersaConfig::getRegisterValue(VersaRegisterEnum registerType)
{
	bool value = false;
	configRegister = getVersaUserConfigUint32(eVersaRegisterValue);
	value = ((configRegister & (1 << registerType)) != 0);
	return value;
}

void VersaConfig::setVersaUserConfigUint32(VersaConfigEnum index, const uint32_t &value)
{
	uint32_t data = value;
	QByteArray Buffer;
	QDataStream stream(&Buffer, QIODevice::WriteOnly);
	stream << data;
	if (Buffer.size() > 0)
	{
		userConfig.listByteArray[index] = Buffer;
	}
}

uint32_t VersaConfig::getVersaUserConfigUint32(VersaConfigEnum index)
{
	QDataStream stream(userConfig.listByteArray[index]);
	uint32_t valueData = 0;
	if (userConfig.listByteArray[index].size() >= 4)
	{
		stream >> valueData;
	}
	return valueData;
}

void VersaConfig::setVersaUserConfig(VersaConfigEnum index, const QByteArray &value)
{
	int indexValue = index;
	if (indexValue < ((int) userConfig.listByteArray.size()))
	{
		if (value.size() > 0)
		{
			userConfig.listByteArray[indexValue].clear();
			userConfig.listByteArray[indexValue].append(value);
		}
	}
}

QByteArray VersaConfig::getVersaUserConfig(VersaConfigEnum index)
{
	QByteArray data;
	int indexValue = index;
	if (indexValue < ((int) userConfig.listByteArray.size()))
	{
		return userConfig.listByteArray[indexValue];
	}
	return data;
}

/*!
 * 保存配置
 * @param config
 * @return
 */
QByteArray VersaConfig::saveVersaUserConfig()
{
	QByteArray mDataBuffer;
	QDataStream stream(&mDataBuffer, QIODevice::WriteOnly);
	QVector<int> byteLength;
	byteLength.resize(((int) eVersaAppConfigNum));
	// 存储数据长度
	for (uint32_t iByet = 0; iByet < userConfig.listByteArray.size(); iByet++)
	{
		byteLength[iByet] = userConfig.listByteArray[iByet].size();
	}
	stream << byteLength;
	
	for (uint32_t iByet = 0; iByet < userConfig.listByteArray.size(); iByet++)
	{
		if (byteLength[iByet] > 0)
		{
			stream << userConfig.listByteArray[iByet];
		}
	}
	return mDataBuffer;
}

VersaUserConfig VersaConfig::loadVersaUserConfig(const QByteArray &buffer)
{
	QDataStream stream(buffer);
	QVector<int> byteLength;
	byteLength.resize(((int) eVersaAppConfigNum));
	stream >> byteLength;
	for (uint32_t iByet = 0; iByet < ((int) userConfig.listByteArray.size()); iByet++)
	{
		if (byteLength[iByet] > 0)
		{
			userConfig.listByteArray[iByet].resize(byteLength[iByet]);
			stream >> userConfig.listByteArray[iByet];
		}
	}
	return userConfig;
}

QByteArray VersaConfig::getSaveQByteArry(void)
{
	return saveVersaUserConfig();
}

void VersaConfig::loadQByteArry(QByteArray &byteArray)
{
	loadVersaUserConfig(byteArray);
}

// Protocol
QIcon VersaConfig::ImageSvgLoad(const QString &File, QColor color, int size)
{
	QIcon imgIcon(File);
	QPixmap pixmap(imgIcon.pixmap(size));
	QPainter painter(&pixmap);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(pixmap.rect(), color);
	painter.end();
	return QIcon(pixmap);
}


void VersaConfig::saveConfig()
{
	QSettings settings(UserSettingPath, QSettings::IniFormat);
	settings.setValue("settings/config", UserConfigFile.toUtf8());
	settings.setValue("settings/theme", UserThemeFile.toUtf8());
	settings.setValue("settings/license", UserLicenseKey.toUtf8());
	settings.sync();
	QSettings config(UserConfigFile, QSettings::IniFormat);
	config.setValue("config/config", getSaveQByteArry());
	config.setValue("config/geometry", UserGeometry);
	config.setValue("config/dockmanager", UserDockState);
	config.sync();
}

void VersaConfig::defaultColorJson(const QString &style)
{
	UserThemeFile = qApp->applicationDirPath() + "/theme/theme.json";
	// 创建 JSON 数据
	QJsonObject json;
	jsonConfigColor.append("#FFFFFF");
	jsonConfigColor.append("#0066FF");
	jsonConfigColor.append("#29A9FF");
	jsonConfigColor.append("#91C6FF");
	jsonConfigColor.append("#118DF0");
	jsonConfigColor.append("#C70039");
	jsonConfigColor.append("#FFC773");
	json["theme-color"] = jsonConfigColor;
	// 创建 JSON 文档
	QJsonDocument jsonDoc(json);
	// 打开文件
	QFile jsonThemeFile(UserThemeFile);
	QFileInfo fileInfo(jsonThemeFile);
	QDir dir = fileInfo.dir();
	if (!dir.exists())
	{
		if (!dir.mkpath(dir.absolutePath()))
		{
			qDebug() << "无法创建目录路径";
			return;
		}
	}
	if (jsonThemeFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
	{
		// 写入 JSON 数据到文件
		jsonThemeFile.write(jsonDoc.toJson());
		// 关闭文件
		jsonThemeFile.close();
	}
	else
	{
		qDebug() << "无法打开文件进行写入";
	}
	
	mVseraThemeQssStyle = style
			.arg(jsonConfigColor.at(0).toString()) // 背景色
			.arg(jsonConfigColor.at(1).toString()) // 标题栏颜色
			.arg(jsonConfigColor.at(2).toString()) // 侧边栏颜色
			.arg(jsonConfigColor.at(3).toString())
			.arg(jsonConfigColor.at(4).toString()) // 鼠标停留颜色
			.arg(jsonConfigColor.at(5).toString()) // 按下的颜色
			.arg(jsonConfigColor.at(6).toString());// 选中的颜色
}

/*!
 * 恢复参数
 * @return
 */
void VersaConfig::restoreConfig()
{
	// 扫描文件夹
	getThemeList();
	getConfigList();
	
	QSettings settings(UserSettingPath, QSettings::IniFormat);
	UserConfigFile = QString::fromUtf8(
			settings.value("settings/config",
			               UserConfigFile.toUtf8()).toByteArray());
	UserThemeFile = QString::fromUtf8(
			settings.value("settings/theme",
			               UserThemeFile.toUtf8()).toByteArray());
	UserLicenseKey = QString::fromUtf8(
			settings.value("settings/license",
			               UserLicenseKey.toUtf8()).toByteArray());
	
	// 如果没有配置文件
	if ((UserConfigFile == "") || (configFileList.isEmpty()))
	{
		UserConfigFile = qApp->applicationDirPath() + "/config/config.ini";
		// 默认配置
		setVersaUserConfig(eByteNormalWidgetSend, "");
		setVersaUserConfig(eByteMultWidgetSend, "");
		setVersaUserConfig(eByteProtocolSend, "");
		setVersaUserConfig(eByteProtocolRead, "");
		setVersaUserConfig(eVersaAppModeFile, "");
		setVersaUserConfig(eVersaTextCodec, "UTF-8");
		setVersaUserConfig(eVersaSideBarInfo, "");
		setRegisterValue(eValueEnableOpenGl, true);
		saveConfig();
	}
	
	// 如果没有主题文件
	if ((UserThemeFile == "") || (themeFileList.isEmpty()))
	{
		restoreStyle();
		saveConfig();
	}
	
	// 读取配置文件
	QSettings config(UserConfigFile, QSettings::IniFormat);
	QByteArray byteArray = config.value("config/config").toByteArray();
	loadQByteArry(byteArray);
	UserGeometry = config.value("config/geometry").toByteArray();
	UserDockState = config.value("config/dockmanager").toByteArray();
	restoreStyle();
	
	bool isFirstEnter = false;
	// 多条发送默认数量
	if (getVersaUserConfig(eByteMultWidgetSend) == "")
	{
		setVersaUserConfig(eByteMultWidgetSend, "");
		isFirstEnter = true;
	}
	
	if (getVersaUserConfig(eByteProtocolSend) == "")
	{
		setVersaUserConfig(eByteProtocolSend, "");
		isFirstEnter = true;
	}
	
	if (getVersaUserConfig(eByteProtocolRead) == "")
	{
		setVersaUserConfig(eByteProtocolRead, "");
		isFirstEnter = true;
	}
	
	if (getVersaUserConfig(eVersaAppModeFile) == "")
	{
		setVersaUserConfig(eVersaAppModeFile, "");
		isFirstEnter = true;
	}
	
	if (getVersaUserConfig(eVersaTextCodec) == "")
	{
		setVersaUserConfig(eVersaTextCodec, "UTF-8");
		isFirstEnter = true;
	}
	
	if (getVersaUserConfig(eVersaRegisterValue).size() == 0)
	{
		setRegisterValue(eValueEnableOpenGl, true);
		isFirstEnter = true;
	}
	if (isFirstEnter == true)
	{
		saveConfig();
	}
}

void VersaConfig::restoreStyle()
{
	QFile fileStyle(":/VersaStyleLight.qss"); // 样式表文件路径
	if (fileStyle.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream stream(&fileStyle);
		QString versaStyle = stream.readAll();
		mVseraThemeQssStyle.clear();
		if ((UserThemeFile != "") && (!themeFileList.isEmpty()))
		{
			QFile fileTheme(UserThemeFile);
			if (fileTheme.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				QString jsonString = fileTheme.readAll();
				// 将 JSON 字符串解析为 QJsonDocument 对象
				QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
				jsonConfigColor = jsonDoc["theme-color"].toArray();
				if (jsonConfigColor.size() >= 7)
				{
					const QString color0 = jsonConfigColor.at(0).toString();
					const QString color1 = jsonConfigColor.at(1).toString();
					const QString color2 = jsonConfigColor.at(2).toString();
					const QString color3 = jsonConfigColor.at(3).toString();
					const QString color4 = jsonConfigColor.at(4).toString();
					const QString color5 = jsonConfigColor.at(5).toString();
					const QString color6 = jsonConfigColor.at(6).toString();
					
					mVseraThemeQssStyle = versaStyle
							.arg(color0)    // 背景色
							.arg(color1)    // 标题栏颜色
							.arg(color2)    // 侧边栏颜色
							.arg(color3)
							.arg(color4)    // 鼠标停留颜色
							.arg(color5)    // 按下的颜色
							.arg(color6);   // 选中的颜色
				}
				else
				{
					defaultColorJson(versaStyle);
				}
				fileTheme.close();
			}
			else
			{
				defaultColorJson(versaStyle);
			}
		}
		else
		{
			defaultColorJson(versaStyle);
		}
		fileStyle.close();
	}
}

void VersaConfig::changeUserThemeFile(const QString &file)
{
	UserThemeFile = file;
	QSettings settings(UserSettingPath, QSettings::IniFormat);
	settings.setValue("settings/config", UserConfigFile.toUtf8());
	settings.setValue("settings/theme", UserThemeFile.toUtf8());
	settings.setValue("settings/license", UserLicenseKey.toUtf8());
	settings.sync();
}

void VersaConfig::changeUserConfigFile(const QString &file)
{
	UserConfigFile = file;
	// 重新加载文件
	QSettings config(UserConfigFile, QSettings::IniFormat);
	QByteArray byteArray = config.value("config/config").toByteArray();
	loadQByteArry(byteArray);
	UserGeometry = config.value("config/geometry").toByteArray();
	UserDockState = config.value("config/dockmanager").toByteArray();
	
	QSettings settings(UserSettingPath, QSettings::IniFormat);
	settings.setValue("settings/config", UserConfigFile.toUtf8());
	settings.setValue("settings/theme", UserThemeFile.toUtf8());
	settings.setValue("settings/license", UserLicenseKey.toUtf8());
	settings.sync();
}

void VersaConfig::changeUserLicenseKey(const QString &license)
{
	UserLicenseKey = license;
	
	QSettings settings(UserSettingPath, QSettings::IniFormat);
	settings.setValue("settings/config", UserConfigFile.toUtf8());
	settings.setValue("settings/theme", UserThemeFile.toUtf8());
	settings.setValue("settings/license", UserLicenseKey.toUtf8());
	settings.sync();
}

QStringList VersaConfig::getThemeList()
{
	// 获取可选的配置文件
	QDir themeDir(qApp->applicationDirPath() + "/theme");
	themeFileList.clear();
	QFileInfoList fileInfoList = themeDir.entryInfoList(QDir::Files);
	for (const QFileInfo &fileInfo: fileInfoList)
	{
		QString fileName = fileInfo.fileName();
		if (fileName.endsWith(".json"))
		{
			themeFileList.append(fileName);
		}
	}
	return themeFileList;
}

QStringList VersaConfig::getConfigList()
{
	// 获取可选的配置文件
	QDir configDir(qApp->applicationDirPath() + "/config");
	configFileList.clear();
	QFileInfoList fileInfoList = configDir.entryInfoList(QDir::Files);
	for (const QFileInfo &fileInfo: fileInfoList)
	{
		QString fileName = fileInfo.fileName();
		if (fileName.endsWith(".ini"))
		{
			configFileList.append(fileName);
		}
	}
	return configFileList;
}