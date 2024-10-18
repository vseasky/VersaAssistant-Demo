/** 
 * @License      : All interpretation rights of this software belong to the author, and operations such as reprinting and
 * @			   sharing are prohibited without permission.
 * @Author       : Copyright (c), vseasky.liu vseasky@yeah.net.
 * @Github       : https://github.com/vseasky
 * @Date         : 2024-06-02 23:53:05
 * @FilePath     : \VersaAssistant\main.cpp
 * @Description  : 
 */
#include "VersaWidget.h"
#include <QApplication>
#include <QFontDatabase>
#include <VersaMessageBox.h>
#include <VersaPhysicalCom.h>
#include <VersaConfig.h>
#include <QObject>
#include <QSvgRenderer>

/*!
 * 添加字体库
 */
void addApplicationFont()
{
	QFontDatabase::addApplicationFont("://Roboto/Roboto-Black.ttf");
	QFontDatabase::addApplicationFont("://Roboto/Roboto-BlackItalic.ttf");
	QFontDatabase::addApplicationFont("://Roboto/Roboto-Bold.ttf");
	QFontDatabase::addApplicationFont("://Roboto/Roboto-BoldItalic.ttf");
	QFontDatabase::addApplicationFont("://Roboto/Roboto-Italic.ttf");
	QFontDatabase::addApplicationFont("://Roboto/Roboto-Light.ttf");
	QFontDatabase::addApplicationFont("://Roboto/Roboto-LightItalic.ttf");
	QFontDatabase::addApplicationFont("://Roboto/Roboto-Medium.ttf");
	QFontDatabase::addApplicationFont("://Roboto/Roboto-MediumItalic.ttf");
	QFontDatabase::addApplicationFont("://Roboto/Roboto-Regular.ttf");
	QFontDatabase::addApplicationFont("://Roboto/Roboto-Thin.ttf");
	QFontDatabase::addApplicationFont("://Roboto/Roboto-ThinItalic.ttf");
	QStringList fontPaths = {
			":/JetBrainsMono/fonts/ttf/JetBrainsMono-Bold.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMono-BoldItalic.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMono-ExtraBold.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMono-ExtraBoldItalic.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMono-ExtraLight.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMono-ExtraLightItalic.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMono-Italic.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMono-Light.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMono-LightItalic.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMono-Medium.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMono-MediumItalic.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMono-Regular.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMono-SemiBold.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMono-SemiBoldItalic.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMono-Thin.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMono-ThinItalic.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMonoNL-Bold.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMonoNL-BoldItalic.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMonoNL-ExtraBold.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMonoNL-ExtraBoldItalic.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMonoNL-ExtraLight.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMonoNL-ExtraLightItalic.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMonoNL-Italic.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMonoNL-Light.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMonoNL-LightItalic.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMonoNL-Medium.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMonoNL-MediumItalic.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMonoNL-Regular.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMonoNL-SemiBold.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMonoNL-SemiBoldItalic.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMonoNL-Thin.ttf",
			":/JetBrainsMono/fonts/ttf/JetBrainsMonoNL-ThinItalic.ttf"
	};
	for (const QString &path: fontPaths)
	{
		int id = QFontDatabase::addApplicationFont(path);
		if (id != -1)
		{
			QString family = QFontDatabase::applicationFontFamilies(id).at(0);
			// qDebug() << family;
			// QFont font(family);
			// 在需要的地方使用这个 font 对象
		}
	}
}


void AppIconColorConfig(const QString color)
{
	QFile fileAppIcon(":/VersaAssistant.svg"); // 样式表文件路径
	if (fileAppIcon.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream stream(&fileAppIcon);
		QString svgIcon = stream.readAll().replace("#26c6da", color);
		QFile svgFile(qApp->applicationDirPath() + "/VersaAssistant.svg");
		if (!svgFile.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			qDebug() << "无法打开 SVG 文件";
			return;
		}
		QTextStream out(&svgFile);
		out << svgIcon;
		svgFile.close();
		
		// 加载 SVG 图标
		QSvgRenderer svgRenderer(qApp->applicationDirPath() + "/VersaAssistant.svg");
		// 创建绘图设备
		QPixmap pixmap(256, 256);
		pixmap.fill(Qt::transparent);
		QPainter painter(&pixmap);
		svgRenderer.render(&painter);
		
		// 存储为 ICO 文件
		QFile iconFile(qApp->applicationDirPath() + "/VersaAssistant.ico");
		iconFile.open(QIODevice::WriteOnly);
		pixmap.save(&iconFile, "ICO");
		iconFile.close();
		
		fileAppIcon.close();
	}
}

int main(int argc, char *argv[])
{
	QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
	QApplication VersaApp(argc, argv);
	VersaConfigInit();
	VersaRestoreInfo();
	VersaPhysicalPort;
	VersaMessageInit;
	// 设置风格样式
	addApplicationFont();
	AppIconColorConfig(VersaUser()->jsonConfigColor.at(1).toString());
	VersaApp.setStyleSheet(ThemeStyleQss());
	VersaApp.setApplicationName(VERSA_ASSISTANT_NAME);
	VersaApp.setQuitOnLastWindowClosed(true);
	QFile fileSvgIcon(qApp->applicationDirPath() + "/VersaAssistant.ico");
	if (fileSvgIcon.exists())
	{
		VersaApp.setWindowIcon(QIcon(qApp->applicationDirPath() + "/VersaAssistant.svg"));
	}
	else
	{
		VersaApp.setWindowIcon(QIcon(":/VersaAssistant.svg"));
	}
	VersaWidget MainWidge(nullptr);
	MainWidge.show();
	return VersaApp.exec();
}

