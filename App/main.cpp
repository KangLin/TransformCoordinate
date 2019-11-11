#include <QApplication>
#if defined(Q_OS_ANDROID)
    #include <QtAndroidExtras/QtAndroid>
#endif

#include "mainwindow.h"
#ifdef RABBITCOMMON
    #include "RabbitCommonDir.h"
    #include "RabbitCommonTools.h"
    #include "FrmUpdater/FrmUpdater.h"
#endif

int main(int argc, char *argv[])
{
#if (QT_VERSION > QT_VERSION_CHECK(5,6,0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
#if defined(Q_OS_ANDROID) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    QtAndroid::hideSplashScreen();
#endif
    QApplication a(argc, argv);
    a.setApplicationName("TransformCoordinate");
    
    QTranslator translator;
    translator.load(RabbitCommon::CDir::Instance()->GetDirTranslations()
                    + "/" + "TransformCoordinateApp_" + QLocale::system().name() + ".qm");
    qApp->installTranslator(&translator);
    qDebug() << "QLocale::system().name():" << QLocale::system().name();

    a.setApplicationDisplayName(QObject::tr("Transform coordinate"));

    RabbitCommon::CTools::Instance()->Init();
#ifdef RABBITCOMMON 
    CFrmUpdater *pUpdate = new CFrmUpdater();
    pUpdate->SetTitle(QImage(":/icon/App"));
    if(!pUpdate->GenerateUpdateXml()) 
        return 0; 
#endif
    MainWindow w;
#if defined(Q_OS_ANDROID)
    w.showMaximized();
#else    
    w.show();
#endif

    return a.exec();
}
